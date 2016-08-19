
// WNpctoolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WNpctool.h"
#include "WNpctoolDlg.h"
#include "cmfuns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT ThreadScanDevice(LPVOID lpParam)
{
	CWNpctoolDlg* pMainDlg = (CWNpctoolDlg*)lpParam;
	pMainDlg->ScanDeviceProc();
	return 0;
}
UINT ThreadWrite(LPVOID lpParam)
{
	CWNpctoolDlg* pMainDlg = (CWNpctoolDlg*)lpParam;
	pMainDlg->WriteProc();
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
	: CDialog(CWNpctoolDlg::IDD, pParent),m_ConfigModeDlg(m_Configs),
	m_bStarWrite(FALSE),m_bStarRead(FALSE),m_pWriteThread(NULL),m_pReadThread(NULL)
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
	ON_COMMAND(ID_SETTING_MODE, &CWNpctoolDlg::OnSettingMode)
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
	m_pScanEvent        = NULL;
	m_pScanThread       = NULL;
	m_bUserStop         = FALSE;
	m_pWriteThread      = NULL;
	m_pReadThread       = NULL;

	m_ConfigModeDlg.Create(IDD_DIALOG_MODE);

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

	} else {
		MessageBox(_T("Loading config file failed!"),_T("ERROR"),MB_ICONERROR|MB_OK);
		exit(-1);
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
	GetDlgItem(IDC_EDIT_SN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WIFIMAC)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BTMAC)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LANMAC)->EnableWindow(FALSE);
	GetDlgItem(ID_BTN_WRITE)->SetWindowText(_T("��ȡ"));

	if (!m_Configs.bReadInfo)
	{
		GetDlgItem(ID_BTN_WRITE)->SetWindowText(_T("д��"));
		if (m_Configs.devsn.bEnable&&m_Configs.devsn.nAutoMode == MODE_MANUAL)
		{
			GetDlgItem(IDC_EDIT_SN)->EnableWindow(TRUE);
		}

		if (m_Configs.WifiMac.bEnable&&m_Configs.WifiMac.nAutoMode == MODE_MANUAL)
		{
			GetDlgItem(IDC_EDIT_WIFIMAC)->EnableWindow(TRUE);
		}

		if (m_Configs.BtMac.bEnable&&m_Configs.BtMac.nAutoMode == MODE_MANUAL)
		{
			GetDlgItem(IDC_EDIT_BTMAC)->EnableWindow(TRUE);
		}

		if (m_Configs.LanMac.bEnable&&m_Configs.LanMac.nAutoMode == MODE_MANUAL)
		{
			GetDlgItem(IDC_EDIT_LANMAC)->EnableWindow(TRUE);
		}
	}
}

BOOL CWNpctoolDlg::LoadConfig()
{
	CString strConfigPath;
	strConfigPath = m_strModulePath + _T("config.ini");
	if (!cmFile::IsExisted(strConfigPath))
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:LoadConfig-->config.ini PathFileExists failed."));
		}
		return FALSE;
	}
	bool bLoadConfig = m_Configs.LoadToolSetting(strConfigPath.GetString());
	if (!bLoadConfig)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("LoadConfig-->Load file failed"));
		}
		return FALSE;
	}
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

