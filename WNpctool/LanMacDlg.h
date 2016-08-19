#pragma once
#include "settings/SettingBase.h"

// CLanMacDlg dialog

class CLanMacDlg : public CDialog
{
	DECLARE_DYNAMIC(CLanMacDlg)

public:
	CLanMacDlg(CIniSettingBase &Config,CWnd* pParent = NULL);   // standard constructor
	virtual ~CLanMacDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LANMAC };

public:
	CIniSettingBase &m_Configs;

public:
	VOID        UpdateInterface();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
