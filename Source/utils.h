#pragma once
#include "Main.h"

class utils
{
public:
	bool bIsValidEnt(struct cl_entity_s* ent);
	bool bPathFree(float* pflFrom, float* pflTo);
	bool bCalcScreen(float* pflOrigin, float* pflVecScreen);
	bool bFileExists(const char* cFileName);

	void VectorAngles(const float* forward, float* angles);
	void MakeAngle(bool addangle, float* angles, struct usercmd_s* cmd);
	void SlowDownSleep(struct usercmd_s* cmd, float fNeedSpeed);
	static void StringReplace(char* buf, const char* search, const char* replace);
	static void RunScript(char* scriptName);

	char* cIniRead(char* cFileName, char* cSection, char* cKey, char* cDef);

	float fCalcFOV(Vector one, Vector two, float lena = 0, float lenb = 0);

	int iGetPoints(const char* hittype);

	string szDirFile(const char* pszName);



	bool bGrenade(int iWeaponID);
	bool bBadWpn(int iWeaponID);
	void HUD_AddEntity(struct cl_entity_s* ent);
};
extern utils g_Utils;



class CSkipFrames
{
public:
	CSkipFrames();
	~CSkipFrames() {}

	bool	IsActive() { return m_norefresh_active; }

	void	Start(const int value);
	void	End();

private:
	enum cvar
	{
		hud_draw,
		r_drawentities,
		net_graph,
		cl_showfps,
		r_norefresh,
		MAX_COUNT
	};

	bool	m_norefresh_active;
	int		m_frames;

	cvar_s* m_cvar[MAX_COUNT];
	float	m_saved[MAX_COUNT];

	bool	m_init;

	void	InitCvars();

	void	NoRefreshEnable();
	void	NoRefreshDisable();
};
//

#define POW(a) ((a)*(a))
#define VectorLengthSquared(v) ((v)[0]*(v)[0]+(v)[1]*(v)[1]+(v)[2]*(v)[2])
#define VectorDistance(a,b) sqrt(POW((a)[0]-(b)[0])+POW((a)[1]-(b)[1])+POW((a)[2]-(b)[2]))
#define VectorLength(a) sqrt(POW((a)[0])+POW((a)[1])+POW((a)[2]))
#define VectorAdd(a,b,c) {(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];}
#define VectorMul(vec,num,res){(res)[0]=(vec)[0]*(num);(res)[1]=(vec)[1]*(num);(res)[2]=(vec)[2]*(num);}
#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}
#define Square(a) ((a)*(a))
#define VectorTransform(a,b,c){(c)[0]=DotProduct((a),(b)[0])+(b)[0][3];(c)[1]=DotProduct((a),(b)[1])+(b)[1][3];(c)[2]=DotProduct((a),(b)[2])+(b)[2][3];}

#define MAX_PLAYER_NAME_LENGTH	32

#define WEAPONLIST_P228 1
#define WEAPONLIST_UNKNOWN1 2
#define WEAPONLIST_SCOUT 3
#define WEAPONLIST_HEGRENADE 4
#define WEAPONLIST_XM1014 5
#define WEAPONLIST_C4 6
#define WEAPONLIST_MAC10 7
#define WEAPONLIST_AUG 8
#define WEAPONLIST_SMOKEGRENADE 9
#define WEAPONLIST_ELITE 10
#define WEAPONLIST_FIVESEVEN 11
#define WEAPONLIST_UMP45 12
#define WEAPONLIST_SG550 13
#define WEAPONLIST_UNKNOWN2 14
#define WEAPONLIST_UNKNOWN3 15
#define WEAPONLIST_USP 16
#define WEAPONLIST_GLOCK18 17
#define WEAPONLIST_AWP 18
#define WEAPONLIST_MP5 19
#define WEAPONLIST_M249 20
#define WEAPONLIST_M3 21
#define WEAPONLIST_M4A1 22
#define WEAPONLIST_TMP 23
#define WEAPONLIST_G3SG1 24
#define WEAPONLIST_FLASHBANG 25
#define WEAPONLIST_DEAGLE 26
#define WEAPONLIST_SG552 27
#define WEAPONLIST_AK47 28
#define WEAPONLIST_KNIFE 29
#define WEAPONLIST_P90 30

bool IsLeftElite(void);
bool IsCurWeaponSilenced(void);

enum
{
	BULLETTYPE_0 = 0,
	BULLETTYPE_1 = 1,
	BULLETTYPE_2 = 2,
	BULLETTYPE_3 = 3,
	BULLETTYPE_4 = 4,
	BULLETTYPE_5 = 5,
	BULLETTYPE_6 = 6,
	BULLETTYPE_7 = 7,
	BULLETTYPE_8 = 8,
	BULLETTYPE_9 = 9,
	BULLETTYPE_10 = 10,
	BULLETTYPE_11 = 11,
	BULLETTYPE_12 = 12,
	BULLETTYPE_13 = 13,
	BULLETTYPE_14 = 14,
	BULLETTYPE_15 = 15
};

//////////////

BOOL __comparemem(const UCHAR* buff1, const UCHAR* buff2, UINT size);
ULONG __findmemoryclone(const ULONG start, const ULONG end, const ULONG clone, UINT size);
ULONG __findreferencee(const ULONG start, const ULONG end, const ULONG address);

#define CompareMemory(Buff1, Buff2, Size) __comparemem((const UCHAR *)Buff1, (const UCHAR *)Buff2, (UINT)Size)
#define FindMemoryClone(Start, End, Clone, Size) __findmemoryclone((const ULONG)Start, (const ULONG)End, (const ULONG)Clone, (UINT)Size)
#define FindReferencee(Start, End, Address) __findreferencee((const ULONG)Start, (const ULONG)End, (const ULONG)Address)


inline float LengthSqr2D(const float v[3])
{
	return v[0] * v[0] + v[1] * v[1];
}

inline float Length2D(const float v[3])
{
	return std::sqrt(LengthSqr2D(v));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define VectorMul(vec,num,res){(res)[0]=(vec)[0]*(num);(res)[1]=(vec)[1]*(num);(res)[2]=(vec)[2]*(num);}

enum TeamName
{
	UNASSIGNED,
	TERRORIST,
	CT,
	SPECTATOR,
};
#define BOMB_FLAG_DROPPED	0 // if the bomb was dropped due to voluntary dropping or death/disconnect
#define BOMB_FLAG_PLANTED	1 // if the bomb has been planted will also trigger the round timer to hide will also show where the dropped bomb on the Terrorist team's radar.

enum ArmorType
{
	ARMOR_NONE,	// no armor
	ARMOR_KEVLAR,	// body vest only
	ARMOR_VESTHELM,	// vest and helmet
};

