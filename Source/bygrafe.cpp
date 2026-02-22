#define _USE_MATH_DEFINES
#include <ctime>
#include "Main.h"
#include "bygrafe.h"
#include <vector>  
#define M_PI 3.14159265358979323846f

////////////////////////////////////
bool bInDuck;bool bGstrafe = false;bool bhop_enabled = false;bool bStrafeHack = false;

void gstrafeON() { bGstrafe = true; }
void gstrafeOFF() { bGstrafe = false; }

void Cmd_Bhop_On(void) { bhop_enabled = true; }
void Cmd_Bhop_Off(void) { bhop_enabled = false; }

void StrafeON() { bStrafeHack = true; }
void StrafeOFF() { bStrafeHack = false; }

////////////////////////////////////
DWORD SpeedPtr;
DWORD CLmoveoffset;

static DWORD dwSpeedptr = 0;

local_s g_Local;
player_s g_Player[33];
//////////////////////////////////////////////////////////////////
// BAZI MİNİK FUNCSS

// Wavspeed is great :)
void AdjustSpeed(double x)
{
	if (dwSpeedptr == 0)
	{
		dwSpeedptr = SpeedPtr;
	}
	static double LastSpeed = 1;
	if (x != LastSpeed)
	{
		*(double*)dwSpeedptr = (x * 1000);
		LastSpeed = x;
	}
}

void bSendpacket(bool status)
{
	static bool bsendpacket_status = true;
	static DWORD NULLTIME = NULL;

	if (status && !bsendpacket_status) {
		bsendpacket_status = true;

		*(DWORD*)(offset.dwSendPacketPointer) = offset.dwSendPacketBackup;
	}
	if (!status && bsendpacket_status) {
		bsendpacket_status = false;

		*(DWORD*)(offset.dwSendPacketPointer) = (DWORD)&NULLTIME;
	}
}

float YawForVec(float* fwd)
{
	if (fwd[1] == 0 && fwd[0] == 0)
	{
		return 0;
	}
	else
	{
		float yaw = (atan2(fwd[1], fwd[0]) * 180 / M_PI);
		if (yaw < 0)
			yaw += 360;
		return yaw;
	}
}

/////////////////////////////////////////////////////////////////////////
void GroundStrafe(struct usercmd_s* cmd)
{
	if (g_Local.iFlags & FL_ONGROUND) {
		cmd->buttons |= IN_DUCK;
	}
	if (((g_Local.iFlags & FL_ONGROUND) && (bInDuck)) || !(g_Local.iFlags & FL_ONGROUND)) {
		cmd->buttons &= ~IN_DUCK;
	}
}

void No_Flash()
{
	screenfade_s fade{};                 
	g_Engine.pfnGetScreenFade(&fade);    // mevcut değerleri doldur

	if (fade.fadealpha > 0) {            // flash aktifse
		fade.fadealpha = 0;
		fade.fadeSpeed = 0;
		fade.fadeEnd = 0;
		fade.fadeTotalEnd = 0;
		g_Engine.pfnSetScreenFade(&fade); 
	}
}


void Bhop(struct usercmd_s* cmd, float speedMultiplier = 1.0f)
{
	if (!cmd)
		return;

	static bool lastJumpPressed = false;
	static float oldYaw = 0.0f;

	// Her frame jump tuşunu basılı göster (otomatik zıplama)
	if ((g_Local.iFlags & FL_ONGROUND)) {
		cmd->buttons |= IN_JUMP;
	}
	else {
		cmd->buttons |= IN_DUCK; //havadayken egilme yani sbj için
		cmd->buttons &= ~IN_JUMP; // havada iken jump’ı temizle
	}

	cmd->sidemove = (int)(cmd->sidemove * speedMultiplier);
	//// Hız çarpanı (isteğe bağlı)
	cmd->forwardmove = (int)(cmd->forwardmove * speedMultiplier);
}

