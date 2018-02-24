
// WNpctoolDlg.cpp : 实现文件
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
static int hexchar2value(const char ch)
{
	int result = 0;
	if(ch >= '0' && ch <= '9') {
		result = (int)(ch - '0');
	} else if(ch >= 'a' && ch <= 'z') {
		result = (int)(ch - 'a') + 10;
	} else if(ch >= 'A' && ch <= 'Z') {
		result = (int)(ch - 'A') + 10;
	} else{
		result = -1;
	}
	return result;
}

static char value2hexchar(const int value)
{
	char result = 0;
	if(value >= 0 && value <= 9) {
		result = (char)(value + '0');
	} else if(value >= 10 && value <= 15) {
		result = (char)(value - 10 + 'A');
	}
	return result;
}
// Convert wstring to string
static std::string wstr2str(const std::wstring& arg)
{
	int requiredSize;
	requiredSize = WideCharToMultiByte(CP_ACP,0,arg.c_str(),arg.length(),NULL,0,NULL,NULL);
	std::string res;
	if (requiredSize<=0) {
		res = "";
		return res;
	}
	res.assign(requiredSize,'\0');
	WideCharToMultiByte(CP_ACP,0,arg.c_str(),arg.length(),const_cast<char*>(res.data()),requiredSize,NULL,NULL);
	return res;
}
// Convert string to wstring
static std::wstring str2wstr(const std::string& arg)
{
	int requiredSize;
	requiredSize = MultiByteToWideChar(CP_ACP,0,arg.c_str(),arg.length(),NULL,0);
	std::wstring res;
	if (requiredSize<=0) {
		res = L"";
		return res;
	}
	res.assign(requiredSize,L'\0');
	MultiByteToWideChar(CP_ACP,0,arg.c_str(),arg.length(),const_cast<wchar_t *>(res.data()),requiredSize);
	return res;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CWNpctoolDlg 对话框




CWNpctoolDlg::CWNpctoolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWNpctoolDlg::IDD, pParent)/*,m_ConfigModeDlg(m_Configs,m_LocalLan)*/,
	m_bStarWrite(FALSE),m_bStarRead(FALSE),m_pWriteThread(NULL),m_pReadThread(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_WNFRAME);
}

void CWNpctoolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_DEVICE, m_lblDevice);
	DDX_Control(pDX, IDC_LIST_REPORT, m_listInfo);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_lblPrompt);
}

BEGIN_MESSAGE_MAP(CWNpctoolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_UPDATE_MSG,&CWNpctoolDlg::OnHandleUpdateMsg)
	ON_COMMAND(ID_LOG_FOLDER, &CWNpctoolDlg::OnLogFolder)
	ON_BN_CLICKED(ID_BTN_WRITE, &CWNpctoolDlg::OnBnClickedBtnWrite)
	ON_COMMAND(ID_SETTING_MODE, &CWNpctoolDlg::OnSettingMode)
	ON_UPDATE_COMMAND_UI(ID_SETTING_READ, &CWNpctoolDlg::OnUpdateSettingRead)
	ON_COMMAND(ID_SETTING_READ, &CWNpctoolDlg::OnSettingRead)
	ON_COMMAND(ID_LANGUAGE_CHINESE, &CWNpctoolDlg::OnLanguageChinese)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE_CHINESE, &CWNpctoolDlg::OnUpdateLanguageChinese)
	ON_COMMAND(ID_LANGUAGE_ENGLISH, &CWNpctoolDlg::OnLanguageEnglish)
	ON_UPDATE_COMMAND_UI(ID_LANGUAGE_ENGLISH, &CWNpctoolDlg::OnUpdateLanguageEnglish)
	ON_COMMAND(ID_SETTING_AUTO, &CWNpctoolDlg::OnSettingAuto)
	ON_UPDATE_COMMAND_UI(ID_SETTING_AUTO, &CWNpctoolDlg::OnUpdateSettingAuto)
	ON_BN_CLICKED(IDC_BUTTON_LOADER, &CWNpctoolDlg::OnBnClickedButtonLoader)
	ON_COMMAND(ID_SETTING_RESET, &CWNpctoolDlg::OnSettingReset)
	ON_UPDATE_COMMAND_UI(ID_SETTING_RESET, &CWNpctoolDlg::OnUpdateSettingReset)
END_MESSAGE_MAP()


// CWNpctoolDlg 消息处理程序

BOOL CWNpctoolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_pScanEvent        = NULL;
	m_pScanThread       = NULL;
	m_bUserStop         = FALSE;
	m_pWriteThread      = NULL;
	m_pReadThread       = NULL;

	//m_ConfigModeDlg.Create(IDD_DIALOG_MODE);

	m_bRedLedLight      = TRUE;
	m_hGreenLedBitmap   = m_hRedLedBitmap = NULL;
	m_hRedLedBitmap     = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BMP_REDLED));
	m_hGreenLedBitmap   = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BMP_GREENLED));
	m_strModulePath     = cmPath::GetModulePath();

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
		if (m_Configs.strLogPath.empty())
		{
			m_Configs.strLogPath=m_strModulePath + _T("Log\\");
			m_strLogPath = m_Configs.strLogPath.c_str();
		}
		else
		{
			m_strLogPath = m_Configs.strLogPath.c_str();
		}
		if ( !cmFile::IsExisted(m_strLogPath) )
		{
			CreateDirectory(m_strLogPath,NULL);
		}

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
		UpdateMenuItem();

	} else {
		MessageBox(_T("Loading config file failed!"),_T("ERROR"),MB_ICONERROR|MB_OK);
		exit(-1);
	}

	CString strTitle;
	GetWindowText(strTitle);
	if (m_Configs.bReadInfo)
	{
		GetDlgItem(ID_BTN_WRITE)->SetWindowText(GetLocalString(_T("IDS_TEXT_READ_BUTTON")).c_str());
	}
	else
	{
		GetDlgItem(ID_BTN_WRITE)->SetWindowText(GetLocalString(_T("IDS_TEXT_WRITE_BUTTON")).c_str());
	}
	
	if(m_Configs.nLogLevel != DLEVEL_NONE ) {
		CLogger::DEBUG_LEVEL level = m_Configs.nLogLevel == DLEVEL_DEBUG?CLogger::DEBUG_ALL:
			(m_Configs.nLogLevel  == DLEVEL_INFO ?CLogger::DEBUG_INFO:CLogger::DEBUG_ERROR);
		m_pLog = CLogger::StartLog((m_Configs.strLogPath + CLogger::TimeStr(true, true)).c_str(), level);    
	}
	LDEGMSGW((CLogger::DEBUG_INFO, _T("%s start run"),(LPCTSTR)strTitle));
	InitUi();


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWNpctoolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // device context for painting
		HBITMAP hLedBitmap;
		if (m_bRedLedLight)
		{
			hLedBitmap = m_hRedLedBitmap;
		}
		else
			hLedBitmap = m_hGreenLedBitmap;
		// TODO: Add your message handler code here
		if (hLedBitmap)
		{
			CPaintDC dc(GetDlgItem(IDC_PICTURE_DEVICE));
			CDC ImageDC;
			ImageDC.CreateCompatibleDC(&dc);
			HGDIOBJ hOldGdiObject;
			hOldGdiObject = ImageDC.SelectObject(hLedBitmap);
			int nDstWidth,nDstHeight;
			RECT dstClientRect;
			dc.GetWindow()->GetClientRect(&dstClientRect);
			nDstHeight = dstClientRect.bottom - dstClientRect.top;
			nDstWidth = dstClientRect.right-dstClientRect.left;
			BITMAP bmpStruct;
			BOOL bRet;
			GetObject(hLedBitmap,sizeof(BITMAP),&bmpStruct);

			bRet = TransparentBlt(dc.m_hDC, 0, 0, nDstWidth, nDstHeight, ImageDC.m_hDC, 0, 0, bmpStruct.bmWidth, bmpStruct.bmHeight, RGB(255,255,255));
			ImageDC.SelectObject(hOldGdiObject);
		}

		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWNpctoolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

