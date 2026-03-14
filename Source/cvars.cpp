#include "cvars.h"
#include "Main.h"

CVars cvar;
CFunc cfunc;



void CFunc::Init() {
	cvar.myspeed = g_Engine.pfnRegisterVariable("arda_showspeed", "0", 0);
	//
	cvar.speed = g_pEngine->pfnRegisterVariable("arda_speed", "1", 0);    // Normal game wav Speed

	cvar.fakelag_enabled = g_pEngine->pfnRegisterVariable("arda_fakelagenable", "0", 0);
	cvar.fakelag_type = g_pEngine->pfnRegisterVariable("arda_fakelagtype", "0", 0);
	cvar.fakelag_choke_limit = g_pEngine->pfnRegisterVariable("arda_fakelagchokelimit", "15", 0);
	///////
	cvar.thirdperson = g_pEngine->pfnRegisterVariable("arda_thirdperson", "0", 0);
	cvar.fov_value = g_pEngine->pfnRegisterVariable("arda_fov", "90", 0);


}