#pragma once

#include <windows.h>
#include <iostream>
#include <vector>
#include "Offset.h"


#include "GameSDK/const.h"  //edited
#include "GameSDK/wrect.h"
#include "GameSDK/cl_dll.h"   //usermsg için eklendii!!!
#include "GameSDK/interface.h"
#include "GameSDK/r_studioint.h"

#include "GameSDK/screenfade.h"// bu ve aþþagýdakileri
#include "GameSDK/progs.h"
#include "GameSDK/progdefs.h"
#include "GameSDK/cl_entity.h"
#include "GameSDK/entity_state.h"
#include "GameSDK/weaponinfo.h"
#include "GameSDK/edict.h"
#include "GameSDK/event_args.h"
#include "GameSDK/event_flags.h"
#include "GameSDK/cvardef.h"
#include "GameSDK/parsemsg.h"
#include "GameSDK/cdll_int.h"
#include "GameSDK/pm_movevars.h"   //bhop için gerekli airaccelerate i alýyor buradan s
#include "GameSDK/pm_defs.H"
#include "GameSDK/event_api.h"
#include "GameSDK/com_model.h"  //name stealer için içindeki makrolarý kullaniciðizz  
#include "GameSDK/ref_params.h"			

//FOR Fakelag
#include "GameSDK/client.h" 
#include "GameSDK/net.h"
#include "GameSDK/archtypes.h"

//////////
#include "Dependencies/detours.h"

///////////
#include "locals.h"
#include "cvars.h"
#include "utils.h"
#include "usermsg.h"
#include "hook.h"
//////////
#include "QAngle.h"		//bu ikisinin orijinalini deðiþtirdim alttaki de dahil bozuk þeyleri deðiþtim
#include "Vector.h"		//evolden alýndý her ikiside QAngle funclarýný kullanabilmek adýna!!!

//////////


typedef struct cl_clientfuncs_s
{
	int(*Initialize) (cl_enginefunc_t *pEnginefuncs, int iVersion);
	int(*HUD_Init) (void);
	int(*HUD_VidInit) (void);
	void(*HUD_Redraw) (float time, int intermission);
	int(*HUD_UpdateClientData) (client_data_t *pcldata, float flTime);
	int(*HUD_Reset) (void);
	void(*HUD_PlayerMove) (struct playermove_s *ppmove, int server);
	void(*HUD_PlayerMoveInit) (struct playermove_s *ppmove);
	char(*HUD_PlayerMoveTexture) (char *name);
	void(*IN_ActivateMouse) (void);
	void(*IN_DeactivateMouse) (void);
	void(*IN_MouseEvent) (int mstate);
	void(*IN_ClearStates) (void);
	void(*IN_Accumulate) (void);
	void(*CL_CreateMove) (float frametime, struct usercmd_s *cmd, int active);
	int(*CL_IsThirdPerson) (void);
	void(*CL_CameraOffset) (float *ofs);
	struct kbutton_s *(*KB_Find) (const char *name);
	void(*CAM_Think) (void);
	void(*V_CalcRefdef) (struct ref_params_s *pparams);
	int(*HUD_AddEntity) (int type, struct cl_entity_s *ent, const char *modelname);
	void(*HUD_CreateEntities) (void);
	void(*HUD_DrawNormalTriangles) (void);
	void(*HUD_DrawTransparentTriangles) (void);
	void(*HUD_StudioEvent) (const struct mstudioevent_s *event, const struct cl_entity_s *entity);
	void(*HUD_PostRunCmd) (struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed);
	void(*HUD_Shutdown) (void);
	void(*HUD_TxferLocalOverrides) (struct entity_state_s *state, const struct clientdata_s *client);
	void(*HUD_ProcessPlayerState) (struct entity_state_s *dst, const struct entity_state_s *src);
	void(*HUD_TxferPredictionData) (struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd);
	void(*Demo_ReadBuffer) (int size, unsigned char *buffer);
	int(*HUD_ConnectionlessPacket) (struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size);
	int(*HUD_GetHullBounds) (int hullnumber, float *mins, float *maxs);
	void(*HUD_Frame) (double time);
	int(*HUD_Key_Event) (int down, int keynum, const char *pszCurrentBinding);
	void(*HUD_TempEntUpdate) (double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int(*Callback_AddVisibleEntity)(struct cl_entity_s *pEntity), void(*Callback_TempEntPlaySound)(struct tempent_s *pTemp, float damp));
	struct cl_entity_s *(*HUD_GetUserEntity) (int index);
	int(*HUD_VoiceStatus) (int entindex, qboolean bTalking);
	int(*HUD_DirectorMessage) (unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags);
	int(*HUD_GetStudioModelInterface) (int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio);
	void(*HUD_CHATINPUTPOSITION_FUNCTION) (int *x, int *y);
	int(*HUD_GETPLAYERTEAM_FUNCTION) (int iplayer);
	void(*CLIENTFACTORY) (void);
} cl_clientfunc_t;

typedef struct Color24
{
	BYTE R, G, B;
} TColor24, *PColor24;
extern PColor24 Console_TextColor;

class IGameConsole : public IBaseInterface
{
public:
	virtual void Activate(void) = 0;
	virtual void Initialize(void) = 0;
	virtual void Hide(void) = 0;
	virtual void Clear(void) = 0;
	virtual bool IsConsoleVisible(void) = 0;
	virtual void Printf(const char *format, ...) = 0;
	virtual void DPrintf(const char *format, ...) = 0;
	virtual void SetParent(int parent) = 0;
};

extern IGameConsole* g_pConsole;

extern cl_clientfunc_t *g_pClient;
extern cl_enginefunc_t *g_pEngine;
extern engine_studio_api_t *g_pStudio;

extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t g_Engine;
extern engine_studio_api_t g_Studio;

extern SCREENINFO g_ScreenInfo;
extern playermove_t* pmove;			//for Some cheeaats :D

extern hud_player_info_t g_PlayerInfoList[MAX_CLIENTS + 1];

typedef void(*CL_Move_t)();
extern CL_Move_t CL_Move_s;
extern PClientUserMsg pUserMsgBase;
extern GameInfo_s BuildInfo;