VOID CWNpctoolDlg::InitUi()
{
	CFont font;

	SetDlgItemText(IDC_EDIT_SN,_T(""));
	SetDlgItemText(IDC_EDIT_WIFIMAC,_T(""));
	SetDlgItemText(IDC_EDIT_BTMAC,_T(""));
	SetDlgItemText(IDC_EDIT_LANMAC,_T(""));
	SetDlgItemText(IDC_EDIT_IMEI,_T(""));
	GetDlgItem(IDC_EDIT_SN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WIFIMAC)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BTMAC)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LANMAC)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_IMEI)->EnableWindow(FALSE);
	//GetDlgItem(IDC_EDIT_LOADER)->EnableWindow(FALSE);
	//GetDlgItem(ID_BTN_WRITE)->SetWindowText(GetLocalString(_T("IDS_TEXT_READ_BUTTON")).c_str());

	if (!m_Configs.bReadInfo)
	{
		//GetDlgItem(ID_BTN_WRITE)->SetWindowText(GetLocalString(_T("IDS_TEXT_WRITE_BUTTON")).c_str());
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

		if (m_Configs.Imei.bEnable&&m_Configs.Imei.nAutoMode == MODE_MANUAL)
		{
			GetDlgItem(IDC_EDIT_IMEI)->EnableWindow(TRUE);
		}
	}

	m_lblPrompt.SetFontStatic(_T("宋体"),50,RGB(0,0,0),FS_BOLD|FS_CENTER|FS_VCENTER);
	PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_EMPTY);
	//list
	font.CreateFont(-15,10,0,0,FW_NORMAL,FALSE,FALSE,0,  
		ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,  
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_T("宋体"));
	m_listInfo.SetFont(&font);
	font.Detach();
	m_listInfo.SetWindowBKColor(RGB(0,0,0));
}

