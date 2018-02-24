#pragma once
#include "settings/SettingBase.h"

// ImeiDlg dialog

class ImeiDlg : public CDialog
{
	DECLARE_DYNAMIC(ImeiDlg)

public:
	ImeiDlg(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent = NULL);   // standard constructor
	virtual ~ImeiDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMEI };
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
	afx_msg void OnBnClickedCheckImeiSelect();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioManualImei();
	afx_msg void OnBnClickedRadioAutoImei();
	afx_msg void OnEnSetfocusEditImeiSegmentCount();
};
