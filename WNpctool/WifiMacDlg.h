#pragma once
#include "settings/SettingBase.h"

// CWifiMacDlg dialog

class CWifiMacDlg : public CDialog
{
	DECLARE_DYNAMIC(CWifiMacDlg)

public:
	CWifiMacDlg(CIniSettingBase &Config,CWnd* pParent = NULL);   // standard constructor
	virtual ~CWifiMacDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_WIFIMAC };

public:
	CIniSettingBase &m_Configs;
public:
	VOID        UpdateInterface();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