BOOL CWNpctoolDlg::WriteProc()
{
	BOOL bSuccess=FALSE;

	//1.��дdevsn,��д�������������ֵ��д��ȥ��ֵһ����ɹ�������ʧ��
	if (m_Configs.devsn.bEnable)
	{
		if(!WriteItem(ITEM_SN))
		{
			m_pLogObject->Record(_T("Write SN failed."));
		}
	}
	//2.��дwifimac
	if (m_Configs.WifiMac.bEnable)
	{
		if(!WriteItem(ITEM_WIFIMAC))
		{
			m_pLogObject->Record(_T("Write WifiMac failed."));
		}
	}
	//3.��дbtmac
	if (m_Configs.BtMac.bEnable)
	{
		if(!WriteItem(ITEM_BTMAC))
		{
			m_pLogObject->Record(_T("Write BtMac failed."));
		}
	}
	//4.��дlanmac
	if (m_Configs.LanMac.bEnable)
	{
		if(!WriteItem(ITEM_LANMAC))
		{
			m_pLogObject->Record(_T("Write LanMac failed."));
		}
	}
	bSuccess = TRUE;
WriteProc_Exit:
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
BOOL CWNpctoolDlg::WriteItem(int nItemID)
{
	BOOL bRet;
	LPSTR lpszData=NULL;
	BYTE readback[512];
	USHORT nReadbackSize;
	int nSize;

	//1.��дdevsn,��д�������������ֵ��д��ȥ��ֵһ����ɹ�������ʧ��
	//��дdevsn
	if (ITEM_SN == nItemID)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurDevSn);
	}else if (ITEM_WIFIMAC == nItemID)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurWifiMac);
	}else if (ITEM_BTMAC == nItemID)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurBtMac);
	}else if (ITEM_LANMAC == nItemID)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurLanMac);
	}
	else
	{

	}
	if (!bRet)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:convert value failed."));
		}
		return FALSE;
	}
	bRet = RK_WriteProvisioningData(nItemID,(PBYTE)lpszData,nSize);		
	if (!bRet)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:RK_WriteProvisioningData failed."));
		}
		return FALSE;
	}
	//��ȡdevsn
	memset(readback,0,512);
	bRet = RK_ReadProvisioningData(nItemID,readback,nReadbackSize);
	if (!bRet)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:RK_WriteProvisioningData readback failed."));
		}
		return FALSE;
	}
	if (memcmp(readback,lpszData,nSize)!=0)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:RK_WriteProvisioningData compare failed."));
		}
		return FALSE;
	}
	if (lpszData)
	{
		delete []lpszData;
		lpszData = NULL;
	}
	return TRUE;
}
BOOL CWNpctoolDlg::ReadItem(int nItemID)
{
	BOOL	bRet;
	CString strID;
	LPWSTR lpszValue=NULL;
	int nSize,nID;
	USHORT nBufSize;
	BYTE buf[512];
	nBufSize = 512;
	bRet = RK_ReadProvisioningData(nItemID,buf,nBufSize);
	if (!bRet)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:RK_ReadProvisioningData failed."));
		}
		return FALSE;
	}
	if (nBufSize==0)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:RK_ReadProvisioningData return size 0."));
		}
		return FALSE;
	}
	bRet = cmStrCode::AnsiToUnicode(lpszValue,nSize,(LPCSTR)buf);
	if (!bRet)
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:convert value failed."));
		}
		return FALSE;
	}
	if (ITEM_SN == nItemID)
	{
		SetDlgItemText(IDC_EDIT_SN,lpszValue);
	}else if (ITEM_WIFIMAC == nItemID)
	{
		SetDlgItemText(IDC_EDIT_WIFIMAC,lpszValue);
	}else if (ITEM_BTMAC == nItemID)
	{
		SetDlgItemText(IDC_EDIT_BTMAC,lpszValue);
	}else if (ITEM_LANMAC == nItemID)
	{
		SetDlgItemText(IDC_EDIT_LANMAC,lpszValue);
	}
	else
	{

	}

	if (lpszValue)
	{
		delete []lpszValue;
	}
	return TRUE;
}
BOOL CWNpctoolDlg::ReadProc()
{
	BOOL bSuccess=FALSE;
	
	if (!ReadItem(ITEM_SN))
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:Read SN failed."));
		}
		goto Exit_Read;
	}

	if (!ReadItem(ITEM_WIFIMAC))
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:Read WifiMac failed."));
		}
		goto Exit_Read;
	}

	if (!ReadItem(ITEM_BTMAC))
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:Read BtMac failed."));
		}
		goto Exit_Read;
	}

	if (!ReadItem(ITEM_LANMAC))
	{
		if (m_pLogObject)
		{
			m_pLogObject->Record(_T("Error:Read LanMac failed."));
		}
		goto Exit_Read;
	}
	bSuccess = TRUE;