void StrafeHack(float frametime, struct usercmd_s* cmd){
	if (!(g_Local.iFlags & FL_ONGROUND)) {
		if (g_Local.flXYspeed < 15)
		{
			cmd->forwardmove = 400;
			cmd->sidemove = 0;
		}
		Vector oldAngles;
		oldAngles = cmd->viewangles;

		float dir = 0;
		if (cmd->buttons & IN_MOVERIGHT)
			dir = 90;
		if (cmd->buttons & IN_BACK)
			dir = 180;
		if (cmd->buttons & IN_MOVELEFT)
			dir = -90;

		Vector va_real;
		g_Engine.GetViewAngles(va_real);
		va_real.y += dir;
		float vspeed[3] = { g_Local.vVelocity.x / g_Local.flVelocity,g_Local.vVelocity.y / g_Local.flVelocity,0.0f };
		float va_speed = YawForVec(vspeed);
		float adif = va_speed - va_real[1];
		while (adif < -180)adif += 360;
		while (adif > 180)adif -= 360;
		cmd->sidemove = (437.8928) * (adif > 0 ? 1 : -1);
		cmd->forwardmove = 0;
		bool onlysidemove = (abs(adif) >= atan(30.f / g_Local.flXYspeed) / M_PI * 180);
		cmd->viewangles[1] -= (-adif);
		float fs = 0;
		if (!onlysidemove)
		{
			static float lv = 0;
			Vector fw = g_Local.vStrafeForward;
			fw[2] = 0;
			fw = fw.Normalize();
			float vel = POW(fw[0] * g_Local.vVelocity[0]) + POW(fw[1] * g_Local.vVelocity[1]);

			fs = lv;
			lv = sqrt(69.f * 100000 / vel);
			static float lastang = 0;
			float ca = abs(adif);
			lastang = ca;
		}
		float ang = atan(fs / cmd->sidemove) / M_PI * 180;
		cmd->viewangles.y += ang;

		float sdmw = cmd->sidemove;
		float fdmw = cmd->forwardmove;

		if (cmd->buttons & IN_MOVERIGHT)
		{
			cmd->forwardmove = -sdmw;
			cmd->sidemove = fdmw;
		}
		if (cmd->buttons & IN_BACK)
		{
			cmd->forwardmove = -fdmw;
			cmd->sidemove = -sdmw;
		}
		if (cmd->buttons & IN_MOVELEFT)
		{
			cmd->forwardmove = sdmw;
			cmd->sidemove = -fdmw;
		}

	}
}


void FakeLag(usercmd_s* cmd) {
	static int choked = 0;
	if (cvar.fakelag_type->value == 0) {
		choked = 0;
		bSendpacket(true);;
		cvar.fakelag_type->value = -1;
	}
	if (cvar.fakelag_type->value == 1) {
		if (choked < cvar.fakelag_choke_limit->value) {
			bSendpacket(false);
			choked++;
		}
		else
		{
			bSendpacket(true);
			choked = 0;
		}
	}

}

///////////////////////////////////////////////////////////////////////////

void ThirdPerson(ref_params_t* p)
{
	float dist = cvar.thirdperson->value; // geri mesafe
	if (dist <= 0)
		return;

	Vector camOrigin = p->vieworg;
	Vector forward = p->forward;
	Vector up = p->up;
	Vector right = p->right;

	Vector camOffset =
		(up * 15.0f) +       // yukarı kaldır
		(right * 0.0f) +     // sağ/sol yok
		(forward * -dist);   // geri

	Vector desiredCam = camOrigin + camOffset;

	pmtrace_t tr;
	g_Engine.pEventAPI->EV_SetTraceHull(2);
	g_Engine.pEventAPI->EV_PlayerTrace(camOrigin, desiredCam, PM_GLASS_IGNORE, -1, &tr);

	// ÇARPTIYSA -> duvara çok girmesin
	p->vieworg[0] = tr.endpos[0];
	p->vieworg[1] = tr.endpos[1];
	p->vieworg[2] = tr.endpos[2];
}

void CustomFOV()
{
	int fov = cvar.fov_value->value;
	if (g_Local.bAlive)
	{
		if (g_Local.iFOV == 90)(*pSetFOV)("SetFOV", 1, &fov);
	}
	else
	{
		fov = 90;
		if (g_Local.iFOV == 90)(*pSetFOV)("SetFOV", 1, &fov);
	}
}


void DrawSpeed()
{
	if (!cvar.myspeed || cvar.myspeed->value <= 0)
		return;

	char buff[64];
	sprintf(buff, "Speed: %.1f", g_Local.flXYspeed);

	int len = 0, height = 0;
	g_Engine.pfnDrawConsoleStringLen(buff, &len, &height);

	// yeşil renk
	g_Engine.pfnDrawSetTextColor(0.0f, 1.0f, 0.0f);
	// sağ tarafa yapıştır
	g_Engine.pfnDrawConsoleString(g_ScreenInfo.iWidth - len, 200, buff);
}

