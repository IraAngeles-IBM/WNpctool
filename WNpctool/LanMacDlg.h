#pragma once


// CLanMacDlg dialog

class CLanMacDlg : public CDialog
{
	DECLARE_DYNAMIC(CLanMacDlg)

public:
	CLanMacDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLanMacDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_LANMAC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
