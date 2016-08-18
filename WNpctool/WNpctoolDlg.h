
// WNpctoolDlg.h : 头文件
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
// CWNpctoolDlg 对话框
class CWNpctoolDlg : public CDialog
{
// 构造
public:
	CWNpctoolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WNPCTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
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

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
