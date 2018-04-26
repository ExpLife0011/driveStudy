
// loadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "load.h"
#include "loadDlg.h"
#include "afxdialogex.h"
#include <winsvc.h> 
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
BOOL LoadNTDriver(char* lpDriverName, char* lpDriverPathName);
BOOL UnLoadSys(char * szSvrName);
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
CString DriverName;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CloadDlg �Ի���



CloadDlg::CloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CloadDlg::IDD, pParent)
	, m_syspathname(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_EDIT_SYSPATHNAME, m_syspathname);
	DDX_Text(pDX, IDC_EDIT_SYSPATHNAME, m_syspathname);
}

BEGIN_MESSAGE_MAP(CloadDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOADSYS, &CloadDlg::OnBnClickedButtonLoadsys)
	ON_BN_CLICKED(IDC_BUTTON_UNLOADSYS, &CloadDlg::OnBnClickedButtonUnloadsys)
END_MESSAGE_MAP()


// CloadDlg ��Ϣ�������

BOOL CloadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CloadDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CloadDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CloadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CloadDlg::OnBnClickedButtonLoadsys()
{
	CFileDialog sysFile(true, NULL, NULL, 0, "�����ļ�sys|*.sys|�����ļ�|*.*|");
	if (IDOK == sysFile.DoModal())
	{
		m_syspathname = sysFile.GetPathName();	//��ȡ·������
		m_syspathname = sysFile.GetFileName();	//�ļ�����
		DriverName = sysFile.GetFileName();
		UpdateData(false);
		//LoadNtDriver;
		LoadNTDriver(sysFile.GetFileName().GetBuffer(256), sysFile.GetPathName().GetBuffer(256));
	}
}

BOOL LoadNTDriver(char* lpDriverName, char* lpDriverPathName)
{
	BOOL bRet = FALSE;

	SC_HANDLE hServiceMgr = NULL;//SCM�������ľ��
	SC_HANDLE hServiceDDK = NULL;//NT��������ķ�����

	//�򿪷�����ƹ�����
	hServiceMgr = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hServiceMgr == NULL)
	{
		//OpenSCManagerʧ��
		TRACE("OpenSCManager() Faild %d ! \n", GetLastError());
		bRet = FALSE;
		goto BExit;
	}
	else
	{
		////OpenSCManager�ɹ�
		TRACE("OpenSCManager() ok ! \n");
	}

	//������������Ӧ�ķ���
	hServiceDDK = CreateService(hServiceMgr,
		lpDriverName, //�����������ע����е�����  
		lpDriverName, // ע������������ DisplayName ֵ  
		SERVICE_ALL_ACCESS, // ������������ķ���Ȩ��  
		SERVICE_KERNEL_DRIVER,// ��ʾ���صķ�������������  
		SERVICE_DEMAND_START, // ע������������ Start ֵ  
		SERVICE_ERROR_IGNORE, // ע������������ ErrorControl ֵ  
		lpDriverPathName, // ע������������ ImagePath ֵ  
		NULL,
		NULL,
		NULL,
		NULL,
		NULL);

	DWORD dwRtn;
	//�жϷ����Ƿ�ʧ��
	if (hServiceDDK == NULL)
	{
		dwRtn = GetLastError();
		if (dwRtn != ERROR_IO_PENDING && dwRtn != ERROR_SERVICE_EXISTS)
		{
			//��������ԭ�򴴽�����ʧ��
			TRACE("CrateService() ʧ�� %d ! \n", dwRtn);
			bRet = FALSE;
			goto BExit;
		}
		else
		{
			//���񴴽�ʧ�ܣ������ڷ����Ѿ�������
			TRACE("CrateService() ���񴴽�ʧ�ܣ������ڷ����Ѿ������� ERROR is ERROR_IO_PENDING or ERROR_SERVICE_EXISTS! \n");
		}

		// ���������Ѿ����أ�ֻ��Ҫ��  
		hServiceDDK = OpenService(hServiceMgr, lpDriverName, SERVICE_ALL_ACCESS);
		if (hServiceDDK == NULL)
		{
			//����򿪷���Ҳʧ�ܣ�����ζ����
			dwRtn = GetLastError();
			TRACE("OpenService() ʧ�� %d ! \n", dwRtn);
			bRet = FALSE;
			goto BExit;
		}
		else
		{
			TRACE("OpenService() �ɹ� ! \n");
		}
	}
	else
	{
		TRACE("CrateService() �ɹ� ! \n");
	}

	//�����������
	bRet = StartService(hServiceDDK, NULL, NULL);
	if (!bRet)  //�������񲻳ɹ�
	{
		TRACE("StartService() ʧ�� ��������Ѿ�����%d ! \n", dwRtn);
	}
	bRet = TRUE;
	//�뿪ǰ�رվ��
BExit:
	if (hServiceDDK)
	{
		CloseServiceHandle(hServiceDDK);
	}
	if (hServiceMgr)
	{
		CloseServiceHandle(hServiceMgr);
	}
	return bRet;
}

//ж�ذ�ť
void CloadDlg::OnBnClickedButtonUnloadsys()
{
	UnLoadSys(DriverName.GetBuffer(256));
}

//ж����������  
BOOL UnLoadSys(char * szSvrName)
{
	//һ�������õ��ı���
	BOOL bRet = FALSE;
	SC_HANDLE hSCM = NULL;//SCM�������ľ��,�������OpenSCManager�ķ���ֵ
	SC_HANDLE hService = NULL;//NT��������ķ��������������OpenService�ķ���ֵ
	SERVICE_STATUS SvrSta;
	//����SCM������
	hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		//����SCM������ʧ��
		TRACE("OpenSCManager() Faild %d ! \n", GetLastError());
		bRet = FALSE;
		goto BeforeLeave;
	}
	else
	{
		//��SCM�������ɹ�
		TRACE("OpenSCManager() ok ! \n");
	}
	//������������Ӧ�ķ���
	hService = OpenService(hSCM, szSvrName, SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		//����������Ӧ�ķ���ʧ�� �˳�
		TRACE("OpenService() Faild %d ! \n", GetLastError());
		bRet = FALSE;
		goto BeforeLeave;
	}
	else
	{
		TRACE("OpenService() ok ! \n");  //����������Ӧ�ķ��� �ɹ�
	}
	//��ֹͣ�����������ֹͣʧ�ܣ�ֻ�������������ܣ��ٶ�̬���ء�  
	if (!ControlService(hService, SERVICE_CONTROL_STOP, &SvrSta))
	{
		TRACE("��ControlService() ֹͣ��������ʧ�� �����:%d !\n", GetLastError());
	}
	else
	{
		//ֹͣ��������ɹ�
		TRACE("��ControlService() ֹͣ��������ɹ� !\n");
	}
	//�嶯̬ж����������  
	if (!DeleteService(hService))  //TRUE//FALSE
	{
		//ж��ʧ��
		TRACE("ж��ʧ��:DeleteSrevice()�����:%d !\n", GetLastError());
	}
	else
	{
		//ж�سɹ�
		TRACE("ж�سɹ� !\n");

	}
	bRet = TRUE;
	//�� �뿪ǰ�رմ򿪵ľ��
BeforeLeave:
	if (hService > 0)
	{
		CloseServiceHandle(hService);
	}
	if (hSCM > 0)
	{
		CloseServiceHandle(hSCM);
	}
	return bRet;
}

