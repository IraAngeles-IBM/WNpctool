// BtMacDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WNpctool.h"
#include "BtMacDlg.h"
#include "cmNumString.h"
using namespace cm;

// CBtMacDlg dialog

IMPLEMENT_DYNAMIC(CBtMacDlg, CDialog)

CBtMacDlg::CBtMacDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent /*=NULL*/)
	: CDialog(CBtMacDlg::IDD, pParent),m_Configs(Config),m_LocalLang(LocalLang)
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
	ON_BN_CLICKED(IDC_RADIO_MANUAL_BTMAC, &CBtMacDlg::OnBnClickedRadioManualBtmac)
	ON_BN_CLICKED(IDC_RADIO_AUTO_BTMAC, &CBtMacDlg::OnBnClickedRadioAutoBtmac)
	ON_BN_CLICKED(IDC_CHECK_BTMAC_SELECT, &CBtMacDlg::OnBnClickedCheckBtmacSelect)
	ON_EN_SETFOCUS(IDC_EDIT_BTMAC_SEGMENT_COUNT, &CBtMacDlg::OnEnSetfocusEditBtmacSegmentCount)
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
	GetDlgItem(IDC_RADIO_FILE_BTMAC)->ShowWindow(SW_HIDE);
#endif
}

BOOL CBtMacDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_LocalLang.TreeControls(m_hWnd,m_Configs.bDebug?TRUE:FALSE,this->IDD,false);
	UpdateInterface();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBtMacDlg::OnBnClickedRadioManualBtmac()
{
	// TODO: Add your control notification handler code here
	m_Configs.BtMac.nAutoMode = MODE_MANUAL;
	GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_CURRENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_END)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_COUNT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_BTMAC_FILE_PATH)->EnableWindow(FALSE);
	//UpdateInterface();
}

void CBtMacDlg::OnBnClickedRadioAutoBtmac()
{
	// TODO: Add your control notification handler code here
	m_Configs.BtMac.nAutoMode = MODE_AUTO;
	GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_START)->EnableWindow(m_Configs.BtMac.bEnable);
	GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_CURRENT)->EnableWindow(m_Configs.BtMac.bEnable);
	GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_END)->EnableWindow(m_Configs.BtMac.bEnable);
	GetDlgItem(IDC_EDIT_BTMAC_SEGMENT_COUNT)->EnableWindow(m_Configs.BtMac.bEnable);
	GetDlgItem(IDC_BUTTON_BTMAC_FILE_PATH)->EnableWindow(FALSE);
	//UpdateInterface();
}
BOOL CBtMacDlg::OnSaveConfig()
{
	CString strValue,strValue2;
	CString strPrompt;
	BOOL    bResult=FALSE;
	UpdateData(TRUE);
	m_Configs.BtMac.bEnable =(BST_CHECKED&((CButton *)GetDlgItem(IDC_CHECK_BTMAC_SELECT))->GetCheck()) ?TRUE:FALSE;
	if (m_Configs.BtMac.bEnable) {
		if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_MANUAL_BTMAC))->GetCheck())
		{
			m_Configs.BtMac.nAutoMode = MODE_MANUAL;
		}
		else if (BST_CHECKED&((CButton *)GetDlgItem(IDC_RADIO_AUTO_BTMAC))->GetCheck())
		{
			m_Configs.BtMac.nAutoMode = MODE_AUTO;
		}
		else
		{
			m_Configs.BtMac.nAutoMode = MODE_FILE;
		}
		if (MODE_AUTO == m_Configs.BtMac.nAutoMode) {
			GetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_START,strValue);
			m_Configs.BtMac.strStartMac = strValue;
			GetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_END,strValue);
			m_Configs.BtMac.strEndMac = strValue;
			GetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_CURRENT,strValue);
			m_Configs.BtMac.strCurrentMac = strValue;
			if (!(CompareNumString(m_Configs.BtMac.strEndMac.c_str(),m_Configs.BtMac.strCurrentMac.c_str())&&
				CompareNumString(m_Configs.BtMac.strCurrentMac.c_str(),m_Configs.BtMac.strStartMac.c_str())&&
				CompareNumString(m_Configs.BtMac.strEndMac.c_str(),m_Configs.BtMac.strStartMac.c_str())))
			{
				strPrompt.Format(GetLocalString(_T("IDS_ERROR_MAC_SEGMENT")).c_str(),TEXT("BTMAC"));
				MessageBox(strPrompt,GetLocalString(_T("IDS_ERROR_CAPTION")).c_str(),MB_ICONERROR|MB_OK);
				return bResult;					
			}
			strValue = m_Configs.BtMac.strEndMac.c_str();
			strValue2 = m_Configs.BtMac.strCurrentMac.c_str();
			m_Configs.BtMac.nRemainCount    = cmNumString::StrToSLong(strValue.Right(6),16) - cmNumString::StrToSLong(strValue2.Right(6),16) + 1;
			m_Configs.BtMac.nCount    = m_Configs.BtMac.nRemainCount;
		}
	}
	return TRUE;
}
std::wstring CBtMacDlg::GetLocalString(std::wstring strKey)
{
	return m_LocalLang.GetLanStr(strKey);
}
bool CBtMacDlg::CompareNumString(CString strMore,CString strLess)
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
void CBtMacDlg::OnBnClickedCheckBtmacSelect()
{
	// TODO: Add your control notification handler code here
	m_Configs.BtMac.bEnable = !m_Configs.BtMac.bEnable;
	UpdateInterface();
}

void CBtMacDlg::OnEnSetfocusEditBtmacSegmentCount()
{
	// TODO: Add your control notification handler code here
	CString strStartMac,strCurrentMac,strEndMac,strPrompt;
	int nCount;
	GetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_START,strStartMac);
	GetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_CURRENT,strCurrentMac);
	GetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_END,strEndMac);

	nCount = cmNumString::StrToSLong(strEndMac.Right(6),16) - cmNumString::StrToSLong(strCurrentMac.Right(6),16) + 1;
	SetDlgItemText(IDC_EDIT_BTMAC_SEGMENT_COUNT,(-1 == nCount)?_T("0"):cmNumString::NumToStr(nCount,10));
}
