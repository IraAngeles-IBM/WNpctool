// SnDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "SnDlg.h"
#include "cmNumString.h"
using namespace cm;


// CSnDlg dialog

IMPLEMENT_DYNAMIC(CSnDlg, CDialog)

CSnDlg::CSnDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent /*=NULL*/)
	: CDialog(CSnDlg::IDD, pParent),m_Configs(Config),m_LocalLang(LocalLang)
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
	ON_BN_CLICKED(IDC_RADIO_MANUAL_DEVSN, &CSnDlg::OnBnClickedRadioManualDevsn)
	ON_BN_CLICKED(IDC_RADIO_AUTO_DEVSN, &CSnDlg::OnBnClickedRadioAutoDevsn)
	ON_BN_CLICKED(IDC_CHECK_SN_SELECT, &CSnDlg::OnBnClickedCheckSnSelect)
	ON_EN_SETFOCUS(IDC_EDIT_DEVSN_SEGMENT_COUNT, &CSnDlg::OnEnSetfocusEditDevsnSegmentCount)
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
	GetDlgItem(IDC_RADIO_FILE_SN)->ShowWindow(SW_HIDE);
#endif
}
BOOL CSnDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_LocalLang.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,false);
	UpdateInterface();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSnDlg::OnBnClickedRadioManualDevsn()
{
	// TODO: Add your control notification handler code here
	m_Configs.devsn.nAutoMode = MODE_MANUAL;
	GetDlgItem(IDC_EDIT_DEVSN_PREFIX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DEVSN_SUFFIX)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_CURRENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_COUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DEVSN_FILE_PATH)->EnableWindow(FALSE);
	//UpdateInterface();
}

void CSnDlg::OnBnClickedRadioAutoDevsn()
{
	// TODO: Add your control notification handler code here
	m_Configs.devsn.nAutoMode = MODE_AUTO;
	GetDlgItem(IDC_EDIT_DEVSN_PREFIX)->EnableWindow(m_Configs.devsn.bEnable);
	GetDlgItem(IDC_EDIT_DEVSN_SUFFIX)->EnableWindow(m_Configs.devsn.bEnable);
	GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_START)->EnableWindow(m_Configs.devsn.bEnable);
	GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_CURRENT)->EnableWindow(m_Configs.devsn.bEnable);
	GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_END)->EnableWindow(m_Configs.devsn.bEnable);
	GetDlgItem(IDC_EDIT_DEVSN_SEGMENT_COUNT)->EnableWindow(m_Configs.devsn.bEnable);
	GetDlgItem(IDC_EDIT_DEVSN_FILE_PATH)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DEVSN_FILE_PATH)->EnableWindow(FALSE);
	//UpdateInterface();
}
BOOL CSnDlg::OnSaveConfig()
{
	CString strValue,strValue2;
	CString strPrompt;
	BOOL    bResult=FALSE;
	UpdateData(TRUE);
	m_Configs.devsn.bEnable =(BST_CHECKED&((CButton *)GetDlgItem(IDC_CHECK_SN_SELECT))->GetCheck()) ?TRUE:FALSE;
	if (m_Configs.devsn.bEnable) {
		GetDlgItemText(IDC_EDIT_DEVSN_PREFIX,strValue);
		GetDlgItemText(IDC_EDIT_DEVSN_SUFFIX,strValue2);
		m_Configs.devsn.strPrefix = strValue.GetString();
		m_Configs.devsn.strSuffix = strValue2.GetString();
		if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_MANUAL_DEVSN))->GetCheck())
		{
			m_Configs.devsn.nAutoMode = MODE_MANUAL;
		}
		else if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_AUTO_DEVSN))->GetCheck())
		{
			m_Configs.devsn.nAutoMode = MODE_AUTO;
		}
		else
		{
			m_Configs.devsn.nAutoMode = MODE_FILE;
		}
		if (MODE_AUTO == m_Configs.devsn.nAutoMode) {
			GetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_START,strValue);
			m_Configs.devsn.strStartSn = strValue;
			GetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_END,strValue);
			m_Configs.devsn.strEndSn = strValue;
			GetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_CURRENT,strValue);
			m_Configs.devsn.strCurrentSn = strValue;
			if (!(CompareNumString(m_Configs.devsn.strEndSn.c_str(),m_Configs.devsn.strCurrentSn.c_str())&&
				CompareNumString(m_Configs.devsn.strCurrentSn.c_str(),m_Configs.devsn.strStartSn.c_str())&&
				CompareNumString(m_Configs.devsn.strEndSn.c_str(),m_Configs.devsn.strStartSn.c_str())))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT")).c_str(),TEXT("DEV SN"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				return bResult;					
			}
			strValue = m_Configs.devsn.strEndSn.c_str();
			strValue2 = m_Configs.devsn.strCurrentSn.c_str();
			m_Configs.devsn.nRemainCount    = cmNumString::StrToSLong(strValue.Right(6),16) - cmNumString::StrToSLong(strValue2.Right(6),16) + 1;
			m_Configs.devsn.nSnCount    = m_Configs.devsn.nRemainCount;
		}
	}

	bResult = TRUE;
	return bResult;
}
std::wstring CSnDlg::GetLocalString(std::wstring strKey)
{
	return m_LocalLang.GetLanStr(strKey);
}
bool CSnDlg::CompareNumString(CString strMore,CString strLess)
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
void CSnDlg::OnBnClickedCheckSnSelect()
{
	// TODO: Add your control notification handler code here
	m_Configs.devsn.bEnable = !m_Configs.devsn.bEnable;
	UpdateInterface();
}

void CSnDlg::OnEnSetfocusEditDevsnSegmentCount()
{
	// TODO: Add your control notification handler code here
	CString strStartSN,strCurrentSN,strEndSN,strPrompt;
	int nCount;
	GetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_START,strStartSN);
	GetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_CURRENT,strStartSN);
	GetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_END,strEndSN);

	nCount = cmNumString::StrToSLong(strEndSN.Right(6),16) - cmNumString::StrToSLong(strStartSN.Right(6),16) + 1;
	SetDlgItemText(IDC_EDIT_DEVSN_SEGMENT_COUNT,(-1 == nCount)?_T("0"):cmNumString::NumToStr(nCount,10));
}