void arda() {
	g_Engine.Con_Printf("\t\t Game Name - %s\n", BuildInfo.GameName);
	g_Engine.Con_Printf("\t\t Game Build - %d\n", (int)BuildInfo.Build);
	g_Engine.Con_Printf("\t\t Game Version - %s\n", BuildInfo.GameVersion);
	g_Engine.Con_Printf("\t\t Game Protocol - %d", (int)BuildInfo.Protocol);
}

void HookUserMessages()
{
	pResetHUD = HookUserMsg("ResetHUD", ResetHUD);
	pBombDrop = HookUserMsg("BombDrop", BombDrop);
	pBattery = HookUserMsg("Battery", Battery);
	pHealth = HookUserMsg("Health", Health);
	pDeathMsg = HookUserMsg("DeathMsg", DeathMsg);
	pSetFOV = HookUserMsg("SetFOV", SetFOV);
	pTeamInfo = HookUserMsg("TeamInfo", TeamInfo);
	pScoreAttrib = HookUserMsg("ScoreAttrib", ScoreAttrib);
}
///////////////////////////////////////////////////////////////////////////
	//g_pConsole->DPrintf("NextCmdTime: %f\n", client_static->nextcmdtime); client console paneline yazar

void CL_CreateMove(float frametime, struct usercmd_s* cmd, int active)
{
	g_Client.CL_CreateMove(frametime, cmd, active);
	g_Local.flFrametime = frametime;

	cl_entity_s* LocalEnt = g_Engine.GetLocalPlayer();
	g_Local.bAlive = LocalEnt && !(LocalEnt->curstate.effects & EF_NODRAW) && LocalEnt->player && LocalEnt->curstate.movetype != 6 && LocalEnt->curstate.movetype != 0;

	/////////////////////////
	/////////////////////////
	static bool init = false;
	if (!init)
	{
		g_Engine.pfnAddCommand("+ardags", gstrafeON);
		g_Engine.pfnAddCommand("-ardags", gstrafeOFF);
		g_Engine.pfnAddCommand("+ardabhop", Cmd_Bhop_On);
		g_Engine.pfnAddCommand("-ardabhop", Cmd_Bhop_Off);
		g_Engine.pfnAddCommand("+ardastrafe", StrafeON);
		g_Engine.pfnAddCommand("-ardastrafe", StrafeOFF);
		init = true;
	}
	/////////////////////////////
	if (g_Local.bAlive)
	{
		if (bhop_enabled) { Bhop(cmd); }
		if (bGstrafe) { GroundStrafe(cmd); }
		if( bStrafeHack) { StrafeHack(frametime, cmd); }

	}
	FakeLag(cmd);
	AdjustSpeed(cvar.speed->value);
	CustomFOV();

}

void HUD_Redraw(float time, int intermission) {
	g_Client.HUD_Redraw(time, intermission);
	g_ScreenInfo.iSize = sizeof(SCREENINFO);
	g_Engine.pfnGetScreenInfo(&g_ScreenInfo);
	No_Flash();
	DrawSpeed();
}

