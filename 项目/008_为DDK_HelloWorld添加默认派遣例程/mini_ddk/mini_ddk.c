//_stdcall
#include "mini_ddk.h"
#pragma  INITCODE
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING B) //TYPEDEF LONG NTSTATUS
{
KdPrint(("�����ɹ�������...OK++++++++"));
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