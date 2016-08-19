// BtMacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "BtMacDlg.h"
#include "cmNumString.h"
using namespace cm;


// CBtMacDlg dialog

IMPLEMENT_DYNAMIC(CBtMacDlg, CDialog)

CBtMacDlg::CBtMacDlg(CIniSettingBase &Config,CWnd* pParent /*=NULL*/)
	: CDialog(CBtMacDlg::IDD, pParent),m_Configs(Config)
{

}

CBtMacDlg::~CBtMacDlg()
{
}

void CBtMacDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBtMacDlg, CDialog)
END_MESSAGE_MAP()


// CBtMacDlg message handlers
VOID CBtMacDlg::UpdateInterface()
{
	SetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_START,m_Configs.BtMac.strStartMac.c_str());
	SetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_CURRENT,m_Configs.BtMac.strCurrentMac.c_str());
	SetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_END,m_Configs.BtMac.strEndMac.c_str());
	SetDlgItemText(IDC_EDIT_BTMAC_FILE_PATH,m_Configs.confPath.filePath[FLAG_BTMAC]);
	SetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_COUNT,(-1 == m_Configs.BtMac.nRemainCount)?_T(""):cmNumString::NumToStr(m_Configs.BtMac.nRemainCount,10));
	((CButton *)GetDlgItem(IDC_CHECK_BTMAC_SELECT))->SetCheck(m_Configs.BtMac.bEnable?BST_CHECKED:BST_UNCHECKED);
	if(MODE_AUTO == m_Configs.BtMac.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_AUTO_BTMAC  ))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_START)->EnableWindow(m_Configs.BtMac.bEnable);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_CURRENT)->EnableWindow(m_Configs.BtMac.bEnable);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_END)->EnableWindow(m_Configs.BtMac.bEnable);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_COUNT)->EnableWindow(m_Configs.BtMac.bEnable);
		GetDlgItem(IDC_BUTTON_BTMAC_FILE_PATH)->EnableWindow(FALSE);
	} else if (MODE_MANUAL == m_Configs.BtMac.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_MANUAL_BTMAC))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_BTMAC_FILE_PATH)->EnableWindow(FALSE);
	} else if (MODE_FILE == m_Configs.BtMac.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_FILE_BTMAC))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_BTMAC_FILE_PATH)->EnableWindow(m_Configs.BtMac.bEnable);
	}
#if 1
	GetDlgItem(IDC_BUTTON_BTMAC_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_BTMAC_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LABEL_BTMAC_FILE_PATH)->ShowWindow(SW_HIDE);
#endif
}
