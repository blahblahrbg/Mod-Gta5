#pragma once

#include "Hacks.h"
extern float autowalldmgtest[65];

extern bool antiAimSide;
class CRageBot : public CHack
{
public:
	void Init();

	void Draw();
	bool EntityIsValid(int i);
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:

	int GetTargetCrosshair();
	int GetTargetDistance();
	int GetTargetNextShot();
	int GetTargetThreat(CUserCmd * pCmd);
	int GetTargetHealth();
	bool TargetMeetsRequirements(IClientEntity* pEntity);
	void aimAtPlayer(CUserCmd * pCmd);
	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
	int HitScan(IClientEntity* pEntity);
	void PositionAdjustment(CUserCmd * pCmd);
	bool AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket);

	void AutoZeus(CUserCmd * pCmd, IClientEntity * pEntity, bool & bSendPacket);

	void DoAimbot(CUserCmd *pCmd, bool &bSendPacket);
	void Prediction(CUserCmd * pCmd, IClientEntity * LocalPlayer);
	void DoNoRecoil(CUserCmd *pCmd);
	bool freestanding(CUserCmd * m_pcmd, bool packet);
	void DoAntiAim(CUserCmd *pCmd, bool&bSendPacket);

	void Fakelag(CUserCmd * pCmd, bool & bSendPacket);

	


	bool IsAimStepping;
	Vector LastAimstepAngle;
	Vector LastAngle;

	bool IsLocked;
	int TargetID;
	int HitBox;
	Vector AimPoint;
};

void b1gJoplinSwitch(CUserCmd *pCmd, bool &bSendPacket);