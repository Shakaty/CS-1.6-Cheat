#include "utils.h"
#include <cmath>
utils g_Utils; 
CSkipFrames g_SkipFrames;

//void utils::SlowDownSleep(struct usercmd_s* cmd, float fNeedSpeed) //tkz
//{
//	float redspeed = g_Local.fSpeed - fNeedSpeed;
//
//	float degspeed = atan2(g_Local.ppmove->velocity[1], g_Local.ppmove->velocity[0]);
//
//	if (degspeed < 0)
//		degspeed += M_PI * 2.0f;
//
//	if (redspeed < 30.0f)
//		redspeed = 300.0f / g_Local.ppmove->movevars->airaccelerate;
//
//	cmd->forwardmove = -redspeed * cos(cmd->viewangles.y * M_PI / 180.0f - degspeed);
//	cmd->sidemove = -redspeed * sin(cmd->viewangles.y * M_PI / 180.0f - degspeed);
//}



CSkipFrames::CSkipFrames() :
	m_frames(0),
	m_norefresh_active(false),
	m_init(false)
{
	RtlSecureZeroMemory(m_cvar, sizeof(m_cvar));
	RtlSecureZeroMemory(m_saved, sizeof(m_saved));
}

void CSkipFrames::InitCvars()
{
	if (!m_init)
	{
		m_cvar[hud_draw] = g_Engine.pfnGetCvarPointer("hud_draw");
		m_cvar[r_drawentities] = g_Engine.pfnGetCvarPointer("r_drawentities");
		m_cvar[net_graph] = g_Engine.pfnGetCvarPointer("net_graph");
		m_cvar[cl_showfps] = g_Engine.pfnGetCvarPointer("cl_showfps");
		m_cvar[r_norefresh] = g_Engine.pfnGetCvarPointer("r_norefresh");

		for (size_t i = 0; i < MAX_COUNT - 1; i++)
			m_saved[i] = m_cvar[i]->value;

		m_init = true;
	}
}

void CSkipFrames::NoRefreshEnable()
{
	for (size_t i = 0; i < MAX_COUNT - 1; i++)
		m_cvar[i]->value = 0.f;

	m_cvar[r_norefresh]->value = 1.f;
	m_norefresh_active = true;
}

void CSkipFrames::NoRefreshDisable()
{
	for (size_t i = 0; i < MAX_COUNT - 1; i++)
		m_cvar[i]->value = m_saved[i];

	m_cvar[r_norefresh]->value = 0.f;
	m_norefresh_active = false;
}

void CSkipFrames::Start(const int value)
{
	InitCvars();

	if (m_frames-- <= 0)
	{
		m_frames = value;
		m_frames += (m_frames % 2 == 0) ? 0 : 1;

		NoRefreshDisable();
	}
	else
	{
		NoRefreshEnable();
	}
}

void CSkipFrames::End()
{
	if (m_norefresh_active)
	{
		m_frames = 0;

		NoRefreshDisable();
	}

	m_init = false;
}

///////////////////////////////////////////////////////////////
BOOL __comparemem(const UCHAR* buff1, const UCHAR* buff2, UINT size)
{
	for (UINT i = 0; i < size; i++, buff1++, buff2++)
	{
		if ((*buff1 != *buff2) && (*buff2 != 0xFF))
			return FALSE;
	}
	return TRUE;
}

//
ULONG __findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size)
{
	for (ULONG ul = start; (ul + size) < end; ul++)
	{
		if (CompareMemory(ul, clone, size))
			return ul;
	}
	return NULL;
}


ULONG __findreferencee(const ULONG start, const ULONG end, const ULONG address)
{
	UCHAR Pattern[5];
	Pattern[0] = 0x68;
	*(ULONG*)&Pattern[1] = address;
	return FindMemoryClone(start, end, Pattern, sizeof(Pattern) - 1);
}
///////////////////////////////

//for aimbot
bool utils::bGrenade(int iWeaponID)
{
	if (iWeaponID == WEAPONLIST_FLASHBANG ||
		iWeaponID == WEAPONLIST_HEGRENADE ||
		iWeaponID == WEAPONLIST_SMOKEGRENADE)
		return true;

	else return false;
}