Exit_Read:
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
	if(m_Configs.bReadInfo) {
		if(m_pReadThread) { 
			m_bUserStop     = TRUE;
			SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("STOPING")).c_str());
		} else {
			if(OnStartRead()) {
				SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("STOP")).c_str());
			}
		}
	} else {
		if(m_pWriteThread) { 
			m_bUserStop     = TRUE;
			SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("STOPING")).c_str());
		} else {
			if(OnStartWrite()) {
				SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("STOP")).c_str());
			}
		}
	}
}
BOOL CWNpctoolDlg::OnStartRead()
{
	CString     strPromt;
	CString strText;

	m_csScanLock.Lock();
	if (m_nDeviceCount!=1)
	{
		m_csScanLock.Unlock();
		strPromt = _T("Not found rockusb to burn.");
		//MessageBox(_T("Not found rockusb to burn."),_T("Error"),MB_OK|MB_ICONERROR);
		goto OnStartReadExit;
	}
	m_lblDevice.GetWindowText(strText);
	if (strText.Find(_T("loader"))!=-1)
	{
		m_bExistLoader = TRUE;
	}
	else 
		m_bExistLoader = FALSE;
	m_csScanLock.Unlock();

	m_bRun = TRUE;
	AfxBeginThread(ThreadReading,(LPVOID)this);

	return TRUE;
OnStartReadExit:
	if(!strPromt.IsEmpty()) {
		MessageBox(strPromt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_OK|MB_ICONERROR);
	}
	return FALSE;
}
BOOL CWNpctoolDlg::OnStartWrite()
{
	CString     strPromt;
	CString		strText;

	m_csScanLock.Lock();
	if (m_nDeviceCount!=1)
	{
		m_csScanLock.Unlock();
		strPromt = _T("Not found rockusb to burn.");
		//MessageBox(_T("Not found rockusb to burn."),_T("Error"),MB_OK|MB_ICONERROR);
		goto OnStartWriteExit;
	}
	m_lblDevice.GetWindowText(strText);
	if (strText.Find(_T("loader"))!=-1)
	{
		m_bExistLoader = TRUE;
	}
	else 
		m_bExistLoader = FALSE;
	m_csScanLock.Unlock();
	if (!(m_Configs.devsn.bEnable||m_Configs.WifiMac.bEnable||m_Configs.BtMac.bEnable||m_Configs.LanMac.bEnable))
	{
		strPromt = GetLocalString(_T("IDS_ERROR_NO_WRITE")).c_str();
		//MessageBox(GetLocalString(_T("IDS_ERROR_NO_WRITE")).c_str(),_T("Error"),MB_OK|MB_ICONERROR);
		goto OnStartWriteExit;
	}

	if (m_Configs.devsn.bEnable)
	{
		if (MODE_MANUAL == m_Configs.devsn.nAutoMode)
		{
			GetDlgItemText(IDC_EDIT_SN,m_strCurDevSn);
		}
		else if (MODE_AUTO == m_Configs.devsn.nAutoMode)
		{
			m_strCurDevSn = m_Configs.devsn.strCurrentSn.c_str();
			SetDlgItemText(IDC_EDIT_SN,m_strCurDevSn);
		}
		else
		{

		}
	}

	if (m_Configs.WifiMac.bEnable)
	{
		if (MODE_MANUAL == m_Configs.WifiMac.nAutoMode)
		{
			GetDlgItemText(IDC_EDIT_WIFIMAC,m_strCurWifiMac);
		}
		else if (MODE_AUTO == m_Configs.devsn.nAutoMode)
		{
			m_strCurWifiMac = m_Configs.WifiMac.strCurrentMac.c_str();
			SetDlgItemText(IDC_EDIT_WIFIMAC,m_strCurWifiMac);
			if (m_strCurWifiMac.IsEmpty()||m_Configs.WifiMac.nRemainCount==0) {
				strPromt = GetLocalString(_T("IDS_ERROR_OUT_OF_BTMAC")).c_str();
				goto OnStartWriteExit;
			}
		}
		else
		{

		}
		if(!CheckMacStr(m_strCurWifiMac)) 
		{
			strPromt.Format(GetLocalString(_T("IDS_ERROR_WIFIMAC_FROM_FILE")).c_str(),m_strCurWifiMac);
			goto OnStartWriteExit;
		}
	}

	if (m_Configs.BtMac.bEnable)
	{
		if (MODE_MANUAL == m_Configs.BtMac.nAutoMode)
		{
			GetDlgItemText(IDC_EDIT_BTMAC,m_strCurBtMac);
		}
		else if (MODE_AUTO == m_Configs.BtMac.nAutoMode)
		{
			m_strCurBtMac = m_Configs.BtMac.strCurrentMac.c_str();
			SetDlgItemText(IDC_EDIT_BTMAC,m_strCurBtMac);
			if (m_strCurBtMac.IsEmpty()||m_Configs.BtMac.nRemainCount==0) {
				strPromt = GetLocalString(_T("IDS_ERROR_OUT_OF_BTMAC")).c_str();
				goto OnStartWriteExit;
			}
		}
		else
		{

		}
		if(!CheckMacStr(m_strCurBtMac)) 
		{
			strPromt.Format(GetLocalString(_T("IDS_ERROR_WIFIMAC_FROM_FILE")).c_str(),m_strCurBtMac);
			goto OnStartWriteExit;
		}
	}

	if (m_Configs.LanMac.bEnable)
	{
		if (MODE_MANUAL == m_Configs.LanMac.nAutoMode)
		{
			GetDlgItemText(IDC_EDIT_LANMAC,m_strCurLanMac);
		}
		else if (MODE_AUTO == m_Configs.LanMac.nAutoMode)
		{
			m_strCurLanMac = m_Configs.LanMac.strCurrentMac.c_str();
			SetDlgItemText(IDC_EDIT_LANMAC,m_strCurLanMac);
			if (m_strCurLanMac.IsEmpty()||m_Configs.LanMac.nRemainCount==0) {
				strPromt = GetLocalString(_T("IDS_ERROR_OUT_OF_BTMAC")).c_str();
				goto OnStartWriteExit;
			}
		}
		else
		{

		}
		if(!CheckMacStr(m_strCurLanMac)) 
		{
			strPromt.Format(GetLocalString(_T("IDS_ERROR_WIFIMAC_FROM_FILE")).c_str(),m_strCurLanMac);
			goto OnStartWriteExit;
		}
	}
	m_bRun = TRUE;
	AfxBeginThread(ThreadWrite,(LPVOID)this);

	return TRUE;
OnStartWriteExit:
	if(!strPromt.IsEmpty()) {
		MessageBox(strPromt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_OK|MB_ICONERROR);
	}
	return FALSE;
}
void CWNpctoolDlg::OnSettingMode()
{
	// TODO: Add your command handler code here
	m_ConfigModeDlg.UpdateInterface();
	m_ConfigModeDlg.ShowWindow(SW_SHOW);
}
