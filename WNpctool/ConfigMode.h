#pragma once
#include "afxcmn.h"
#include "BtMacDlg.h"
#include "LanMacDlg.h"
#include "SnDlg.h"
#include "WifiMacDlg.h"
#include "ImeiDlg.h"
#include "settings/SettingBase.h"


// CConfigMode dialog

class CConfigMode : public CDialog
{
	DECLARE_DYNAMIC(CConfigMode)

public:
	CConfigMode(CIniSettingBase &Config,CIniLocalLan &LocalLang,CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigMode();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
public:
	CIniSettingBase &m_Configs;
	CIniLocalLan    &m_LocalLang;
	CDialog			*m_ChildCWnds[5];
	int				m_iCurSelTab;
	CSnDlg			m_SnDlg;

	CWifiMacDlg		m_WifiMacDlg;
	CLanMacDlg		m_LanMacDlg;
	CBtMacDlg		m_BtMacDlg;
	ImeiDlg			m_ImeiDlg;
	VOID			UpdateInterface();
	void			OnSizeCtl( int cx, int cy);

public:
	virtual BOOL OnInitDialog();
	CTabCtrl	m_TabMode;
	afx_msg void OnBnClickedBtnModeApply();
	afx_msg void OnBnClickedBtnModeOk();
	afx_msg void OnTcnSelchangeTabSnmac(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnModeCancel();
};