void HUD_PlayerMove(struct playermove_s* ppmove, int server)
{
	g_Client.HUD_PlayerMove(ppmove, server);
	bInDuck = ppmove->bInDuck;

	g_Engine.pEventAPI->EV_LocalPlayerViewheight(g_Local.vEye);
	g_Local.vEye = g_Local.vEye + ppmove->origin;
	g_Local.iFlags = ppmove->flags;
	g_Local.vOrigin = ppmove->origin;
	g_Local.flXYspeed = sqrt(POW(ppmove->velocity[0]) + POW(ppmove->velocity[1]));
	g_Local.vVelocity = ppmove->velocity;
	g_Local.iWaterLevel = ppmove->waterlevel;
	g_Local.flMaxSpeed = ppmove->maxspeed;
	g_Local.iUseHull = ppmove->usehull;
	g_Local.flZspeed = (ppmove->velocity[2] != 0) ? -ppmove->velocity[2] : 0;
	g_Local.flFallSpeed = ppmove->flFallVelocity;
	g_Local.fOnLadder = ppmove->movetype == 5;
	g_Local.fAngleSpeed = (g_Local.vAngles)[1] - (ppmove->angles)[1];while (g_Local.fAngleSpeed < -180) { g_Local.fAngleSpeed += 360; }while (g_Local.fAngleSpeed > 180) { g_Local.fAngleSpeed -= 360; }
	g_Local.vAngles = ppmove->angles;
	g_Local.iMovetype = ppmove->movetype;

	Vector vel = ppmove->velocity;
	g_Local.fVSpeed = vel.Length();

	float fYaw = ppmove->angles[1] * (3.1415 / 180.0);
	g_Local.fsin_yaw = sin(fYaw);
	g_Local.fminus_cos_yaw = -cos(fYaw);

	Vector vTemp1 = g_Local.vOrigin;
	vTemp1[2] -= 8192;
	pmtrace_t* trace = g_Engine.PM_TraceLine(g_Local.vOrigin, vTemp1, 1, ppmove->usehull, -1);
	g_Local.flHeight = abs(trace->endpos.z - g_Local.vOrigin.z);

	if (g_Local.flHeight <= 60) g_Local.flGroundAngle = acos(trace->plane.normal[2]) / M_PI * 180;
	else g_Local.flGroundAngle = 0;

	Vector vTemp2 = trace->endpos;
	pmtrace_t pTrace;
	g_Engine.pEventAPI->EV_SetTraceHull(ppmove->usehull);
	g_Engine.pEventAPI->EV_PlayerTrace(g_Local.vOrigin, vTemp2, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_Local.iIndex, &pTrace);
	if (pTrace.fraction < 1.0f)
	{
		g_Local.flHeight = abs(pTrace.endpos.z - g_Local.vOrigin.z);
		int ind = g_Engine.pEventAPI->EV_IndexFromTrace(&pTrace);
		if (ind > 0 && ind < 33)
		{
			float dst = g_Local.vOrigin.z - (g_Local.iUseHull == 0 ? 32 : 18) - g_Player[ind].vOrigin.z - g_Local.flHeight;
			if (dst < 30)
			{
				g_Local.flHeight -= 14.0;
			}
		}
	}
	//for hpp
	const auto pGameEntity = g_Engine.GetLocalPlayer();
	g_Local.flFrametime = pGameEntity->curstate.animtime - pGameEntity->prevstate.animtime;
	// for strafe hackms 

	cl_entity_s* pLocal = g_Engine.GetLocalPlayer();
	g_Local.pEnt = g_pEngine->GetMaxClients() && pLocal ? pLocal : NULL;
	static bool asdd =true;
	if (!pmove && asdd) {
		MessageBoxA(0, "hata", "pmove nullptr la", MB_OK | MB_ICONERROR);
		g_Local.flVelocity = 0.0f;
		asdd = false;
	}											//pmove nullptr !! o yüzden olmamış!!!
												//offsetini çekmek gerekiyormuş :D
	if (g_Local.pEnt) {
		g_Local.flVelocity = pmove->velocity.Length();
	}
}

int HUD_AddEntity(int type, struct cl_entity_s* ent, const char* modelname)
{
	// g_Utils.HUD_AddEntity(ent); //aimbot yaparken yarıda kalmış xd kapattım şimdi kullanmadıgım için

	return g_Client.HUD_AddEntity(type, ent, modelname);
}

void CL_Move() //Create and send the command packet to the server
{
	bSendpacket(true);

	CL_Move_s();
}

void V_CalcRefdef(struct ref_params_s* pparams)
{
	g_Client.V_CalcRefdef(pparams);
	if (cvar.thirdperson->value > 0) {
		ThirdPerson(pparams);
	}
	g_Local.vStrafeForward = pparams->forward;

}

void HookClientFunctions()
{
	g_pClient->CL_CreateMove = CL_CreateMove;
	g_pClient->HUD_PlayerMove = HUD_PlayerMove;
	g_pClient->HUD_Redraw = HUD_Redraw;
	g_pClient->HUD_AddEntity = HUD_AddEntity;

	CL_Move_s = (CL_Move_t)DetourAttach((PVOID*)offset.CL_Move(), (PVOID)&CL_Move);

	g_pClient->V_CalcRefdef = V_CalcRefdef;
	HookUserMessages();

}