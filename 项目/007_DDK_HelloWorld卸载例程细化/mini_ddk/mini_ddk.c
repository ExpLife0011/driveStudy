//_stdcall
#include "mini_ddk.h"
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject,PUNICODE_STRING B) //TYPEDEF LONG NTSTATUS
{
KdPrint(("�����ɹ�������...OK++++++++"));
//jmpָ��
 CreateMyDevice(pDriverObject);//������Ӧ���豸
 pDriverObject->DriverUnload=DDK_Unload;
return (1);
}
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