bool utils::bBadWpn(int iWeaponID)
{
	if (iWeaponID == WEAPONLIST_C4 ||
		iWeaponID == WEAPONLIST_KNIFE ||
		bGrenade(iWeaponID))
		return true;

	else return false;
}

//void utils::HUD_AddEntity(struct cl_entity_s* ent)
//{
//	if (ent && ent->player && ent->index >= 1 && ent->index <= 32 && ent->index != g_Local.iIndex)
//	{
//		g_Player[ent->index].bUpdated = g_Utils.bIsValidEnt(ent);
//
//		g_Engine.pfnGetPlayerInfo(ent->index, &g_Player[ent->index].Info);
//
//		{
//			g_Player[ent->index].iNearestBoneID = -1;
//			float Dist = 9999999.9f;
//
//			for (int i = 0; i < g_Local.iMaxBones; i++)
//			{
//				if (g_Player[ent->index].fBoneFOV[i] < Dist || g_Player[ent->index].iNearestBoneID == -1)
//				{
//					Dist = g_Player[ent->index].fBoneFOV[i];
//					g_Player[ent->index].iNearestBoneID = i;
//				}
//			}
//		}
//
//		{
//			g_Player[ent->index].iNearestHitboxID = -1;
//			float Dist = 9999999.9f;
//
//			for (int i = 0; i < g_Local.iMaxHitboxes; i++)
//			{
//				if (g_Player[ent->index].fHitboxFOV[i] < Dist || g_Player[ent->index].iNearestHitboxID == -1)
//				{
//					Dist = g_Player[ent->index].fHitboxFOV[i];
//					g_Player[ent->index].iNearestHitboxID = i;
//				}
//			}
//		}
//
//		////////// check for visibility by all bones
//		CalcPlayerVisibility(ent);
//
//		//g_Engine.Con_NPrintf(ent->index, "%f", ent->prevstate.animtime - ent->ph[(ent->current_position + HISTORY_MAX - 1) % HISTORY_MAX].animtime);
//
//
//		g_Player[ent->index].fFrametime = ent->curstate.animtime - ent->prevstate.animtime;
//		if (g_Player[ent->index].fFrametime == 0)
//			g_Player[ent->index].fFrametime = g_Local.fFrametime;
//
//		g_Player[ent->index].fFps = 1.0f / g_Player[ent->index].fFrametime;
//		g_Player[ent->index].bDucked = ((ent->curstate.maxs[2] - ent->curstate.mins[2]) < 54 ? true : false);
//		g_Player[ent->index].vVelocity = ent->curstate.origin - ent->prevstate.origin;
//		g_Player[ent->index].vOrigin = ent->curstate.origin + (g_Player[ent->index].vVelocity * g_Player[ent->index].fFrametime) * cvar.prediction->value;
//		g_Player[ent->index].fSpeed = g_Player[ent->index].vVelocity.Length2D() / g_Player[ent->index].fFrametime;
//		g_Player[ent->index].fFallSpeed = -g_Player[ent->index].vVelocity[2] / g_Player[ent->index].fFrametime;
//		g_Player[ent->index].fHeight = fGetPlayerHeight(ent);
//		g_Player[ent->index].vAngles = ent->curstate.angles;
//		g_Player[ent->index].fFov = g_Utils.fCalcFOV(g_Local.vForward, g_Player[ent->index].vHitbox[11] - g_Local.vEye);
//		g_Player[ent->index].fAimbotFOV = fCalcAimbotFOV(ent);
//
//		//if(g_Player[ent->index].iTeam < 0 || g_Player[ent->index].iTeam > 2)
//			//g_Player[ent->index].iTeam			= PlayerInfo[ent->index]->Team;
//
//		GetPlayerDistance(ent);
//		GetPlayerWeapon(ent);
//
//		if (g_Player[ent->index].iTeam == g_Local.iTeam)
//			g_Player[ent->index].iHealth = PlayerInfo[ent->index]->Health;
//		else
//		{
//			if (g_Player[ent->index].iHealth <= 0 && g_Player[ent->index].bAlive)
//				g_Player[ent->index].iHealth = 10;
//		}
//	}
//}

