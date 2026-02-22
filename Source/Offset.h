#pragma once
#include "../GameSDK/cl_dll.h"

//cl_dll.h olarak yazcan Main.h ekleyince mal hata veriyor !!!!!! pfnusermsghooku bulamadý diye!!!!!

typedef struct ClientUserMsg
{
	int msg;
	int size;
	char name[16];
	struct ClientUserMsg* next;
	pfnUserMsgHook pfn;
} *PClientUserMsg;


typedef struct cmd_s
{
	struct cmd_s* pNext;
	PCHAR			pszName;
	DWORD			pfnFunc;
	BYTE			bCrap[20];
} cmd_t, * pcmd_t;
struct GameInfo_s
{
	char* GameName;
	char* GameVersion;
	BYTE		Protocol;
	DWORD		Build;
};

class cOffset{
private:
	DWORD dwOldPageProtection;//For EnablePageWrite & RestorePageProtection

public:

	DWORD HwDll, SwDll, HlMod;
	DWORD HwBase, HwSize, HwEnd;
	DWORD ClBase, ClSize, ClEnd;
	DWORD HlBase, HlSize, HlEnd;
	DWORD VgBase, VgSize, VgEnd;

	BYTE HLType;

	void GetRenderType();
	bool GetModuleInfo();

	void Error(PCHAR Msg);

	DWORD FindClientTable();
	DWORD FindEngineTable();
	DWORD FindStudioTable();

	void CopyClient();
	void CopyEngine();
	void CopyStudio();

	DWORD FindGameConsole();
	void ConsoleColorInitalize();

	DWORD Absolute(DWORD Address);

	DWORD FarProc(DWORD Address, DWORD LB, DWORD HB);
	DWORD FindReference(DWORD start, DWORD end, DWORD Address);

	DWORD FindPattern(PCHAR pattern, PCHAR mask, DWORD start, DWORD end, DWORD offset);
	DWORD FindPattern(PCHAR pattern, DWORD start, DWORD end, DWORD offset);

	DWORD GetModuleSize(DWORD Address);
	// ben ekledim
	void* SpeedHackPtr(void);

	DWORD dwSendPacketPointer, dwSendPacketBackup; // saved offsets
	DWORD CL_Move(void);
	PClientUserMsg FindUserMsgBase(void);
	void GameInfo(void);
	PVOID FindPlayerMove(void);
	

	void GlobalTime();
	DWORD FindPatternbenimki(PCHAR pattern, PCHAR mask, DWORD start, DWORD end, DWORD offset);
	DWORD FindPatternbenimki(PCHAR pattern, DWORD start, DWORD end, DWORD offset);

	//utillerimss
	bool EnablePageWrite(DWORD addr, DWORD size);
	bool RestorePageProtection(DWORD addr, DWORD size);


};
extern cOffset offset;

//Hook.cpp dosyasý sezarda buraya yazýyorum ayrý dosya açmýyorum 


