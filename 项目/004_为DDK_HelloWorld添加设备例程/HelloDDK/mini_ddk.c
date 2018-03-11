//_stdcall
#include <ntddk.h>
#define INITCODE code_seg("INIT")//��ʼ���ڴ�
#pragma INITCODE
#define PAGEDDATA data_seg("PAGE")//��ҳ�ڴ�
VOID DDK_Unload(PDRIVER_OBJECT pdriver);
NTSTATUS CreateMyDevice(IN PDRIVER_OBJECT pDriverObject);

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING path_reg)
{
	DbgPrint("������������!++++++");
	CreateMyDevice(pDriverObject);
   pDriverObject->DriverUnload=DDK_Unload;
   return 1;
}


NTSTATUS CreateMyDevice(IN PDRIVER_OBJECT pDriverObject)
{
	UNICODE_STRING devName;
	UNICODE_STRING symLinkName;
	PDEVICE_OBJECT pDevObj;
	NTSTATUS status;
	RtlInitUnicodeString(&devName,L"\\Device\\HelloDDK");
	status=IoCreateDevice(pDriverObject,\
		           0,\
				   &devName,\
				   FILE_DEVICE_UNKNOWN,\
				   0,\
				   TRUE,\
				   &pDevObj);
	if(!NT_SUCCESS(status))
	{
		if(status==STATUS_INSUFFICIENT_RESOURCES)
		{
			DbgPrint("��Դ����");
		}
		if(status==STATUS_OBJECT_NAME_EXISTS)
		{
            DbgPrint("ָ������������");
		}
		if(status==STATUS_OBJECT_NAME_COLLISION)
		{
			DbgPrint("�������г�ͻ");
		}
		return status;
	}
   DbgPrint("�����ɹ���");
    pDevObj->Flags |= DO_BUFFERED_IO;
   //������������
   	RtlInitUnicodeString(&symLinkName,L"\\??\\zqsyr888");
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	return STATUS_SUCCESS;

}



VOID DDK_Unload(PDRIVER_OBJECT pdriver)
{
	DbgPrint("������ж����!------");

}