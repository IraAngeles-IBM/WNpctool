// ConfigMode.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "ConfigMode.h"


// CConfigMode dialog

IMPLEMENT_DYNAMIC(CConfigMode, CDialog)

CConfigMode::CConfigMode(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent /*=NULL*/)
	: CDialog(CConfigMode::IDD, pParent),m_Configs(Config),m_LocalLang(LocalLang),m_SnDlg(Config,LocalLang),
	m_WifiMacDlg(Config,LocalLang),m_BtMacDlg(Config,LocalLang),m_LanMacDlg(Config,LocalLang)
{

}

CConfigMode::~CConfigMode()
{
}

void CConfigMode::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_SNMAC, m_TabMode);
}


BEGIN_MESSAGE_MAP(CConfigMode, CDialog)
	ON_BN_CLICKED(IDC_BTN_MODE_APPLY, &CConfigMode::OnBnClickedBtnModeApply)
	ON_BN_CLICKED(IDC_BTN_MODE_OK, &CConfigMode::OnBnClickedBtnModeOk)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SNMAC, &CConfigMode::OnTcnSelchangeTabSnmac)
END_MESSAGE_MAP()


// CConfigMode message handlers
BOOL CConfigMode::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect   rc;
	memset(m_ChildCWnds,0,sizeof(CDialog*)* 5);
	m_TabMode.InsertItem(0,_T("SN"));
	m_TabMode.InsertItem(1,_T("WIFI MAC"));
	m_TabMode.InsertItem(2,_T("LAN MAC"));
	m_TabMode.InsertItem(3,_T("BT MAC"));
	m_LocalLang.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,false);

	m_SnDlg.Create(IDD_DIALOG_SN,&m_TabMode);
	m_WifiMacDlg.Create(IDD_DIALOG_WIFIMAC,&m_TabMode);
	m_LanMacDlg.Create(IDD_DIALOG_LANMAC,&m_TabMode);
	m_BtMacDlg.Create(IDD_DIALOG_BTMAC,&m_TabMode);
	GetClientRect(rc);
	m_ChildCWnds[0] = &m_SnDlg;
	m_ChildCWnds[1] = &m_WifiMacDlg;
	m_ChildCWnds[2] = &m_LanMacDlg;
	m_ChildCWnds[3] = &m_BtMacDlg;
	OnSizeCtl( rc.right - rc.left ,rc.bottom - rc.top);
	m_iCurSelTab    = 0;
	m_ChildCWnds[m_iCurSelTab]->ShowWindow(SW_SHOW);
	//CFont   font;
	//font.CreateFont(-13,15,0,0,FW_BOLD,FALSE,FALSE,0,  
	//	ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,  
	//	DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,_T("ËÎÌו"));
	//GetDlgItem(IDC_BTN_MODE_OK)->SetFont(&font);
	//GetDlgItem(IDC_BTN_MODE_CANCEL)->SetFont(&font);
	//GetDlgItem(IDC_BTN_MODE_APPLY)->SetFont(&font);
	//font.Detach();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

VOID CConfigMode::UpdateInterface()
{
	m_SnDlg.UpdateInterface();
	m_WifiMacDlg.UpdateInterface();
	m_BtMacDlg.UpdateInterface();
	m_LanMacDlg.UpdateInterface();
}

void CConfigMode::OnSizeCtl( int cx, int cy)
{
	int i;
	if (::IsWindow(m_TabMode.GetSafeHwnd())) {
		m_TabMode.MoveWindow(10,10,cx - 20 ,cy - 60,TRUE);
		for( i = 0; i < sizeof(m_ChildCWnds)/sizeof(m_ChildCWnds[0]); i++ ) {
			if(m_ChildCWnds[i]) m_ChildCWnds[i]->MoveWindow(0,20,cx - 20,cy - 80 ,TRUE);
		}
	}
}
void CConfigMode::OnBnClickedBtnModeApply()
{
	// TODO: Add your control notification handler code here
	if(!m_SnDlg.OnSaveConfig()) {
		return ;
	}
	if(!m_WifiMacDlg.OnSaveConfig()) {
		return ;
	}
	if(!m_BtMacDlg.OnSaveConfig()) {
		return ;
	}
	if(!m_LanMacDlg.OnSaveConfig()) {
		return ;
	}
}

void CConfigMode::OnBnClickedBtnModeOk()
{
	// TODO: Add your control notification handler code here
	if(!m_SnDlg.OnSaveConfig()) {
		return ;
	}
	if(!m_WifiMacDlg.OnSaveConfig()) {
		return ;
	}
	if(!m_BtMacDlg.OnSaveConfig()) {
		return ;
	}
	if(!m_LanMacDlg.OnSaveConfig()) {
		return ;
	}
	CDialog::OnOK();
}

void CConfigMode::OnTcnSelchangeTabSnmac(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int     iNewSel = m_TabMode.GetCurSel();
	if(m_ChildCWnds[iNewSel]) {
		m_ChildCWnds[m_iCurSelTab]->ShowWindow(SW_HIDE);
		m_iCurSelTab = iNewSel;
		m_ChildCWnds[m_iCurSelTab]->ShowWindow(SW_SHOW);
	}else {
		//m_tabconfsw.SetCurSel(m_iCurSelTab);
	}
}
