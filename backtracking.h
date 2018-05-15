#pragma once


#include "Hooks.h"
#include "Hacks.h"
#include "ESP.h"
#include "Entities.h"
#include "Resolver.h"


#define TIME_TO_TICKS( dt )     ( (int)( 0.5f + (float)(dt) / Interfaces::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )      ( Interfaces::Globals->interval_per_tick * ( t ) )
#define LC_ORIGIN_CHANGED	(1<<8)
#define LC_ANGLES_CHANGED	(1<<9)

template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

struct LayerRecord
{
	int m_sequence;
	float m_cycle;
	float m_weight;
	int m_order;

	LayerRecord()
	{
		Clear();
	}

	LayerRecord(const LayerRecord& src)
	{
		m_sequence = src.m_sequence;
		m_cycle = src.m_cycle;
		m_weight = src.m_weight;
		m_order = src.m_order;
	}

	void Clear()
	{
		m_sequence = 0;
		m_cycle = 0;
		m_weight = 0;
		m_order = 0;
	}
};

struct LagRecord
{
public:
	LagRecord()
	{
		m_fFlags = 0;
		m_vecOrigin.Init();
		m_vecAngles.Init();
		m_vecMinsPreScaled.Init();
		m_vecMaxsPreScaled.Init();
		m_vecPreviousAngles.Init();
		m_EyeAngles.Init();
		m_flSimulationTime = -1;
		m_masterSequence = 0;
		m_masterCycle = 0;
		m_bValidRecord = false;
		m_flPreviousSimulationTime = -1;
	}

	LagRecord(const LagRecord& src)
	{
		m_fFlags = src.m_fFlags;
		m_vecOrigin = src.m_vecOrigin;
		m_vecAngles = src.m_vecAngles;
		m_vecMinsPreScaled = src.m_vecMinsPreScaled;
		m_vecMaxsPreScaled = src.m_vecMaxsPreScaled;
		m_flSimulationTime = src.m_flSimulationTime;
		m_EyeAngles = src.m_EyeAngles;
		m_vecPreviousAngles = src.m_vecPreviousAngles;
		m_vecPreviousOrigin = src.m_vecPreviousOrigin;
		m_flPreviousSimulationTime = src.m_flPreviousSimulationTime;
		m_bValidRecord = src.m_bValidRecord;
		for (int layerIndex = 0; layerIndex < 15; ++layerIndex)
		{
			m_layerRecords[layerIndex] = src.m_layerRecords[layerIndex];
		}
		m_masterSequence = src.m_masterSequence;
		m_masterCycle = src.m_masterCycle;
	}

	// Did player die this frame
	int                     m_fFlags;

	// Player position, orientation and bbox
	Vector                  m_vecOrigin;
	QAngle                  m_vecAngles;
	Vector                  m_vecMinsPreScaled;
	Vector org;
	Vector                  m_vecMaxsPreScaled;
	Vector                  m_EyeAngles;
	Vector                      m_vecPreviousAngles;
	Vector                      m_vecPreviousOrigin;
	float                       m_flPreviousSimulationTime;

	float                   m_flSimulationTime;
	bool                    m_bValidRecord;
	// Player animation details, so we can get the legs in the right spot.
	LayerRecord             m_layerRecords[15];
	int                     m_masterSequence;
	float                   m_masterCycle;
};

class CLagCompensation
{
private:
	//Player Data
	LagRecord CurPlayerData[64];
	LagRecord PrevPlayerData[64];

	LagRecord RestoreData[128];
	LagRecord ChangedData[128];

	bool Changed = false;
	bool m_bNeedToRestore = false;
	bool CurrentlyDoingLagCompensation = false;

