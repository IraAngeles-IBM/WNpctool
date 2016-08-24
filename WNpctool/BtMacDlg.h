#pragma once
#include "settings/SettingBase.h"

// CBtMacDlg dialog

class CBtMacDlg : public CDialog
{
	DECLARE_DYNAMIC(CBtMacDlg)

public:
	CBtMacDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent = NULL);   // standard constructor
	virtual ~CBtMacDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BTMAC };
public:
	CIniSettingBase &m_Configs;
	CIniLocalLan    &m_LocalLang;

public:
	VOID        UpdateInterface();
	BOOL		OnSaveConfig();
	std::wstring GetLocalString(std::wstring strKey);
	bool	CompareNumString(CString strMore,CString strLess);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioManualBtmac();
	afx_msg void OnBnClickedRadioAutoBtmac();
	afx_msg void OnBnClickedCheckBtmacSelect();
	afx_msg void OnEnSetfocusEditBtmacSegmentCount();
};
