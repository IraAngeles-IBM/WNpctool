// LanMacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "LanMacDlg.h"
#include "cmfuns.h"
#include "cmNumString.h"
using namespace cm;

// CLanMacDlg dialog

IMPLEMENT_DYNAMIC(CLanMacDlg, CDialog)

CLanMacDlg::CLanMacDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent /*=NULL*/)
	: CDialog(CLanMacDlg::IDD, pParent),m_Configs(Config),m_LocalLang(LocalLang)
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
	ON_BN_CLICKED(IDC_RADIO_MANUAL_LANMAC, &CLanMacDlg::OnBnClickedRadioManualLanmac)
	ON_BN_CLICKED(IDC_RADIO_AUTO_LANMAC, &CLanMacDlg::OnBnClickedRadioAutoLanmac)
	ON_BN_CLICKED(IDC_CHECK_LANMAC_SELECT, &CLanMacDlg::OnBnClickedCheckLanmacSelect)
	ON_EN_SETFOCUS(IDC_EDIT_LANMAC_SEGMENT_COUNT, &CLanMacDlg::OnEnSetfocusEditLanmacSegmentCount)
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
	GetDlgItem(IDC_RADIO_FILE_LANMAC)->ShowWindow(SW_HIDE);
#endif
}
BOOL CLanMacDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_LocalLang.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,false);
	UpdateInterface();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLanMacDlg::OnBnClickedRadioManualLanmac()
{
	// TODO: Add your control notification handler code here
	m_Configs.LanMac.nAutoMode = MODE_MANUAL;
	GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_CURRENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_COUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LANMAC_FILE_PATH)->EnableWindow(FALSE);
	//UpdateInterface();
}

void CLanMacDlg::OnBnClickedRadioAutoLanmac()
{
	// TODO: Add your control notification handler code here
	m_Configs.LanMac.nAutoMode = MODE_AUTO;
	GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_START)->EnableWindow(m_Configs.LanMac.bEnable);
	GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_CURRENT)->EnableWindow(m_Configs.LanMac.bEnable);
	GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_END)->EnableWindow(m_Configs.LanMac.bEnable);
	GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_COUNT)->EnableWindow(m_Configs.LanMac.bEnable);
	GetDlgItem(IDC_BUTTON_LANMAC_FILE_PATH)->EnableWindow(FALSE);
	//UpdateInterface();
}
BOOL CLanMacDlg::OnSaveConfig()
{
	CString strValue,strValue2;
	CString strStartMac,strCurrentMac,strEndMac;
	CString strPrompt;
	BOOL    bResult=FALSE;
	UpdateData(TRUE);
	m_Configs.LanMac.bEnable =(BST_CHECKED&((CButton *)GetDlgItem(IDC_CHECK_LANMAC_SELECT))->GetCheck()) ?TRUE:FALSE;
	if (m_Configs.LanMac.bEnable) {
		if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_MANUAL_LANMAC))->GetCheck())
		{
			m_Configs.LanMac.nAutoMode = MODE_MANUAL;
		}
		else if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_AUTO_LANMAC))->GetCheck())
		{
			m_Configs.LanMac.nAutoMode = MODE_AUTO;
		}
		else
		{
			m_Configs.LanMac.nAutoMode = MODE_FILE;
		}
		if (MODE_AUTO == m_Configs.LanMac.nAutoMode) {
			GetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_START,strValue);
			strStartMac = strValue;
			GetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_END,strValue);
			strEndMac = strValue;
			GetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_CURRENT,strValue);
			strCurrentMac = strValue;
			if (!CheckMacStr(strStartMac))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT_START")).c_str(),TEXT("LAN MAC"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_START)->SetFocus();
				return bResult;
			}
			if (!CheckMacStr(strCurrentMac))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT_CURRENT")).c_str(),TEXT("LAN MAC"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_CURRENT)->SetFocus();
				return bResult;
			}
			if (!CheckMacStr(strEndMac))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT_CURRENT")).c_str(),TEXT("LAN MAC"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				GetDlgItem(IDC_EDIT_LANMAC_SEGMENT_END)->SetFocus();
				return bResult;
			}

			if (!(CompareNumString(strEndMac,strCurrentMac)&&
				CompareNumString(strCurrentMac,strStartMac)&&
				CompareNumString(strEndMac,strStartMac)))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_MAC_SEGMENT")).c_str(),TEXT("LAN MAC"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				return bResult;					
			}
			m_Configs.LanMac.strStartMac = strStartMac;
			m_Configs.LanMac.strCurrentMac = strCurrentMac;
			m_Configs.LanMac.strEndMac = strEndMac;
			strValue = m_Configs.LanMac.strEndMac.c_str();
			strValue2 = m_Configs.LanMac.strCurrentMac.c_str();
			m_Configs.LanMac.nRemainCount    = cmNumString::StrToSLong(strValue.Right(6),16) - cmNumString::StrToSLong(strValue2.Right(6),16) + 1;
			m_Configs.LanMac.nCount    = m_Configs.LanMac.nRemainCount;
		}
	}
	return TRUE;
}
std::wstring CLanMacDlg::GetLocalString(std::wstring strKey)
{
	return m_LocalLang.GetLanStr(strKey);
}

bool CLanMacDlg::CompareNumString(CString strMore,CString strLess)
{
	int nCount;
	CString stemp = strMore.Left(6);
	nCount = cmNumString::StrToSLong(strMore.Left(6),16) - cmNumString::StrToSLong(strLess.Left(6),16);
	if (nCount < 0)
	{
		return false;
	}

	stemp = strMore.Right(6);
	nCount = cmNumString::StrToSLong(strMore.Right(6),16) - cmNumString::StrToSLong(strLess.Right(6),16);
	if (nCount < 0)
	{
		return false;
	}
	return true;
}
void CLanMacDlg::OnBnClickedCheckLanmacSelect()
{
	// TODO: Add your control notification handler code here
	m_Configs.LanMac.bEnable = !m_Configs.LanMac.bEnable;
	UpdateInterface();
}

void CLanMacDlg::OnEnSetfocusEditLanmacSegmentCount()
{
	// TODO: Add your control notification handler code here
	CString strStartMac,strCurrentMac,strEndMac,strPrompt;
	int nCount;
	GetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_START,strStartMac);
	GetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_CURRENT,strCurrentMac);
	GetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_END,strEndMac);

	nCount = cmNumString::StrToSLong(strEndMac.Right(6),16) - cmNumString::StrToSLong(strCurrentMac.Right(6),16) + 1;
	SetDlgItemText(IDC_EDIT_LANMAC_SEGMENT_COUNT,(-1 == nCount)?_T("0"):cmNumString::NumToStr(nCount,10));
}
