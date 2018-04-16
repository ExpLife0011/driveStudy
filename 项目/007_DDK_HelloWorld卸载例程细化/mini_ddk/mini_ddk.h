#include <ntddk.h>
#define INITCODE code_seg("INIT") 
#define PAGECODE code_seg("PAGE") /*��ʾ�ڴ治��ʱ�����Ա��û���Ӳ��*/
#pragma INITCODE /*ָ�Ĵ������к� �ʹ��ڴ��ͷŵ�*/
NTSTATUS CreateMyDevice (IN PDRIVER_OBJECT pDriverObject) 
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;/*�������ش����豸*/

	//�����豸����
	UNICODE_STRING devName;
	UNICODE_STRING symLinkName; // 
	RtlInitUnicodeString(&devName,L"\\Device\\yjxDDK_Device");/*��devName��ʼ���ִ�Ϊ "\\Device\\yjxDDK_Device"*/

	//�����豸
	status = IoCreateDevice( pDriverObject,\
		0,\
		&devName,\
		FILE_DEVICE_UNKNOWN,\
		0, TRUE,\
		&pDevObj);
	if (!NT_SUCCESS(status))
	{
		if (status==STATUS_INSUFFICIENT_RESOURCES)
		{
			KdPrint(("��Դ���� STATUS_INSUFFICIENT_RESOURCES"));
		}
		if (status==STATUS_OBJECT_NAME_EXISTS )
		{
			KdPrint(("ָ������������"));
		}
		if (status==STATUS_OBJECT_NAME_COLLISION)
		{
			KdPrint(("//�������г�ͻ"));
		}
		KdPrint(("�豸����ʧ��...++++++++"));
		return status;
	}
	KdPrint(("�豸�����ɹ�...++++++++"));

	pDevObj->Flags |= DO_BUFFERED_IO;
	//������������

	RtlInitUnicodeString(&symLinkName,L"\\??\\yjx888");
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	if (!NT_SUCCESS(status)) /*status����0*/
	{
		IoDeleteDevice( pDevObj );
		return status;
	}
	return STATUS_SUCCESS;
}
#pragma  INITCODE
VOID DDK_Unload (IN PDRIVER_OBJECT pDriverObject); //ǰ��˵�� ж������