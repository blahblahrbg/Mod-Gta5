#include <array>
#include <string>
#include <deque>
#include <algorithm>
#include "Entities.h"
#include "CommonIncludes.h"
#include "Entities.h"
#include "Vector.h"
#include <map>
#include "Interfaces.h"
#include "Hacks.h"
#include "Hooks.h"
extern int missedLogHits[65];
class ResolverSetup
{
public:

	bool didhit;
	void FSN(IClientEntity* pEntity, ClientFrameStage_t stage);
	bool bruteforceforresolvo();
	//used to get the fresh THINGS and the resolve
	void lbyresolvevo(IClientEntity * pEntity);
	void Auto();
	void Pitches(IClientEntity * pEntity);
	void OverrideYaw();
	void LowerBody(IClientEntity * pEntity);
	bool IsFakewalking(IClientEntity * player, int entID);
	void nospread_resolve(IClientEntity * player, int entID);
	void OverrideResolver(IClientEntity * pEntity);
	bool EntityIsValid(int i);
	void Resolve2(IClientEntity * pEntity);
	void simpleshit(IClientEntity * pEntity);
	void LegitAAResolver(IClientEntity * pEntity);
	void CM(IClientEntity* pEntity); //cause this is slower than FSN so we are going to get older info and not updated at all ez to compare between etc.
	void FSN(IClientEntity * pEntity, ClientFrameStage_t stage, int entID);
	void Resolve(IClientEntity* pEntity); //resolve
	void NospreadResolve(IClientEntity * pEntity);
	void NospreadResolve2(IClientEntity * pEntity);
	void ResolverSetup::Resolve3(IClientEntity *pEntity);
	void StoreThings(IClientEntity* pEntity);
	void StoreThings2(IClientEntity* pEntity);
	std::map<int, QAngle>StoredAngles; //y and x lol (stored)
	std::map<int, QAngle>NewANgles; //y and x lol (new)
	std::map<int, float>storedlby;
	std::map<int, float>newlby;
	std::map<int, float>storeddelta;
	std::map<int, float>newdelta;
	std::map<int, float>finaldelta;
	std::map<int, float>storedlbydelta;
	std::map<int, float>newlbydelta;
	std::map<int, float>finallbydelta;
	float newsimtime;
	float storedsimtime;
	bool lbyupdated;
	float storedlbyFGE;
	float storedanglesFGE;
	bool YawResolve2(IClientEntity *player);
	bool YawResolve3(IClientEntity *player);
	bool YawResolve4(IClientEntity *player);
	bool YawResolve5(IClientEntity *player);
	bool YawResolve6(IClientEntity *player);
	bool YawResolve7(IClientEntity *player);
	bool YawResolve8(IClientEntity *player);
	bool YawResolve9(IClientEntity *player);

	float storedsimtimeFGE;
	bool didhitHS;
	void StoreFGE(IClientEntity* pEntity);
	static ResolverSetup GetInst()
	{
		static ResolverSetup instance;
		return instance;
	}
};
namespace Globals
{
	extern 	bool shouldflip;
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	extern int Shots;
	extern bool Overriding;
	extern bool OverridingYaw;
	extern bool change;
	extern int TargetID;
	extern bool didhitHS;
	extern int missedshots;
	extern std::map<int, QAngle> storedshit;
}