// ImeiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "ImeiDlg.h"
#include "cmfuns.h"
#include "cmNumString.h"
using namespace cm;

// ImeiDlg dialog

IMPLEMENT_DYNAMIC(ImeiDlg, CDialog)

ImeiDlg::ImeiDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent /*=NULL*/)
	: CDialog(ImeiDlg::IDD, pParent),m_Configs(Config),m_LocalLang(LocalLang)
{

}

ImeiDlg::~ImeiDlg()
{
}

void ImeiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ImeiDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_IMEI_SELECT, &ImeiDlg::OnBnClickedCheckImeiSelect)
	ON_BN_CLICKED(IDC_RADIO_MANUAL_IMEI, &ImeiDlg::OnBnClickedRadioManualImei)
	ON_BN_CLICKED(IDC_RADIO_AUTO_IMEI, &ImeiDlg::OnBnClickedRadioAutoImei)
	ON_EN_SETFOCUS(IDC_EDIT_IMEI_SEGMENT_COUNT, &ImeiDlg::OnEnSetfocusEditImeiSegmentCount)
END_MESSAGE_MAP()


// ImeiDlg message handlers

void ImeiDlg::OnBnClickedCheckImeiSelect()
{
	// TODO: Add your control notification handler code here
	m_Configs.Imei.bEnable = !m_Configs.Imei.bEnable;
	UpdateInterface();
}
VOID ImeiDlg::UpdateInterface()
{
	SetDlgItemText(IDC_EDIT_IMEI_SEGMENT_START,m_Configs.Imei.strStartImei1.c_str());
	SetDlgItemText(IDC_EDIT_IMEI_SEGMENT_CURRENT,m_Configs.Imei.strCurrentImei1.c_str());
	SetDlgItemText(IDC_EDIT_IMEI_SEGMENT_END,m_Configs.Imei.strEndImei1.c_str());
	SetDlgItemText(IDC_EDIT_IMEI_FILE_PATH,m_Configs.confPath.filePath[FLAG_IMEI1]);
	SetDlgItemText(IDC_EDIT_IMEI_SEGMENT_COUNT,(-1 == m_Configs.Imei.nRemainCount1)?_T(""):cmNumString::NumToStr(m_Configs.Imei.nRemainCount1,10));
	((CButton *)GetDlgItem(IDC_CHECK_IMEI_SELECT))->SetCheck(m_Configs.Imei.bEnable?BST_CHECKED:BST_UNCHECKED);
	if(MODE_AUTO == m_Configs.Imei.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_AUTO_IMEI  ))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_START)->EnableWindow(m_Configs.Imei.bEnable);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_CURRENT)->EnableWindow(m_Configs.Imei.bEnable);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_END)->EnableWindow(m_Configs.Imei.bEnable);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_COUNT)->EnableWindow(m_Configs.Imei.bEnable);
		GetDlgItem(IDC_BUTTON_IMEI_FILE_PATH)->EnableWindow(FALSE);
	} else if (MODE_MANUAL == m_Configs.Imei.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_MANUAL_IMEI))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IMEI_FILE_PATH)->EnableWindow(FALSE);
	} else if (MODE_FILE == m_Configs.Imei.nAutoMode) {
		((CButton *)GetDlgItem(IDC_RADIO_FILE_IMEI))->SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_CURRENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_END)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_IMEI_SEGMENT_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_IMEI_FILE_PATH)->EnableWindow(m_Configs.Imei.bEnable);
	}
#if 1
	GetDlgItem(IDC_BUTTON_IMEI_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_IMEI_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LABEL_IMEI_FILE_PATH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO_FILE_IMEI)->ShowWindow(SW_HIDE);
#endif
}
BOOL ImeiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_LocalLang.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,false);
	UpdateInterface();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ImeiDlg::OnBnClickedRadioManualImei()
{
	// TODO: Add your control notification handler code here
	m_Configs.Imei.nAutoMode = MODE_MANUAL;
	GetDlgItem(IDC_EDIT_IMEI_SEGMENT_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_IMEI_SEGMENT_CURRENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_IMEI_SEGMENT_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_IMEI_SEGMENT_COUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_IMEI_FILE_PATH)->EnableWindow(FALSE);
}

void ImeiDlg::OnBnClickedRadioAutoImei()
{
	// TODO: Add your control notification handler code here
	m_Configs.Imei.nAutoMode = MODE_AUTO;
	GetDlgItem(IDC_EDIT_IMEI_SEGMENT_START)->EnableWindow(m_Configs.Imei.bEnable);
	GetDlgItem(IDC_EDIT_IMEI_SEGMENT_CURRENT)->EnableWindow(m_Configs.Imei.bEnable);
	GetDlgItem(IDC_EDIT_IMEI_SEGMENT_END)->EnableWindow(m_Configs.Imei.bEnable);
	GetDlgItem(IDC_EDIT_IMEI_SEGMENT_COUNT)->EnableWindow(m_Configs.Imei.bEnable);
	GetDlgItem(IDC_BUTTON_IMEI_FILE_PATH)->EnableWindow(FALSE);
}

