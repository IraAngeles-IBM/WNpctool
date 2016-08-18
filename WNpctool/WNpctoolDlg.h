
// WNpctoolDlg.h : ͷ�ļ�
//

#pragma once
#include "cmLog.h"
#include "cmIniFile.h"
#include "cmPath.h"
#include "cmFile.h"
#include "cmNumString.h"
#include "cmStrCode.h"
#include "cmCommonDlg.h"
using namespace cm;
#include "settings/SettingBase.h"
#include "afxwin.h"
#include "ConfigMode.h"


typedef struct _STRUCT_BURNINGITEM
{
	USHORT nID;
	CString strValue;
}STRUCT_BURNINGITEM,*PSTRUCT_BURNINGITEM;
typedef vector<STRUCT_BURNINGITEM>	BURNINGITEM_VECTOR;
// CWNpctoolDlg �Ի���
class CWNpctoolDlg : public CDialog
{
// ����
public:
	CWNpctoolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WNPCTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	CIniSettingBase m_Configs;
	void ScanDeviceProc();
	BOOL WriteProc();
	BOOL ReadProc();
private:
	CString         m_strModulePath;
	CString m_strLogPath;
	CString m_strLoader;
	cmLog   *m_pLogObject;
	CWinThread *m_pScanThread;
	CEvent  *m_pScanEvent;
	CCriticalSection m_csScanLock;
	BOOL m_bUpgradeDllInitOK;
	BOOL m_bTerminated;
	UINT m_nDeviceCount;
	BOOL m_bExistMsc;
	BOOL m_bExistAdb;
	BOOL m_bExistLoader;
	BOOL m_bDownBoot;
	BOOL m_bRun;

	CConfigMode m_ConfigModeDlg;

	BOOL LoadConfig();
	VOID InitUi();

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	BURNINGITEM_VECTOR m_arrayDownloadItem;
	afx_msg void OnLogFolder();
	afx_msg void OnClose();
	CStatic m_lblDevice;
	afx_msg void OnBnClickedBtnWrite();
	afx_msg void OnSettingMode();
};
