
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
#include "afxcmn.h"
#include "./XListBox/XListBox.h"
#include "FontStatic/FontStatic.h"
#include "debug.h"
#include "scriptexe.h"

#define WM_COM_ADD		    1
#define WM_COM_RM		    2
#define UPDATE_LIST         3
#define LIST_EMPTY          4
#define UPDATE_TEST_EXIT    5
#define UPDATE_WINDOW		6
#define UPDATE_PROMPT		7

#define PROMPT_TESTING 1
#define PROMPT_PASS 2
#define PROMPT_FAIL 3
#define PROMPT_EMPTY 4

#define LIST_INFO 0
#define LIST_TIME 1
#define LIST_WARN 2
#define LIST_ERR  3

#define FIELD_DEVSN     1
#define FIELD_WIFIMAC   2
#define FIELD_BTMAC		4
#define FIELD_LANMAC    8
#define FIELD_IMEI	   16
#define FIELD_ALL      32

enum ENUM_WRITEITEM_ID{
	ITEM_SN = 1,
	ITEM_WIFIMAC,
	ITEM_LANMAC,
	ITEM_BTMAC,
	ITEM_IMEI,

};
typedef struct  
{
	int flag;
	TCHAR   pszLineText[MAX_PATH];
}STRUCT_LIST_LINE,*PSTRUCT_LIST_LINE;

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
	CIniLocalLan	m_LocalLan;

	void			ScanDeviceProc();
	BOOL			WriteProc();
	BOOL			ReadProc();
	BOOL			WriteItem(int nItemID);
	BOOL			ReadItem(int nItemID);
	BOOL			WriteMac(CString strMac,int nItemID);
	BOOL			ReadMac(int nItemID);
	CString			BytesToHexStr(PBYTE pBuf,int nBufLen);
	BOOL			HexStrToBytes(CString strHex,PBYTE pBuf,int &nBufLen);
	LRESULT			OnHandleUpdateMsg(WPARAM wParam,LPARAM lParam);
	void			AddPrompt(CString strPrompt,int flag);
	VOID			WalkMenu(CMenu *pMenu,CString strMainKeyPart);
private:
	CString         m_strModulePath;
	CString			m_strLogPath;
	CString			m_strLoader;
	CLogger         *m_pLog;

	CString         m_strCurDevSn;
	CString         m_strCurWifiMac;
	CString         m_strCurBtMac;
	CString         m_strCurLanMac;
	CString         m_strCurImei;

	BOOL			m_bUserStop;
	BOOL            m_bStarWrite;
	BOOL            m_bStarRead;
	CWinThread      *m_pReadThread;
	CWinThread      *m_pWriteThread;
	CWinThread		*m_pScanThread;
	CEvent			*m_pScanEvent;
	CCriticalSection m_csScanLock;
	BOOL			m_bUpgradeDllInitOK;
	BOOL			m_bTerminated;
	UINT			m_nDeviceCount;
	BOOL			m_bExistMsc;
	BOOL			m_bExistAdb;
	BOOL			m_bExistLoader;
	BOOL			m_bDownBoot;
	BOOL			m_bRun;

	BOOL            m_bRedLedLight;
	HBITMAP         m_hGreenLedBitmap;
	HBITMAP         m_hRedLedBitmap;
	//CConfigMode m_ConfigModeDlg;

	CStatic		m_lblDevice;
	CFontStatic m_lblPrompt;
	CXListBox	m_listInfo;

	BOOL    SaveWriteResultOnPass(BOOL,DWORD);
	BOOL	LoadConfig();
	VOID	InitUi();
	BOOL	OnStartRead();
	BOOL	OnStartWrite(bool bAuto);
	std::wstring GetLocalString(std::wstring strKey);
	VOID    UpdateMenuItem();
	BOOL	AdbToRockusb();

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
	afx_msg void OnBnClickedBtnWrite();
	afx_msg void OnSettingMode();
	afx_msg void OnUpdateSettingRead(CCmdUI *pCmdUI);
	afx_msg void OnSettingRead();
	afx_msg void OnLanguageChinese();
	afx_msg void OnUpdateLanguageChinese(CCmdUI *pCmdUI);
	afx_msg void OnLanguageEnglish();
	afx_msg void OnUpdateLanguageEnglish(CCmdUI *pCmdUI);
	afx_msg void OnSettingAuto();
	afx_msg void OnUpdateSettingAuto(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedButtonLoader();
	afx_msg void OnSettingReset();
	afx_msg void OnUpdateSettingReset(CCmdUI *pCmdUI);
};
