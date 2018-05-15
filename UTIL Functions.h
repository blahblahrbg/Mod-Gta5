#pragma once

#include "SDK.h"
#include <Psapi.h>

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

using ServerRankRevealAllFn = bool(__cdecl*)(float*);

struct Hitbox
{
	Hitbox(void)
	{
		hitbox = -1;
	}

	Hitbox(int newHitBox)
	{
		hitbox = newHitBox;
	}

	int  hitbox;
	Vector points[9];
};

enum
{
	FL_HIGH = (1 << 0),
	FL_LOW = (1 << 1),
	FL_SPECIAL = (1 << 2)
};

struct BestPoint
{
	BestPoint(void)
	{
		hitbox = -1;
		point = Vector(0, 0, 0);
		index = -1;
		dmg = -1;
		flags = 0;
	}

	BestPoint(int newHitBox)
	{
		hitbox = newHitBox;
		point = Vector(0, 0, 0);
		index = -1;
		dmg = -1;
		flags = 0;
	}

	Vector point;
	int  index;
	int  dmg;
	int  flags;
	int  hitbox;
};


namespace GameUtils
{
	extern DWORD FindPattern1(std::string moduleName, std::string pattern);
	extern ServerRankRevealAllFn ServerRankRevealAllEx;
	void NormaliseViewAngle(Vector &angle);
	void CL_FixMove(CUserCmd* cmd, Vector viewangles);
	bool IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID);
	bool IsBallisticWeapon(void* weapon);
	bool IsRevolver(void * weapon);
	bool IsGLOCK(void * weapon);
	bool IsP250(void * weapon);
	bool IsTEC9(void * weapon);
	bool IsFSEVEN(void * weapon);
	bool IsUMP(void * weapon);
	bool IsMAC10(void * weapon);
	bool AWP(void * weapon);
	bool IsDuals(void * weapon);
	bool IsPistol(void* weapon);
	bool SSG08Z(void * weapon);
	bool IsPistol2(void * weapon);
	bool DESERT2(void * weapon);
	bool CZ2(void * weapon);
	bool TEC9(void * weapon);
	bool P250ZZ(void * weapon);
	bool glockzz(void * weapon);
	bool FIVEN7(void * weapon);
	bool IsG(void * weapon);
	bool IsMP(void * weapon);
	void ClampViewAngle(Vector & angle);
	bool IsShotgun(void * weapon);
	bool IsMachinegun(void * weapon);
	bool IsSSG08(void * weapon);
	bool IsAK47(void * weapon);
	bool IsDEAGLE(void * weapon);
	bool IsAutoSniper(void * weapon);
	bool IsMP9(void * weapon);
	bool IsMP7(void * weapon);
	bool IsM4A1(void * weapon);
	bool IsM4A1S(void * weapon);
	bool IsFAMAS(void * weapon);
	bool IsAWP(void * weapon);
	bool IsBizon(void * weapon);
	bool IsUSPS(void * weapon);
	bool IsGALIL(void * weapon);
	bool IsSniper(void* weapon);
	bool AWPZZ(void * weapon);
	bool IsBomb(void * weapon);
	bool IsNotPistol(void* weapon);
	bool primaryweapon(void * weapon);
	bool segundaryweapon(void * weapon);
	bool IsGrenade(void * weapon);
	bool Zeus(void * weapon);
	bool Knifez(void * weapon);
	bool IsCZ75(void * weapon);
	bool SCAR20ZZ(void * weapon);
	bool G3SG1ZZ(void * weapon);
	bool SSG08zz(void * weapon);
	bool IsRifle(void * weapon);
	bool M4A4S(void * weapon);
	bool AUGZZ(void * weapon);
	bool FAMASZZ(void * weapon);
	bool GALILZZ(void * weapon);
	bool MP9ZZ(void * weapon);
	bool p90zz(void * weapon);
	bool IsSniper(void* weapon);
	bool IsRevolver(void* weapon);
	bool IsScopedWeapon(void* weapon);
	int GetPlayerCompRank(IClientEntity* pEntity);
	extern void ServerRankRevealAll();
	unsigned short UTIL_GetAchievementEventMask();
};

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, const IClientEntity *ignore, int collisionGroup, trace_t *ptr);

void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr);

bool IsBreakableEntity(IClientEntity* ent);

bool TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace);

void SayInChat(const char *text);

void SayInTeamChat(const char *text);

void SetName(std::string name);

Vector GetHitpointPosition(IClientEntity* pEntity, int Hitbox, Vector *Point);

//bool GetBestPoint(IClientEntity* pEntity, Hitbox* hitbox, Vector *point);

Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox);

//Vector GetHitscan(IClientEntity* pEntity, int Hitbox);

Vector GetEyePosition(IClientEntity* pEntity);

void ForceUpdate();

//unsigned short UTIL_GetAchievementEventMask();