#pragma once
#include <Windows.h>
#include <vector>
#include <iostream>
#include <time.h>

using namespace std;

class CBomb
{
public:
	Vector vOrigin;
	int iFlag;
};


struct local_s
{
	int prcflags;

	int iTeam;
	int iClip;
	int iWeaponID;
	int iIndex;
	int iWaterLevel;
	int iHealth;
	int iArmor;
	int iFlags;
	int iUseHull;
	int iFOV;

	float NospreadOffset;
	float m_flNextPrimaryAttack;

	float fOnLadder;
	float flHeight;
	float flGroundAngle;
	float flFpsKbMessTime;
	float flDuckTimeMax;
	float flFrametime;
	float flJumpMessTime;
	float flFallSpeed;
	float flZspeed;
	float flJumpDist;
	float flXYspeed;
	float flVelocity;
	float flMaxSpeed;
	float fAngleSpeed;
	float fsin_yaw;
	float fminus_cos_yaw;
	float sHudTimeDown;
	float fVSpeed;

	//from hpp
	float m_flFrametimee;


	bool bAlive;
	bool bFPS;
	bool bJumped;
	bool bInReload;

	Vector vRight;
	Vector vForward;
	Vector vEye;
	Vector vt1, vt2;
	Vector vOrigin;
	Vector vNorecoilAng;
	Vector vPunchangle;
	Vector vVelocity;
	Vector vAngles;
	Vector StartJumpPos;
	Vector pmEyePos;

	Vector vStrafeForward;

	playermove_t* ppmove;

	float fTrigRadius;
	float fTrigDiff;

	CBomb Bomb;
	//my additions
	int iMovetype;
	cl_entity_s* pEnt;

};
extern local_s g_Local;

struct player_sound_t
{
	vec3_t vOrigin;
	DWORD dwTime;
	bool bValid;
};


struct player_s
{
	bool bAlive;
	bool bUpdated;
	bool bDucked;
	bool bVisible;
	float distance;
	float fldmFixTime;
	vec3_t vHitboxOrigin[12];
	vec3_t vOrigin;
	vec3_t vSoundOrigin;
	player_sound_t currentSound, previousSound;
	hud_player_info_t Info;
	// prediction
	vec3_t vPredVec, vPredOldVec;
	float flPredTime, flPredOldTime;
	//
	int iTeam;
	int iHealth;
	int iArmorType;
	int iShotsFired;
};
extern player_s g_Player[33];