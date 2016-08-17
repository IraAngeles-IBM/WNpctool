
// WNpctoolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WNpctool.h"
#include "WNpctoolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT ThreadScanDevice(LPVOID lpParam)
{
	CWNpctoolDlg* pMainDlg = (CWNpctoolDlg*)lpParam;
	pMainDlg->ScanDeviceProc();
	return 0;
}
UINT ThreadBurning(LPVOID lpParam)
{
	CWNpctoolDlg* pMainDlg = (CWNpctoolDlg*)lpParam;
	pMainDlg->BurningProc();
	return 0;
}
UINT ThreadReading(LPVOID lpParam)
{
	CWNpctoolDlg* pMainDlg = (CWNpctoolDlg*)lpParam;
	pMainDlg->ReadProc();
	return 0;
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CWNpctoolDlg �Ի���




CWNpctoolDlg::CWNpctoolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWNpctoolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWNpctoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_DEVICE, m_lblDevice);
}

BEGIN_MESSAGE_MAP(CWNpctoolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_LOG_FOLDER, &CWNpctoolDlg::OnLogFolder)
	ON_BN_CLICKED(ID_BTN_WRITE, &CWNpctoolDlg::OnBnClickedBtnWrite)
END_MESSAGE_MAP()


// CWNpctoolDlg ��Ϣ�������

BOOL CWNpctoolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_strModulePath     = cmPath::GetModulePath();
	m_strLogPath = m_strModulePath + _T("Log\\");


	if ( !cmFile::IsExisted(m_strLogPath) )
	{
		CreateDirectory(m_strLogPath,NULL);
	}

	BOOL bRet;
	m_pLogObject = new cmLog(m_strLogPath,bRet);
	if (bRet)
	{
		m_pLogObject->LogFileName = _T("AppLog");
	}
	else
	{
		if (m_pLogObject)
		{
			delete m_pLogObject;
			m_pLogObject = NULL;
		}
	}


	m_bUpgradeDllInitOK = FALSE;
	m_bTerminated = FALSE;

	m_pScanEvent = NULL;
	m_pScanThread = NULL;
	m_bRun = FALSE;
	m_bDownBoot = FALSE;
	if (LoadConfig())
	{
		INIT_DEV_INFO InitDevInfo;
		INIT_LOG_INFO InitLogInfo;
		INIT_CALLBACK_INFO InitCallbackInfo;

		InitDevInfo.bScan4FsUsb = FALSE;
		InitDevInfo.emSupportDevice = 0;
		InitDevInfo.uiRockMscTimeout = 30;
		InitDevInfo.uiRockusbTimeout = 30;
		InitDevInfo.usRockMscPid = 0;
		InitDevInfo.usRockMscVid = 0;
		InitDevInfo.usRockusbPid = 0;
		InitDevInfo.usRockusbVid = 0;

		InitLogInfo.bLogEnable = TRUE;
		InitLogInfo.lpszLogPathName = (LPTSTR)(LPCTSTR)m_strLogPath;

		InitCallbackInfo.pProgressPromptProc = NULL;//you can set it to ProgressPromptProc for showing upgrade info;
		InitCallbackInfo.pUpgradeStepPromptProc = NULL;//you can set it to UpgradeStepPromptProc for showing progress info;
		m_bUpgradeDllInitOK = RK_Initialize(InitDevInfo, InitLogInfo, InitCallbackInfo);
		if (!m_bUpgradeDllInitOK)
		{
			MessageBox(_T("Initialize RKUpgrade dll failed!"),_T("ERROR"),MB_ICONERROR|MB_OK);
		}
		else
		{
			m_pScanThread = AfxBeginThread(ThreadScanDevice,(LPVOID)this);
		}

	}
	InitUi();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWNpctoolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWNpctoolDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWNpctoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID CWNpctoolDlg::InitUi()
{

}
BOOL CWNpctoolDlg::LoadConfig()
{
	//CString strConfigPath;
	//strConfigPath = m_strModulePath + _T("config.ini");
	//if (!cmFile::IsExisted(strConfigPath))
	//{
	//	if (m_pLogObject)
	//	{
	//		m_pLogObject->Record(_T("Error:LoadConfig-->config.ini PathFileExists failed."));
	//	}
	//	return FALSE;
	//}
	//bool bLoadConfig = m_Configs.LoadToolSetting(strConfigPath.GetString());
	//if (!bLoadConfig)
	//{
	//	if (m_pLogObject)
	//	{
	//		m_pLogObject->Record(_T("LoadConfig-->Load file failed"));
	//	}
	//	return FALSE;
	//}
	return TRUE;
}
void CWNpctoolDlg::OnLogFolder()
{
	// TODO: Add your command handler code here
	//ShellExecute(NULL, _T("open"), m_Configs.basic.strLogPath, NULL, NULL, SW_SHOWNORMAL);
}
void CWNpctoolDlg::ScanDeviceProc()
{
	m_pScanEvent = new CEvent(FALSE,TRUE);
	m_pScanEvent->ResetEvent();
	m_nDeviceCount = 0;
	m_bExistMsc = FALSE;
	m_bExistAdb = FALSE;
	while (!m_bTerminated)
	{
		m_csScanLock.Lock();
		RK_ScanDevice(m_nDeviceCount,m_bExistMsc,m_bExistAdb);
		m_csScanLock.Unlock();
		if (m_nDeviceCount==0)
		{
			m_bDownBoot = FALSE;
			m_lblDevice.SetWindowText(_T("Not found rockusb"));
		}
		else if (m_nDeviceCount==1)
		{
			if (m_bExistMsc)
			{
				m_lblDevice.SetWindowText(_T("Found one msc"));
			}
			else if (m_bExistAdb)
			{
				m_lblDevice.SetWindowText(_T("Found one adb"));
			}
			else
			{
				DWORD dwUsbType,dwDevType,dwLayer;
				RK_GetDeviceInfo(&dwLayer,&dwUsbType,&dwDevType,1);
				if (dwUsbType==1)
				{
					m_lblDevice.SetWindowText(_T("Found one maskrom"));
				}
				else
					m_lblDevice.SetWindowText(_T("Found one loader"));

			}
		}
		else if (m_nDeviceCount>1)
		{
			m_lblDevice.SetWindowText(_T("Found many rockusb"));
		}
		m_lblDevice.RedrawWindow();
		Sleep(200);
	}

	m_pScanEvent->SetEvent();
}

void CWNpctoolDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (m_bRun)
	{
		MessageBox(_T("Running now,please wait!"),NULL,MB_OK|MB_ICONERROR);
		return;
	}

	m_bTerminated = TRUE;
	if (m_pScanThread)
	{
		MSG msg;
		DWORD dwRet;
		while (TRUE)
		{
			dwRet = MsgWaitForMultipleObjects(1, &m_pScanEvent->m_hObject,FALSE, 10000, QS_ALLINPUT);
			if(WAIT_OBJECT_0 ==dwRet )
			{
				break;
			}
			else if( (WAIT_OBJECT_0+1)==dwRet )
			{
				while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

			}
			else if (WAIT_TIMEOUT==dwRet)
			{
				TerminateThread(m_pScanThread->m_hThread,0);
				break;
			}
		}//end while
		m_pScanThread = NULL;
		delete m_pScanEvent;
		m_pScanEvent = NULL;
	}

	if (m_pLogObject)
	{
		delete m_pLogObject;
		m_pLogObject = NULL;
	}
	if (m_bUpgradeDllInitOK)
	{
		RK_Uninitialize();
	}
	CDialog::OnClose();
}

BOOL CWNpctoolDlg::BurningProc()
{
	BOOL bSuccess=FALSE,bRet;
	LPSTR lpszData=NULL;
	BYTE readback[512];
	USHORT nReadbackSize;
	int i,nSize;
	//download boot
	if ((!m_bExistLoader)&&(!m_bDownBoot))
	{
		bRet = RK_SetFirmware((LPTSTR)(LPCTSTR)m_strLoader);
		if (!bRet)
		{
			if (m_pLogObject)
			{
				m_pLogObject->Record(_T("Error:RK_SetFirmware failed."));
			}
			goto Exit_Burn;
		}
		bRet = RK_DownloadBoot();
		if (!bRet)
		{
			if (m_pLogObject)
			{
				m_pLogObject->Record(_T("Error:RK_DownloadBoot failed."));
			}
			goto Exit_Burn;
		}
		m_bDownBoot = TRUE;
	}
	/*for (i=0;i<m_arrayDownloadItem.size();i++)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_arrayDownloadItem[i].strValue);
		if (!bRet)
		{
			if (m_pLogObject)
			{
				m_pLogObject->Record(_T("Error:convert value failed."));
			}
			goto Exit_Burn;
		}
		bRet = RK_WriteProvisioningData(m_arrayDownloadItem[i].nID,(PBYTE)lpszData,nSize);		
		if (!bRet)
		{
			if (m_pLogObject)
			{
				m_pLogObject->Record(_T("Error:RK_WriteProvisioningData failed."));
			}
			goto Exit_Burn;
		}
		memset(readback,0,512);
		bRet = RK_ReadProvisioningData(m_arrayDownloadItem[i].nID,readback,nReadbackSize);
		if (!bRet)
		{
			if (m_pLogObject)
			{
				m_pLogObject->Record(_T("Error:RK_WriteProvisioningData readback failed."));
			}
			goto Exit_Burn;
		}
		if (memcmp(readback,lpszData,nSize)!=0)
		{
			if (m_pLogObject)
			{
				m_pLogObject->Record(_T("Error:RK_WriteProvisioningData compare failed."));
			}
			goto Exit_Burn;
		}
		if (lpszData)
		{
			delete []lpszData;
			lpszData = NULL;
		}
	}*/
	bSuccess = TRUE;
Exit_Burn:
	if (lpszData)
	{
		delete []lpszData;
		lpszData = NULL;
	}
	m_bRun = FALSE;
	//EnableCtrl();
	if (bSuccess)
	{
		MessageBox(_T("Burning OK."),_T("Info"),MB_OK|MB_ICONINFORMATION);
	}
	else
		MessageBox(_T("Burning Failed."),_T("Error"),MB_OK|MB_ICONERROR);
	return bSuccess;
}
BOOL CWNpctoolDlg::ReadProc()
{
	BOOL bSuccess=FALSE,bRet;
	CString strID;
	LPWSTR lpszValue=NULL;
	int nSize,nID;
	USHORT nBufSize;
	BYTE buf[512];
	nBufSize = 512;
	//download boot
	if ((!m_bExistLoader)&&(!m_bDownBoot))
	{
		bRet = RK_SetFirmware((LPTSTR)(LPCTSTR)m_strLoader);
		if (!bRet)
		{
			if (m_pLogObject)
			{
				m_pLogObject->Record(_T("Error:RK_SetFirmware failed."));
			}
			goto Exit_Read;
		}
		bRet = RK_DownloadBoot();
		if (!bRet)
		{
			if (m_pLogObject)
			{
				m_pLogObject->Record(_T("Error:RK_DownloadBoot failed."));
			}
			goto Exit_Read;
		}
		m_bDownBoot = TRUE;
	}
	//GetDlgItemText(IDC_EDIT_ID,strID);
	if (strID.IsEmpty())
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:ID must be specified."));
		}
		goto Exit_Read;
	}
	nID = cmNumString::StrToInt32(strID);
	bRet = RK_ReadProvisioningData(nID,buf,nBufSize);
	if (!bRet)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:RK_ReadProvisioningData failed."));
		}
		goto Exit_Read;
	}
	if (nBufSize==0)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:RK_ReadProvisioningData return size 0."));
		}
		goto Exit_Read;
	}
	bRet = cmStrCode::AnsiToUnicode(lpszValue,nSize,(LPCSTR)buf);
	if (!bRet)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:convert value failed."));
		}
		goto Exit_Read;
	}
	//SetDlgItemText(IDC_EDIT_VALUE,lpszValue);
	bSuccess = TRUE;
Exit_Read:
	if (lpszValue)
	{
		delete []lpszValue;
	}
	m_bRun = FALSE;
	//EnableCtrl();
	if (bSuccess)
	{
		MessageBox(_T("Reading OK."),_T("Info"),MB_OK|MB_ICONINFORMATION);
	}
	else
		MessageBox(_T("Reading Failed."),_T("Error"),MB_OK|MB_ICONERROR);
	return bSuccess;
}
void CWNpctoolDlg::OnBnClickedBtnWrite()
{
	// TODO: Add your control notification handler code here
	
}
