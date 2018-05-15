/*credits by ibanned*/

#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"
#include "Hacks.h"

void CRageBot::Init()
{

	IsAimStepping = false;
	IsLocked = false;
	TargetID = -1;
}


void FakeWalk(CUserCmd * pCmd, bool & bSendPacket)
{

	IClientEntity* pLocal = hackManager.pLocal();
	if (GetAsyncKeyState(VK_SHIFT))
	{

		static int iChoked = -1;
		iChoked++;

		if (iChoked < 1)
		{
			bSendPacket = false;



			pCmd->tick_count += 10.95; // 10.95
			pCmd->command_number += 5.07 + pCmd->tick_count % 2 ? 0 : 1; // 5
	
			pCmd->buttons |= pLocal->GetMoveType() == IN_BACK;
			pCmd->forwardmove = pCmd->sidemove = 0.f;
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;

			Interfaces::Globals->frametime *= (pLocal->GetVelocity().Length2D()) / 10; // 10
			pCmd->buttons |= pLocal->GetMoveType() == IN_FORWARD;
		}
	}
}

void CRageBot::Draw()
{

}

bool IsAbleToShoot(IClientEntity* pLocal)
{
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (!pLocal)return false;
	if (!pWeapon)return false;
	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;
	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

float hitchance(IClientEntity* pLocal, C_BaseCombatWeapon* pWeapon)
{
	float hitchance = 101;
	if (!pWeapon) return 0;
	if (g_menu::window.RageBotTab.AccuracyHitchance.GetValue() > 1)
	{
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;
	}
	return hitchance;
}

bool CanOpenFire() 
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	C_BaseCombatWeapon* entwep = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());

	float flServerTime = (float)pLocalEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}

void CRageBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{

	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());


	Fakelag(pCmd, bSendPacket);

	if (g_menu::window.AntiAimtab.AntiAimEnable.GetState())
	{
		static int ChokedPackets = -1;

		C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
		if (!pWeapon)
			return;

		if (ChokedPackets < 1 && pLocalEntity->GetLifeState() == LIFE_ALIVE && pCmd->buttons & IN_ATTACK && CanOpenFire() && GameUtils::IsBallisticWeapon(pWeapon))
		{
			bSendPacket = false;
		}
		else
		{
			if (pLocalEntity->GetLifeState() == LIFE_ALIVE)
			{
				
				DoAntiAim(pCmd, bSendPacket);
			
			

			}
			ChokedPackets = -1;
		}
	}
	
	

	if (g_menu::window.RageBotTab.AimbotEnable.GetState())
		DoAimbot(pCmd, bSendPacket);

	if (g_menu::window.RageBotTab.AccuracyRecoil.GetState())
		DoNoRecoil(pCmd);

	if (g_menu::window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 25.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 25;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}

	LastAngle = pCmd->viewangles;
}

Vector BestPoint(IClientEntity *targetPlayer, Vector &final)
{
	IClientEntity* pLocal = hackManager.pLocal();

	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = targetPlayer;
	ray.Init(final + Vector(0, 0, 10), final);
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	final = tr.endpos;
	return final;
}

