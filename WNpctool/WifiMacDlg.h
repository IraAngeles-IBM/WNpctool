#pragma once


// CWifiMacDlg dialog

class CWifiMacDlg : public CDialog
{
	DECLARE_DYNAMIC(CWifiMacDlg)

public:
	CWifiMacDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWifiMacDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_WIFIMAC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