BOOL CWNpctoolDlg::LoadConfig()
{
	CString strConfigPath;
	strConfigPath = m_strModulePath + _T("config.ini");
	if (!cmFile::IsExisted(strConfigPath))
	{
		return FALSE;
	}
	bool bLoadConfig = m_Configs.LoadToolSetting(strConfigPath.GetString());
	if (!bLoadConfig)
	{
		return FALSE;
	}
	strConfigPath = m_strModulePath + m_Configs.strLanPath.c_str();
	if (m_Configs.nCurLan == 1)
	{
		strConfigPath = strConfigPath + m_Configs.strCnFilename.c_str();
	}
	else
	{
		strConfigPath = strConfigPath + m_Configs.strEnFilename.c_str();
	}
	if (!cmFile::IsExisted(strConfigPath))
	{
		return FALSE;
	}
	bLoadConfig = m_LocalLan.LoadToolSetting(strConfigPath.GetString());
	m_LocalLan.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,true);
	if (!bLoadConfig)
	{
		return FALSE;
	}
	if(!m_Configs.bDebug) {
		WalkMenu(GetMenu(),TEXT("MENU"));
	}
	GetLocalString(TEXT("LANG:IDS_TEXT_APPNAME"));
	return TRUE;
}
VOID CWNpctoolDlg::WalkMenu(CMenu *pMenu,CString strMainKeyPart)
{
	CString strKey;
	int     id ;
	UINT    i;
	if(NULL == pMenu) return ;
	for (i=0;i < pMenu->GetMenuItemCount();i++) {
		strKey.Format(_T("%s_%d"),strMainKeyPart,i);
		id = pMenu->GetMenuItemID(i);
		if (0 == id) { 
			/*If nPos corresponds to a SEPARATOR menu item, the return value is 0. **/
		} else if (-1 == id) { 
			/*If the specified item is a pop-up menu (as opposed to an item within the pop-up menu), the return value is –1 **/
			pMenu->ModifyMenu(i, MF_BYPOSITION, i, m_LocalLan.GetStr((LPCTSTR)strKey).c_str());
			WalkMenu(pMenu->GetSubMenu(i),strKey);
		} else {
			pMenu->ModifyMenu(id, MF_BYCOMMAND, id,m_LocalLan.GetStr((LPCTSTR)strKey).c_str());
		}
	}
}
void CWNpctoolDlg::OnLogFolder()
{
	// TODO: Add your command handler code here
	ShellExecute(NULL, _T("open"), m_Configs.strLogPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
void CWNpctoolDlg::ScanDeviceProc()
{
	UINT nDeviceCount = 0;
	bool bSendMsg = FALSE;
	m_pScanEvent = new CEvent(FALSE,TRUE);
	m_pScanEvent->ResetEvent();
	m_nDeviceCount = 0;
	m_bExistMsc = FALSE;
	m_bExistAdb = FALSE;
	while (!m_bTerminated)
	{
		nDeviceCount = 0;
		bSendMsg = FALSE;
		m_csScanLock.Lock();
		RK_ScanDevice(nDeviceCount,m_bExistMsc,m_bExistAdb);
		if (m_nDeviceCount != nDeviceCount)
		{
			m_nDeviceCount = nDeviceCount;
			bSendMsg = TRUE;

		}
		m_csScanLock.Unlock();
		if (m_nDeviceCount==0)
		{
			m_bRedLedLight = TRUE;
			m_bDownBoot = FALSE;
			m_lblDevice.SetWindowText(GetLocalString(_T("IDS_INFO_DEVICE_ON")).c_str());
		}
		else if (m_nDeviceCount==1)
		{
			m_bRedLedLight = FALSE;
			if (m_bExistMsc)
			{
				m_lblDevice.SetWindowText(GetLocalString(_T("IDS_INFO_DEVICE_MSC")).c_str());
			}
			else if (m_bExistAdb)
			{
				m_lblDevice.SetWindowText(GetLocalString(_T("IDS_INFO_DEVICE_ADB")).c_str());
			}
			else
			{
				DWORD dwUsbType,dwDevType,dwLayer;
				RK_GetDeviceInfo(&dwLayer,&dwUsbType,&dwDevType,1);
				if (dwUsbType==1)
				{
					m_lblDevice.SetWindowText(GetLocalString(_T("IDS_INFO_DEVICE_MASKROM")).c_str());
				}
				else
					m_lblDevice.SetWindowText(GetLocalString(_T("IDS_INFO_DEVICE_LOADER")).c_str());

			}
		}
		else if (m_nDeviceCount>1)
		{
			m_bRedLedLight = FALSE;
			m_lblDevice.SetWindowText(GetLocalString(_T("IDS_INFO_DEVICE_MANY")).c_str());
		}
		m_lblDevice.RedrawWindow();
		if (bSendMsg)
		{
			if (m_nDeviceCount > 0)
			{
				if (m_Configs.bAutoTest&&m_bRun) 
				{
					if (m_Configs.bReadInfo)
					{
					}
					else
					{
						OnStartWrite(TRUE);
					}
				}
			}
			//PostMessage(WM_UPDATE_MSG,UPDATE_LIST,LIST_EMPTY);
			PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_EMPTY);
			PostMessage(WM_UPDATE_MSG,UPDATE_WINDOW,0);
		}
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
	if (m_pLog)
	{
		delete m_pLog;
		m_pLog = NULL;
	}
	if (m_bUpgradeDllInitOK)
	{
		RK_Uninitialize();
	}
	if (m_hRedLedBitmap) {
		DeleteObject(m_hRedLedBitmap);
		m_hRedLedBitmap = NULL;
	}

	if (m_hGreenLedBitmap) {
		DeleteObject(m_hGreenLedBitmap);
		m_hGreenLedBitmap = NULL;
	}

	m_Configs.SaveToolSetting(std::wstring(_T("")));
	CDialog::OnClose();
}
CString CWNpctoolDlg::BytesToHexStr(PBYTE pBuf,int nBufLen)
{
	CString strRet = _T("");
	CString strValue;
	int i;
	for (i=0;i<nBufLen;i++)
	{
		strValue = cmNumString::NumToStr(pBuf[i],16);
		if (strValue.GetLength()==1)
		{
			strValue = _T("0")+strValue;
		}
		else if (strValue.GetLength()>2)
		{
			strValue = strValue.Left(2);
		}
		strRet = strRet + strValue;
	}
	strRet.MakeUpper();
	return strRet;
}
BOOL CWNpctoolDlg::HexStrToBytes(CString strHex,PBYTE pBuf,int &nBufLen)
{
	CString strValue;
	int nStrLen,i,nCount;
	if (nBufLen<=0)
	{
		return FALSE;
	}
	nCount=i=0;nStrLen=strHex.GetLength();
	while((i+2)<=nStrLen)
	{
		strValue = strHex.Mid(i,2);
		pBuf[nCount] = cmNumString::StrToULong(strValue,16);
		nCount++;
		i+=2;
		if (nCount>=nBufLen)
		{
			break;
		}
	}
	nBufLen = nCount;
	return TRUE;
}
BOOL CWNpctoolDlg::WriteProc()
{
	BOOL			bSuccess=FALSE;
	BOOL			bRet;
	bool            bskip_force_wifimac = FALSE;
	bool            bskip_force_btmac   = FALSE;
	bool            bskip_force_devsn   = FALSE;
	bool            bskip_force_lanmac  = FALSE;
	bool			bskip_force_imei	= FALSE;
	CString         strPrompt;
	DWORD   dwTotalTick;
	dwTotalTick     = GetTickCount();

	if (m_listInfo.GetCount()>0) {
		PostMessage(WM_UPDATE_MSG,UPDATE_LIST,LIST_EMPTY);
	}

	AddPrompt(GetLocalString(_T("IDS_INFO_START_WRITE")).c_str(),LIST_INFO);
	//download boot
	if ((!m_bExistLoader)&&(!m_bDownBoot))
	{
		AddPrompt(GetLocalString(_T("IDS_SET_LOADER")).c_str(),LIST_INFO);
		bRet = RK_SetFirmware((LPTSTR)(LPCTSTR)m_strLoader);
		if (!bRet)
		{
			strPrompt.Format(GetLocalString(_T("IDS_SET_LOADER_FAIL")).c_str(),TEXT("SN"));
			AddPrompt(strPrompt,LIST_ERR);
			LDEGMSG((CLogger::DEBUG_ERROR,"Error:RK_SetFirmware failed."));
			goto WriteProc_Exit;
		}
		AddPrompt(GetLocalString(_T("IDS_SET_LOADER_SUCCESS")).c_str(),LIST_INFO);

		AddPrompt(GetLocalString(_T("IDS_DOWN_BOOT")).c_str(),LIST_INFO);
		bRet = RK_DownloadBoot();
		if (!bRet)
		{
			strPrompt.Format(GetLocalString(_T("IDS_DOWN_BOOT_FAIL")).c_str(),TEXT("SN"));
			AddPrompt(strPrompt,LIST_ERR);
			LDEGMSG((CLogger::DEBUG_ERROR,"Error:RK_DownloadBoot failed."));
			goto WriteProc_Exit;
		}
		m_bDownBoot = TRUE;
		AddPrompt(GetLocalString(_T("IDS_DOWN_BOOT_SUCCESS")).c_str(),LIST_INFO);
	}
	//1.烧写devsn,先写后读，读出来的值与写进去的值一致则成功，否则失败
	if (m_bUserStop) {
		goto WriteProc_Exit;
	}  
	if (m_Configs.devsn.bEnable)
	{
		AddPrompt(GetLocalString(_T("IDS_INFO_SN_WRITE")).c_str(),LIST_INFO);
		strPrompt.Format(TEXT("DEVSN:%s"),(LPCTSTR)m_strCurDevSn);
		AddPrompt(strPrompt,LIST_INFO);
		if(!WriteItem(ITEM_SN))
		{
			LDEGMSG((CLogger::DEBUG_ERROR,"Write SN failed."));
			AddPrompt(GetLocalString(_T("IDS_ERROR_SN_FAIL")).c_str(),LIST_ERR);
			goto WriteProc_Exit;
		}
		AddPrompt(GetLocalString(_T("IDS_INFO_SN_PASS")).c_str(),LIST_INFO);
	}
	
	//2.烧写wifimac
	if (m_bUserStop) {
		goto WriteProc_Exit;
	}  
	if (m_Configs.WifiMac.bEnable)
	{
		AddPrompt(GetLocalString(_T("IDS_INFO_WIFIMAC_WRITE")).c_str(),LIST_INFO);
		strPrompt.Format(TEXT("WIFIMAC:%s"),(LPCTSTR)m_strCurWifiMac);
		AddPrompt(strPrompt,LIST_INFO);
		if(!WriteMac(m_strCurWifiMac,ITEM_WIFIMAC))
		{
			LDEGMSG((CLogger::DEBUG_ERROR,"Write WifiMac failed."));
			AddPrompt(GetLocalString(_T("IDS_INFO_WIFIMAC_FAIL")).c_str(),LIST_ERR);
			goto WriteProc_Exit;
		}
		AddPrompt(GetLocalString(_T("IDS_INFO_WIFIMAC_PASS")).c_str(),LIST_INFO);
	}
	
	//3.烧写btmac
	if (m_bUserStop) {
		goto WriteProc_Exit;
	}  
	if (m_Configs.BtMac.bEnable)
	{
		AddPrompt(GetLocalString(_T("IDS_INFO_BTMAC_WRITE")).c_str(),LIST_INFO);
		strPrompt.Format(TEXT("BTMAC:%s"),(LPCTSTR)m_strCurBtMac);
		AddPrompt(strPrompt,LIST_INFO);
		if(!WriteMac(m_strCurBtMac,ITEM_BTMAC))
		{
			LDEGMSG((CLogger::DEBUG_ERROR,"Write BtMac failed."));
			AddPrompt(GetLocalString(_T("IDS_INFO_BTMAC_FAIL")).c_str(),LIST_ERR);
			goto WriteProc_Exit;
		}
		AddPrompt(GetLocalString(_T("IDS_INFO_BTMAC_PASS")).c_str(),LIST_INFO);
	}
	
	//4.烧写lanmac
	if (m_bUserStop) {
		goto WriteProc_Exit;
	}  
	if (m_Configs.LanMac.bEnable)
	{
		AddPrompt(GetLocalString(_T("IDS_INFO_LANMAC_WRITE")).c_str(),LIST_INFO);
		strPrompt.Format(TEXT("LANMAC:%s"),(LPCTSTR)m_strCurLanMac);
		AddPrompt(strPrompt,LIST_INFO);
		if(!WriteMac(m_strCurLanMac,ITEM_LANMAC))
		{
			LDEGMSG((CLogger::DEBUG_ERROR,"Write LanMac failed."));
			AddPrompt(GetLocalString(_T("IDS_INFO_LANMAC_FAIL")).c_str(),LIST_ERR);
			goto WriteProc_Exit;
		}
		AddPrompt(GetLocalString(_T("IDS_INFO_LANMAC_PASS")).c_str(),LIST_INFO);
	}

	//5.烧写imei,先写后读，读出来的值与写进去的值一致则成功，否则失败
	if (m_bUserStop) {
		goto WriteProc_Exit;
	}  
	if (m_Configs.Imei.bEnable)
	{
		AddPrompt(GetLocalString(_T("IDS_INFO_IMEI_WRITE")).c_str(),LIST_INFO);
		strPrompt.Format(TEXT("IMEI:%s"),(LPCTSTR)m_strCurImei);
		AddPrompt(strPrompt,LIST_INFO);
		if(!WriteItem(ITEM_IMEI))
		{
			LDEGMSG((CLogger::DEBUG_ERROR,"Write IMEI failed."));
			AddPrompt(GetLocalString(_T("IDS_ERROR_IMEI_FAIL")).c_str(),LIST_ERR);
			goto WriteProc_Exit;
		}
		AddPrompt(GetLocalString(_T("IDS_INFO_IMEI_PASS")).c_str(),LIST_INFO);
	}

	if (m_Configs.bReboot)
	{
		AddPrompt(GetLocalString(_T("IDS_INFO_DEVICE_REBOOT")).c_str(),LIST_INFO);
		if (!RK_ResetRockusb())
		{
			AddPrompt(GetLocalString(_T("IDS_INFO_DEVICE_REBOOT_FAIL")).c_str(),LIST_ERR);
			goto WriteProc_Exit;
		}
		Sleep(1000);//sleep for device offline
		AddPrompt(GetLocalString(_T("IDS_INFO_DEVICE_REBOOT_PASS")).c_str(),LIST_INFO);
	}
	bSuccess = TRUE;
WriteProc_Exit:
	if(!m_bUserStop){
		SaveWriteResultOnPass(bSuccess,((bskip_force_devsn)  ?0:FIELD_DEVSN  )|
			((bskip_force_wifimac)?0:FIELD_WIFIMAC)|
			((bskip_force_btmac)  ?0:FIELD_BTMAC  )|
			((bskip_force_lanmac)  ?0:FIELD_LANMAC)|
			((bskip_force_imei)  ?0:FIELD_IMEI));
	}
	if (m_bUserStop) {
		strPrompt.Format(GetLocalString(_T("IDS_INFO_USER_ABORT")).c_str());
		AddPrompt(strPrompt,LIST_ERR);
	}
	if(m_bUserStop) {
		PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_EMPTY);
	} else if (bSuccess) {
		//InitUi();
		PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_PASS);
	} else {
		PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_FAIL);
	}
	strPrompt.Format(GetLocalString(_T("IDS_INFO_TIME_ELAPSE")).c_str(),(GetTickCount() - dwTotalTick )/1000,(GetTickCount() - dwTotalTick )%1000);
	AddPrompt(strPrompt,LIST_TIME);
	//SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_WRITE_BUTTON")).c_str());
	m_pWriteThread = NULL;
	/*exit will set button status,so we must set m_pWorkThread = NULL before **/
	if (!(m_Configs.bAutoTest)||(m_bUserStop)) {
		if (m_bUserStop) {
			m_bUserStop = FALSE;
		}
		PostMessage(WM_UPDATE_MSG,UPDATE_TEST_EXIT);
	}
	return bSuccess;
}
BOOL CWNpctoolDlg::ReadMac(int nItemID)
{
	BOOL	bRet;
	//int nSize;
	USHORT nBufSize=30;
	BYTE buf[512];
	CString strMac;

	LDEGMSG((CLogger::DEBUG_ERROR,"-------------------Read Mac----------------"));
	bRet = RK_ReadProvisioningData(nItemID,buf,nBufSize);
	if (!bRet)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,("RK_WriteProvisioningData readback failed.nReadbackSize=%d"),nBufSize));
		return FALSE;
	}

	if (nBufSize == 0)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,"RK_ReadProvisioningData return size 0"));
		return FALSE;
	}
	LDEGMSG((CLogger::DEBUG_ERROR,("item=%d,read mac %02x:%02x:%02x:%02x:%02x:%02x"),nItemID,buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]));
	//if (!bRet)
	//{
	//	LDEGMSG((CLogger::DEBUG_ERROR,"convert value failed."));
	//	return FALSE;
	//}
	strMac = BytesToHexStr(buf,nBufSize);
	if (ITEM_WIFIMAC == nItemID)
	{
		SetDlgItemText(IDC_EDIT_WIFIMAC,strMac);
	}else if (ITEM_BTMAC == nItemID)
	{
		SetDlgItemText(IDC_EDIT_BTMAC,strMac);
	}else if (ITEM_LANMAC == nItemID)
	{
		SetDlgItemText(IDC_EDIT_LANMAC,strMac);
	}
	else
	{

	}

	return TRUE;
}
BOOL CWNpctoolDlg::WriteMac(CString strMac,int nItemID)
{
	BOOL bRet;
	BYTE readback[512];
	USHORT nReadbackSize=30;
	int nSize=6;
	BYTE mac[6];

	LDEGMSG((CLogger::DEBUG_ERROR,"-------------------WriteMac----------------"));
	HexStrToBytes(strMac,mac,nSize);
	//1.烧写mac,先写后读，读出来的值与写进去的值一致则成功，否则失败
	//烧写mac
	//if (ITEM_WIFIMAC == nItemID)
	//{
	//	//bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurWifiMac);
	//	for(int i = 0; i < len ; i++ ) {
	//		mac[i] = (hexchar2value(wifi_mac[2*i]) << 4)|(hexchar2value(wifi_mac[2*i + 1]));
	//	}
	//	LDEGMSG((CLogger::DEBUG_ERROR,("wifi mac %02x:%02x:%02x:%02x:%02x:%02x\r\n"),Wifi[0],Wifi[1],Wifi[2],Wifi[3],Wifi[4],Wifi[5]));
	//	LDEGMSG((CLogger::DEBUG_ERROR,("wifi mac:%s\r\n"),mac));
	//}else if (ITEM_BTMAC == nItemID)
	//{
	//	//bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurBtMac);
	//}else if (ITEM_LANMAC == nItemID)
	//{
	//	//bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurLanMac);
	//}
	//else
	//{

	//}
	LDEGMSG((CLogger::DEBUG_ERROR,("item=%d,write mac %02x:%02x:%02x:%02x:%02x:%02x"),nItemID,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]));
	bRet = RK_WriteProvisioningData(nItemID,mac,nSize);		
	if (!bRet)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,"RK_WriteProvisioningData failed."));
		return FALSE;
	}
	//读取mac
	memset(readback,0,512);
	bRet = RK_ReadProvisioningData(nItemID,readback,nReadbackSize);
	if (!bRet)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,("RK_WriteProvisioningData readback failed.nReadbackSize=%d"),nReadbackSize));
		return FALSE;
	}
	LDEGMSG((CLogger::DEBUG_ERROR,("item=%d,read mac %02x:%02x:%02x:%02x:%02x:%02x"),nItemID,readback[0],readback[1],readback[2],readback[3],readback[4],readback[5]));
	if (memcmp(readback,mac,nSize)!=0)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,"RK_WriteProvisioningData compare failed."));
		return FALSE;
	}
	return TRUE;
}
BOOL CWNpctoolDlg::WriteItem(int nItemID)
{
	BOOL bRet;
	LPSTR lpszData=NULL;
	BYTE readback[512];
	USHORT nReadbackSize;
	int nSize;
	unsigned char   mac[6];

	memset(mac,0,6);
	//1.烧写devsn,先写后读，读出来的值与写进去的值一致则成功，否则失败
	//烧写devsn
	if (ITEM_SN == nItemID)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurDevSn);
	}else if (ITEM_WIFIMAC == nItemID)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurWifiMac);
		//for(int i = 0; i < 6 ; i++ ) {
		//	mac[i] = (hexchar2value(m_strCurWifiMac[2*i]) << 4)|(hexchar2value(m_strCurWifiMac[2*i + 1]));
		//}
		//LDEGMSG((CLogger::DEBUG_ERROR,("wifi mac %02x:%02x:%02x:%02x:%02x:%02x\r\n"),mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]));
		//lpszData = (char*)mac;
	}else if (ITEM_BTMAC == nItemID)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurBtMac);
	}else if (ITEM_LANMAC == nItemID)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurLanMac);
	}else if (ITEM_IMEI == nItemID)
	{
		bRet = cmStrCode::UnicodeToAnsi(lpszData,nSize,m_strCurImei);
	}
	else
	{

	}
	if (!bRet)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,"convert value failed."));
		return FALSE;
	}
	bRet = RK_WriteProvisioningData(nItemID,(PBYTE)lpszData,nSize);		
	if (!bRet)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,"RK_WriteProvisioningData failed."));
		return FALSE;
	}
	//读取devsn
	memset(readback,0,512);
	bRet = RK_ReadProvisioningData(nItemID,readback,nReadbackSize);
	if (!bRet)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,"RK_WriteProvisioningData readback failed."));
		return FALSE;
	}

	LDEGMSG((CLogger::DEBUG_ERROR,"Write:%s,Read:%s",lpszData,readback));
	
	if (memcmp(readback,lpszData,nSize)!=0)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,"Write:%s,Read:%s",lpszData,readback));
		LDEGMSG((CLogger::DEBUG_ERROR,"RK_WriteProvisioningData compare failed."));
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
	int nSize;
	USHORT nBufSize;
	BYTE buf[512];
	nBufSize = 512;
	bRet = RK_ReadProvisioningData(nItemID,buf,nBufSize);
	if (!bRet)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,"RK_ReadProvisioningData failed."));
		return FALSE;
	}
	//if (nBufSize==0)
	//{
	//	if (m_pLogObject)
	//	{
	//		m_pLogObject->Record(_T("Error:RK_ReadProvisioningData return size 0\r\n."));
	//	}
	//	return FALSE;
	//}

	if (nBufSize != 0)
	{
		bRet = cmStrCode::AnsiToUnicode(lpszValue,nSize,(LPCSTR)buf);
	}
	if (!bRet)
	{
		LDEGMSG((CLogger::DEBUG_ERROR,"convert value failed."));
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
	}else if (ITEM_IMEI == nItemID)
	{
		SetDlgItemText(IDC_EDIT_IMEI,lpszValue);
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
	BOOL bRet;
	CString strPrompt;
	DWORD   dwTotalTick;
	dwTotalTick     = GetTickCount();
	if (m_listInfo.GetCount()>0) {
		PostMessage(WM_UPDATE_MSG,UPDATE_LIST,LIST_EMPTY);
	}

	AddPrompt(GetLocalString(_T("IDS_READ_START")).c_str(),LIST_INFO);

	//download boot
	if ((!m_bExistLoader)&&(!m_bDownBoot))
	{
		AddPrompt(GetLocalString(_T("IDS_SET_LOADER")).c_str(),LIST_INFO);
		bRet = RK_SetFirmware((LPTSTR)(LPCTSTR)m_strLoader);
		if (!bRet)
		{
			strPrompt.Format(GetLocalString(_T("IDS_SET_LOADER_FAIL")).c_str(),TEXT("SN"));
			AddPrompt(strPrompt,LIST_ERR);
			LDEGMSG((CLogger::DEBUG_ERROR,"Error:RK_SetFirmware failed."));
			goto Exit_Read;
		}
		AddPrompt(GetLocalString(_T("IDS_SET_LOADER_SUCCESS")).c_str(),LIST_INFO);

		AddPrompt(GetLocalString(_T("IDS_DOWN_BOOT")).c_str(),LIST_INFO);
		bRet = RK_DownloadBoot();
		if (!bRet)
		{
			strPrompt.Format(GetLocalString(_T("IDS_DOWN_BOOT_FAIL")).c_str(),TEXT("SN"));
			AddPrompt(strPrompt,LIST_ERR);
			LDEGMSG((CLogger::DEBUG_ERROR,"Error:RK_DownloadBoot failed."));
			goto Exit_Read;
		}
		m_bDownBoot = TRUE;
		AddPrompt(GetLocalString(_T("IDS_DOWN_BOOT_SUCCESS")).c_str(),LIST_INFO);
	}

    if (m_Configs.devsn.bEnable)
    {
        strPrompt.Format(GetLocalString(_T("IDS_READ_SS")).c_str(),TEXT("SN"));
        AddPrompt(strPrompt,LIST_INFO);
        if (ReadItem(ITEM_SN))
        {
            strPrompt.Format(GetLocalString(_T("IDS_READ_SS_PASS")).c_str(),TEXT("SN"));
            AddPrompt(strPrompt,LIST_INFO);
            LDEGMSG((CLogger::DEBUG_INFO,"Read SN successfully."));
        }
        else
        {
            strPrompt.Format(GetLocalString(_T("IDS_READ_SS_FAIL")).c_str(),TEXT("SN"));
            AddPrompt(strPrompt,LIST_ERR);
            LDEGMSG((CLogger::DEBUG_ERROR,"Read SN failed."));
            goto Exit_Read;
        }
    }

    if (m_Configs.WifiMac.bEnable)
    {
        strPrompt.Format(GetLocalString(_T("IDS_READ_SS")).c_str(),TEXT("WIFIMAC"));
        AddPrompt(strPrompt,LIST_INFO);
		if (ReadMac(ITEM_WIFIMAC))
        {
            strPrompt.Format(GetLocalString(_T("IDS_READ_SS_PASS")).c_str(),TEXT("WIFIMAC"));
            AddPrompt(strPrompt,LIST_INFO);
            LDEGMSG((CLogger::DEBUG_INFO,"Read WifiMac successfully."));
        }
        else
        {
            strPrompt.Format(GetLocalString(_T("IDS_READ_SS_FAIL")).c_str(),TEXT("WIFIMAC"));
            AddPrompt(strPrompt,LIST_ERR);
            LDEGMSG((CLogger::DEBUG_ERROR,"Read WifiMac failed."));
            goto Exit_Read;
        }
    }

    if (m_Configs.BtMac.bEnable)
    {
        strPrompt.Format(GetLocalString(_T("IDS_READ_SS")).c_str(),TEXT("BTMAC"));
        AddPrompt(strPrompt,LIST_INFO);
        if (ReadMac(ITEM_BTMAC))
        {
            strPrompt.Format(GetLocalString(_T("IDS_READ_SS_PASS")).c_str(),TEXT("BTMAC"));
            AddPrompt(strPrompt,LIST_INFO);
            LDEGMSG((CLogger::DEBUG_INFO,"Read BtMac successfully."));
        }
        else
        {
            strPrompt.Format(GetLocalString(_T("IDS_READ_SS_FAIL")).c_str(),TEXT("BTMAC"));
            AddPrompt(strPrompt,LIST_ERR);
            LDEGMSG((CLogger::DEBUG_ERROR,"Read BtMac failed."));
            goto Exit_Read;
        }
    }

    if (m_Configs.LanMac.bEnable)
    {
        strPrompt.Format(GetLocalString(_T("IDS_READ_SS")).c_str(),TEXT("LANMAC"));
        AddPrompt(strPrompt,LIST_INFO);
        if (ReadMac(ITEM_LANMAC))
        {
            strPrompt.Format(GetLocalString(_T("IDS_READ_SS_PASS")).c_str(),TEXT("LANMAC"));
            AddPrompt(strPrompt,LIST_INFO);
            LDEGMSG((CLogger::DEBUG_INFO,"Read LanMac successfully."));
        }
        else
        {
            strPrompt.Format(GetLocalString(_T("IDS_READ_SS_FAIL")).c_str(),TEXT("LANMAC"));
            AddPrompt(strPrompt,LIST_ERR);
            LDEGMSG((CLogger::DEBUG_ERROR,"Read LanMac failed."));
            goto Exit_Read;
        }
    }

	if (m_Configs.Imei.bEnable)
	{
		strPrompt.Format(GetLocalString(_T("IDS_READ_SS")).c_str(),TEXT("IMEI"));
		AddPrompt(strPrompt,LIST_INFO);
		if (ReadItem(ITEM_IMEI))
		{
			strPrompt.Format(GetLocalString(_T("IDS_READ_SS_PASS")).c_str(),TEXT("IMEI"));
			AddPrompt(strPrompt,LIST_INFO);
			LDEGMSG((CLogger::DEBUG_INFO,"Read LanMac successfully."));
		}
		else
		{
			strPrompt.Format(GetLocalString(_T("IDS_READ_SS_FAIL")).c_str(),TEXT("IMEI"));
			AddPrompt(strPrompt,LIST_ERR);
			LDEGMSG((CLogger::DEBUG_ERROR,"Read LanMac failed."));
			goto Exit_Read;
		}
	}

	if (m_Configs.bReboot)
	{
		AddPrompt(GetLocalString(_T("IDS_INFO_DEVICE_REBOOT")).c_str(),LIST_INFO);
		if (!RK_ResetRockusb())
		{
			AddPrompt(GetLocalString(_T("IDS_INFO_DEVICE_REBOOT_FAIL")).c_str(),LIST_ERR);
			goto Exit_Read;
		}
		Sleep(1000);//sleep for device offline
		AddPrompt(GetLocalString(_T("IDS_INFO_DEVICE_REBOOT_PASS")).c_str(),LIST_INFO);
	}
	bSuccess = TRUE;
Exit_Read:
	m_bRun = FALSE;
	if (m_bUserStop) {
		strPrompt.Format(GetLocalString(_T("IDS_INFO_USER_ABORT")).c_str());
		AddPrompt(strPrompt,LIST_ERR);
	}
	//EnableCtrl();
	SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_READ_BUTTON")).c_str());
	if(m_bUserStop) {
		PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_EMPTY);
	} else if (bSuccess) {
		PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_PASS);
	} else {
		PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_FAIL);
	}
	strPrompt.Format(GetLocalString(_T("IDS_INFO_TIME_ELAPSE")).c_str(),(GetTickCount() - dwTotalTick )/1000,(GetTickCount() - dwTotalTick )%1000);
	AddPrompt(strPrompt,LIST_TIME);
	m_pReadThread = NULL;
	/*exit will set button status,so we must set m_pWorkThread = NULL before **/
	if (!m_Configs.bAutoTest||m_bUserStop) {
		if (m_bUserStop) {
			m_bUserStop = FALSE;
		}
		PostMessage(WM_UPDATE_MSG,UPDATE_TEST_EXIT);
	}
	return bSuccess;
}
BOOL CWNpctoolDlg::SaveWriteResultOnPass(BOOL bPass,DWORD mask)
{
	CString     strTmp;
	BOOL        inc;
	DWORD       dwIfiInc;
	dwIfiInc    = 1;

	/*basic **/
	//m_Configs.basic.dwTotal ++;
	//if(bPass) {
	//	m_Configs.basic.dwPass ++;
	//}
	if(!bPass )goto swrop_exit;

	/*snw **/
	if(m_Configs.devsn.bEnable) {
		if (FIELD_DEVSN&mask) {
			if(MODE_AUTO == m_Configs.devsn.nAutoMode) {
				inc = TRUE;
				//if (m_Configs.devsn.nRemainCount!=-1) { /*-1 means no limit*/
				//                m_Configs.devsn.nRemainCount--;
				//                if (m_Configs.devsn.nRemainCount==0) {
				//                    m_Configs.devsn.strCurrentSn = _T("");
				//                    inc = FALSE;
				//                }
				//}
				if (m_Configs.devsn.nRemainCount!=0&&m_Configs.devsn.nRemainCount!=-1)
				{
					m_Configs.devsn.nRemainCount--;
				}
				if(inc) {
					m_Configs.devsn.strCurrentSn = m_Configs.devsn.strCurrentSn.substr(m_Configs.devsn.strPrefix.length());
					strTmp = m_Configs.devsn.strCurrentSn.substr(0,m_Configs.devsn.strCurrentSn.length()-m_Configs.devsn.strSuffix.length()).c_str();
					if (m_Configs.devsn.bHexCarry) {
						HexStrIncreaseSkipAlpha(strTmp.GetBuffer());
						m_Configs.devsn.strCurrentSn = strTmp.GetString();
						strTmp.ReleaseBuffer();
					} else {
						IntStrIncreaseSkipAlpha(strTmp.GetBuffer());
						m_Configs.devsn.strCurrentSn = strTmp.GetString();
						strTmp.ReleaseBuffer();
					}
					//strTmp = m_Configs.devsn.strPrefix + m_Configs.devsn.strCurrentSn + m_Configs.devsn.strSuffix;
					m_Configs.devsn.strCurrentSn = m_Configs.devsn.strPrefix + m_Configs.devsn.strCurrentSn + m_Configs.devsn.strSuffix;
				}
			}
		}
	}
	if(m_Configs.WifiMac.bEnable) {
		if (FIELD_WIFIMAC&mask) {
			if(MODE_AUTO == m_Configs.WifiMac.nAutoMode) {
				//unsigned short mac0 = chartohex((LPCTSTR)m_Configs.WifiMac.strCurrentMac,2);
				//if( mac0& 0x2 ) { /*LOCAL_ADMIN**/
				//	dwIfiInc = 4;
				//} else {            /*GLOBAL_UNIQUE**/ 
				//	dwIfiInc = 1;
				//}
				dwIfiInc = 1;
				inc              = TRUE;
				//if (m_Configs.WifiMac.nRemainCount!=-1) { /*-1 means no limit*/
				//    m_Configs.WifiMac.nRemainCount -= dwIfiInc;
				//    if (m_Configs.WifiMac.nRemainCount < dwIfiInc ) {
				//        m_Configs.WifiMac.strCurrentMac = _T("");
				//        inc = FALSE;
				//    }
				//}
				if (m_Configs.WifiMac.nRemainCount!=0&&m_Configs.WifiMac.nRemainCount!=-1) { /*-1 means no limit*/
					m_Configs.WifiMac.nRemainCount--;
				}
				if(inc) {
					strTmp = m_Configs.WifiMac.strCurrentMac.c_str();
					HexStrIncrease(strTmp.GetBuffer(),dwIfiInc); /*Intel wifi mac occupy four**/
					m_Configs.WifiMac.strCurrentMac = strTmp.GetString();
					strTmp.ReleaseBuffer();
				}
			}
			if (MODE_FILE == m_Configs.WifiMac.nAutoMode)
			{
				m_Configs.confPath.lFilePos[FLAG_WIFIMAC] = m_Configs.curFilePos[FLAG_WIFIMAC];
				m_Configs.confPath.dwLinePos[FLAG_WIFIMAC] ++;  //just inccrease when get a correct item
			}
		}
	}
	if(m_Configs.BtMac.bEnable) {
		if (FIELD_BTMAC&mask)  {
			if(MODE_AUTO == m_Configs.BtMac.nAutoMode) {
				inc = TRUE;
				//if (m_Configs.BtMac.nRemainCount!=-1) { /*-1 means no limit*/
				//    m_Configs.BtMac.nRemainCount--;
				//    if (m_Configs.BtMac.nRemainCount == 0) {
				//        m_Configs.BtMac.strCurrentMac = _T("");
				//        inc = FALSE;
				//    } 
				//}
				if (m_Configs.BtMac.nRemainCount!=0&&m_Configs.BtMac.nRemainCount!=-1) { /*-1 means no limit*/
					m_Configs.BtMac.nRemainCount--;
				}
				if(inc) {
					strTmp = m_Configs.BtMac.strCurrentMac.c_str();
					HexStrIncrease(strTmp.GetBuffer());
					m_Configs.BtMac.strCurrentMac = strTmp.GetString();
					strTmp.ReleaseBuffer();
				}
			}
			if (MODE_FILE == m_Configs.BtMac.nAutoMode)
			{
				m_Configs.confPath.lFilePos[FLAG_BTMAC] = m_Configs.curFilePos[FLAG_BTMAC];
				m_Configs.confPath.dwLinePos[FLAG_BTMAC] ++;  //just inccrease when get a correct item
			}
		}
	}

	if(m_Configs.LanMac.bEnable) {
		if (FIELD_LANMAC&mask)  {
			if(MODE_AUTO == m_Configs.LanMac.nAutoMode) {
				inc = TRUE;
				if (m_Configs.LanMac.nRemainCount!=0&&m_Configs.LanMac.nRemainCount!=-1) { /*-1 means no limit*/
					m_Configs.LanMac.nRemainCount--;
				}
				if(inc) {
					strTmp = m_Configs.LanMac.strCurrentMac.c_str();
					HexStrIncrease(strTmp.GetBuffer());
					m_Configs.LanMac.strCurrentMac = strTmp.GetString();
					strTmp.ReleaseBuffer();
				}
			}
			if (MODE_FILE == m_Configs.LanMac.nAutoMode)
			{
				m_Configs.confPath.lFilePos[FLAG_LANMAC] = m_Configs.curFilePos[FLAG_LANMAC];
				m_Configs.confPath.dwLinePos[FLAG_LANMAC] ++;  //just inccrease when get a correct item
			}
		}
	}

	if(m_Configs.Imei.bEnable) {
		if (FIELD_IMEI&mask)  {
			if(MODE_AUTO == m_Configs.Imei.nAutoMode) {
				inc = TRUE;
				if (m_Configs.Imei.nRemainCount1!=0&&m_Configs.Imei.nRemainCount1!=-1) { /*-1 means no limit*/
					m_Configs.Imei.nRemainCount1--;
				}
				if(inc) {
					strTmp = m_Configs.Imei.strCurrentImei1.c_str();
					HexStrIncrease(strTmp.GetBuffer());
					m_Configs.Imei.strCurrentImei1 = strTmp.GetString();
					strTmp.ReleaseBuffer();
				}
			}
			if (MODE_FILE == m_Configs.Imei.nAutoMode)
			{
				m_Configs.confPath.lFilePos[FLAG_IMEI1] = m_Configs.curFilePos[FLAG_IMEI1];
				m_Configs.confPath.dwLinePos[FLAG_IMEI1] ++;  //just inccrease when get a correct item
			}
		}
	}

swrop_exit:
	m_Configs.SaveToolSetting(std::wstring(TEXT("")));
	return TRUE;
}
void CWNpctoolDlg::OnBnClickedBtnWrite()
{
	// TODO: Add your control notification handler code here
	if(m_Configs.bReadInfo) {
		if (m_bRun)
		{
			if(m_pReadThread) { 
				m_bUserStop     = TRUE;
				SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_STOPING_BUTTON")).c_str());
			} else {
				m_bRun = FALSE;
				SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_READ_BUTTON")).c_str());
				//if(OnStartRead()) {
				//	SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_STOP_BUTTON")).c_str());
				//}
			}
		}else {
			m_bRun    = TRUE;
			SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_STOP_BUTTON")).c_str());
			OnStartRead();
		}

	} else {
		if (m_Configs.bAutoTest)
		{
			if( m_bRun ) {
				if( m_pWriteThread) { 
					m_bUserStop     = TRUE; /*just set m_bUserStop,and wait for thread exit **/
					SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_STOPING_BUTTON")).c_str());
				} else {
					m_bRun    = FALSE;
					SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_WRITE_BUTTON")).c_str());                
				}
			} else {
				/*m_bUserStop     = FALSE; **/
				m_bRun    = TRUE;
				SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_STOP_BUTTON")).c_str());
				OnStartWrite(TRUE);
			}
		}
		else
		{
			if (m_bRun)
			{
				if(m_pWriteThread) { 
					m_bUserStop     = TRUE;
					SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_STOPING_BUTTON")).c_str());
				} else {
					m_bRun = FALSE;
					SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_WRITE_BUTTON")).c_str());
					//if(OnStartWrite()) {
					//	SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("STOP")).c_str());
					//}
				}
			}else {
				m_bRun    = TRUE;
				SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_STOP_BUTTON")).c_str());
				OnStartWrite(FALSE);
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
		strPromt =GetLocalString(_T("IDS_ERROR_NO_USB_READ")).c_str();
		//MessageBox(_T("Not found rockusb to burn."),_T("Error"),MB_OK|MB_ICONERROR);
		goto OnStartReadExit;
	}
	m_lblDevice.GetWindowText(strText);
	if (strText.Find(_T("LOADER"))!=-1)
	{
		m_bExistLoader = TRUE;
	}
	else 
		m_bExistLoader = FALSE;
	m_csScanLock.Unlock();

    if (!(m_Configs.devsn.bEnable||m_Configs.WifiMac.bEnable||m_Configs.BtMac.bEnable||m_Configs.LanMac.bEnable||m_Configs.Imei.bEnable))
    {
        strPromt = GetLocalString(_T("IDS_ERROR_NO_WRITE")).c_str();
        goto OnStartReadExit;
    }
	if (!m_bExistLoader)
	{
		GetDlgItemText(IDC_EDIT_LOADER,m_strLoader);
		if (m_strLoader.IsEmpty()&&strText.Find(_T("MASKROM"))!=-1)
		{
			strPromt.Format(GetLocalString(_T("IDS_ERROR_INVALID_LOADER")).c_str(),m_strCurLanMac);
			goto OnStartReadExit;
		}
	}
	m_bRun = TRUE;
	AfxBeginThread(ThreadReading,(LPVOID)this);

	return TRUE;
OnStartReadExit:
	if(!strPromt.IsEmpty()) {
		MessageBox(strPromt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_OK|MB_ICONERROR);
	}
	return FALSE;
}
std::wstring CWNpctoolDlg::GetLocalString(std::wstring strKey)
{
	return m_LocalLan.GetLanStr(strKey);
}
BOOL CWNpctoolDlg::OnStartWrite(bool bAuto)
{
	CString     strPromt;
	CString		strText;

	if (!(m_Configs.devsn.bEnable||m_Configs.WifiMac.bEnable||m_Configs.BtMac.bEnable||m_Configs.LanMac.bEnable||m_Configs.Imei.bEnable))
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
			if (m_Configs.devsn.nRemainCount > 0)
			{
				m_strCurDevSn = m_Configs.devsn.strCurrentSn.c_str();
			}
			else
			{
				strPromt = GetLocalString(_T("IDS_ERROR_OUT_OF_DEVSN")).c_str();
				goto OnStartWriteExit;
			}
		}
		else
		{

		}
		if(m_strCurDevSn.IsEmpty()) 
		{
			strPromt.Format(GetLocalString(_T("IDS_ERROR_INVALID_DEVSN")).c_str(),m_strCurBtMac);
			goto OnStartWriteExit;
		}
		SetDlgItemText(IDC_EDIT_SN,m_strCurDevSn);
	}

	if (m_Configs.WifiMac.bEnable)
	{
		if (MODE_MANUAL == m_Configs.WifiMac.nAutoMode)
		{
			GetDlgItemText(IDC_EDIT_WIFIMAC,m_strCurWifiMac);
		}
		else if (MODE_AUTO == m_Configs.WifiMac.nAutoMode)
		{
			m_strCurWifiMac = m_Configs.WifiMac.strCurrentMac.c_str();
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
			strPromt.Format(GetLocalString(_T("IDS_ERROR_INVALID_WIFIMAC")).c_str(),m_strCurWifiMac);
			goto OnStartWriteExit;
		}
		SetDlgItemText(IDC_EDIT_WIFIMAC,m_strCurWifiMac);
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
			strPromt.Format(GetLocalString(_T("IDS_ERROR_INVALID_BTMAC")).c_str(),m_strCurBtMac);
			goto OnStartWriteExit;
		}
		SetDlgItemText(IDC_EDIT_BTMAC,m_strCurBtMac);
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
			if (m_strCurLanMac.IsEmpty()||m_Configs.LanMac.nRemainCount==0) {
				strPromt = GetLocalString(_T("IDS_ERROR_OUT_OF_LANMAC")).c_str();
				goto OnStartWriteExit;
			}
		}
		else
		{

		}
		if(!CheckMacStr(m_strCurLanMac)) 
		{
			strPromt.Format(GetLocalString(_T("IDS_ERROR_INVALID_LANMAC")).c_str(),m_strCurLanMac);
			goto OnStartWriteExit;
		}
		SetDlgItemText(IDC_EDIT_LANMAC,m_strCurLanMac);
	}

	if (m_Configs.Imei.bEnable)
	{
		if (MODE_MANUAL == m_Configs.Imei.nAutoMode)
		{
			GetDlgItemText(IDC_EDIT_LANMAC,m_strCurImei);
		}
		else if (MODE_AUTO == m_Configs.Imei.nAutoMode)
		{
			m_strCurImei = m_Configs.Imei.strCurrentImei1.c_str();
			if (m_strCurImei.IsEmpty()||m_Configs.Imei.nRemainCount1==0) {
				strPromt = GetLocalString(_T("IDS_ERROR_OUT_OF_IMEI")).c_str();
				goto OnStartWriteExit;
			}
		}
		else
		{

		}
		if(!CheckImeiStr(m_strCurImei)) 
		{
			strPromt.Format(GetLocalString(_T("IDS_ERROR_INVALID_IMEI")).c_str(),m_strCurImei);
			goto OnStartWriteExit;
		}
		SetDlgItemText(IDC_EDIT_IMEI,m_strCurImei);
	}

	m_csScanLock.Lock();
	if (m_nDeviceCount < 1)
	{
		if(!m_Configs.bAutoTest)
		{
			strPromt = GetLocalString(_T("IDS_ERROR_NO_USB_WRITE")).c_str();
		}
		m_csScanLock.Unlock();
		goto OnStartWriteExit;
	}
	m_lblDevice.GetWindowText(strText);
	if (strText.Find(_T("LOADER"))!=-1)
	{
		m_bExistLoader = TRUE;
	}
	else 
		m_bExistLoader = FALSE;
	m_csScanLock.Unlock();
	if (!m_bExistLoader)
	{
		GetDlgItemText(IDC_EDIT_LOADER,m_strLoader);
		if (m_strLoader.IsEmpty()&&strText.Find(_T("MASKROM"))!=-1)
		{
			strPromt.Format(GetLocalString(_T("IDS_ERROR_INVALID_LOADER")).c_str(),m_strCurLanMac);
			goto OnStartWriteExit;
		}
	}

	AfxBeginThread(ThreadWrite,(LPVOID)this);

	return TRUE;
