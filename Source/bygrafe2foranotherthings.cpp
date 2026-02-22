//#include <ctime>
//#include "Main.h"
//#include "bygrafe.h"
//#include <vector>  
//#include "locals.h"
//#include "bygrafe2foranotherthings.h"
//#include "Main.h"
//# define M_PI 3.14159265358979323846f
//
//struct local_s g_Local;			//burada bunu eklemezsen linking hatasý veriyor
//
//int hSetFOV(const char* pszName, int iSize, void* pbuf)
//{
//	BEGIN_READ(pbuf, iSize);
//	g_Local.iFOV = READ_BYTE();
//	if (!g_Local.iFOV) { g_Local.iFOV = 90; }
//	return (*pSetFOV)(pszName, iSize, pbuf);
//}


////////////////////////////////////////////////    SONRADAN ÝNCELEYECEKLERÝM


//
//static inline bool IsFinite(float v) { return std::isfinite(v) != 0; }
//static inline float RadToDeg(float r) { return r * (180.0f / 3.14159265358979323846f); }
//static inline float DegToRad(float d) { return d * (3.14159265358979323846f / 180.0f); }
//
//// GoldSrc view açýlarýný normalize/clamp
//static inline void NormalizeAngles(float ang[3]) {
//	// pitch: [-89, +89], yaw: wrap to [-180,180], roll: 0
//	if (ang[0] > 89.0f) ang[0] = 89.0f;
//	else if (ang[0] < -89.0f) ang[0] = -89.0f;
//
//	while (ang[1] > 180.0f) ang[1] -= 360.0f;
//	while (ang[1] < -180.0f) ang[1] += 360.0f;
//
//	ang[2] = 0.0f;
//}
//
//static inline float VecLength(const float v[3]) {
//	return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
//}
//
//// delta -> pitch,yaw (GoldSrc: pitch up/down, yaw left/right)
//static void VectorAngles(const float delta[3], float outAngles[3]) {
//	float tmp = std::sqrt(delta[0] * delta[0] + delta[1] * delta[1]); // planar length
//	float pitch = -RadToDeg(std::atan2f(delta[2], tmp));          // (-) GoldSrc konvansiyonu
//	float yaw = RadToDeg(std::atan2f(delta[1], delta[0]));
//	outAngles[0] = pitch;
//	outAngles[1] = yaw;
//	outAngles[2] = 0.0f;
//	NormalizeAngles(outAngles);
//}
//
//// Ýsteðe baðlý FOV açýsý (mevcut view ile hedef arasýndaki açýsal fark)
//static float AngleDelta(const float a[3], const float b[3]) {
//	float dP = a[0] - b[0];
//	float dY = a[1] - b[1];
//	while (dY > 180.f) dY -= 360.f;
//	while (dY < -180.f) dY += 360.f;
//	return std::sqrt(dP * dP + dY * dY);
//}
//
//// Görünürlük kontrolü (opsiyonel): myloc -> targetloc hat üzerinde engel var mý?
//static bool IsVisible(const float myloc[3], const float targetloc[3]) {
//	pmtrace_t* tr = g_Engine.PM_TraceLine((float*)myloc, (float*)targetloc, PM_TRACELINE_PHYSENTSONLY, 0, 1);
//	if (!tr) return false;                 // güvenlik
//	// fraction ~1.0 ise ray bloklanmadan geçti sayýlýr
//	return tr->fraction > 0.97f;
//}
//
//// Basit takým kontrolü (opsiyonel) — örnek iskelet
//// Not: Takým bilgisini nasýl aldýðýn projene baðlý. Yoksa bu fonksiyonu false döndür.
//static bool IsSameTeam(const cl_entity_s* me, const cl_entity_s* other) {
//	if (!me || !other) return false;
//
//	// GoldSrc/CS 1.6: curstate.team üzerinden takým bilgisi alýnýr
//	int myTeam = me->curstate.team;
//	int otherTeam = other->curstate.team;
//
//	// 0 = bilinmiyor/invalid, bunlarý pas geç
//	if (myTeam == 0 || otherTeam == 0) return false;
//
//	return myTeam == otherTeam;
//}
//
//template <typename T>
//inline T clamp(T val, T minVal, T maxVal) {
//	if (val < minVal) return minVal;
//	if (val > maxVal) return maxVal;
//	return val;
//}
//
//void Aimbot_Think() {
//
//	cl_entity_s* me = g_Engine.GetLocalPlayer();
//	if (!me) return;
//
//	const int maxPlayers = max(1, g_Engine.GetMaxClients());
//	const float myloc[3] = { me->origin[0], me->origin[1], me->origin[2] };
//
//	// Mevcut view açýlarýný al
//	float view[3] = { 0,0,0 };
//	g_Engine.GetViewAngles(view);
//	NormalizeAngles(view);
//
//	float bestScore = 1e9f;
//	int bestIdx = -1;
//	float bestTargetAngles[3] = { 0,0,0 };
//
//	const float fovLimit = (cvar.aimbotfov ? clamp(cvar.aimbotfov->value, 0.0f, 180.0f) : 35.0f);
//	const float smoothFact = (cvar.aimbotsmooth ? max(cvar.aimbotsmooth->value, 1.0f) : 8.0f);
//
//	for (int i = 1; i <= maxPlayers; ++i) { // genelde 1..MaxClients
//		cl_entity_s* ent = g_Engine.GetEntityByIndex(i);
//		if (!ent) continue;
//		if (!ent->player) continue;
//		if (ent == me) continue;
//
//		// (Opsiyonel) takým filtresi
//		if (IsSameTeam(me, ent)) continue;
//
//		// Geçersiz/ölü/etkisiz oyuncular (solid=3 genelde player)
//		const entity_state_t& st = ent->curstate;
//		if (st.solid != 3) continue;
//
//		// Pozisyon ve delta
//		float target[3] = { ent->origin[0], ent->origin[1], ent->origin[2] };
//		float delta[3] = { target[0] - myloc[0], target[1] - myloc[1], target[2] - myloc[2] };
//		if (VecLength(delta) < 1e-3f) continue;
//
//		// (Opsiyonel) görünürlük
//		if (!IsVisible(myloc, target)) continue;
//
//		// Hedef açýlarý
//		float want[3];
//		VectorAngles(delta, want);           // (pitch,yaw,0)
//		if (!IsFinite(want[0]) || !IsFinite(want[1])) continue;
//
//		// FOV sýnýrý
//		float d = AngleDelta(want, view);
//		if (d > fovLimit) continue;
//
//		// Skor: küçük açýsal fark + yakýnlýk
//		float score = d + 0.001f * VecLength(delta);
//		if (score < bestScore) {
//			bestScore = score;
//			bestIdx = i;
//			bestTargetAngles[0] = want[0];
//			bestTargetAngles[1] = want[1];
//			bestTargetAngles[2] = 0.0f;
//		}
//	}
//
//	if (bestIdx == -1) return; // hedef yok
//
//	// Yumuþatma (smooth): view -> target
//	float out[3] = { view[0], view[1], 0.0f };
//	float stepP = (bestTargetAngles[0] - view[0]) / smoothFact;
//	float stepY = (bestTargetAngles[1] - view[1]);
//
//	// yaw wrap düzeltmesi
//	while (stepY > 180.f) stepY -= 360.f;
//	while (stepY < -180.f) stepY += 360.f;
//	stepY /= smoothFact;
//
//	out[0] = view[0] + stepP;
//	out[1] = view[1] + stepY;
//
//	NormalizeAngles(out);
//
//	if (IsFinite(out[0]) && IsFinite(out[1])) {
//		g_Engine.SetViewAngles(out);
//	}
//}

//////////////////////////////////////////////////////////////////