//_stdcall
#include "mini_ddk.h"
#pragma  INITCODE
JMPCODE oldCode;	//��������ǰ5�ֽ� �Ա�ָ�
PJMPCODE pcur;

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING B) //TYPEDEF LONG NTSTATUS
{  
	ULONG cur,old;
	JMPCODE JmpCode;
	
	cur=GetNt_CurAddr();//A
	old=GetNt_OldAddr();//C
	if (cur!=old){ 	//��hook��
		pcur=(PJMPCODE)(cur)	//��ʼ��ָ��
		//û�޸�ǰ����
		oldCode.E9=pcur->E9;
		oldCode.JMPADDR=pcur->JMPADDR;	//4�ֽ�
		//
		
	    JmpCode.E9=0xE9; 
		JmpCode.JMPADDR=old-cur-5;
	    KdPrint(("Ҫд��ĵ�ַ%X",JmpCode.JMPADDR));
		
        //д��JMP   C-A-5=B //ʵ��Ҫд���ַ
		__asm//ȥ��ҳ�汣��
		{
			cli
			mov eax,cr0
			and eax,not 10000h //and eax,0FFFEFFFFh
			mov cr0,eax

		}
		
		pcur->E9=0xE9;//jmp
		pcur->JMPADDR=JmpCode.JMPADDR;//Ҫ��ת�ĵ�ַ

		 __asm //�ָ�ҳ����
		 {
			mov eax,cr0
			or  eax,10000h //or eax,not 0FFFEFFFFh
			mov cr0,eax
			sti
		 }
		
		KdPrint(("NtOpenProcess��HOOK��"));
	}
 /* ULONG SSDT_NtOpenProcess_Cur_Addr;
KdPrint(("�����ɹ�������...OK++++++++\n\n"));
 //��ȡSSDT���� NtOpenProcess��ǰ��ַ KeServiceDescriptorTable
 // [[KeServiceDescriptorTable]+0x7A*4] 

__asm
{    int 3
	push ebx
	push eax
		mov ebx,KeServiceDescriptorTable
		mov ebx,[ebx] //��Ļ���ַ
		mov eax,0x7a
		shl eax,2//0x7A*4 //imul eax,eax,4//shl eax,2
		add ebx,eax//[KeServiceDescriptorTable]+0x7A*4
		mov ebx,[ebx]
        mov SSDT_NtOpenProcess_Cur_Addr,ebx
	pop  eax	
	pop  ebx
}
KdPrint(("SSDT_NtOpenProcess_Cur_Addr=%x\n\n",SSDT_NtOpenProcess_Cur_Addr));*/
 //ע����ǲ����
pDriverObject->MajorFunction[IRP_MJ_CREATE]=ddk_DispatchRoutine_CONTROL; //IRP_MJ_CREATE���IRP������
pDriverObject->MajorFunction[IRP_MJ_CLOSE]=ddk_DispatchRoutine_CONTROL; //IRP_MJ_CREATE���IRP������
pDriverObject->MajorFunction[IRP_MJ_READ]=ddk_DispatchRoutine_CONTROL; //IRP_MJ_CREATE���IRP������
pDriverObject->MajorFunction[IRP_MJ_CLOSE]=ddk_DispatchRoutine_CONTROL; //IRP_MJ_CREATE���IRP������
pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]=ddk_DispatchRoutine_CONTROL; //IRP_MJ_CREATE���IRP������
 CreateMyDevice(pDriverObject);//������Ӧ���豸
 pDriverObject->DriverUnload=DDK_Unload;
return (1);
}
//#pragma code_seg("PAGE")
#pragma PAGECODE
VOID DDK_Unload (IN PDRIVER_OBJECT pDriverObject)
{
  PDEVICE_OBJECT pDev;//����ȡ��Ҫɾ���豸����
  UNICODE_STRING symLinkName; // 
  
		__asm//ȥ��ҳ�汣��
		{
			cli
			mov eax,cr0
			and eax,not 10000h //and eax,0FFFEFFFFh
			mov cr0,eax
		}
		
		//pcur=(PJMPCODE)(cur)
		pcur->E9 = oldCode.E9;
		pcur->JMPADDR = oldCode.JMPADDR;	//4�ֽ�

		 __asm //�ָ�ҳ����
		 {
			mov eax,cr0
			or  eax,10000h //or eax,not 0FFFEFFFFh
			mov cr0,eax
			sti
		 }
 
  pDev=pDriverObject->DeviceObject;
  IoDeleteDevice(pDev); //ɾ���豸
  
  //ȡ������������
   RtlInitUnicodeString(&symLinkName,L"\\??\\yjx888");
  //ɾ����������
   IoDeleteSymbolicLink(&symLinkName);
 KdPrint(("�����ɹ���ж��...OK-----------")); //sprintf,printf
 //ȡ��Ҫɾ���豸����
//ɾ�������豸
 DbgPrint("ж�سɹ�");
}
#pragma PAGECODE
NTSTATUS ddk_DispatchRoutine_CONTROL(IN PDEVICE_OBJECT pDevobj,IN PIRP pIrp	)
{
	//����Ӧ��IPR���д���
	pIrp->IoStatus.Information=0;//���ò������ֽ���Ϊ0��������ʵ������
	pIrp->IoStatus.Status=STATUS_SUCCESS;//���سɹ�
	IoCompleteRequest(pIrp,IO_NO_INCREMENT);//ָʾ��ɴ�IRP
	KdPrint(("�뿪��ǲ����\n"));//������Ϣ
	return STATUS_SUCCESS; //���سɹ�
}