void CRageBot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	Vector Start = pLocal->GetViewOffset() + pLocal->GetOrigin();
	bool FindNewTarget = true;
	CSWeaponInfo* weapInfo = ((C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle()))->GetCSWpnData();
	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (GameUtils::IsRevolver(pWeapon) && g_menu::window.RageBotTab.AutoRevolver.GetState())
	{
		static int delay = 0;
		delay++;

		if (delay <= 15)
			pCmd->buttons |= IN_ATTACK;
		else
			delay = 0;
	}

	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon)) return;
	}
	else return;
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			HitBox = HitScan(pTarget);
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset(), View;
				Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (FoV < g_menu::window.RageBotTab.AimbotFov.GetValue())	FindNewTarget = false;
			}
		}
	}


	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;
		switch (g_menu::window.RageBotTab.TargetSelection.GetIndex())
		{
		case 0:TargetID = GetTargetCrosshair(); break;
		case 1:TargetID = GetTargetDistance(); break;
		case 2:TargetID = GetTargetHealth(); break;
		case 3:TargetID = GetTargetThreat(pCmd); break;
		case 4:TargetID = GetTargetNextShot(); break;
		}
		if (TargetID >= 0) pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		else
		{


			pTarget = nullptr;
			HitBox = -1;
		}
	} 
	Globals::Target = pTarget;
	Globals::TargetID = TargetID;
	if (TargetID >= 0 && pTarget)
	{
		HitBox = HitScan(pTarget);

		if (!CanOpenFire()) return;

		if (g_menu::window.RageBotTab.AimbotKeyPress.GetState())
		{


			int Key = g_menu::window.RageBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}
		float pointscale = g_menu::window.RageBotTab.TargetPointscale.GetValue() - 5.f; 
		Vector Point;
		Vector AimPoint = GetHitboxPosition(pTarget, HitBox) + Vector(0, 0, pointscale);
		if (g_menu::window.RageBotTab.TargetMultipoint.GetState()) Point = BestPoint(pTarget, AimPoint);
		else Point = AimPoint;



		if (GameUtils::IsScopedWeapon(pWeapon) && !pWeapon->IsScoped() && g_menu::window.RageBotTab.AccuracyAutoScope.GetState()) pCmd->buttons |= IN_ATTACK2;
		else if ((g_menu::window.RageBotTab.AccuracyHitchance.GetValue() * 1.5 <= hitchance(pLocal, pWeapon)) || g_menu::window.RageBotTab.AccuracyHitchance.GetValue() == 0 || *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64)
			{
				if (AimAtPoint(pLocal, Point, pCmd, bSendPacket))
					if (g_menu::window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))pCmd->buttons |= IN_ATTACK;
					else if (pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2)return;
			}
		if (IsAbleToShoot(pLocal) && pCmd->buttons & IN_ATTACK) Globals::Shots += 1;
	}
	if (g_menu::window.RageBotTab.AutoHitChance.GetState())
	{
		if (GameUtils::IsSSG08(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(75);
		}

		else if (GameUtils::IsAK47(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(20);
		}

		else if (GameUtils::IsDEAGLE(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(65);
		}

		else if (GameUtils::IsAutoSniper(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(60);
		}

		else if (GameUtils::IsDuals(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(8);
		}

		else if (GameUtils::IsMP9(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(0);
		}

		else if (GameUtils::IsMP7(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(0);
		}

		else if (GameUtils::IsM4A1(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(35);
		}

		else if (GameUtils::IsM4A1S(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(15);
		}

		else if (GameUtils::IsFAMAS(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(20);
		}

		else if (GameUtils::IsMAC10(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(0);
		}

		else if (GameUtils::IsAWP(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(90);
		}

		else if (GameUtils::IsBizon(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(0);
		}

		else if (GameUtils::IsMP7(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(0);
		}

		else if (GameUtils::IsMP9(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(0);
		}

		else if (GameUtils::IsUMP(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(0);
		}

		else if (GameUtils::IsUSPS(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(8);
		}

		else if (GameUtils::IsGALIL(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(15);
		}

		else if (GameUtils::IsGLOCK(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(8);
		}

		else if (GameUtils::IsFSEVEN(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(8);
		}

		else if (GameUtils::IsP250(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(10);
		}

		else if (GameUtils::IsTEC9(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(15);
		}
		else if (GameUtils::IsRevolver(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(55);
		}
		else if (GameUtils::IsCZ75(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(15);
		}
		else if (GameUtils::Zeus(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyHitchance.SetValue(0);
		}

	}
	else
	{
		g_menu::window.RageBotTab.AccuracyHitchance.GetValue();
	}


	if (g_menu::window.RageBotTab.AutoMinimumDamage.GetState())
	{
		if (GameUtils::IsSSG08(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(65);
		}

		else if (GameUtils::IsAK47(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsDEAGLE(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(65);
		}

		else if (GameUtils::IsAutoSniper(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsDuals(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(10);
		}

		else if (GameUtils::IsMP9(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(10);
		}

		else if (GameUtils::IsMP7(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(10);
		}

		else if (GameUtils::IsM4A1(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsM4A1S(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsFAMAS(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsMAC10(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(1);
		}

		else if (GameUtils::IsAWP(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(85);
		}

		else if (GameUtils::IsBizon(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(1);
		}

		else if (GameUtils::IsMP7(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(1);
		}

		else if (GameUtils::IsMP9(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(1);
		}

		else if (GameUtils::IsUMP(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsUSPS(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsGALIL(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(9);
		}

		else if (GameUtils::IsGLOCK(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsFSEVEN(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsP250(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}

		else if (GameUtils::IsTEC9(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(15);
		}
		else if (GameUtils::IsRevolver(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(35);
		}
		else if (GameUtils::IsCZ75(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(35);
		}
		else if (GameUtils::Zeus(pWeapon))
		{
			g_menu::window.RageBotTab.AccuracyMinimumDamage.SetValue(20);
		}

	}
	else
	{
		g_menu::window.RageBotTab.AccuracyMinimumDamage.GetValue();
	}
}
bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{

		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || g_menu::window.RageBotTab.TargetFriendlyFire.GetState())
			{
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}

	}

	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.0f;

	Vector Angles = View;

	Vector Origin = ViewOffSet;

	Vector Delta(0, 0, 0);

	Vector Forward(0, 0, 0);

	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);

	VectorSubtract(AimPos, Origin, Delta);

	Normalize(Delta, Delta);

	FLOAT DotProduct = Forward.Dot(Delta);

	return (acos(DotProduct) * (MaxDegrees / PI));
}

int CRageBot::GetTargetCrosshair()
{

	int target = -1;
	float minFoV = g_menu::window.RageBotTab.AimbotFov.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}

		}
	}

	return target;
}

int CRageBot::GetTargetDistance()
{

	int target = -1;
	int minDist = 99999;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{

			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Distance < minDist && fov < g_menu::window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}

		}
	}

	return target;
}

int CRageBot::GetTargetNextShot()
{
	int target = -1;
	int minfov = 361;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{

		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minfov && fov < g_menu::window.RageBotTab.AimbotFov.GetValue())
				{
					minfov = fov;
					target = i;
				}
				else
					minfov = 361;
			}

		}
	}

	return target;
}

float GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	Vector ang, aim;

	AngleVectors(viewAngle, &aim);
	AngleVectors(aimAngle, &ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}

double inline __declspec (naked) __fastcall FASTSQRT(double n)
{
	_asm fld qword ptr[esp + 4]
		_asm fsqrt
	_asm ret 8
}

float VectorDistance(Vector v1, Vector v2)
{
	return FASTSQRT(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
}

int CRageBot::GetTargetThreat(CUserCmd* pCmd)
{
	auto iBestTarget = -1;
	float flDistance = 8192.f;

	IClientEntity* pLocal = hackManager.pLocal();

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			auto vecHitbox = pEntity->GetBonePos(NewHitBox);
			if (NewHitBox >= 0)
			{

				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				QAngle TempTargetAbs;
				CalcAngle(pLocal->GetEyePosition(), vecHitbox, TempTargetAbs);
				float flTempFOVs = GetFov(pCmd->viewangles, TempTargetAbs);
				float flTempDistance = VectorDistance(pLocal->GetOrigin(), pEntity->GetOrigin());
				if (flTempDistance < flDistance && flTempFOVs < g_menu::window.RageBotTab.AimbotFov.GetValue())
				{
					flDistance = flTempDistance;
					iBestTarget = i;
				}
			}
		}
	}
	return iBestTarget;
}

int CRageBot::GetTargetHealth()
{

	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);


	for (int i = 0; i < Interfaces::EntList->GetMaxEntities(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov < g_menu::window.RageBotTab.AimbotFov.GetValue())
				{
					minHealth = Health;
					target = i;
				}
			}
		}

	}

	return target;
}

int CRageBot::HitScan(IClientEntity* pEntity)
{
	IClientEntity* pLocal = hackManager.pLocal();
	std::vector<int> HitBoxesToScan;

	// Get the hitboxes to scan
#pragma region GetHitboxesToScan
	int HitScanMode = g_menu::window.RageBotTab.TargetHitscan.GetIndex();
	int huso = (pEntity->GetHealth());
	int health = g_menu::window.RageBotTab.BaimIfUnderXHealth.GetValue();
	bool AWall = g_menu::window.RageBotTab.AccuracyAutoWall.GetState();
	bool Multipoint = g_menu::window.RageBotTab.TargetMultipoint.GetState();

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	std::vector<int> hitboxes;
	bool bRevert = true;

	if (huso < health)
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
	}
	else
	{
		if (g_menu::window.RageBotTab.Hitboxhead.GetState())
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
		}
		if (g_menu::window.RageBotTab.Hitboxneck.GetState())
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
			HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
		}
		if (g_menu::window.RageBotTab.Hitboxchest.GetState())
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
		}
		if (g_menu::window.RageBotTab.Hitboxarms.GetState())
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
		}
		if (g_menu::window.RageBotTab.Hitboxstomach.GetState())
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
		}
		if (g_menu::window.RageBotTab.Hitboxlegs.GetState())
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
			HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
		}
		else if (g_menu::window.RageBotTab.AimbotAutoFire.GetState() && GameUtils::Zeus(pWeapon))
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
		}

		else if (pEntity->GetVelocity().Length2D() < 36.f && pEntity->GetVelocity().Length2D() > 20.f)
		{
			HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
			HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
			HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
			HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
		}
	
	}
	
#pragma endregion Get the list of shit to scan

	// check hits
	for (auto HitBoxID : HitBoxesToScan)
	{
		if (HitBoxID >= 0)
		{
			Vector Point = GetHitboxPosition(pEntity, HitBoxID);
			Vector Pointhd = GetHitboxPosition(pEntity, 0);
			float headdmg = 0.f;
			float Damage = 0.f;
			Color c = Color(255, 255, 255, 255);

			if (CanHit(pEntity, Point, &Damage))
			{
				/*if (headdmg > Damage && !g_menu::window.RageBotTab.AWPAtBody.GetState() && !g_menu::window.RageBotTab.PreferBodyAim.GetState())
				bestHitbox = 0;*/
				autowalldmgtest[pEntity->GetIndex()] = Damage;
				if ((Damage >= g_menu::window.RageBotTab.AccuracyMinimumDamage.GetValue()) || (g_menu::window.RageBotTab.AccuracyMinimumDamage.GetValue() > pEntity->GetHealth()))
				{
					return HitBoxID;
				}
			}
		}
	}
	return -1;
}
#pragma endregion Get the list of shit to scan


void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{

	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}

}

void CRageBot::aimAtPlayer(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon)
		return;

	Vector eye_position = pLocal->GetEyePosition();

	float best_dist = pWeapon->GetCSWpnData()->m_flRange;

	IClientEntity* target = nullptr;

	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			if (Globals::TargetID != -1)
				target = Interfaces::EntList->GetClientEntity(Globals::TargetID);
			else
				target = pEntity;

			Vector target_position = target->GetEyePosition();

			float temp_dist = eye_position.DistTo(target_position);

			if (best_dist > temp_dist)
			{
				best_dist = temp_dist;
				CalcAngle(eye_position, target_position, pCmd->viewangles);
			}
		}

	}
}


bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket)
{
	bool ReturnValue = false;

	if (point.Length() == 0) return ReturnValue;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return ReturnValue;
	}

	IsLocked = true;

	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	if (!IsAimStepping)
		LastAimstepAngle = LastAngle; 

	float fovLeft = FovToPlayer(ViewOffset, LastAimstepAngle, Interfaces::EntList->GetClientEntity(TargetID), 0);

	if (fovLeft > 25.0f && g_menu::window.RageBotTab.AimbotAimStep.GetState())
	{

		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 25;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}

	if (g_menu::window.RageBotTab.AimbotSilentAim.GetState())
	{
		pCmd->viewangles = angles;

	}

	if (!g_menu::window.RageBotTab.AimbotSilentAim.GetState())
	{

		Interfaces::Engine->SetViewAngles(angles);
	}

	return ReturnValue;
}

namespace AntiAims 
{

	void b1gJoplinSwitch(CUserCmd *pCmd, bool &bSendPacket)
	{
		IClientEntity* pLocal = hackManager.pLocal();

		if (!bSendPacket) {
			if (g_menu::window.AntiAimtab.SWSwitchKey.GetKey()) {
				pCmd->viewangles.y -= 90;
				if (GetKeyState(g_menu::window.AntiAimtab.SWSwitchKey.GetKey()) && pLocal->IsAlive()) {
					pCmd->viewangles.y += 180;
				}
			}
		}
		else {
			if (g_menu::window.AntiAimtab.SWSwitchKey.GetKey()) {
				pCmd->viewangles.y += 90;
				if (GetKeyState(g_menu::window.AntiAimtab.SWSwitchKey.GetKey()) && pLocal->IsAlive()) {
					pCmd->viewangles.y -= 180;
				}
			}
		}
	}

	bool jitter = false;
	void minimal(CUserCmd *pCmd)
	{
		IClientEntity* plocalminimal = hackManager.pLocal();
		static float Timer = 0;
		Timer++;
		if (!plocalminimal->IsAlive())
			Timer = 0;

		if (plocalminimal->GetFlags() & FL_ONGROUND)
		{
			if (Timer > 1 && Timer < 50)
			{
				pCmd->viewangles.x = 1080;
			}
			else
				pCmd->viewangles.x = 87;
		}
		else
		{
			Timer = 0;
			pCmd->viewangles.x = 87;
		}
	}
	void JitterPitch(CUserCmd *pCmd)
	{
		static bool up = true;
		if (up)
		{
			pCmd->viewangles.x = 45;
			up = !up;
		}
		else
		{
			pCmd->viewangles.x = 89;
			up = !up;
		}
	}



	void JitterBackward1(CUserCmd *pCmd, bool &bSendPacket)
	{
		int random;
		int maxJitter;
		float temp;
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 180.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}
	void jitteraa(CUserCmd *pCmd, bool &bSendPacket)
	{
		if (bSendPacket = false)
		{
			static bool flip;
			if (flip)
				pCmd->viewangles.y += 179;
			else
				pCmd->viewangles.y += 149;
			flip = !flip;
		}
		else
		{
			static bool flip;
			if (flip)
				pCmd->viewangles.y += 149;
			else
				pCmd->viewangles.y += 179;
			flip = !flip;
		}
	}
	void swap(CUserCmd *pCmd)
	{
		static bool flip = true;

		if (flip) pCmd->viewangles.y += 90.0f;
		else pCmd->viewangles.y -= 90.0f;

		static clock_t start_t = clock();
		double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
		if (timeSoFar < .75)
			return;
		flip = !flip;
		start_t = clock();
	}
	void lowerbody(CUserCmd *pCmd, bool &bSendPacket)
	{
		int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); ++i;
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		static bool isMoving;
		float PlayerIsMoving = abs(pLocal->GetVelocity().Length());
		if (PlayerIsMoving > 0.1) isMoving = true;
		else if (PlayerIsMoving <= 0.1) isMoving = false;
		int meme = rand() % 75;
		if (meme < 17)
			meme = 17;
		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		static bool bFlipYaw;
		float flInterval = Interfaces::Globals->interval_per_tick;
		float flTickcount = pCmd->tick_count;
		float flTime = flInterval * flTickcount;
		if (std::fmod(flTime, 1) == 0.f)
			bFlipYaw = !bFlipYaw;

		if (PlayerIsMoving <= 0.4)
		{
			if (bSendPacket)
			{
				pCmd->viewangles.y += bFlipYaw ? (90.f + meme) : (-90 - meme);
			}
			else
			{
				if (flip)
				{
					pCmd->viewangles.y += bFlipYaw ? (90.f + meme) : (-90 - meme);

				}
				else
				{
					pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + bFlipYaw ? (90.f + meme) : (-90 - meme);
				}
			}
		}
		else
		{
			if (bSendPacket)
			{
				pCmd->viewangles.y += 5.f - meme;
			}
			else
			{
				pCmd->viewangles.y += 160.f - meme;
			}
		}
	}
	
	void z180(CUserCmd *pCmd)
	{
		static int Ticks;
		pCmd->viewangles.y += Ticks;
		
		Ticks += 4;

		if (Ticks > 180)
			Ticks = 15;
	}

	// Yaws

	void FastSpin(CUserCmd *pCmd)
	{
		static int y2 = -179;
		int spinBotSpeedFast = 100;

		y2 += spinBotSpeedFast;

		if (y2 >= 179)
			y2 = -179;

		pCmd->viewangles.y = y2;
	}

	
	void BackJitter(CUserCmd *pCmd)
	{
		int random = rand() % 100;

		if (random < 98)

			pCmd->viewangles.y -= 180;

		if (random < 15)
		{
			float change = -70 + (rand() % (int)(140 + 1));
			pCmd->viewangles.y += change;
		}
		if (random == 69)
		{
			float change = -90 + (rand() % (int)(180 + 1));
			pCmd->viewangles.y += change;
		}
	}

	void BackwardJitter(CUserCmd *pCmd)
	{
		int random = rand() % 100;

		if (random < 98)

			pCmd->viewangles.y -= 180;

		if (random < 15)
		{
			float change = -70 + (rand() % (int)(140 + 1));
			pCmd->viewangles.y += change;
		}
		if (random == 69)
		{
			float change = -90 + (rand() % (int)(180 + 1));
			pCmd->viewangles.y += change;
		}
	}

	void Jitter(CUserCmd *pCmd)
	{
		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 90;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y -= 90;
		}

		int re = rand() % 4 + 1;


		if (jitterangle <= 1)
		{
			if (re == 4)
				pCmd->viewangles.y += 180;
			jitterangle += 1;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			if (re == 4)
				pCmd->viewangles.y -= 180;
			jitterangle += 1;
		}
		else
		{
			jitterangle = 0;
		}
	}

	void Up(CUserCmd *pCmd)
	{
		pCmd->viewangles.x = -89.0f;
	}

	void Static(CUserCmd *pCmd)
	{
		static bool aa1 = false;
		aa1 = !aa1;
		if (aa1)
		{
			static bool turbo = false;
			turbo = !turbo;
			if (turbo)
			{
				pCmd->viewangles.y -= 90;
			}
			else
			{
				pCmd->viewangles.y += 90;
			}
		}
		else
		{
			pCmd->viewangles.y -= 180;
		}
	}
}

void CorrectMovement(Vector old_angles, CUserCmd* cmd, float old_forwardmove, float old_sidemove)
{
	float delta_view, first_function, second_function;

	if (old_angles.y < 0.f) first_function = 360.0f + old_angles.y;
	else first_function = old_angles.y;
	if (cmd->viewangles.y < 0.0f) second_function = 360.0f + cmd->viewangles.y;
	else second_function = cmd->viewangles.y;

	if (second_function < first_function) delta_view = abs(second_function - first_function);
	else delta_view = 360.0f - abs(first_function - second_function);

	delta_view = 360.0f - delta_view;

	cmd->forwardmove = cos(DEG2RAD(delta_view)) * old_forwardmove + cos(DEG2RAD(delta_view + 90.f)) * old_sidemove;
	cmd->sidemove = sin(DEG2RAD(delta_view)) * old_forwardmove + sin(DEG2RAD(delta_view + 90.f)) * old_sidemove;
}

float GetLatency()
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{

		float Latency = nci->GetAvgLatency(FLOW_OUTGOING) + nci->GetAvgLatency(FLOW_INCOMING);
		return Latency;
	}
	else
	{

		return 0.0f;
	}
}
float GetOutgoingLatency()
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{

		float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
		return OutgoingLatency;
	}
	else
	{

		return 0.0f;
	}
}
float GetIncomingLatency()
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{
		float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
		return IncomingLatency;
	}
	else
	{

		return 0.0f;
	}
}

float OldLBY;

float LastLBYUpdateTime;
bool bSwitch;
static float next1 = 0;
float CurrentVelocity(IClientEntity* LocalPlayer)
{
	int vel = LocalPlayer->GetVelocity().Length2D();
	return vel;
}

static bool fuckassnig = false;

bool NextLBYUpdate()
{
	bool lby_flip = false;

	IClientEntity* LocalPlayer = hackManager.pLocal();
	if (!LocalPlayer)//null check
		return false;

	float curtime = (float)(LocalPlayer->GetTickBase()  * Interfaces::Globals->interval_per_tick);
	static float NextLBYUpdate1 = 0;

	if (NextLBYUpdate1 > curtime + 1.1)
	{
		NextLBYUpdate1 = 0;
	}
	auto key1 = g_menu::window.MiscTab.FakeWalk.GetKey();
	if (LocalPlayer->GetVelocity().Length2D() > 0.1f && !GetAsyncKeyState(key1))
	{
		NextLBYUpdate1 = curtime + 0.22 + Interfaces::Globals->interval_per_tick;
		lby_flip = false;
		return false;
	}

	if ((NextLBYUpdate1 < curtime) && (LocalPlayer->GetFlags() & FL_ONGROUND) && LocalPlayer->GetVelocity().Length2D() < 1.f)
	{
		NextLBYUpdate1 = curtime + 1.1 + Interfaces::Globals->interval_per_tick;
		lby_flip = true;
		fuckassnig = true;
		return true;
	}
	lby_flip = false;
	fuckassnig = false;
	return false;
}


float curtime_fixed(CUserCmd* ucmd) {
	IClientEntity* local_player = hackManager.pLocal();
	static int g_tick = 0;
	static CUserCmd* g_pLastCmd = nullptr;
	if (!g_pLastCmd || g_pLastCmd->hasbeenpredicted) {
		g_tick = local_player->GetTickBase();
	}
	else {
		// Required because prediction only runs on frames, not ticks
		// So if your framerate goes below tickrate, m_nTickBase won't update every tick
		++g_tick;
	}
	g_pLastCmd = ucmd;
	float curtime = g_tick * Interfaces::Globals->interval_per_tick;
	return curtime;
}

bool NextLBYUpdate2(CUserCmd* pcmd)
{
	IClientEntity* LocalPlayer = hackManager.pLocal();

	float curtime = curtime_fixed(pcmd);

	static float NextLBYUpdate1 = 0;

	if (NextLBYUpdate1 > curtime + 1.1)
	{
		NextLBYUpdate1 = 0;
	}
	if (LocalPlayer->GetVelocity().Length2D() > 0.1f)
	{
		NextLBYUpdate1 = curtime + 0.22 + Interfaces::Globals->interval_per_tick;
		return false;
	}

	if ((NextLBYUpdate1 < curtime) && (LocalPlayer->GetFlags() & FL_ONGROUND) && LocalPlayer->GetVelocity().Length2D() < 1.f)
	{
		NextLBYUpdate1 = curtime + 1.1 + Interfaces::Globals->interval_per_tick;
		return true;
	}
	return false;
}



void SideJitterALT(CUserCmd * pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	if (!bSendPacket)
	{
		static bool Fast2 = false;
		if (Fast2)
		{
			pCmd->viewangles.y += 75;
		}
		else
		{
			pCmd->viewangles.y += 105;
		}
		Fast2 = !Fast2;
	}
	else
	{
		pCmd->viewangles.y += 90;
	}
}

void SideJitter(CUserCmd * pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	if (!bSendPacket)
	{
		static bool Fast2 = false;
		if (Fast2)
		{
			pCmd->viewangles.y -= 75;
		}
		else
		{
			pCmd->viewangles.y -= 105;
		}
		Fast2 = !Fast2;
	}
	else
	{
		pCmd->viewangles.y -= 90;
	}
}


void DoLBYBreak(CUserCmd * pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	if (!bSendPacket)
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y += 90;
		else
			pCmd->viewangles.y -= 90;
	}
	else
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y -= 90;
		else
			pCmd->viewangles.y += 90;
	}
}

void DoLBYBreakReal(CUserCmd * pCmd, IClientEntity* pLocal, bool& bSendPacket)
{
	if (!bSendPacket)
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y -= 90;
		else
			pCmd->viewangles.y += 90;
	}
	else
	{
		if (NextLBYUpdate())
			pCmd->viewangles.y += 90;
		else
			pCmd->viewangles.y -= 90;
	}
}

static bool peja;
static bool switchbrak;
static bool safdsfs;

void AngleVectors3(const QAngle &angles, Vector& forward, Vector& right, Vector& up)
{
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[0]), &sp, &cp);
	SinCos(DEG2RAD(angles[1]), &sy, &cy);
	SinCos(DEG2RAD(angles[2]), &sr, &cr);

	forward.x = (cp * cy);
	forward.y = (cp * sy);
	forward.z = (-sp);
	right.x = (-1 * sr * sp * cy + -1 * cr * -sy);
	right.y = (-1 * sr * sp * sy + -1 * cr *  cy);
	right.z = (-1 * sr * cp);
	up.x = (cr * sp * cy + -sr * -sy);
	up.y = (cr * sp * sy + -sr * cy);
	up.z = (cr * cp);
}

float GetBestHeadAngle(IClientEntity* player, float yaw)
{
	float Back, Right, Left;

	Vector src3D, dst3D, forward, right, up, src, dst;
	trace_t tr;
	Ray_t backray, rightray, leftray;
	CTraceFilter filter;

	QAngle angles;
	Interfaces::Engine->GetViewAngles(angles);
	angles.x = 0.f;

	AngleVectors3(angles, forward, right, up);

	filter.pSkip = player;
	src3D = player->GetEyePosition();
	dst3D = src3D + (forward * 384.f);

	backray.Init(src3D, dst3D);
	Interfaces::Trace->TraceRay(backray, MASK_SHOT, &filter, &tr);
	Back = (tr.endpos - tr.startpos).Length();

	rightray.Init(src3D + right * 35.f, dst3D + right * 35.f);
	Interfaces::Trace->TraceRay(rightray, MASK_SHOT, &filter, &tr);
	Right = (tr.endpos - tr.startpos).Length();

	leftray.Init(src3D - right * 35.f, dst3D - right * 35.f);
	Interfaces::Trace->TraceRay(leftray, MASK_SHOT, &filter, &tr);
	Left = (tr.endpos - tr.startpos).Length();

	if (Left > Right)
		return (yaw - 90.f);
	else if (Right > Left)
		return (yaw + 90.f);
	else if (Back > Right || Back > Left)
		return (yaw - 180.f);

	return 0;
}
static bool dir = false;
static bool back = false;

void DoRealAA(CUserCmd* pCmd, IClientEntity* pLocal, bool& bSendPacket)
{

	if (GetAsyncKeyState(VK_RIGHT)) // right
	{
		dir = false; back = false; 	bigboi::indicator = 1;
	}
	else if (GetAsyncKeyState(VK_LEFT)) // left
	{
		dir = true; back = false; 	bigboi::indicator = 2;
	}
	else if (GetAsyncKeyState(VK_DOWN)) // back
	{
		back = true; 	bigboi::indicator = 3;
	}


	Vector SpinAngles = { 0,0,0 };
	Vector StartAngles;
	static bool switch2;
	Vector oldAngle = pCmd->viewangles;
	float antilby = g_menu::window.AntiAimtab.Antilby.GetValue();
	float oldForward = pCmd->forwardmove;
	float oldSideMove = pCmd->sidemove;
	if (!g_menu::window.AntiAimtab.AntiAimEnable.GetState())
		return;

	CUserCmd* pcmd;
	if (g_menu::window.AntiAimtab.BreakLBY.GetIndex() == 0)
	{
		//nothing nigger
	}



	if (g_menu::window.AntiAimtab.BreakLBY.GetIndex() == 1)
	{
		if (NextLBYUpdate())
		{
			//    if (side == AAYaw::ADAPTIVE_LEFT) //prob what u are here for, ignore the adaptive shit, it was for testing. hf
			pCmd->viewangles.y += antilby;
			//    else if (side == AAYaw::ADAPTIVE_RIGHT)
			pCmd->viewangles.y -= antilby;
			//else
			{
				pCmd->viewangles.y += antilby;
			}
		}
	}

	if (g_menu::window.AntiAimtab.BreakLBY.GetIndex() == 2)
	{
	

				if (NextLBYUpdate) {

					pCmd->viewangles.y += 135;
					// pCmd->viewangles.x -= 180;
				}
			
	}

	if (g_menu::window.AntiAimtab.BreakLBY.GetIndex() == 3)
	{
		//pwned cuz antipasta
	}


	if (g_menu::window.AntiAimtab.SWSwitchEnable.GetState())
	{
		AntiAims::b1gJoplinSwitch(pCmd, bSendPacket);
	}

	static QAngle angles;
	IClientEntity* localp = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	QAngle RealAngle;


	float temp = pCmd->viewangles.y;
	static int Ticks;
	static int Side;

	switch (g_menu::window.AntiAimtab.AntiAimYaw.GetIndex())
	{
	case 0:
		break;
	case 1:
		// 180jitter
		pCmd->viewangles.y -= 180;
		break;
	case 2:
		AntiAims::jitteraa(pCmd, bSendPacket);
		break;
	case 3:
		AntiAims::lowerbody(pCmd, bSendPacket);
		break;
	case 4:
		// backjitter
		AntiAims::swap(pCmd);
		break;
	case 5:
		// backjitter
		AntiAims::FastSpin(pCmd);
		break;
	case 6:
		Interfaces::Engine->GetViewAngles(angles);

		temp = GetBestHeadAngle(localp, angles.y);
		break;
	case 7:
	{
		if (dir && !back)
		{
			back = false;
			pCmd->viewangles.y += 90;
			bigboi::indicator = 2;
		}
		else if (!dir && !back)
		{
			back = false;
			pCmd->viewangles.y -= 90;
			bigboi::indicator = 1;
		}
		else if (back)
		{
			pCmd->viewangles.y += 180;
			bigboi::indicator = 3;
		}
		break;
	}
	}
	IClientEntity* Localplayer = hackManager.pLocal();
	if (!(Localplayer->GetFlags() & FL_ONGROUND)) {
		switch (g_menu::window.AntiAimtab.InAir.GetIndex())
		{
			//bSendPacket = false;
		case 0:
			break;
		case 1:
			AntiAims::z180(pCmd);
			break;
		case 2:
			pCmd->viewangles.y -= 180;
			break;
		
		}
	}
}

void DoFakeAA(CUserCmd* pCmd, bool& bSendPacket, IClientEntity* pLocal)
{
	int spinspeed = 61;
	int y2 = -90;

	static bool switch2;
	Vector oldAngle = pCmd->viewangles;
	float oldForward = pCmd->forwardmove;
	float oldSideMove = pCmd->sidemove;
	if (!g_menu::window.AntiAimtab.AntiAimEnable.GetState())
		return;
	switch (g_menu::window.AntiAimtab.FakeYaw.GetIndex())
	{
	case 0:
		break;
	case 1:
		AntiAims::JitterBackward1(pCmd, bSendPacket);
		break;
	case 2:
	{

		pCmd->viewangles.y = (pLocal->GetLowerBodyYaw() + 140 + rand() % 123) - rand() % 20;
	}
	break;
	case 3:
	{



		float localview = pCmd->viewangles.y;

		if (pLocal->GetHealth() > 45)
		{
			y2 += spinspeed;

			if (y2 >= (localview + (117 + rand() % 5)))
				y2 = (localview - (117 - rand() % 5));

			pCmd->viewangles.y = y2;
		}
		else if (CanOpenFire())
		{
			if (NextLBYUpdate())
			{
				if (switch2)
					pCmd->viewangles.y = pLocal->GetLowerBodyYaw() + 119.2;
				else
					pCmd->viewangles.y = pLocal->GetLowerBodyYaw() + (170 + rand() % 9);

			}
			else
			{
				pCmd->viewangles.y = pLocal->GetLowerBodyYaw() + 90;
			}
		}

		else
		{

			if (switch2)
				pCmd->viewangles.y = pLocal->GetLowerBodyYaw() - 119.2;
			else
				pCmd->viewangles.y = pLocal->GetLowerBodyYaw() + 119.2;


		}
		switch2 = !switch2;



	}
	break;
	case 4:
	{
#define RandomInt(min, max) (rand() % (max - min + 1) + min)
		static bool fakeantiaim;
		int rand2;
		{
			int var1;
			int var2;
			float var3;

			pCmd->viewangles.y -= 179.9;
			var1 = rand() % 100;
			var2 = rand() % (10 - 6 + 1) + 10;
			var3 = var2 - (rand() % var2);
			if (var1 < 60 + (rand() % 14))
				pCmd->viewangles.y -= var3;
			else if (var1 < 100 + (rand() % 14))
				pCmd->viewangles.y += var3;
		}

		if (fakeantiaim)
		{
			rand2 = RandomInt(1, 100);

			if (rand2 < 2.0)
			{
				bSendPacket = true;
				pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 92.3 - 0 - 31.3;
			}

			else
			{
				bSendPacket = true;
				pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 91.7;
			}
			fakeantiaim = false;
		}
		else
		{
			bSendPacket = false;
			pCmd->viewangles.y -= 154.4;
			fakeantiaim = true;
		}

	}
		break;
	case 5:
	{
		y2 += spinspeed;

		if (y2 >= 175)
			y2 = -175;

		pCmd->viewangles.y = y2;
	}
		break;




		


	
	}

		if (hackManager.pLocal()->GetVelocity().Length() > 0) {
		switch (g_menu::window.AntiAimtab.MoveYawFake.GetIndex())
		{
			//bSendPacket = false;
		case 0:
			break;
		case 1:
			// 180 Jitter 
			AntiAims::BackJitter(pCmd);
			break;
		case 2:
			//backwards
			pCmd->viewangles.y -= 180;
			break;
		case 3:
			AntiAims::Jitter(pCmd);
			break;
		case 4:
			AntiAims::BackwardJitter(pCmd);
			break;
		}
	}
}

void CRageBot::DoAntiAim(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();
	static bool switch4 = false;
	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER)
		return;

	if (IsAimStepping || pCmd->buttons & IN_ATTACK)
		return;

	if (warmup)
		return;

	C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (pWeapon)
	{
		CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();

		if (!GameUtils::IsBallisticWeapon(pWeapon))
		{
			if (!CanOpenFire() || pCmd->buttons & IN_ATTACK2)
				return;

		}
	}
	if (g_menu::window.RageBotTab.AntiAimTarget.GetState())
	{
		aimAtPlayer(pCmd);

	}

	FakeWalk(pCmd, bSendPacket);



	switch (g_menu::window.AntiAimtab.AntiAimPitch.GetIndex())
	{
	case 0:
		break;
	case 1:
		pCmd->viewangles.x = 89.000000;
		break;
	case 2:
		AntiAims::minimal(pCmd);
		break;
	case 3:
	//fakedown
	{
		if (pLocal->IsScoped() && pLocal->GetVelocity().Length() < 10)
		{
			if (IN_DUCK)
			{

				if (NextLBYUpdate())
					pCmd->viewangles.x = -65;
				else
					pCmd->viewangles.x = 88.9;


			}
			else
			{
				if (NextLBYUpdate())
					pCmd->viewangles.x = -88.999;
				else
				{
					if (switch4)
						pCmd->viewangles.x = 86;
					else
						pCmd->viewangles.x = 88;
				}
			}
			switch4 = !switch4;
		}
		else if (pLocal->GetVelocity().Length() >= 190 && pLocal->GetFlags() && FL_ONGROUND)
		{
			pCmd->viewangles.x;
		}
		else
		{
			if (NextLBYUpdate())
				pCmd->viewangles.x = -79;
			else
				pCmd->viewangles.x = 88.9;
		}

	}
	break;
	case 4:
		AntiAims::JitterPitch(pCmd);
		break;

	}
	switch (g_menu::window.AntiAimtab.AntiAimEdge.GetIndex())
	{
	case 0:
		break;
	case 1:
		//EdgeDetect(pCmd, bSendPacket);
		break;
	case 2:
		//FakeEdge(pCmd, bSendPacket);
		break;


	}

	if (g_menu::window.AntiAimtab.AntiAimEnable.GetState())
	{
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{

			bSendPacket = true;
			DoFakeAA(pCmd, bSendPacket, pLocal);
		}
		else
		{

			bSendPacket = false;
			DoRealAA(pCmd, pLocal, bSendPacket);
			ChokedPackets = -1;
		}

		if (flipAA)
		{
			pCmd->viewangles.y -= 25;
		}
	}

}


void EdgeDetect(CUserCmd* pCmd, bool &bSendPacket)
{
	Ray_t ray;
	trace_t tr;

	IClientEntity* pLocal = hackManager.pLocal();

	CTraceFilter traceFilter;
	traceFilter.pSkip = pLocal;

	bool bEdge = false;

	Vector angle;
	Vector eyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

	for (float i = 0; i < 360; i++)
	{
		Vector vecDummy(10.f, pCmd->viewangles.y, 0.f);
		vecDummy.y += i;

		Vector forward = vecDummy.Forward();

		//vecDummy.NormalizeInPlace();

		float flLength = ((16.f + 3.f) + ((16.f + 3.f) * sin(DEG2RAD(10.f)))) + 7.f;
		forward *= flLength;

		Ray_t ray;
		CGameTrace tr;

		ray.Init(eyePos, (eyePos + forward));
		Interfaces::Trace->EdgeTraceRay(ray, traceFilter, tr, true);

		if (tr.fraction != 1.0f)
		{
			Vector negate = tr.plane.normal;
			negate *= -1;

			Vector vecAng = negate.Angle();

			vecDummy.y = vecAng.y;

			//vecDummy.NormalizeInPlace();
			trace_t leftTrace, rightTrace;

			Vector left = (vecDummy + Vector(0, 45, 0)).Forward(); // or 45
			Vector right = (vecDummy - Vector(0, 45, 0)).Forward();

			left *= (flLength * cosf(rad(30)) * 2); //left *= (len * cosf(rad(30)) * 2);
			right *= (flLength * cosf(rad(30)) * 2); // right *= (len * cosf(rad(30)) * 2);

			ray.Init(eyePos, (eyePos + left));
			Interfaces::Trace->EdgeTraceRay(ray, traceFilter, leftTrace, true);

			ray.Init(eyePos, (eyePos + right));
			Interfaces::Trace->EdgeTraceRay(ray, traceFilter, rightTrace, true);

			if ((leftTrace.fraction == 1.f) && (rightTrace.fraction != 1.f))
			{
				vecDummy.y -= 45; // left
			}
			else if ((leftTrace.fraction != 1.f) && (rightTrace.fraction == 1.f))
			{
				vecDummy.y += 45; // right     
			}

			angle.y = vecDummy.y;
			angle.y += 360;
			bEdge = true;
		}
	}

	if (bEdge)
	{
		static bool turbo = true;

		switch (g_menu::window.AntiAimtab.AntiAimEdge.GetIndex())
		{
		case 0:
			// Nothing
			break;
		case 1:
			// Regular
			pCmd->viewangles.y = angle.y;
			break;
			
		}
	}
}

void FakeEdge(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	Vector vEyePos = pLocal->GetOrigin() + pLocal->GetViewOffset();

	CTraceFilter filter;
	filter.pSkip = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	for (int y = 0; y < 360; y++)
	{
		Vector qTmp(10.0f, pCmd->viewangles.y, 0.0f);
		qTmp.y += y;

		if (qTmp.y > 180.0)
			qTmp.y -= 360.0;
		else if (qTmp.y < -180.0)
			qTmp.y += 360.0;

		GameUtils::NormaliseViewAngle(qTmp);

		Vector vForward;

		VectorAngles(qTmp, vForward);

		float fLength = (19.0f + (19.0f * sinf(DEG2RAD(10.0f)))) + 7.0f;
		vForward *= fLength;

		trace_t tr;

		Vector vTraceEnd = vEyePos + vForward;

		Ray_t ray;

		ray.Init(vEyePos, vTraceEnd);
		Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &tr);

		if (tr.fraction != 1.0f)
		{
			Vector angles;

			Vector vNegative = Vector(tr.plane.normal.x * -1.0f, tr.plane.normal.y * -1.0f, tr.plane.normal.z * -1.0f);

			VectorAngles(vNegative, angles);

			GameUtils::NormaliseViewAngle(angles);

			qTmp.y = angles.y;

			GameUtils::NormaliseViewAngle(qTmp);

			trace_t trLeft, trRight;

			Vector vLeft, vRight;
			VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vLeft);
			VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vRight);

			vLeft *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
			vRight *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));

			vTraceEnd = vEyePos + vLeft;

			ray.Init(vEyePos, vTraceEnd);
			Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trLeft);

			vTraceEnd = vEyePos + vRight;

			ray.Init(vEyePos, vTraceEnd);
			Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trRight);

			if ((trLeft.fraction == 1.0f) && (trRight.fraction != 1.0f))
				qTmp.y -= 90.f;
			else if ((trLeft.fraction != 1.0f) && (trRight.fraction == 1.0f))
				qTmp.y += 90.f;

			if (qTmp.y > 180.0)
				qTmp.y -= 360.0;
			else if (qTmp.y < -180.0)
				qTmp.y += 360.0;

			pCmd->viewangles.y = qTmp.y;

			int offset = g_menu::window.AntiAimtab.AntiAimOffset.GetValue();

			static int ChokedPackets = -1;
			ChokedPackets++;
			if (ChokedPackets < 1)
			{
				bSendPacket = false; // +=180?
			}
			else
			{
				bSendPacket = true;
				pCmd->viewangles.y -= offset;
				ChokedPackets = -1;
			}
			return;
		}
	}
	pCmd->viewangles.y += 360.0f;
}


void CRageBot::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
	int iChoke = g_menu::window.RageBotTab.Fakelagvalue.GetValue();

	static int iFakeLag = -1;
	iFakeLag++;

	if (iFakeLag <= iChoke && iFakeLag > -1)
	{
		bSendPacket = false;
	}
	else
	{
		bSendPacket = true;
		iFakeLag = -1;
	}
}





