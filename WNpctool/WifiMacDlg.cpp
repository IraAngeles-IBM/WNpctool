// WifiMacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "WifiMacDlg.h"
#include "cmfuns.h"
#include "cmNumString.h"
using namespace cm;

// CWifiMacDlg dialog

IMPLEMENT_DYNAMIC(CWifiMacDlg, CDialog)

CWifiMacDlg::CWifiMacDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent /*=NULL*/)
	: CDialog(CWifiMacDlg::IDD, pParent),m_Configs(Config),m_LocalLang(LocalLang)
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
	ON_BN_CLICKED(IDC_RADIO_MANUAL_WIFIMAC, &CWifiMacDlg::OnBnClickedRadioManualWifimac)
	ON_BN_CLICKED(IDC_RADIO_AUTO_WIFIMAC, &CWifiMacDlg::OnBnClickedRadioAutoWifimac)
	ON_BN_CLICKED(IDC_CHECK_WIFIMAC_SELECT, &CWifiMacDlg::OnBnClickedCheckWifimacSelect)
	ON_EN_SETFOCUS(IDC_EDIT_WIFIMAC_SEGMENT_COUNT, &CWifiMacDlg::OnEnSetfocusEditWifimacSegmentCount)
	ON_BN_CLICKED(IDC_RADIO_FILE_WIFIMAC, &CWifiMacDlg::OnBnClickedRadioFileWifimac)
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
	GetDlgItem(IDC_RADIO_FILE_WIFIMAC)->ShowWindow(SW_HIDE);
#endif
}
BOOL CWifiMacDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_LocalLang.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,false);
	UpdateInterface();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CWifiMacDlg::OnBnClickedRadioManualWifimac()
{
	// TODO: Add your control notification handler code here
	m_Configs.WifiMac.nAutoMode = MODE_MANUAL;
	GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_CURRENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_COUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_WIFIMAC_FILE_PATH)->EnableWindow(FALSE);
	//UpdateInterface();
}

void CWifiMacDlg::OnBnClickedRadioAutoWifimac()
{
	// TODO: Add your control notification handler code here
	m_Configs.WifiMac.nAutoMode = MODE_AUTO;
	GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_START)->EnableWindow(m_Configs.WifiMac.bEnable);
	GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_CURRENT)->EnableWindow(m_Configs.WifiMac.bEnable);
	GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_END)->EnableWindow(m_Configs.WifiMac.bEnable);
	GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_COUNT)->EnableWindow(m_Configs.WifiMac.bEnable);
	GetDlgItem(IDC_BUTTON_WIFIMAC_FILE_PATH)->EnableWindow(FALSE);
	//UpdateInterface();
}
BOOL CWifiMacDlg::OnSaveConfig()
{
	CString strValue,strValue2;
	CString strStartMac,strCurrentMac,strEndMac;
	CString strPrompt;
	BOOL    bResult=FALSE;
	UpdateData(TRUE);
	m_Configs.WifiMac.bEnable =(BST_CHECKED&((CButton *)GetDlgItem(IDC_CHECK_WIFIMAC_SELECT))->GetCheck()) ?TRUE:FALSE;
	if (m_Configs.WifiMac.bEnable) {
		if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_MANUAL_WIFIMAC))->GetCheck())
		{
			m_Configs.WifiMac.nAutoMode = MODE_MANUAL;
		}
		else if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_AUTO_WIFIMAC))->GetCheck())
		{
			m_Configs.WifiMac.nAutoMode = MODE_AUTO;
		}
		else
		{
			m_Configs.WifiMac.nAutoMode = MODE_FILE;
		}
		if (MODE_AUTO == m_Configs.WifiMac.nAutoMode) {
			GetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_START,strValue);
			strStartMac = strValue;
			GetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_END,strValue);
			strEndMac = strValue;
			GetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_CURRENT,strValue);
			strCurrentMac = strValue;

			if (!CheckMacStr(strStartMac))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT_START")).c_str(),TEXT("WIFI MAC"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_START)->SetFocus();
				return bResult;
			}
			if (!CheckMacStr(strCurrentMac))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT_CURRENT")).c_str(),TEXT("WIFI MAC"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_CURRENT)->SetFocus();
				return bResult;
			}
			if (!CheckMacStr(strEndMac))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT_CURRENT")).c_str(),TEXT("WIFI MAC"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				GetDlgItem(IDC_EDIT_WIFIMAC_SEGMENT_END)->SetFocus();
				return bResult;
			}

			if (!(CompareNumString(strEndMac,strCurrentMac)&&
				CompareNumString(strCurrentMac,strStartMac)&&
				CompareNumString(strEndMac,strStartMac)))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_MAC_SEGMENT")).c_str(),TEXT("WIFI MAC"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				return bResult;					
			}
			m_Configs.WifiMac.strStartMac = strStartMac;
			m_Configs.WifiMac.strCurrentMac = strCurrentMac;
			m_Configs.WifiMac.strEndMac = strEndMac;
			strValue = m_Configs.WifiMac.strEndMac.c_str();
			strValue2 = m_Configs.WifiMac.strCurrentMac.c_str();
			m_Configs.WifiMac.nRemainCount    = cmNumString::StrToSLong(strValue.Right(6),16) - cmNumString::StrToSLong(strValue2.Right(6),16) + 1;
			m_Configs.WifiMac.nCount    = m_Configs.WifiMac.nRemainCount;
		}
	}
	return TRUE;
}
std::wstring CWifiMacDlg::GetLocalString(std::wstring strKey)
{
	return m_LocalLang.GetLanStr(strKey);
}
bool CWifiMacDlg::CompareNumString(CString strMore,CString strLess)
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
void CWifiMacDlg::OnBnClickedCheckWifimacSelect()
{
	// TODO: Add your control notification handler code here
	m_Configs.WifiMac.bEnable = !m_Configs.WifiMac.bEnable;
	UpdateInterface();
}

void CWifiMacDlg::OnEnSetfocusEditWifimacSegmentCount()
{
	// TODO: Add your control notification handler code here
	CString strStartMac,strCurrentMac,strEndMac,strPrompt;
	int nCount;
	GetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_START,strStartMac);
	GetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_CURRENT,strCurrentMac);
	GetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_END,strEndMac);

	nCount = cmNumString::StrToSLong(strEndMac.Right(6),16) - cmNumString::StrToSLong(strCurrentMac.Right(6),16) + 1;
	SetDlgItemText(IDC_EDIT_WIFIMAC_SEGMENT_COUNT,(-1 == nCount)?_T("0"):cmNumString::NumToStr(nCount,10));
}

void CWifiMacDlg::OnBnClickedRadioFileWifimac()
{
	// TODO: Add your control notification handler code here
}
