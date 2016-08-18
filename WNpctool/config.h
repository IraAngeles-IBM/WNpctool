#ifndef _CONFIG_H_
#define _CONFIG_H_
#define MAC_LEN        6
#define MAC_STR_LEN    18
#define WIFI_OCUPY_CNT 4
#define MODE_MANUAL    0
#define MODE_START     MODE_MANUAL
#define MODE_AUTO      1
#define MODE_FILE      2
#define MODE_END       MODE_FILE

/********** FLAG_TYPE ***********/
#define FLAG_SN                 0
#define FLAG_MAC                1
#define FLAG_WIFIMAC            1
#define FLAG_BTMAC              2
#define FLAG_IMEI1              3
#define FLAG_IMEI2              4
#define FLAG_CNT                5
#define FLAG_AUTO               6
typedef struct _CFG_PATH_
{
    TCHAR           filePath[FLAG_CNT][MAX_PATH];
    LONG            lFilePos[FLAG_CNT];
    DWORD           dwLineCnt[FLAG_CNT];
    DWORD           dwLinePos[FLAG_CNT];
    unsigned char   sha[FLAG_CNT][20];
    unsigned short  CRC;
}CFGPATH,*PCFGPATH;

typedef struct
{
	std::wstring	strLogPath;
	BOOL			bAutoTest;
	BOOL			bEndIfFail;
	BOOL			bSetNormalMode;
	BOOL			bWriteImei;
	BOOL			bWriteImei2;
    BOOL			bIpcsTrace;
    int				nDutLog;
    DWORD			dwDLCloseSleep;
    BOOL			bUseATCom;
    BOOL			bStressTest;
    DWORD			dwTotal;
    DWORD			dwPass;
    BOOL			bRebootDevice;
    double			d3GMaxloss;
    double			d2GMaxloss;
    int				nWCDMAChannels[8][3];
    int				GSMChannels[4][3];
    int				DeviceType;

}STRUCT_BASIC_CONFIG,*PSTRUCT_BASIC_CONFIG;
typedef struct{
    BOOL			bEnable;
    int				nSnLength;
    std::wstring	strPrefix;
    std::wstring	strSuffix;
    BOOL			bForcingWrite;
    INT				bAutoMode;      /*0 manual 1 auto 2 file **/
    std::wstring	strStartSn;
    int				nSnCount;
    std::wstring	strCurrentSn;
    int				nRemainCount;
    BOOL			bHexCarry;
}STRUCT_SN_CONFIG,*PSTRUCT_SN_CONFIG;

/*added by lanshh */
typedef struct{
    BOOL			bEnable;
    int				nSnLength;
    std::wstring	strPrefix;
    std::wstring	strSuffix;
    BOOL			bForcingWrite;
    INT				bAutoMode;      /*0:manual 1:auto 2:file **/
    std::wstring	strStartSn;
    int				nSnCount;
    std::wstring	strCurrentSn;
    int				nRemainCount;
    BOOL			bHexCarry;
}STRUCT_DEVSN_CONFIG,*PSTRUCT_DEVSN_CONFIG;
#define LOCAL_ADMIN   1
#define GLOBAL_UNIQUE 2 
typedef struct{
	BOOL			bEnable;
	BOOL			bForcingWrite;
	INT				bAutoMode;
	std::wstring	strStartMac;
	std::wstring	strCurrentMac;
    int				nCount;
	int				nRemainCount;
    DWORD			dwType;
}STRUCT_MAC_CONFIG,*PSTRUCT__MAC_CONFIG;

typedef struct{
	BOOL			bEnable;
	BOOL			bForcingWrite;
	INT				bAutoMode;
 
	std::wstring	strStartImei1;
	std::wstring	strCurrentImei1;
    int				nCount1;
	int				nRemainCount1;

    BOOL			bImei2;
	std::wstring	strStartImei2;
	std::wstring	strCurrentImei2;
    int				nCount2;
	int				nRemainCount2;

}STRUCT_IMEI_CONFIG,*PSTRUCT__IMEI_CONFIG;


/*added end*/

typedef struct{
    BOOL bEnable;
    int  nEvmLimit[10];
    int  nFELossCH[13];
}STRUCT_WIFI_CONFIG,*PSTRUCT_WIFI_CONFIG;
typedef struct{
    BOOL    bEnable;
    bool    bMeasureGoldenPower;/*Measure Tx power of golden sample on selected channels **/
	bool    bMeasureCableLoss;  /*Measure cable loss on selected channels **/
#if 0 
    double  nAttenuation[60];
#endif
    double   fLoss[60];
    double   fFreqMHz[60];
    DWORD   nAttNum;
    BOOL    bCheckTest[8];
    DWORD   bDoCal;
    DWORD   bFastCal;
    BOOL    bCalVerification;
    BOOL    bRFVerification;
    BOOL    bDoAFC;
}STRUCT_RF_CONFIG,*PSTRUCT_RF_CONFIG;

typedef struct{
    BOOL    bUsePS;
    BOOL    bDualPS;
    int     iBatteryGPIBCard;
    int     iChargerGPIBCard;
    int     iBatteryAddr;
    int     iChargerAddr;
    double  fBatteryVolt;
    double  fChargerVolt;
    double  fBatteryCurrent;
    double  fChargerCurrent;
    BOOL    bStopIfFail;
}STRUCT_POWER_CONFIG,*PSTRUCT_POWER_CONFIG;

#endif