OnStartWriteExit:
	if(!strPromt.IsEmpty()) {
		MessageBox(strPromt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_OK|MB_ICONERROR);
		//AddPrompt(strPromt,LIST_ERR);
	}
	if (!m_Configs.bAutoTest)
	{
		PostMessage(WM_UPDATE_MSG,UPDATE_PROMPT,PROMPT_FAIL);
	}
	return FALSE;
}
void CWNpctoolDlg::OnSettingMode()
{
	// TODO: Add your command handler code here
	CConfigMode ConfigModeDlg(m_Configs,m_LocalLan);
	//ConfigModeDlg.UpdateInterface();
	if (IDOK == ConfigModeDlg.DoModal())
	{
		m_Configs.SaveToolSetting(std::wstring(TEXT("")));
		InitUi();
	}

	//m_ConfigModeDlg.ShowWindow(SW_SHOW);
}

void CWNpctoolDlg::AddPrompt(CString strPrompt,int flag)
{
	PSTRUCT_LIST_LINE   pLine=NULL;
	SYSTEMTIME          curTime;

	//LDEGMSGW((CLogger::DEBUG_INFO,(LPCTSTR)strPrompt));
	GetLocalTime( &curTime );
	pLine       = new STRUCT_LIST_LINE;
	if (!pLine) {
		return;
	}
	wsprintf(pLine->pszLineText,
		_T("%02d:%02d:%02d %03d\t%s "),
		curTime.wHour,curTime.wMinute,curTime.wSecond,curTime.wMilliseconds,
		(LPCTSTR)strPrompt);
	pLine->flag = flag;
	if(0 == PostMessage(WM_UPDATE_MSG,UPDATE_LIST,(LPARAM)pLine)) {
		delete pLine;
	}
	return;

}

