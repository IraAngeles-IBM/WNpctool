#pragma once
#include "afxcmn.h"
#include "BtMacDlg.h"
#include "LanMacDlg.h"
#include "SnDlg.h"
#include "WifiMacDlg.h"


// CConfigMode dialog

class CConfigMode : public CDialog
{
	DECLARE_DYNAMIC(CConfigMode)

public:
	CConfigMode(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConfigMode();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CDialog     *m_ChildCWnds[5];
	int         m_iCurSelTab;
	CSnDlg		m_SnDlg;
	CWifiMacDlg	m_WifiMacDlg;
	CLanMacDlg	m_LanMacDlg;
	CBtMacDlg	m_BtMacDlg;

	void        OnSizeCtl( int cx, int cy);

public:
	virtual BOOL OnInitDialog();
	CTabCtrl	m_TabMode;
	afx_msg void OnBnClickedBtnModeApply();
	afx_msg void OnBnClickedBtnModeOk();
	afx_msg void OnTcnSelchangeTabSnmac(NMHDR *pNMHDR, LRESULT *pResult);
};