	int GetServerTick() // logic errors, doesnt matter too much tbh
	{
		INetChannelInfo* nci = Interfaces::Engine->GetNetChannelInfo();
		if (!nci) return Interfaces::Globals->tickcount;

		int LagTime = TIME_TO_TICKS(nci->GetLatency(FLOW_INCOMING) + nci->GetLatency(FLOW_OUTGOING));
		// +1 cause server is 1 tick ahead, potentially more if our FPS fucking blows
		return Interfaces::Globals->tickcount + LagTime + max(Interfaces::Globals->frametime / Interfaces::Globals->interval_per_tick, 1);
	}
	float LerpTime() // toast is a fucking retard
	{
		auto cl_updaterate = Interfaces::CVar->FindVar("cl_updaterate");
		auto sv_minupdaterate = Interfaces::CVar->FindVar("sv_minupdaterate");
		auto sv_maxupdaterate = Interfaces::CVar->FindVar("sv_maxupdaterate");

		auto cl_interp = Interfaces::CVar->FindVar("cl_interp");
		auto sv_client_min_interp_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
		auto sv_client_max_interp_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");

		auto updateRate = cl_updaterate->GetInt();
		auto interpRatio = cl_interp->GetFloat();
		auto minInterpRatio = sv_client_min_interp_ratio->GetFloat();
		auto maxInterpRatio = sv_client_max_interp_ratio->GetFloat();
		auto minUpdateRate = sv_minupdaterate->GetInt();
		auto maxUpdateRate = sv_maxupdaterate->GetInt();

		auto clampedUpdateRate = clamp(updateRate, minUpdateRate, maxUpdateRate);
		auto clampedInterpRatio = clamp(interpRatio, minInterpRatio, maxInterpRatio);

		auto lerp = clampedInterpRatio / clampedUpdateRate;

		if (lerp <= cl_interp->GetFloat())
			lerp = cl_interp->GetFloat();

		return lerp;
	}

	template <class T>
	FORCEINLINE T Lerp(float flPercent, T const &A, T const &B)
	{
		return A + (B - A) * flPercent;
	}
public:
	matrix3x4 BoneMatrix[128];
	void LogPlayer(IClientEntity* Entity, CUserCmd* cmd)
	{
		if (!Entity || !cmd) // nullptr checks
			return;
		if (Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum())
			return;
		// correct is the amout of time we have to correct game time
		CurrentlyDoingLagCompensation = true;
		m_bNeedToRestore = false;
		float correct = 0.0f;

		INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
		auto sv_maxunlag = Interfaces::CVar->FindVar("sv_maxunlag");

		if (nci)
		{
			// add network latency
			correct += nci->GetLatency(FLOW_OUTGOING);
		}

		// calc number of view interpolation ticks - 1
		int lerpTicks = TIME_TO_TICKS(LerpTime());

		// add view interpolation latency see C_BaseEntity::GetInterpolationAmount()
		correct += TICKS_TO_TIME(lerpTicks);

		// check bouns [0,sv_maxunlag]
		correct = clamp(correct, 0.0f, sv_maxunlag->GetFloat());

		// correct tick send by player 
		int targettick = cmd->tick_count - lerpTicks;

		// calc difference between tick send by player and our latency based tick
		float deltaTime = correct - TICKS_TO_TIME(GetServerTick() - targettick);


		if (fabs(deltaTime) > 0.2f)
		{
			// difference between cmd time and latency is too big > 200ms, use time correction based on latency
			Utilities::Log("StartLagCompensation: delta too big (%.3f)\n", deltaTime);
			targettick = Interfaces::Globals->tickcount - TIME_TO_TICKS(correct);
			return;
		}

		BacktrackPlayer(Entity, cmd, TICKS_TO_TIME(targettick));

	}
	void BacktrackPlayer(IClientEntity* Entity, CUserCmd* cmd, float flTargetTime)
	{
		if (!Entity || !cmd)
			return;

		if (Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum())
			return;

		Vector org;
		Vector minsPreScaled;
		Vector maxsPreScaled;
		QAngle ang;

		printf("1");

		Vector prevOrg = Entity->GetAbsAngles2();

		Vector delta = CurPlayerData[Entity->GetIndex()].m_vecOrigin - prevOrg;
		if (delta.Length() > 64.0f)
		{
			// lost track, too much difference
			return;
		}

		prevOrg = CurPlayerData[Entity->GetIndex()].m_vecOrigin;
		float frac = 0.0f;
		if (CurPlayerData &&
			(CurPlayerData[Entity->GetIndex()].m_flSimulationTime < flTargetTime) &&
			(CurPlayerData[Entity->GetIndex()].m_flSimulationTime < PrevPlayerData[Entity->GetIndex()].m_flSimulationTime))
		{
			// we didn't find the exact time but have a valid previous record
			// so interpolate between these two records;

			Assert(PrevPlayerData[Entity->GetIndex()].m_flSimulationTime > CurPlayerData[Entity->GetIndex()].m_flSimulationTime);
			Assert(flTargetTime < PrevPlayerData[Entity->GetIndex()].m_flSimulationTime);

			// calc fraction between both records
			frac = (flTargetTime - CurPlayerData[Entity->GetIndex()].m_flSimulationTime) /
				(PrevPlayerData[Entity->GetIndex()].m_flSimulationTime - CurPlayerData[Entity->GetIndex()].m_flSimulationTime);

			Assert(frac > 0 && frac < 1); // should never extrapolate

			ang = Lerp(frac, CurPlayerData[Entity->GetIndex()].m_vecAngles, PrevPlayerData[Entity->GetIndex()].m_vecAngles);
			org = Lerp(frac, CurPlayerData[Entity->GetIndex()].m_vecOrigin, PrevPlayerData[Entity->GetIndex()].m_vecOrigin);
			minsPreScaled = Lerp(frac, CurPlayerData[Entity->GetIndex()].m_vecMinsPreScaled, PrevPlayerData[Entity->GetIndex()].m_vecMinsPreScaled);
			maxsPreScaled = Lerp(frac, CurPlayerData[Entity->GetIndex()].m_vecMaxsPreScaled, PrevPlayerData[Entity->GetIndex()].m_vecMaxsPreScaled);
		}
		else
		{
			// we found the exact record or no other record to interpolate with
			// just copy these values since they are the best we have
			org = CurPlayerData[Entity->GetIndex()].m_vecOrigin;
			ang = CurPlayerData[Entity->GetIndex()].m_vecAngles;
			minsPreScaled = CurPlayerData[Entity->GetIndex()].m_vecMinsPreScaled;
			maxsPreScaled = CurPlayerData[Entity->GetIndex()].m_vecMaxsPreScaled;
		}
		LagRecord* restore = &RestoreData[Entity->GetIndex()];
		LagRecord* change = &ChangedData[Entity->GetIndex()];

		QAngle angdiff = Entity->GetAbsAngles2() - ang;
		Vector orgdiff = Entity->GetAbsAngles2() - org;

		restore->m_flSimulationTime = Entity->GetSimulationTime();

		int flags = 0;


		flags |= LC_ANGLES_CHANGED;
		restore->m_vecAngles = Entity->GetAbsAngles2();
		change->m_vecAngles = ang;

		flags |= LC_ORIGIN_CHANGED;
		restore->m_vecOrigin = Entity->GetAbsAngles2();
		change->m_vecOrigin = org;

		printf("3");


		m_bNeedToRestore = true;  // we changed at least one player
		restore->m_fFlags = flags; // we need to restore these flags
		change->m_fFlags = flags; // we have changed these flags
		Entity->SetupBones(BoneMatrix, 128, 0x00000100, GetTickCount64());
	}