LRESULT CWNpctoolDlg::OnHandleUpdateMsg(WPARAM wParam,LPARAM lParam)
{
	switch(wParam) {
	case UPDATE_TEST_EXIT:
		m_bRun = FALSE;
		if (m_Configs.bReadInfo)
		{
			SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_READ_BUTTON")).c_str());
		}
		else
		{
			SetDlgItemText(ID_BTN_WRITE,GetLocalString(_T("IDS_TEXT_WRITE_BUTTON")).c_str());
		}
		break;
	case UPDATE_WINDOW:
		InitUi();
		RedrawWindow();
		break;
	case UPDATE_PROMPT:
		if (lParam == PROMPT_TESTING) {
			m_lblPrompt.SetTextColor(RGB(0,0,0));
			if (m_Configs.bReadInfo)
			{
				m_lblPrompt.SetWindowText(GetLocalString(_T("IDS_TEXT_READ")).c_str());
			}
			else
			{
				m_lblPrompt.SetWindowText(GetLocalString(_T("IDS_TEXT_WRITE")).c_str());				
			}
		} else if (lParam==PROMPT_PASS) {
			m_lblPrompt.SetTextColor(RGB(0,0,0));
			m_lblPrompt.SetBackground(RGB(0,255,0));
			m_lblPrompt.SetWindowText(GetLocalString(_T("IDS_TEXT_SUCCESS")).c_str());
		} else if(lParam==PROMPT_FAIL) {
			m_lblPrompt.SetTextColor(RGB(0,0,0));
			m_lblPrompt.SetBackground(RGB(255,0,0));
			m_lblPrompt.SetWindowText(GetLocalString(_T("IDS_TEXT_FAIL")).c_str());
		} else {
			m_lblPrompt.SetWindowText(_T(""));
			m_lblPrompt.SetBackground(GetSysColor(COLOR_3DFACE));
		}
		m_lblPrompt.RedrawWindow();
		break;
	case UPDATE_LIST:
		if (lParam == LIST_EMPTY) {
			m_listInfo.ResetContent();
		} else {
			PSTRUCT_LIST_LINE pLine = (PSTRUCT_LIST_LINE)lParam;
			if (pLine->flag == LIST_INFO) {
				m_listInfo.AddLine(CXListBox::White,CXListBox::Black,pLine->pszLineText); 
			} else if (pLine->flag == LIST_TIME) {
				m_listInfo.AddLine(CXListBox::White,CXListBox::Purple,pLine->pszLineText);
			} else if (pLine->flag == LIST_WARN){ 
				m_listInfo.AddLine(CXListBox::Black,CXListBox::Yellow,pLine->pszLineText);
			} else {
				m_listInfo.AddLine(CXListBox::White,CXListBox::Red,pLine->pszLineText);
			}
			//m_listInfo.SetCurSel(m_listInfo.GetCount()-1);
			delete pLine;
		}
		break;

	}
	return 0;
}
void CWNpctoolDlg::OnUpdateSettingRead(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CWNpctoolDlg::OnSettingRead()
{
	// TODO: Add your command handler code here
	CMenu	*pMenu	= GetMenu();
	if (!pMenu)	return;
	m_Configs.bReadInfo = !m_Configs.bReadInfo;
	if (m_Configs.bReadInfo)
	{
		pMenu->CheckMenuItem(ID_SETTING_READ, MF_CHECKED |MF_BYCOMMAND);
		GetDlgItem(ID_BTN_WRITE)->SetWindowText(GetLocalString(_T("IDS_TEXT_READ_BUTTON")).c_str());
	}
	else
	{
		pMenu->CheckMenuItem(ID_SETTING_READ, MF_UNCHECKED |MF_BYCOMMAND);
		GetDlgItem(ID_BTN_WRITE)->SetWindowText(GetLocalString(_T("IDS_TEXT_WRITE_BUTTON")).c_str());
	}
	m_Configs.SaveToolSetting(std::wstring(TEXT("")));
	InitUi();
	m_listInfo.ResetContent();
}

void CWNpctoolDlg::OnLanguageChinese()
{
	// TODO: Add your command handler code here
	CString strConfigPath;

	m_Configs.nCurLan = 1;
	UpdateMenuItem();
	m_Configs.SaveToolSetting(std::wstring(TEXT("")));
	MessageBox(GetLocalString(_T("IDS_INFO_SELECT_LAN")).c_str(),_T("Info"),MB_OK|MB_ICONINFORMATION);
	//strConfigPath = m_strModulePath + m_Configs.strLanPath.c_str();
	//strConfigPath = strConfigPath + m_Configs.strCnFilename.c_str();
	//m_LocalLan.LoadToolSetting(strConfigPath.GetString());
	//m_LocalLan.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,true);
	//Invalidate();
	//UpdateWindow();
	//PostMessage(WM_UPDATE_MSG,UPDATE_WINDOW);
}

void CWNpctoolDlg::OnUpdateLanguageChinese(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CWNpctoolDlg::OnLanguageEnglish()
{
	// TODO: Add your command handler code here
	m_Configs.nCurLan = 2;
	UpdateMenuItem();
	m_Configs.SaveToolSetting(std::wstring(TEXT("")));
	MessageBox(GetLocalString(_T("IDS_INFO_SELECT_LAN")).c_str(),_T("Info"),MB_OK|MB_ICONINFORMATION);
	//Invalidate();
	//UpdateWindow();
	//PostMessage(WM_UPDATE_MSG,UPDATE_WINDOW);
}

void CWNpctoolDlg::OnUpdateLanguageEnglish(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}
VOID CWNpctoolDlg::UpdateMenuItem()
{
	CMenu	*pMenu	= GetMenu();
	if (!pMenu)	return;
	CMenu	*pMenuLoop 	= pMenu->GetSubMenu(0);
	if(m_Configs.bReadInfo) {
		pMenuLoop->CheckMenuItem( ID_SETTING_READ      , MF_CHECKED  |MF_BYCOMMAND);
	} else {
		pMenuLoop->CheckMenuItem( ID_SETTING_READ      , MF_UNCHECKED|MF_BYCOMMAND);
	}
	if(m_Configs.bAutoTest) {
		pMenuLoop->CheckMenuItem( ID_SETTING_AUTO      , MF_CHECKED  |MF_BYCOMMAND);
	} else {
		pMenuLoop->CheckMenuItem( ID_SETTING_AUTO      , MF_UNCHECKED|MF_BYCOMMAND);
	}
	if(m_Configs.bReboot) {
		pMenuLoop->CheckMenuItem( ID_SETTING_RESET      , MF_CHECKED  |MF_BYCOMMAND);
	} else {
		pMenuLoop->CheckMenuItem( ID_SETTING_RESET      , MF_UNCHECKED|MF_BYCOMMAND);
	}
	pMenuLoop 	= pMenu->GetSubMenu(1);
	if (m_Configs.nCurLan == 1)
	{
		pMenuLoop->CheckMenuItem( ID_LANGUAGE_CHINESE      , MF_CHECKED  |MF_BYCOMMAND);
		pMenuLoop->CheckMenuItem( ID_LANGUAGE_ENGLISH      , MF_UNCHECKED  |MF_BYCOMMAND);
	}
	else
	{
		pMenuLoop->CheckMenuItem( ID_LANGUAGE_ENGLISH      , MF_CHECKED  |MF_BYCOMMAND);
		pMenuLoop->CheckMenuItem( ID_LANGUAGE_CHINESE      , MF_UNCHECKED  |MF_BYCOMMAND);
	}

}
void CWNpctoolDlg::OnSettingAuto()
{
	// TODO: Add your command handler code here
	CMenu	*pMenu	= GetMenu();
	if (!pMenu)	return;
	m_Configs.bAutoTest = !m_Configs.bAutoTest;
	if (m_Configs.bAutoTest)
	{
		pMenu->CheckMenuItem(ID_SETTING_AUTO, MF_CHECKED |MF_BYCOMMAND);
	}
	else
	{
		pMenu->CheckMenuItem(ID_SETTING_AUTO, MF_UNCHECKED |MF_BYCOMMAND);
	}
	m_Configs.SaveToolSetting(std::wstring(TEXT("")));
}

void CWNpctoolDlg::OnUpdateSettingAuto(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}

void CWNpctoolDlg::OnBnClickedButtonLoader()
{
	// TODO: Add your control notification handler code here
	CString strFile;
	BOOL bRet;
	bRet = cmCommonDlg::OpenDialog(strFile,_T("Firmware(*.img),Loader(*.bin)|*.img;*.bin|All File(*.*)|*.*||"));
	if (!bRet)
	{
		SetDlgItemText(IDC_EDIT_LOADER,_T(""));
		return ;
	}
	SetDlgItemText(IDC_EDIT_LOADER,strFile);
}

void CWNpctoolDlg::OnSettingReset()
{
	// TODO: Add your command handler code here
	CMenu	*pMenu	= GetMenu();
	if (!pMenu)	return;
	m_Configs.bReboot = !m_Configs.bReboot;
	if (m_Configs.bReboot)
	{
		pMenu->CheckMenuItem(ID_SETTING_RESET, MF_CHECKED |MF_BYCOMMAND);
	}
	else
	{
		pMenu->CheckMenuItem(ID_SETTING_RESET, MF_UNCHECKED |MF_BYCOMMAND);
	}
	m_Configs.SaveToolSetting(std::wstring(TEXT("")));
}

void CWNpctoolDlg::OnUpdateSettingReset(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(TRUE);
}
