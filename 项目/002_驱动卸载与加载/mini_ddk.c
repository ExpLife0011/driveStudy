//_stdcall
#include <ntddk.h>
#define INITCODE code_seg("INIT")//��ʼ���ڴ�
#pragma INITCODE
#define PAGEDDATA data_seg("PAGE")//��ҳ�ڴ�
VOID DDK_Unload(PDRIVER_OBJECT pdriver);



int DriverEntry(PDRIVER_OBJECT pdriver, PUNICODE_STRING path_reg)
{
	DbgPrint("������������!++++++");
   pdriver->DriverUnload=DDK_Unload;
   return 1;
}
VOID DDK_Unload(PDRIVER_OBJECT pdriver)
{
	DbgPrint("������ж����!------");

}
