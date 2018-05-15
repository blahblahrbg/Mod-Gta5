/*credits by ibanned*/

#include "Resolver.h"
#include "Ragebot.h"
#include "Hooks.h"
#include "lagcomp.h"

int IClientEntity::GetSequenceActivity(int sequence)
{
	auto hdr = Interfaces::ModelInfo->GetStudiomodel(this->GetModel());

	if (!hdr)
		return -1;

	static auto getSequenceActivity = (DWORD)(Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 7D 08 FF 56 8B F1 74"));
	static auto GetSequenceActivity = reinterpret_cast<int(__fastcall*)(void*, studiohdr_t*, int)>(getSequenceActivity);

	return GetSequenceActivity(this, hdr, sequence);
}

BackTrack* lbybacktrack;
bool lowerBodyYawUpdated(IClientEntity* pEntity) {
	for (int w = 0; w < 13; w++)
	{
		AnimationLayer currentLayer = pEntity->GetAnimOverlays()[1];
		const int activity = pEntity->GetSequenceActivity(currentLayer.m_nSequence);
		float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
		uint32_t norder = currentLayer.m_nOrder;
		if (activity == 973 && flweight == 1.f || activity == 979 && flweight == 1.0f && currentLayer.m_flPrevCycle != currentLayer.m_flCycle)// 961 looks like they shot //flweight seems like right as the animation or right after 972 could be moving
			return true;
	}
	return false;
}
//gonna fix resolver for you. with correct animation statements from vids i watched and other resolvers that showed the same results.
bool IsBreakingLBY120(IClientEntity* pEntity)
{
	for (int w = 0; w < 13; w++)
	{
		AnimationLayer prevlayer;
		AnimationLayer currentLayer = pEntity->GetAnimOverlays()[1];
		const int activity = pEntity->GetSequenceActivity(currentLayer.m_nSequence);
		float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
		uint32_t norder = currentLayer.m_nOrder;
		if (activity == ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING || activity == ACT_CSGO_IDLE_TURN_BALANCEADJUST && flweight >= .99 && currentLayer.m_flPrevCycle != currentLayer.m_flCycle)
		{
			float flanimTime = currentLayer.m_flCycle, flsimtime = pEntity->GetSimulationTime();

			return true;//force lby-180?
		}
		prevlayer = currentLayer;
		return false;
	}
	return false;
}
bool IsResolvableByLBY(IClientEntity* pEntity)
{
	for (int w = 0; w < 13; w++)
	{
		AnimationLayer prevlayer;
		AnimationLayer currentLayer = pEntity->GetAnimOverlays()[1];
		const int activity = pEntity->GetSequenceActivity(currentLayer.m_nSequence);
		float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
		uint32_t norder = currentLayer.m_nOrder;
		if (activity == 979 && currentLayer.m_flWeight == 0.f && currentLayer.m_flPrevCycle != currentLayer.m_flCycle)
		{
			return true;//bruteeee because breaking lby <120
		}
		prevlayer = currentLayer;
	}
	return false;
}
bool playerStoppedMoving(IClientEntity* pEntity)
{
	for (int w = 0; w < 13; w++)
	{
		AnimationLayer currentLayer = pEntity->GetAnimOverlays()[1];
		const int activity = pEntity->GetSequenceActivity(currentLayer.m_nSequence);
		float flcycle = currentLayer.m_flCycle, flprevcycle = currentLayer.m_flPrevCycle, flweight = currentLayer.m_flWeight, flweightdatarate = currentLayer.m_flWeightDeltaRate;
		uint32_t norder = currentLayer.m_nOrder;
		if (activity == ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING)
			return true;
	}
	return false;
}

bool lowerBodyPrediction(IClientEntity* pEntity)
{
	static float prediction = 0.f;
	static bool secondrun = false;
	float flServerTime = (float)pEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	if (playerStoppedMoving(pEntity) && !secondrun)
	{
		prediction = flServerTime + 0.22;
		secondrun = true;
	}
	else if (pEntity->GetVelocity().Length2D() < 0.1f && secondrun && prediction <= pEntity->GetSimulationTime())
	{
		prediction = pEntity->GetTickBase() + 1.1f;
	}
	else//theyre moving
	{
		secondrun = false;
		return false;
	}
	if (prediction <= pEntity->GetSimulationTime())
	{
		return true;
	}
	return false;
}

bool ResolverSetup::IsFakewalking(IClientEntity* player, int entID)
{
	if (g_menu::window.RageBotTab.Resolverfakewalk.GetState());
	{
		for (int s = 0; s < 13; s++)
		{
			auto anim_layer = player->GetAnimOverlay(s);
			if (!anim_layer.m_pOwner)
				continue;

			auto activity = player->GetSequenceActivity(anim_layer.m_nSequence);
			bool
				bFakewalking = false,
				stage1 = false,         // stages needed cause we are iterating all layers, otherwise won't work :)
				stage2 = false,
				stage3 = false;

			for (int i = 0; i < 16; i++)
			{
				if (activity == 26 && anim_layer.m_flWeight < 0.4f)
					stage1 = true;
				if (activity == 7 && anim_layer.m_flWeight > 0.001f)
					stage2 = true;
				if (activity == 2 && anim_layer.m_flWeight == 0)
					stage3 = true;
			}

			if (stage1 && stage2)
				bFakewalking = true;

			else
				bFakewalking = false;

			return bFakewalking;
		}
	}
}

bool isflying(IClientEntity* pEntity)
{
	if (!pEntity->GetFlags() & FL_ONGROUND)
	{
		return true;
	}
	return false;
}

//nospread resolver
void ResolverSetup::Pitches(IClientEntity* pEntity)
{
	if (g_menu::window.RageBotTab.PitchAimbotResolver.GetState())
	{
		Vector* eyeAngles = pEntity->GetEyeAnglesXY();
		if (eyeAngles->x < -179.f) eyeAngles->x += 360.f;
		else if (eyeAngles->x > 90.0 || eyeAngles->x < -90.0) eyeAngles->x = 89.f;
		else if (eyeAngles->x > 89.0 && eyeAngles->x < 91.0) eyeAngles->x -= 90.f;
		else if (eyeAngles->x > 179.0 && eyeAngles->x < 181.0) eyeAngles->x -= 180;
		else if (eyeAngles->x > -179.0 && eyeAngles->x < -181.0) eyeAngles->x += 180;
		else if (fabs(eyeAngles->x) == 0) eyeAngles->x = std::copysign(89.0f, eyeAngles->x);
	}
}

void ResolverSetup::OverrideResolver(IClientEntity* pEntity)
{

	bool MeetsLBYReq;
	if (pEntity->GetFlags() & FL_ONGROUND)
		MeetsLBYReq = true;
	else
		MeetsLBYReq = false;

	int OverrideKey = g_menu::window.RageBotTab.SomeShit.GetKey();

	if (g_menu::window.RageBotTab.SomeShit.GetKey());
	{
		if (GetAsyncKeyState(g_menu::window.RageBotTab.SomeShit.GetKey()))
		{
			pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 180.f;

			{
				resolvokek::resolvemode = 2;
				if (Globals::missedshots > 4 && Globals::missedshots < 5)
				{
					if (MeetsLBYReq && lbyupdated)
					{
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
					}
					else if (!MeetsLBYReq && lbyupdated)
					{
						switch (Globals::Shots % 4)
						{
						case 1:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 180.f;
							break;
						case 2:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 90.f;
							break;
						case 3:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 90.f;
							break;
						case 4:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 180.f;
							break;
						}
					}
				}
			}
		}
	}
}

#include "RenderManager.h"
#include "RageBot.h"
/*void ResolverSetup::OverrideYaw() {
	Globals::Overriding = -1;

	if (!g_menu::window.RageBotTab.Overridex2.GetState()) return;
	if (!g_menu::window.RageBotTab.Overridex.GetKey()) return;

	int w, h, cx, cy;
	Interfaces::Engine->GetScreenSize(w, h);
	cx = w / 2;
	cy = h / 2;

	Vector crosshair = Vector(cx, cy, 0);
	int bestEnt = -1;
	float bestFOV = 0;
	Vector bestHead2D;
	for (int i = 0; i <= Interfaces::Globals->maxClients; i++) {
		IClientEntity* player = Interfaces::EntList->GetClientEntity(i);
		IClientEntity *pLocalPlayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (!IsBreakableEntity(player))
			continue;

		Vector HeadPos3D = player->GetBonePos(i), HeadPos2D;
		if (Render::WorldToScreen(HeadPos3D, HeadPos2D)) continue;
		float fov2D = crosshair.DistTo(HeadPos2D);
		if (bestEnt == -1 || fov2D < bestFOV) {
			bestEnt = i;
			bestFOV = fov2D;
			bestHead2D = HeadPos2D;
		}
	}
	if (bestEnt > -1) {
		IClientEntity* player = Interfaces::EntList->GetClientEntity(bestEnt);
		int minX = cx - (w / 10), maxX = cx + (w, 10);
		if (bestHead2D.x < minX || bestHead2D.x > maxX) return;
		int totalWidth = maxX - minX;
		int playerX = bestHead2D.x - minX;
		int yawAddtive = -(((playerX * 360) / totalWidth) - 180); //- 180 to 0 to 180
		player->GetHeadRotation()->y += yawAddtive;
		Globals::Overriding = bestEnt;
		Globals::OverridingYaw = yawAddtive;
	}
}*/

void ResolverSetup::LowerBody(IClientEntity* pEntity)
{
	if (g_menu::window.RageBotTab.AimbotResolver.GetIndex() == 1)
	{
		static int missedshots[65];
		static int lbyupdated2[64];
		static float Yaw[64];
		static float lastLby[64];
		static float newYaw[64];
		auto lby = *pEntity->GetLowerBodyYawTarget();
		auto idx = pEntity->GetIndex();
		auto velocity = pEntity->GetVelocity().Length();
		auto length2d = pEntity->GetVelocity().Length2D();
		auto target = Globals::TargetID;

		AnimationLayer prevLay, curLayer;
		curLayer = pEntity->GetAnimOverlays()[1];
		prevLay = pEntity->GetAnimOverlays()[1];
		const int cur_activity = pEntity->GetSequenceActivity(curLayer.m_nSequence);
		const int prev_activity = pEntity->GetSequenceActivity(prevLay.m_nSequence);

		Yaw[idx] = pEntity->GetEyeAnglesXY()->y;
		{
			if (cur_activity == 979) {
				if (prev_activity == 979) {
					if ((prevLay.m_flCycle != curLayer.m_flCycle) || curLayer.m_flWeight == 1.f) {
						float
							flAnimTime = curLayer.m_flCycle,
							flSimTime = pEntity->GetSimulationTime();

						if (flAnimTime < 0.01f && prevLay.m_flCycle > 0.01f && TIME_TO_TICKS(flSimTime - flAnimTime))
						{
							lbybacktrack->Update(GetTickCount64()), (flSimTime - flAnimTime);
						}

						Yaw[idx] = lastLby[idx] - 180;
					}
				}
			}

			else if (velocity > 0.1 && !isflying(pEntity)) {
				Yaw[idx] = lby;
				lastLby[idx] = lby;
			}
			else if (length2d > 6.0f && length2d < 45.f && !isflying(pEntity)) {
				Yaw[idx] = lby - lby;
			}
			else if (IsResolvableByLBY(pEntity) && missedshots[target] < 2 && !isflying(pEntity) && !IsBreakingLBY120(pEntity) && !lowerBodyPrediction(pEntity)) {
				Yaw[idx] = lby;
			}
			else if (IsBreakingLBY120(pEntity) && missedshots[target] < 2 && !lowerBodyPrediction(pEntity) && !isflying(pEntity)) {
				Yaw[idx] = newYaw[idx] + 160;
			}
			else if (lowerBodyPrediction(pEntity))
			{
				Yaw[idx] = lby;
				newYaw[idx] = lby;

				lbyupdated2[idx] = true;
			}
			if (missedshots[target] > 2 || isflying(pEntity)) {
				Yaw[idx] = lby + 60 * missedshots[Globals::TargetID];
			}
			else {
				Yaw[idx] = lastLby[idx];
			}
			pEntity->GetEyeAnglesXY()->y = Yaw[idx];
		}
	}
}

void ResolverSetup::CM(IClientEntity* pEntity)
{
	for (int x = 1; x < Interfaces::Engine->GetMaxClients(); x++)
	{
		pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(x);

		if (!pEntity
			|| pEntity == hackManager.pLocal()
			|| pEntity->IsDormant()
			|| !pEntity->IsAlive())
			continue;

		//ResolverSetup::StoreThings(pEntity);
	}
}

void ResolverSetup::FSN(IClientEntity* pEntity, ClientFrameStage_t stage)
{
	for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
	{
		pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);
		if (!pEntity || pEntity == hackManager.pLocal() || pEntity->IsDormant() || !pEntity->IsAlive())
			continue;

		int entID;

		ResolverSetup::LowerBody(pEntity);
		ResolverSetup::IsFakewalking(pEntity, entID);
		ResolverSetup::OverrideResolver(pEntity);
		//ResolverSetup::OverrideYaw()

	}
}