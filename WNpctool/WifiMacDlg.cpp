// WifiMacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "WifiMacDlg.h"
#include "cmNumString.h"
using namespace cm;



// CWifiMacDlg dialog

IMPLEMENT_DYNAMIC(CWifiMacDlg, CDialog)

CWifiMacDlg::CWifiMacDlg(CIniSettingBase &Config,CWnd* pParent /*=NULL*/)
	: CDialog(CWifiMacDlg::IDD, pParent),m_Configs(Config)
{

}

CWifiMacDlg::~CWifiMacDlg()
{
}

void CWifiMacDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWifiMacDlg, CDialog)
END_MESSAGE_MAP()


// CWifiMacDlg message handlers
VOID CWifiMacDlg::UpdateInterface()
{
	SetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_START,m_Configs.WifiMac.strStartMac.c_str());
	SetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_CURRENT,m_Configs.WifiMac.strCurrentMac.c_str());
	SetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_END,m_Configs.WifiMac.strEndMac.c_str());
	SetDlgItemText(IDC_EDIT_WIFIMAC_FILE_PATH,m_Configs.confPath.filePath[FLAG_WIFIMAC]);
	SetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_COUNT,(-1 == m_Configs.WifiMac.nRemainCount)?_T(""):cmNumString::NumToStr(m_Configs.WifiMac.nRemainCount,10));
	((CButton *)GetDlgItem(IDC_CHECK_WIFIMAC_SELECT))->SetCheck(m_Configs.WifiMac.bEnable?BST_CHECKED:BST_UNCHECKED);
	if(MODE_AUTO == m_Configs.WifiMac.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_AUTO_WIFIMAC  ))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_START)->EnableWindow(m_Configs.WifiMac.bEnable);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_CURRENT)->EnableWindow(m_Configs.WifiMac.bEnable);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_END)->EnableWindow(m_Configs.WifiMac.bEnable);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_COUNT)->EnableWindow(m_Configs.WifiMac.bEnable);
		GetDlgItem(IDC_BUTTON_WIFIMAC_FILE_PATH)->EnableWindow(FALSE);
	} else if (MODE_MANUAL == m_Configs.WifiMac.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_MANUAL_WIFIMAC))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WIFIMAC_FILE_PATH)->EnableWindow(FALSE);
	} else if (MODE_FILE == m_Configs.WifiMac.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_FILE_WIFIMAC))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_WIFIMAC_FILE_PATH)->EnableWindow(m_Configs.WifiMac.bEnable);
	}
#if 1
	GetDlgItem(IDC_BUTTON_WIFIMAC_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_WIFIMAC_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LABEL_WIFIMAC_FILE_PATH)->ShowWindow(SW_HIDE);
#endif
}