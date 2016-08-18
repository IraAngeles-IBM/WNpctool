#pragma once


// CBtMacDlg dialog

class CBtMacDlg : public CDialog
{
	DECLARE_DYNAMIC(CBtMacDlg)

public:
	CBtMacDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBtMacDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_BTMAC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
