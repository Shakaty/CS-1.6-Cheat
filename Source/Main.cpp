#include "Main.h"
#include "bygrafe.h"
#include "bygrafe2foranotherthings.h"

cl_clientfunc_t g_Client;
cl_enginefunc_t g_Engine;
engine_studio_api_t g_Studio;

cl_clientfunc_t* g_pClient = NULL;
cl_enginefunc_t* g_pEngine = NULL;
engine_studio_api_t* g_pStudio = NULL;

IGameConsole* g_pConsole = NULL;

PColor24 Console_TextColor;

static bool FirstFrame = false;

HANDLE hProcessReloadThread = 0;

extern DWORD SpeedPtr;
extern DWORD CLmoveoffset;

SCREENINFO g_ScreenInfo;
playermove_t* pmove = NULL;

player_info_t* plyinf = NULL;
GameInfo_s BuildInfo;

PClientUserMsg pUserMsgBase = nullptr;
CL_Move_t CL_Move_s = nullptr;

//extern float set_angle_y = 0;
//extern float set_angle_x = 0;
//extern float* view_x_ptr;


void ConsolePrintColor(BYTE R, BYTE G, BYTE B, const char* fmt, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, fmt);
	_vsnprintf_s(buf, sizeof(buf), fmt, va_alist);
	va_end(va_alist);
	TColor24 DefaultColor;
	PColor24 Ptr;
	Ptr = Console_TextColor;
	DefaultColor = *Ptr;
	Ptr->R = R;
	Ptr->G = G;
	Ptr->B = B;
	g_Engine.Con_Printf(buf);
	*Ptr = DefaultColor;
}

CreateInterfaceFn CaptureFactory(char* FactoryModule)
{
	CreateInterfaceFn Interface = 0;

	while (!Interface)
	{
		HMODULE hFactoryModule = GetModuleHandleA(FactoryModule);

		if (hFactoryModule)
		{
			Interface = (CreateInterfaceFn)(GetProcAddress(hFactoryModule, CREATEINTERFACE_PROCNAME));
		}

		Sleep(100);
	}

	return Interface;
}

PVOID CaptureInterface(CreateInterfaceFn Interface, char* InterfaceName)
{
	PVOID dwPointer = nullptr;

	while (!dwPointer)
	{
		dwPointer = (PVOID)(Interface(InterfaceName, 0));

		Sleep(100);
	}

	return dwPointer;
}


void FuncInitialize()
{
	if (!FirstFrame)
	{
		g_pConsole = (IGameConsole*)(CaptureInterface(CaptureFactory("GameUI.dll"), "GameConsole003"));
		offset.HLType = g_Studio.IsHardware() + 1;
		offset.ConsoleColorInitalize();
		SpeedPtr = (DWORD)offset.SpeedHackPtr();
		CLmoveoffset = (DWORD)offset.CL_Move();
		offset.GlobalTime();
		offset.GameInfo();
		pmove = (playermove_t*)offset.FindPlayerMove();
		pUserMsgBase = offset.FindUserMsgBase();

		if (!CLmoveoffset)
		{
			MessageBox(0, "Error:cl_move offsetini bulamadı", "Error", MB_OK | MB_ICONERROR);
			return;
		}

		if (!g_pConsole->IsConsoleVisible())
			g_pConsole->Activate();

		g_pEngine->pfnClientCmd("exec mych.cfg"); 
		g_Engine.pfnAddCommand("arda", arda); // bazı veriler
		HookClientFunctions();
		cfunc.Init();

		ConsolePrintColor(220, 20, 60, "\n\n\n\t\t\tHile Y4klendi By Grafe!\n");
		ConsolePrintColor(57, 255, 20, "\t\t\tdiscord:grafe_\n\n\n");
		ConsolePrintColor(255, 192, 203, R"(
									  ,`````.           ______
									' CiCi  `,         /_  ___   \
									'  ^_^   `.    /@ \/@  \   \
									 ` , . , '  `..   \__/\___/    /
															\__\/_____/
															/     /\\\\\
															|     |\\\\\\
															 \      \\\\\\
															  \______/\\\\     
							  							  _______ ||_||_______
															  (______(((_(((______(@))");

		FirstFrame = true;
	}
}

DWORD WINAPI ThreadEntry(LPVOID lpThreadParameter);

DWORD WINAPI ProcessReload(LPVOID lpThreadParameter)
{
	while (true)
	{
		if (FirstFrame)
		{
			offset.GetRenderType();

			if (!offset.GetModuleInfo())
				FirstFrame = false;
		}
		else
		{
			CreateThread(0, 0, ThreadEntry, 0, 0, 0);
		}

		Sleep(500);
	}

	return 0;
}

DWORD WINAPI ThreadEntry(LPVOID lpThreadParameter)
{
	if (hProcessReloadThread)
	{
		TerminateThread(hProcessReloadThread, 0);
		CloseHandle(hProcessReloadThread);
	}

	BYTE counter_find = 0;

start_hook:

	if (counter_find == 100)
	{
		offset.Error("Find Modules Error");
	}

	Sleep(100);
	counter_find++;
	if (!offset.GetModuleInfo())
	{
		goto start_hook;
	}

	DWORD ClientTable = offset.FindClientTable();
	if (ClientTable)
	{
		g_pClient = (cl_clientfunc_t*)ClientTable;
		offset.CopyClient();

		if ((DWORD)g_Client.Initialize)
		{
			DWORD EngineTable = offset.FindEngineTable();

			if (EngineTable)
			{
				g_pEngine = (cl_enginefunc_t*)EngineTable;
				offset.CopyEngine();

				if ((DWORD)g_Engine.V_CalcShake)
				{
					DWORD StudioTable = offset.FindStudioTable();

					if (StudioTable)
					{
						g_pStudio = (engine_studio_api_t*)StudioTable;
						offset.CopyStudio();

						if ((DWORD)g_Studio.StudioSetupSkin)
						{

							while (!FirstFrame)
							{
								FuncInitialize();
								Sleep(500);
							}

							BYTE bPreType = offset.HLType;

							hProcessReloadThread = CreateThread(0, 0, ProcessReload, 0, 0, 0);
						}
						else
							goto start_hook;
					}
					else
					{
						goto start_hook;
					}
				}
				else
					goto start_hook;
			}
			else
			{
				goto start_hook;
			}
		}
		else
			goto start_hook;
	}
	else
	{
		goto start_hook;
	}

	return 0;
}

char hackdir[256];
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		TCHAR Module[MAX_PATH];
		GetModuleFileName(hinstDLL, Module, ARRAYSIZE(Module));
		LoadLibrary(Module);

		if (GetLastError() == ERROR_ALREADY_EXISTS)
			return TRUE;

		DisableThreadLibraryCalls(hinstDLL);
		GetModuleFileName(hinstDLL, hackdir, 255);
		char* pos = hackdir + strlen(hackdir);
		while (pos >= hackdir && *pos != '\\')
			--pos;
		pos[1] = 0;
		CreateThread(0, 0, ThreadEntry, 0, 0, 0);

		return TRUE;
	}

	return FALSE;
}