void ImeiDlg::OnEnSetfocusEditImeiSegmentCount()
{
	// TODO: Add your control notification handler code here
	CString strStartImei,strCurrentImei,strEndImei,strPrompt;
	int nCount;
	GetDlgItemText(IDC_EDIT_IMEI_SEGMENT_START,strStartImei);
	GetDlgItemText(IDC_EDIT_IMEI_SEGMENT_CURRENT,strCurrentImei);
	GetDlgItemText(IDC_EDIT_IMEI_SEGMENT_END,strEndImei);

	nCount = cmNumString::StrToSLong(strEndImei.Right(6),10) - cmNumString::StrToSLong(strStartImei.Right(6),10) + 1;
	SetDlgItemText(IDC_EDIT_IMEI_SEGMENT_COUNT,(-1 == nCount)?_T("0"):cmNumString::NumToStr(nCount,10));
}

BOOL ImeiDlg::OnSaveConfig()
{
	CString strValue,strValue2;
	CString strStartImei,strCurrentImei,strEndImei;
	CString strPrompt;
	BOOL    bResult=FALSE;
	UpdateData(TRUE);
	m_Configs.Imei.bEnable =(BST_CHECKED&((CButton *)GetDlgItem(IDC_CHECK_IMEI_SELECT))->GetCheck()) ?TRUE:FALSE;
	if (m_Configs.Imei.bEnable) {
		if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_MANUAL_IMEI))->GetCheck())
		{
			m_Configs.Imei.nAutoMode = MODE_MANUAL;
		}
		else if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_AUTO_IMEI))->GetCheck())
		{
			m_Configs.Imei.nAutoMode = MODE_AUTO;
		}
		else
		{
			m_Configs.Imei.nAutoMode = MODE_FILE;
		}
		if (MODE_AUTO == m_Configs.Imei.nAutoMode) {
			GetDlgItemText(IDC_EDIT_IMEI_SEGMENT_START,strValue);
			strStartImei = strValue;
			GetDlgItemText(IDC_EDIT_IMEI_SEGMENT_END,strValue);
			strEndImei = strValue;
			GetDlgItemText(IDC_EDIT_IMEI_SEGMENT_CURRENT,strValue);
			strCurrentImei = strValue;

			if (!CheckImeiStr(strStartImei))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT_START")).c_str(),TEXT("IMEI"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				GetDlgItem(IDC_EDIT_IMEI_SEGMENT_START)->SetFocus();
				return bResult;
			}
			if (!CheckImeiStr(strCurrentImei))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT_CURRENT")).c_str(),TEXT("IMEI"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				GetDlgItem(IDC_EDIT_IMEI_SEGMENT_CURRENT)->SetFocus();
				return bResult;
			}
			if (!CheckImeiStr(strEndImei))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_SS_SEGMENT_CURRENT")).c_str(),TEXT("IMEI"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				GetDlgItem(IDC_EDIT_IMEI_SEGMENT_END)->SetFocus();
				return bResult;
			}

			if (!(CompareNumString(strEndImei,strCurrentImei)&&
				CompareNumString(strCurrentImei,strStartImei)&&
				CompareNumString(strEndImei,strStartImei)))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_MAC_SEGMENT")).c_str(),TEXT("IMEI"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				return bResult;					
			};
			m_Configs.Imei.strStartImei1 = strStartImei;
			m_Configs.Imei.strCurrentImei1 = strCurrentImei;
			m_Configs.Imei.strEndImei1 = strEndImei;
			strValue = m_Configs.Imei.strEndImei1.c_str();
			strValue2 = m_Configs.Imei.strCurrentImei1.c_str();
			m_Configs.Imei.nRemainCount1    = cmNumString::StrToSLong(strValue.Right(6),10) - cmNumString::StrToSLong(strValue2.Right(6),10) + 1;
			m_Configs.Imei.nCount1    = m_Configs.Imei.nRemainCount1;
		}
	}
	return TRUE;
}

std::wstring ImeiDlg::GetLocalString(std::wstring strKey)
{
	return m_LocalLang.GetLanStr(strKey);
}

bool ImeiDlg::CompareNumString(CString strMore,CString strLess)
{
	int nCount;
	CString stemp = strMore.Left(6);
	nCount = cmNumString::StrToSLong(strMore.Left(6),10) - cmNumString::StrToSLong(strLess.Left(6),10);
	if (nCount < 0)
	{
		return false;
	}

	stemp = strMore.Right(6);
	nCount = cmNumString::StrToSLong(strMore.Right(6),10) - cmNumString::StrToSLong(strLess.Right(6),10);
	if (nCount < 0)
	{
		return false;
	}
	return true;
}