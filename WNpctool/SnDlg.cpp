// SnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "SnDlg.h"
#include "cmNumString.h"
using namespace cm;


// CSnDlg dialog

IMPLEMENT_DYNAMIC(CSnDlg, CDialog)

CSnDlg::CSnDlg(CIniSettingBase &Config,CWnd* pParent /*=NULL*/)
	: CDialog(CSnDlg::IDD, pParent),m_Configs(Config)
{

}

CSnDlg::~CSnDlg()
{
}

void CSnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSnDlg, CDialog)
END_MESSAGE_MAP()


// CSnDlg message handlers
VOID CSnDlg::UpdateInterface()
{
	if(MODE_AUTO == m_Configs.devsn.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_AUTO_DEVSN  ))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_DEVSN_PREFIX)->EnableWindow(m_Configs.devsn.bEnable);
		GetDlgItem(IDC_EDIT_DEVSN_SUFFIX)->EnableWindow(m_Configs.devsn.bEnable);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_START)->EnableWindow(m_Configs.devsn.bEnable);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_CURRENT)->EnableWindow(m_Configs.devsn.bEnable);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_END)->EnableWindow(m_Configs.devsn.bEnable);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_COUNT)->EnableWindow(m_Configs.devsn.bEnable);
		GetDlgItem(IDC_EDIT_DEVSN_FILE_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DEVSN_FILE_PATH)->EnableWindow(FALSE);
	} else if (MODE_MANUAL == m_Configs.devsn.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_MANUAL_DEVSN))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_DEVSN_PREFIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SUFFIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DEVSN_FILE_PATH)->EnableWindow(FALSE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_FILE_SN))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_DEVSN_PREFIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SUFFIX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DEVSN_FILE_PATH)->EnableWindow(m_Configs.devsn.bEnable);
	}
	((CButton *)GetDlgItem(IDC_CHECK_SN_SELECT))->SetCheck(m_Configs.devsn.bEnable?BST_CHECKED:BST_UNCHECKED);
	SetDlgItemText(IDC_EDIT_DEVSN_PREFIX,m_Configs.devsn.strPrefix.c_str());
	SetDlgItemText(IDC_EDIT_DEVSN_SUFFIX,m_Configs.devsn.strSuffix.c_str());
	SetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_START,m_Configs.devsn.strStartSn.c_str());
	SetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_CURRENT,m_Configs.devsn.strCurrentSn.c_str());
	SetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_END,m_Configs.devsn.strEndSn.c_str());
	SetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_COUNT,(-1 == m_Configs.devsn.nRemainCount)?_T(""):cmNumString::NumToStr(m_Configs.devsn.nRemainCount,10));
#if 1
	GetDlgItem(IDC_BUTTON_DEVSN_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_DEVSN_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LABEL_DEVSN_FILE_PATH)->ShowWindow(SW_HIDE);
#endif
}