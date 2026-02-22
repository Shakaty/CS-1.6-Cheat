#include "Main.h"
struct ClientUserMsg* UserMsgByName(char* szMsgName)
{
	PClientUserMsg Ptr = nullptr;
	Ptr = pUserMsgBase;

	while (Ptr->next)
	{
		if (!strcmp(Ptr->name, szMsgName))
			return Ptr;
		Ptr = Ptr->next;
	}

	Ptr->pfn = 0;
	return Ptr;
}

pfnUserMsgHook HookUserMsg(char* szMsgName, pfnUserMsgHook pfn)
{
	PClientUserMsg Ptr = nullptr;
	pfnUserMsgHook Original = nullptr;
	Ptr = UserMsgByName(szMsgName);

	if (Ptr->pfn != 0) {
		Original = Ptr->pfn;
		Ptr->pfn = pfn;
		return Original;
	}
	else
	{
		offset.Error(szMsgName);
		return NULL;
	}
}

pcmd_t CommandByName(char* szName)
{
	pcmd_t pCmd = nullptr;
	pCmd = g_Engine.pfnGetCmdList();

	while (pCmd)
	{
		if (!strcmp(pCmd->pszName, szName))
			return pCmd;
		pCmd = pCmd->pNext;
	}

	return NULL;
}

