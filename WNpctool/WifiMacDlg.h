#pragma once
#include "settings/SettingBase.h"

// CWifiMacDlg dialog

class CWifiMacDlg : public CDialog
{
	DECLARE_DYNAMIC(CWifiMacDlg)

public:
	CWifiMacDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent = NULL);   // standard constructor
	virtual ~CWifiMacDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_WIFIMAC };

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
	afx_msg void OnBnClickedRadioManualWifimac();
	afx_msg void OnBnClickedRadioAutoWifimac();
	afx_msg void OnBnClickedCheckWifimacSelect();
	afx_msg void OnEnSetfocusEditWifimacSegmentCount();
	afx_msg void OnBnClickedRadioFileWifimac();
};