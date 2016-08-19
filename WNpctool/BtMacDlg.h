#pragma once
#include "settings/SettingBase.h"

// CBtMacDlg dialog

class CBtMacDlg : public CDialog
{
	DECLARE_DYNAMIC(CBtMacDlg)

public:
	CBtMacDlg(CIniSettingBase &Config,CWnd* pParent = NULL);   // standard constructor
	virtual ~CBtMacDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BTMAC };
public:
	CIniSettingBase &m_Configs;

public:
	VOID        UpdateInterface();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
};
