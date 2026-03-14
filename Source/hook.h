#pragma once
#include "Main.h"

PClientUserMsg UserMsgByName(char* szMsgName);
pfnUserMsgHook HookUserMsg(char* szMsgName, pfnUserMsgHook pfn);
pcmd_t CommandByName(char* szName);