	bool CLagCompensation::ProcessCmd(CUserCmd* pCmd)
	{
		int iTargetIndex;
		if (iTargetIndex == -1 || iTargetIndex > Interfaces::Engine->GetMaxClients())
			return false;

		if (!CurPlayerData[iTargetIndex].m_bValidRecord)
			return false;

		// forgot lerptime storage for oldest record but fuck it w.e
		pCmd->tick_count = TIME_TO_TICKS(CurPlayerData[iTargetIndex].m_flSimulationTime);
		return true;
	};

	void CLagCompensation::log(ClientFrameStage_t stage)
	{
		if (!Interfaces::Engine->IsInGame())
			return;

		if (!Interfaces::Engine->GetLocalPlayer() || Interfaces::Engine->GetLocalPlayer() == NULL)
			return;

		if (g_menu::window.RageBotTab.NADAAAA.GetState())
		{
			if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_END)
			{
				for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
				{
					IClientEntity *player = reinterpret_cast<IClientEntity*>(player = Interfaces::EntList->GetClientEntity(i));

					if (!player || player == nullptr)
						continue;

					if (Interfaces::Engine->GetLocalPlayer())
						continue;

					if (!player->IsPlayer())
						continue;

					if (player->IsImmune())
						continue;

					if (player->IsAlive() <= 0)
						continue;

					this->LogPlayer(player, Globals::UserCmd);
				}
			}
		}
	}
};