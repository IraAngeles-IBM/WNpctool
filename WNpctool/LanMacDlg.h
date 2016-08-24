#pragma once
#include "settings/SettingBase.h"

// CLanMacDlg dialog

class CLanMacDlg : public CDialog
{
	DECLARE_DYNAMIC(CLanMacDlg)

public:
	CLanMacDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent = NULL);   // standard constructor
	virtual ~CLanMacDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LANMAC };

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
	afx_msg void OnBnClickedRadioManualLanmac();
	afx_msg void OnBnClickedRadioAutoLanmac();
	afx_msg void OnBnClickedCheckLanmacSelect();
	afx_msg void OnEnSetfocusEditLanmacSegmentCount();
};