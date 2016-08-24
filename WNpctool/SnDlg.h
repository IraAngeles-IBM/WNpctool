#pragma once
#include "settings/SettingBase.h"

// CSnDlg dialog

class CSnDlg : public CDialog
{
	DECLARE_DYNAMIC(CSnDlg)

public:
	CSnDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent = NULL);   // standard constructor
	virtual ~CSnDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SN };

public:
	CIniSettingBase &m_Configs;
	CIniLocalLan    &m_LocalLang;
public:
	VOID        UpdateInterface();
	BOOL		OnSaveConfig();
	std::wstring GetLocalString(std::wstring strKey);
	bool		CompareNumString(CString strMore,CString strLess);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioManualDevsn();
	afx_msg void OnBnClickedRadioAutoDevsn();
};