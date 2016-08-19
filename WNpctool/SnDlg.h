#pragma once
#include "settings/SettingBase.h"

// CSnDlg dialog

class CSnDlg : public CDialog
{
	DECLARE_DYNAMIC(CSnDlg)

public:
	CSnDlg(CIniSettingBase &Config,CWnd* pParent = NULL);   // standard constructor
	virtual ~CSnDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SN };

public:
	CIniSettingBase &m_Configs;
public:
	VOID        UpdateInterface();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
