#pragma once

#include "Hacks.h"
extern float lineLBY;
extern float lineRealAngle;
extern float lineFakeAngle;


namespace espstuff
{
	extern int								FakeDetection[64];
}
class CEsp : public CHack
{
public:
	void Init();
	void Draw();
	void offscreen(IClientEntity * entity, C_BaseCombatWeapon * CBaeEntity);
	void offscreen(IClientEntity * pEntity);
	void Move(CUserCmd *pCmd, bool &bSendPacket);
private:

	IClientEntity *BombCarrier;

	struct ESPBox
	{
		int x, y, w, h;
	};


	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);
	void DrawBox69(CEsp::ESPBox size, Color color);
	void DrawBox33(IClientEntity * entity, Color color);
	void DrawHealth33(IClientEntity * entity, CEsp::ESPBox size, Color dormant, Color color);
	void DrawHealth(IClientEntity * entity, CEsp::ESPBox size, Color dormant, Color color);
	void DrawHealth2(IClientEntity * pEntity, CEsp::ESPBox size);
	void DoEnemyCircle(IClientEntity * pLocalPlayer, const Vector & vecDelta, float * flRotation);
	void ThreeDBox(Vector minin, Vector maxin, Vector pos, Color Col);
	Color GetPlayerColor(IClientEntity* pEntity);
	void DrawLinesAA(Color color);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);
	void Fakedetect(IClientEntity * pEntity);
	void Fakedetect(CEsp::ESPBox size, Color color, IClientEntity * pEntity);
	void Corners(CEsp::ESPBox size, Color color, IClientEntity * pEntity);
	void cl_righthand(IClientEntity * pEntity);
	void DrawBox(ESPBox size, Color color);
	void Barrel(CEsp::ESPBox size, Color color, IClientEntity * pEntity);
	void DrawWeapon(IClientEntity * pEntity, CEsp::ESPBox size);
	void Ammo(IClientEntity * pEntity, player_info_t pinfo, CEsp::ESPBox size);
	void DrawGlow();
	void EntityGlow();
	void BacktrackingCross(IClientEntity * pEntity);
	void BombTimer(IClientEntity * pEntity, ClientClass * cClass);
	CEsp::ESPBox GetBOXX(IClientEntity * pEntity);
	void IwebzHealth(IClientEntity * pEntity, CEsp::ESPBox size);
	void DrawVitals(IClientEntity * pEntity, CEsp::ESPBox size);
	void DrawHealth(IClientEntity * pEntity, CEsp::ESPBox size);
	void DrawHealth(IClientEntity * pEntity, CEsp::ESPBox size, int x, int y, int height);
	void DrawHealth2(IClientEntity * pEntity, CEsp::ESPBox size, int x, int y, int height);
	void DrawName(player_info_t pinfo, ESPBox size);
	void DrawInfo(IClientEntity* pEntity, ESPBox size);
	void backtrackchams(IClientEntity * pEntity);
	void DrawCross(IClientEntity* pEntity);
	void DrawSkeleton(IClientEntity* pEntity);
	void DrawMoney(IClientEntity * pEntity, CEsp::ESPBox size);
	void Armor(IClientEntity * pEntity, CEsp::ESPBox size);
	void BoxAndText(IClientEntity * entity, std::string text);
	void DrawThrowable(IClientEntity * throwable);
	void DrawDrop(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
};

