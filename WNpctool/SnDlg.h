#pragma once


// CSnDlg dialog

class CSnDlg : public CDialog
{
	DECLARE_DYNAMIC(CSnDlg)

public:
	CSnDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSnDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
