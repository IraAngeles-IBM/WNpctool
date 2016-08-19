// LanMacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "LanMacDlg.h"
#include "cmNumString.h"
using namespace cm;


// CLanMacDlg dialog

IMPLEMENT_DYNAMIC(CLanMacDlg, CDialog)

CLanMacDlg::CLanMacDlg(CIniSettingBase &Config,CWnd* pParent /*=NULL*/)
	: CDialog(CLanMacDlg::IDD, pParent),m_Configs(Config)
{

}

CLanMacDlg::~CLanMacDlg()
{
}

void CLanMacDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLanMacDlg, CDialog)
END_MESSAGE_MAP()


// CLanMacDlg message handlers
VOID CLanMacDlg::UpdateInterface()
{
	SetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_START,m_Configs.LanMac.strStartMac.c_str());
	SetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_CURRENT,m_Configs.LanMac.strCurrentMac.c_str());
	SetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_END,m_Configs.LanMac.strEndMac.c_str());
	SetDlgItemText(IDC_EDIT_LANMAC_FILE_PATH,m_Configs.confPath.filePath[FLAG_BTMAC]);
	SetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_COUNT,(-1 == m_Configs.LanMac.nRemainCount)?_T(""):cmNumString::NumToStr(m_Configs.LanMac.nRemainCount,10));
	((CButton *)GetDlgItem(IDC_CHECK_LANMAC_SELECT))->SetCheck(m_Configs.LanMac.bEnable?BST_CHECKED:BST_UNCHECKED);
	if(MODE_AUTO == m_Configs.LanMac.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_AUTO_LANMAC  ))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_START)->EnableWindow(m_Configs.LanMac.bEnable);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_CURRENT)->EnableWindow(m_Configs.LanMac.bEnable);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_END)->EnableWindow(m_Configs.LanMac.bEnable);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_COUNT)->EnableWindow(m_Configs.LanMac.bEnable);
		GetDlgItem(IDC_BUTTON_LANMAC_FILE_PATH)->EnableWindow(FALSE);
	} else if (MODE_MANUAL == m_Configs.LanMac.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_MANUAL_LANMAC))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LANMAC_FILE_PATH)->EnableWindow(FALSE);
	} else if (MODE_FILE == m_Configs.LanMac.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_FILE_LANMAC))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LANMAC_FILE_PATH)->EnableWindow(m_Configs.LanMac.bEnable);
	}
#if 1
	GetDlgItem(IDC_BUTTON_LANMAC_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_LANMAC_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LABEL_LANMAC_FILE_PATH)->ShowWindow(SW_HIDE);
#endif
}