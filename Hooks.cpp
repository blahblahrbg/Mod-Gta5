/*credits by ibanned*/

#include "LagComp.h"
#include "backtracking.h"
#include "Backtracking Rage.h"
//#include "Resolver.h"
#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include "MiscHacks.h"
#include "CRC32.h"
#include "hitmarker.h"
#include <intrin.h>
#include "Entities.h"
#include "CBulletListener.h"
#include "damageindicator.h"
#define M_PI 3.14159265358979323846
extern float lineLBY;
extern float fakeangle;
extern float lineRealAngle;
extern float lineFakeAngle;
Vector LastAngleAAReal;
Vector LBYThirdpersonAngle;



#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))

#ifdef NDEBUG
#define strenc( s ) std::string( cx_make_encrypted_string( s ) )
#define charenc( s ) strenc( s ).c_str()
#define wstrenc( s ) std::wstring( strenc( s ).begin(), strenc( s ).end() )
#define wcharenc( s ) wstrenc( s ).c_str()
#else
#define strenc( s ) ( s )
#define charenc( s ) ( s )
#define wstrenc( s ) ( s )
#define wcharenc( s ) ( s )
#endif

#ifdef NDEBUG
#define XorStr( s ) ( XorCompileTime::XorString< sizeof( s ) - 1, __COUNTER__ >( s, std::make_index_sequence< sizeof( s ) - 1>() ).decrypt() )
#else
#define XorStr( s ) ( s )
#endif


                          /*includes*/
/*--------------------------------------------------------------*/
int currentfov;
Vector LastAngleAA;
bool Resolver::didhitHS;
CUserCmd* Globals::UserCmd;
IClientEntity* Globals::Target;
int Globals::Shots;
bool Globals::change;
int Globals::TargetID;
std::map<int, QAngle>Globals::storedshit;
int Globals::missedshots;
int bigboi::indicator;
/*--------------------------------------------------------------*/


typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef bool(__thiscall *FireEventClientSideFn)(PVOID, IGameEvent*);
typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);
using OverrideViewFn = void(__fastcall*)(void*, void*, CViewSetup*);
typedef float(__stdcall *oGetViewModelFOV)();
typedef void(__thiscall *SceneEnd_t)(void *pCmd);

PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
OverrideViewFn oOverrideView;
FireEventClientSideFn oFireEventClientSide;
RenderViewFn oRenderView;
SceneEnd_t pSceneEnd;


std::vector<trace_info> trace_logs;
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event);
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw);
void __fastcall	hkSceneEnd(void *pEcx, void *pEdx);
float __stdcall GGetViewModelFOV();

namespace Hooks
{
	Utilities::Memory::VMTManager VMTPanel; 
	Utilities::Memory::VMTManager VMTClient;
	Utilities::Memory::VMTManager VMTClientMode;
	Utilities::Memory::VMTManager VMTModelRender; 
	Utilities::Memory::VMTManager VMTPrediction; 
	Utilities::Memory::VMTManager VMTRenderView;
	Utilities::Memory::VMTManager VMTEventManager;
};

void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();
}

void Hooks::Initialise()
{

	Interfaces::Engine->ExecuteClientCmd("clear");
	Interfaces::CVar->ConsoleColorPrintf(Color(252, 112, 231, 255), ("\n insert open menu \n"));
	VMTPanel.Initialise((DWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);

	VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
	VMTPrediction.HookMethod((DWORD)&Hooked_InPrediction, 14);

	VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)&Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);

	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);

	VMTRenderView.Initialise((DWORD*)Interfaces::RenderView);
	pSceneEnd = (SceneEnd_t)VMTRenderView.HookMethod((DWORD)&hkSceneEnd, 9);

	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	VMTEventManager.Initialise((DWORD*)Interfaces::EventManager);
	oFireEventClientSide = (FireEventClientSideFn)VMTEventManager.HookMethod((DWORD)&Hooked_FireEventClientSide, 9);

	oOverrideView = (OverrideViewFn)VMTClientMode.HookMethod((DWORD)&Hooked_OverrideView, 18);
	VMTClientMode.HookMethod((DWORD)&GGetViewModelFOV, 35);

}

void MovementCorrection(CUserCmd* pCmd)
{

}

float clip(float n, float lower, float upper)
{

	return (std::max)(lower, (std::min)(n, upper));
}

int LagCompBreak() {
	IClientEntity *pLocalPlayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	Vector velocity = pLocalPlayer->GetVelocity();
	velocity.z = 0;
	float speed = velocity.Length();
	if (speed > 0.f) {
		auto distance_per_tick = speed *
			Interfaces::Globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
	return 1;
}

BYTE bMoveData[0x200];
/*pPa$te*/
void Prediction(CUserCmd* pCmd, IClientEntity* LocalPlayer)
{

	if (Interfaces::MoveHelper && g_menu::window.RageBotTab.EnginePrediction.GetState() && LocalPlayer->IsAlive())
	{
		float curtime = Interfaces::Globals->curtime;
		float frametime = Interfaces::Globals->frametime;
		int iFlags = LocalPlayer->GetFlags();

		Interfaces::Globals->curtime = (float)LocalPlayer->GetTickBase() * Interfaces::Globals->interval_per_tick;
		Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;

		Interfaces::MoveHelper->SetHost(LocalPlayer);

		BYTE bMoveData[0x200];
		Interfaces::Prediction1->SetupMove(LocalPlayer, pCmd, nullptr, bMoveData);
		Interfaces::GameMovement->ProcessMovement(LocalPlayer, bMoveData);
		Interfaces::Prediction1->FinishMove(LocalPlayer, pCmd, bMoveData);

		Interfaces::MoveHelper->SetHost(0);

		Interfaces::Globals->curtime = curtime;
		Interfaces::Globals->frametime = frametime;
		*LocalPlayer->GetPointerFlags() = iFlags;
	}
}

int kek = 0;
int autism = 0;

void SetClanTag(const char* tag, const char* name)//190% paste
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}
void NoClantag()
{
	SetClanTag("", "");
}

void ClanTag()
{
	static int counter = 0;
	switch (g_menu::window.MiscTab.OtherClantag.GetIndex())
	{
	case 0:
		break;
	case 1:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 2;
		switch (value) {
		case 0:SetClanTag("fuckware", ""); break;
		case 1:SetClanTag("1tap", ""); break;
		}
		counter++;

		break;
	}
	case 2:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;
		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 7;
		switch (value) {
		case 0:SetClanTag("fuckware    ", ""); break;
		case 1:SetClanTag("yougame.biz ", ""); break;
		case 2:SetClanTag(" 0  101 10  ", ""); break;
		case 3:SetClanTag("101 0  0 1  ", ""); break;
		case 4:SetClanTag("0 01 10  1 ", ""); break;
		case 5:SetClanTag("11  1 0 00 ", ""); break;
		case 6:SetClanTag("1 0 11 1 0 ", ""); break;
		}
		counter++;

		break;
	}
	}
}

	bool flipAA;
bool __stdcall CreateMoveClient_Hooked(float frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number)
		return true;

	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal)
	{

		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		if (g_menu::window.MiscTab.OtherClantag.GetIndex() > 0)
			ClanTag();

		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

		IClientEntity* pEntity;
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
		{
			Hacks::MoveHacks(pCmd, bSendPacket);
			ResolverSetup::GetInst().CM(pEntity);
		}
		backtracking->legitBackTrack(pCmd, pLocal);
		backtracking->ragebacktrack(pCmd, pLocal);
		static bool abc = false;

		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);

		float forward = pCmd->forwardmove;
		float right = pCmd->sidemove;
		float up = pCmd->upmove;
		if (forward > 450) forward = 450;
		if (right > 450) right = 450;
		if (up > 450) up = 450;
		if (forward < -450) forward = -450;
		if (right < -450) right = -450;
		if (up < -450) up = -450;
		pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);

		if (g_menu::window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);

			if (pCmd->viewangles.z != 0.0f)
			{
				pCmd->viewangles.z = 0.00;
			}

			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
			{
				Utilities::Log("Having to re-normalise!");
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				Beep(750, 800);
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
				}
			}
		}

		if (pCmd->viewangles.x > 90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (pCmd->viewangles.x < -90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}
		// LBY
		LBYThirdpersonAngle = Vector(pLocal->GetEyeAnglesXY()->x, pLocal->GetLowerBodyYaw(), pLocal->GetEyeAnglesXY()->z);

		switch (g_menu::window.MiscTab.SeeTpangle.GetIndex())
		{
		case 0:
			if (bSendPacket)
				fakeangle = pCmd->viewangles.y;
			break;
		case 1:
			if (!bSendPacket)
				fakeangle = pCmd->viewangles.y;
			break;
		}

		switch (g_menu::window.MiscTab.SeeTpangle.GetIndex())
		{
		case 0:
			if (!bSendPacket)
				LastAngleAA = pCmd->viewangles;
			break;
		case 1:
			if (bSendPacket)
				LastAngleAA = pCmd->viewangles;
			break;
		}

		if (g_menu::window.AntiAimtab.fakelag25.GetState()) {

			if (GetAsyncKeyState(VK_SPACE))
			{
				int Amount = g_menu::window.MiscTab.FakeLagChoke.GetValue();
				static int KeyPressedLag = -1;
				KeyPressedLag++;

				if (KeyPressedLag <= Amount && KeyPressedLag > -1)
				{
					bSendPacket = false;
				}
				else
				{
					bSendPacket = true;
					KeyPressedLag = -1;
				}
			}
		}
		lineLBY = pLocal->GetLowerBodyYaw();
		if (bSendPacket == true) {
			lineFakeAngle = pCmd->viewangles.y;
		}
		else if (bSendPacket == false) {
			lineRealAngle = pCmd->viewangles.y;
		}

	}
	return false;
}

std::string GetTimeString()
{
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}

/*--------------------------------------------------------------*//*--------------------------------------------------------------*/
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	
	if (g_menu::window.VisualsTab.OtherNoScope.GetState() && strcmp("HudZoom", Interfaces::Panels->GetName(vguiPanel)) == 0)
		return;

	IClientEntity* pLocalPlayer = hackManager.pLocal();
	IClientEntity* pEnt;
	CUserCmd pCmd;
	bool done = false;
	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{

		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, XorStr("MatSystemTopPanel")))
		{

			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}

	}
	else if (FocusOverlayPanel == vguiPanel)
	{
		if (g_menu::window.VisualsTab.ManualaaIndicator.GetState())
		{
			int W, H, cW, cH;
			Interfaces::Engine->GetScreenSize(W, H);
			cW = W / 2;
			cH = H / 2;

			if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			{
				if (GetKeyState(g_menu::window.AntiAimtab.SWSwitchKey.GetKey()))
				{
					Render::Text(cW + 34, cH - 20, Color(0, 128, 255, 160), Render::Fonts::LBY2, L"▶");
					Render::Text(cW - 50, cH - 20, Color(255, 255, 255, 160), Render::Fonts::LBY2, L"◀");
				}
				else
				{
					Render::Text(cW + 34, cH - 20, Color(255, 255, 255, 160), Render::Fonts::LBY2, L"▶");
					Render::Text(cW - 50, cH - 20, Color(0, 128, 255, 160), Render::Fonts::LBY2, L"◀");
				}
			}
			else
			{
				if (GetKeyState(g_menu::window.AntiAimtab.SWSwitchKey.GetKey()))
				{
					Render::Text(cW + 34, cH - 20, Color(255, 255, 255, 160), Render::Fonts::LBY2, L"▶");
					Render::Text(cW - 50, cH - 20, Color(255, 255, 255, 160), Render::Fonts::LBY2, L"◀");
				}
				else
				{
					Render::Text(cW + 34, cH - 20, Color(255, 255, 255, 160), Render::Fonts::LBY2, L"▶");
					Render::Text(cW - 50, cH - 20, Color(255, 255, 255, 160), Render::Fonts::LBY2, L"◀");
				}
			}
		}
	
		if (g_menu::window.VisualsTab.NightMode.GetState()) {
			if (!done)
			{
				ConVar* staticdrop = Interfaces::CVar->FindVar("r_DrawSpecificStaticProp");
				SpoofedConvar* staticdrop_spoofed = new SpoofedConvar(staticdrop);
				staticdrop_spoofed->SetInt(0);

				{
					for (MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
					{
						IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

						if (!pMaterial)
							continue;

						if (!strcmp(pMaterial->GetTextureGroupName(), "World textures"))
						{
							pMaterial->ColorModulate(0.1f, 0.1f, 0.1f);
						}
						if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
						{
							pMaterial->ColorModulate(0.3f, 0.3f, 0.3f);
						}
					}
				}
			
			done = true;
		}
		else
		{
			if (done)
			{
				for (MaterialHandle_t i = Interfaces::MaterialSystem->FirstMaterial(); i != Interfaces::MaterialSystem->InvalidMaterial(); i = Interfaces::MaterialSystem->NextMaterial(i))
				{
					IMaterial *pMaterial = Interfaces::MaterialSystem->GetMaterial(i);

					if (!pMaterial)
						continue;

					if (!strcmp(pMaterial->GetTextureGroupName(), "World textures"))
					{
						pMaterial->ColorModulate(1.f, 1.f, 1.f);
					}
					if (!strcmp(pMaterial->GetTextureGroupName(), "StaticProp textures"))
					{
						pMaterial->ColorModulate(0.4f, 0.4f, 0.4f);
					}
				}
				done = false;
			}
		}
	}

		if (g_menu::window.VisualsTab.LBYIdicador.GetState())
		{
			CUserCmd* cmdlist = *(CUserCmd**)((DWORD)Interfaces::pInput + 0xEC);
			CUserCmd* pCmd = cmdlist;

			IClientEntity* localplayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			RECT TextSize = Render::GetTextSize(Render::Fonts::LBY, "LBY");
			RECT scrn = Render::GetViewport();
			if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
				if (pCmd->viewangles.y - *localplayer->GetLowerBodyYawTarget() >= -35 && pCmd->viewangles.y - *localplayer->GetLowerBodyYawTarget() <= 35)
					Render::Text(10, scrn.bottom - 80, Color(255, 0, 0, 255), Render::Fonts::LBY, "LBY");
				else
					Render::Text(10, scrn.bottom - 80, Color(0, 255, 0, 255), Render::Fonts::LBY, "LBY");
		}
	    {

		    /*watermark background 229, 150, 255, 255*/
		    Render::Clear(1170, 13, 175, 18, Color(252, 112, 231, 87));
			Render::Text(1173, 15, Color(255, 255, 255, 255), Render::Fonts::supremacy, ("fuckware.tk |"));
			Render::Textf(1242, 15, Color(255, 255, 255, 255), Render::Fonts::supremacy, ("%s | 14 may"), GetTimeString().c_str());
		}

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			Hacks::DrawHacks();

		g_menu::DoUIFrame();

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && g_menu::window.SettingsTab.OtherHitmarker.GetState())
			hitmarker::singleton()->on_paint();
		
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && g_menu::window.VisualsTab.DamageIndicator.GetState())
			damage_indicators.paint();
	}
	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);
}


void __fastcall  hkSceneEnd(void *pEcx, void *pEdx) {


	 if (g_menu::window.MiscTab.FakeAngleChams.GetState())
	{
		IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		if (pLocal)
		{
			static  IMaterial* CoveredLit = CreateMaterial(true);
			if (CoveredLit)
			{
				Vector OrigAng;
				OrigAng = pLocal->GetEyeAngles();
				pLocal->SetAngle2(Vector(0, lineFakeAngle, 0));

				bool LbyColor = false;
				float NormalColor[3] = { 1, 1, 1 };
				float lbyUpdateColor[3] = { 0, 1, 0 };
				Interfaces::RenderView->SetColorModulation(LbyColor ? lbyUpdateColor : NormalColor);
				Interfaces::ModelRender->ForcedMaterialOverride(CoveredLit);
				pLocal->draw_model(STUDIO_RENDER, 255);
				Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
				pLocal->SetAngle2(OrigAng);
			}
		}
	}
}
/*--------------------------------------------------------------*//*--------------------------------------------------------------*/



bool __stdcall Hooked_InPrediction()
{

	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.GetOriginalFunction(14);
	static DWORD *ecxVal = Interfaces::Prediction;
	result = origFunc(ecxVal);

	if (g_menu::window.VisualsTab.OtherNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{

		IClientEntity* pLocalEntity = NULL;

		float* m_LocalViewAngles = NULL;

		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}

		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();

		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}
	return result;
}


player_info_t GetInfo(int Index) {
	player_info_t Info;
	Interfaces::Engine->GetPlayerInfo(Index, &Info);
	return Info;
}

typedef void(__cdecl* MsgFn)(const char* msg, va_list);
void Msg(const char* msg, ...)
{

	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); //This gets the address of export "Msg" in the dll "tier0.dll". The static keyword means it's only called once and then isn't called again (but the variable is still there)
	char buffer[989];
	va_list list; //Normal varargs stuff http://stackoverflow.com/questions/10482960/varargs-to-printf-all-arguments
	va_start(list, msg);

	vsprintf(buffer, msg, list);
	va_end(list);

	fn(buffer, list); //Calls the function, we got the address above.
}

int Kills2 = 0;
int Kills = 0;
bool RoundInfo = false;
size_t Delay = 0;
bool warmup = false;
bool pEvent = false;

/*--------------------------------------------------------------*//*--------------------------------------------------------------*/
bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event)
{
	CBulletListener::singleton()->OnStudioRender();//

	if (g_menu::window.RageBotTab.AimbotEnable.GetState())
	{
		if (g_menu::window.SettingsTab.BuyBot.GetIndex() == 1)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy ssg08;");
		}
		else if (g_menu::window.SettingsTab.BuyBot.GetIndex() == 2)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy scar20; buy g3sg1;");
		}
		else if (g_menu::window.SettingsTab.BuyBot.GetIndex() == 3)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy negev;");
		}
	}

	if (g_menu::window.RageBotTab.AimbotEnable.GetState())
	{
		if (g_menu::window.SettingsTab.BuyBotGrenades.GetIndex() == 1)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy taser; buy vest; buy vesthelm; buy molotov; buy smokegrenade; buy hegrenade;");
		}
		else if (g_menu::window.SettingsTab.BuyBotGrenades.GetIndex() == 2)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("defuser; buy vest; buy vesthelm; buy taser; buy molotov; buy smokegrenade; buy hegrenade;");
		}
	}

	if (g_menu::window.RageBotTab.AimbotEnable.GetState())
	{
		if (g_menu::window.SettingsTab.BuyBotPistol.GetIndex() == 1)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy elite;");
		}
		else if (g_menu::window.SettingsTab.BuyBotPistol.GetIndex() == 2)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy revolver;");
		}
	}

	if (!Event)

		IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	std::string event_name = Event->GetName();
	if (event_name.find("round_prestart") != std::string::npos || event_name.find("round_end") != std::string::npos)
	{
		if (event_name.find("round_end") != std::string::npos)
		{

			warmup = false;
		}
		else
		{

			warmup = true;
		}

	}

	if (event_name.find("round_freeze_end") != std::string::npos)
	{
		warmup = false;
	}

	if (event_name.find("round_end") != std::string::npos)
	{
		warmup = true;
	}

	if (g_menu::window.SettingsTab.Logs.GetIndex())
	{
		if (!strcmp(Event->GetName(), "item_purchase"))
		{

			int nUserID = Event->GetInt("attacker");
			int nDead = Event->GetInt("userid");
			if (nUserID || nDead)
			{
				player_info_t killed_info = GetInfo(Interfaces::Engine->GetPlayerForUserID(nDead));
				player_info_t killer_info = GetInfo(Interfaces::Engine->GetPlayerForUserID(nUserID));
				std::string before = (" ");
				std::string one = killed_info.name;
				std::string two = ("bought ");
				std::string three = Event->GetString("weapon");
				std::string six = "\n";
				if (g_menu::window.SettingsTab.Logs.GetIndex())
				{

					Msg((before + one + two + three + six).c_str());

				}

			}
		}
		if (g_menu::window.SettingsTab.Logs.GetIndex())
		{
			if (!strcmp(Event->GetName(), "player_hurt"))
			{

				int attackerid = Event->GetInt("attacker");
				int entityid = Interfaces::Engine->GetPlayerForUserID(attackerid);


				if (entityid == Interfaces::Engine->GetLocalPlayer())
				{

					int nUserID = Event->GetInt("attacker");
					int nDead = Event->GetInt("userid");
					if (nUserID || nDead)
					{

						player_info_t killed_info = GetInfo(Interfaces::Engine->GetPlayerForUserID(nDead));
						player_info_t killer_info = GetInfo(Interfaces::Engine->GetPlayerForUserID(nUserID));
						std::string before = (" ");
						std::string two = ("Hit ");
						std::string three = killed_info.name;
						std::string foura = " for ";
						std::string fivea = Event->GetString("dmg_health");
						std::string damage = " damage";
						std::string fourb = " (";
						std::string fiveb = Event->GetString("health");
						std::string six = " health remaining)";
						std::string newline = "\n";

					}
				}

			}

		}
	}

	return oFireEventClientSide(ECX, Event);
}
/*--------------------------------------------------------------*//*--------------------------------------------------------------*/

void Hooks::DrawBeamd(Vector src, Vector end, Color color)
{
	BeamInfo_t beamInfo;
	beamInfo.m_nType = TE_BEAMPOINTS;
	beamInfo.m_pszModelName = "sprites/physbeam.vmt";
	beamInfo.m_nModelIndex = -1;
	beamInfo.m_flHaloScale = 0.0f;
	beamInfo.m_flLife = 3.0f;
	beamInfo.m_flWidth = 7.0f;
	beamInfo.m_flEndWidth = 7.0f;
	beamInfo.m_flFadeLength = 0.0f;
	beamInfo.m_flAmplitude = 2.0f;
	beamInfo.m_flBrightness = color.a();
	beamInfo.m_flSpeed = 0.2f;
	beamInfo.m_nStartFrame = 0.f;
	beamInfo.m_flFrameRate = 0.f;
	beamInfo.m_flRed = color.r();
	beamInfo.m_flGreen = color.g();
	beamInfo.m_flBlue = color.b();
	beamInfo.m_nSegments = 2;
	beamInfo.m_bRenderable = true;
	beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;
	beamInfo.m_vecStart = src;
	beamInfo.m_vecEnd = end;

	Beam_t* myBeam = Interfaces::g_pViewRenderBeams->CreateBeamPoints(beamInfo);

	if (myBeam)
		Interfaces::g_pViewRenderBeams->DrawBeam(myBeam);
}

void StartPrediction(IClientEntity* LocalPlayer, CUserCmd* pCmd)
{
	static bool done = false;
	if (LocalPlayer->IsAlive() && g_menu::window.RageBotTab.EnginePrediction.GetState() && !done)
	{
		//Interfaces::CVar->FindVar("rate")->SetValue(1048576);
		//Interface->Cvar()->GetVTable<ICvar>()->FindVar( "viewmodel_fov" )->SetValue( 80 );
		Interfaces::CVar->FindVar("cl_interp")->SetValue(0.01f);
		Interfaces::CVar->FindVar("cl_cmdrate")->SetValue(66);
		Interfaces::CVar->FindVar("cl_updaterate")->SetValue(66);
		Interfaces::CVar->FindVar("cl_interp_all")->SetValue(0.0f);
		Interfaces::CVar->FindVar("cl_interp_ratio")->SetValue(1.0f);
		Interfaces::CVar->FindVar("cl_smooth")->SetValue(0.0f);
		Interfaces::CVar->FindVar("cl_smoothtime")->SetValue(0.01f);
		done = true;
	}
}


#define TEXTURE_GROUP_OTHER							"Other textures"
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* OpenLit = CreateMaterial(false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	static IMaterial* Chrome = CreateMaterial("$envmap env_cube");
	bool DontDraw = false;


		const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);
		IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		int ChamsStyle = g_menu::window.VisualsTab.OptionsChams.GetIndex();
		int HandsStyle = g_menu::window.VisualsTab.OtherNoHands.GetIndex();

		if (strstr(ModelName, "models/player"))
		{
			if (pLocal && pModelEntity && ChamsStyle != 0)
			{
				IMaterial *material1 = Interfaces::MaterialSystem->FindMaterial("models/player/ct_fbi/ct_fbi_glass", TEXTURE_GROUP_OTHER);
				if (pLocal->IsScoped())
				{
					//color.SetColor(255, 255, 255, 255);
					//ForceMaterial(color, material1);
					Interfaces::RenderView->SetBlend(0.3);

				}
				if ((g_menu::window.VisualsTab.FiltersAll.GetState() || pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
				{
					IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
					IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;

					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 /*&& pModelEntity->GetTeamNum() != local->GetTeamNum()*/)
					{
						float alpha = 1.f;

						if (pModelEntity->HasGunGameImmunity())
							alpha = 0.5f;

						if (pModelEntity->GetTeamNum() != pLocal->GetTeamNum())
						{
							flColor[0] = 60.f / 255.f;
							flColor[1] = 120.f / 255.f;
							flColor[2] = 180.f / 255.f;
						}
						else
						{
							flColor[0] = 60.f / 255.f;
							flColor[1] = 120.f / 255.f;
							flColor[2] = 180.f / 255.f;
						}
						if (g_menu::window.VisualsTab.OptionsChams.GetIndex() == 1 || g_menu::window.VisualsTab.OptionsChams.GetIndex() == 2 && !pLocal->IsScoped())
						{
							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(covered);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}

						if (pModelEntity->GetTeamNum() == pLocal->GetTeamNum())
						{
							flColor[0] = 150.f / 255.f;
							flColor[1] = 200.f / 255.f;
							flColor[2] = 60.f / 255.f;
						}
						else
						{
							flColor[0] = 150.f / 255.f;
							flColor[1] = 200.f / 255.f;
							flColor[2] = 60.f / 255.f;
						}
						if (g_menu::window.VisualsTab.OptionsChams.GetIndex() == 3)
						{
							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
						}
						else {
							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(open);
						}
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
						ForceMaterial(color, open);
					}
				}
			}
		}
		else if (strstr(ModelName, "arms"))
		{
			/*
			models/player/ct_fbi/ct_fbi_glass - platinum
			models/inventory_items/cologne_prediction/cologne_prediction_glass - glass
			models/inventory_items/trophy_majors/crystal_clear - crystal
			models/inventory_items/trophy_majors/gold - gold
			models/gibs/glass/glass - dark chrome
			models/inventory_items/trophy_majors/gloss - plastic/glass
			vgui/achievements/glow - glow
			*/
			IMaterial *material = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
			IMaterial *material1 = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);

			if (HandsStyle != 0 && pLocal && pLocal->IsAlive())
			{
				if (HandsStyle == 1)
				{
					ForceMaterial(color, material1);
				}
				else if (HandsStyle == 2)
				{
					Interfaces::RenderView->SetBlend(0.3);
				}
				else if (HandsStyle == 3)
				{
					IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
					IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
					if (pLocal)
					{
						if (pLocal->IsAlive())
						{
							int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

							if (pLocal->GetTeamNum() == 2)
								color.SetColor(240, 240, 240, alpha);
							else
								color.SetColor(240, 240, 240, alpha);

							ForceMaterial(color, covered);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

							if (pLocal->GetTeamNum() == 2)
								color.SetColor(240, 240, 240, alpha);
							else
								color.SetColor(240, 240, 240, alpha);
						}
						else
						{
							color.SetColor(240, 240, 240, 255);
						}

						ForceMaterial(color, covered);
					}
				}
				else
				{
					static int counter = 0;
					static float colors[3] = { 1.f, 0.f, 0.f };

					if (colors[counter] >= 1.0f)
					{
						colors[counter] = 1.0f;
						counter += 1;
						if (counter > 2)
							counter = 0;
					}
					else
					{
						int prev = counter - 1;
						if (prev < 0) prev = 2;
						colors[prev] -= 0.05f;
						colors[counter] += 0.05f;
					}

					Interfaces::RenderView->SetColorModulation(colors);
					Interfaces::RenderView->SetBlend(0.3);
					Interfaces::ModelRender->ForcedMaterialOverride(CoveredLit);
				}
			}
		}
		else if (g_menu::window.VisualsTab.FiltersWeapons.GetState() && strstr(ModelName, "_dropped.mdl"))
		{
			if (ChamsStyle != 0)
			{
				IMaterial *material1 = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
		
			}
		}
	

	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}

int RandomInt(int min, int max)
{
	return rand() % max + min;

}

std::vector<const char*> smoke_materials = {
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",

};


float Bolbilize(float Yaw)
{
	if (Yaw > 180)
	{
		Yaw -= (round(Yaw / 360) * 360.f);
	}
	else if (Yaw < -180)
	{
		Yaw += (round(Yaw / 360) * -360.f);
	}
	return Yaw;
}

Vector CalcAngle69(Vector dst, Vector src)
{
	Vector angles;

	double delta[3] = { (src.x - dst.x), (src.y - dst.y), (src.z - dst.z) };
	double hyp = sqrt(delta[1] * delta[1] + delta[2] * delta[2]);
	angles.x = (float)(atan(delta[2] / hyp) * 180.0 / 3.14159265);
	angles.y = (float)(atanf(delta[1] / delta[0]) * 57.295779513082f);
	angles.z = 0.0f;

	if (delta[0] >= 1.9)
	{
		angles.y += 180.0f;
	}

	return angles;
}
//Vector LastAngleAA;

namespace resolvokek
{
	int Shots;
	int missedshots;
	float RealAngle;
	float FakeAngle;
	Vector AimPoint;
	bool shouldflip;
	bool ySwitch;
	float NextTime;
	int resolvemode = 1;
	float fakeAngle;
	float OldSimulationTime[65];
	bool error;
}

#define SETANGLE 180
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{

	DWORD eyeangles = NetVar.GetNetVar(0xBFEA4E7B);
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	IClientEntity* pEntity = nullptr;

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_RENDER_START)
	{


		if (curStage == FRAME_RENDER_START) {
			if (*(bool*)((DWORD)Interfaces::pInput + 0xA5))
				*(Vector*)((DWORD)pLocal + 0x31C8) = LastAngleAA;

			if ((g_menu::window.MiscTab.OtherThirdperson.GetState())) {
				static bool rekt = false;
				if (!rekt) {
					ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
					SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
					sv_cheats_spoofed->SetInt(1);
					rekt = true;
				}
			}

			static bool rekt1 = false;
			if (g_menu::window.MiscTab.OtherThirdperson.GetState()) {
				if (g_menu::window.MiscTab.ThirdPersonKeyBind.GetKey()) {
					*(bool*)((DWORD)Interfaces::pInput + 0xA5) = false;
					*(float*)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = 0;

					if (GetKeyState(g_menu::window.MiscTab.ThirdPersonKeyBind.GetKey()) && pLocal->IsAlive()) {
						*(bool*)((DWORD)Interfaces::pInput + 0xA5) = true;
						*(float*)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = g_menu::window.MiscTab.OtherThirdpersonRange.GetValue();
					}
				}
			}

			if (!g_menu::window.MiscTab.OtherThirdperson.GetState()) {
				*(bool*)((DWORD)Interfaces::pInput + 0xA5) = false;
				*(float*)((DWORD)Interfaces::pInput + 0xA8 + 0x8) = 0;
			}

			static auto linegoesthrusmoke = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0", "xxxxxxxx????xxx");
			static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x8);

			if (g_menu::window.VisualsTab.OtherNoSmoke.GetState())
			{
				std::vector<const char*> vistasmoke_mats =
				{
					"particle/vistasmokev1/vistasmokev1_fire",
					"particle/vistasmokev1/vistasmokev1_smokegrenade",
					"particle/vistasmokev1/vistasmokev1_emods",
					"particle/vistasmokev1/vistasmokev1_emods_impactdust",
				};

				for (auto matName : vistasmoke_mats) {
					IMaterial* mat = Interfaces::MaterialSystem->FindMaterial(matName, "Other textures");
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
				}


				if (g_menu::window.VisualsTab.OtherNoSmoke.GetState()) {
					*(int*)(smokecout) = 0;
				}
			}
		}

		if (curStage == FRAME_RENDER_START)
		{
			for (int i = 1; i <= Interfaces::Globals->maxClients; i++)
			{
				if (i == Interfaces::Engine->GetLocalPlayer()) continue;

				IClientEntity* pCurEntity = Interfaces::EntList->GetClientEntity(i);
				if (!pCurEntity) continue;

				*(int*)((uintptr_t)pCurEntity + 0xA30) = Interfaces::Globals->framecount;
				*(int*)((uintptr_t)pCurEntity + 0xA28) = 0;
			}
			for (auto matName : smoke_materials)
			{
				IMaterial* mat = Interfaces::MaterialSystem->FindMaterial(matName, "Other textures");
			
			}
		}

	}

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		{
			ResolverSetup::GetInst().FSN(pEntity, curStage);
			//backtracking->Update(GetTickCount64());

		}
	}

	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {
		//Utilities::Log("APPLY SKIN APPLY SKIN");
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
		int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
		int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
		int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
		int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
		int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
		int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
		int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
		int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

		int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

		for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (pEntity)
			{
				ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);

				IClientEntity* pOwner = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);

				if (pOwner)
				{
					if (pOwner == pLocal)
					{
						std::string sWeapon = Interfaces::ModelInfo->GetModelName(pEntity->GetModel());
						auto weps = pLocal->Weapons();
						for (size_t i = 0; weps[i] != nullptr; i++)
						{
							auto pWeapons = reinterpret_cast<C_BaseCombatWeapon*>(Interfaces::EntList->GetClientEntityFromHandle(weps[i]));
						}

						//if (sWeapon.find("models/weapons", 0) != std::string::npos)
						//	continue;

						if (sWeapon.find("c4_planted", 0) != std::string::npos)
							continue;

						if (sWeapon.find("thrown", 0) != std::string::npos)
							continue;

						if (sWeapon.find("smokegrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("flashbang", 0) != std::string::npos)
							continue;

						if (sWeapon.find("fraggrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("molotov", 0) != std::string::npos)
							continue;

						if (sWeapon.find("decoy", 0) != std::string::npos)
							continue;

						if (sWeapon.find("incendiarygrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("ied", 0) != std::string::npos)
							continue;

						if (sWeapon.find("w_eq_", 0) != std::string::npos)
							continue;

						C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)pEntity;

						ClientClass *pClass = Interfaces::Client->GetAllClasses();


						if (g_menu::window.Skins.SkinEnable.GetState())
						{
							int Model = g_menu::window.Skins.KnifeModel.GetIndex();

							int M41S = g_menu::window.Skins.M41SSkin.GetIndex();
							int M4A4 = g_menu::window.Skins.M4A4Skin.GetIndex();
							int AK47 = g_menu::window.Skins.AK47Skin.GetIndex();
							int AWP = g_menu::window.Skins.AWPSkin.GetIndex();
							int GLOCK = g_menu::window.Skins.GLOCKSkin.GetIndex();
							int USPS = g_menu::window.Skins.USPSSkin.GetIndex();
							int DEAGLE = g_menu::window.Skins.DEAGLESkin.GetIndex();
							int FIVE7 = g_menu::window.Skins.FIVESEVENSkin.GetIndex();
							int AUG = g_menu::window.Skins.AUGSkin.GetIndex();
							int FAMAS = g_menu::window.Skins.FAMASSkin.GetIndex();
							int G3SG1 = g_menu::window.Skins.G3SG1Skin.GetIndex();
							int Galil = g_menu::window.Skins.GALILSkin.GetIndex();
							int M249 = g_menu::window.Skins.M249Skin.GetIndex();
							int MAC10 = g_menu::window.Skins.MAC10Skin.GetIndex();
							int P90 = g_menu::window.Skins.P90Skin.GetIndex();
							int UMP45 = g_menu::window.Skins.UMP45Skin.GetIndex();
							int XM1014 = g_menu::window.Skins.XM1014Skin.GetIndex();
							int BIZON = g_menu::window.Skins.BIZONSkin.GetIndex();
							int MAG7 = g_menu::window.Skins.MAG7Skin.GetIndex();
							int NEGEV = g_menu::window.Skins.NEGEVSkin.GetIndex();
							int SAWEDOFF = g_menu::window.Skins.SAWEDOFFSkin.GetIndex();
							int TEC9 = g_menu::window.Skins.TECNINESkin.GetIndex();
							int P2000 = g_menu::window.Skins.P2000Skin.GetIndex();
							int MP7 = g_menu::window.Skins.MP7Skin.GetIndex();
							int MP9 = g_menu::window.Skins.MP9Skin.GetIndex();
							int NOVA = g_menu::window.Skins.NOVASkin.GetIndex();
							int P250 = g_menu::window.Skins.P250Skin.GetIndex();
							int SCAR20 = g_menu::window.Skins.SCAR20Skin.GetIndex();
							int SG553 = g_menu::window.Skins.SG553Skin.GetIndex();
							int SSG08 = g_menu::window.Skins.SSG08Skin.GetIndex();
							int Magnum = g_menu::window.Skins.r8skin.GetIndex();
							int CZ = g_menu::window.Skins.CZSkin.GetIndex();
							int dualies = CZ = g_menu::window.Skins.DUALSSkin.GetIndex();


							C_BaseCombatWeapon* pWeapon = (C_BaseCombatWeapon*)pEntity;
							if (GameUtils::IsG(pWeapon))
								return;

							int weapon = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();//crash with nade?

								switch (weapon)
								{
								case 2:
									switch (dualies)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 28;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 43;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 46;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 47;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 153;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 190;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 249;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 220;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 336;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 261;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 276;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 307;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 330;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 447;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 450;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 491;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 528;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 544;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 658;
										break;
									}
									break;
								case 7: // AK47 
								{
									switch (AK47)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 341;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 14;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 44;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 172;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 180;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 394;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 300;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 226;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 282;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 302;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 316;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 340;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 380;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 422;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 456;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 474;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 490;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 506;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 524;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 600;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 597;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 656;
										break;

									default:
										break;
									}
								}
								break;
								case 16: // M4A4
								{
									switch (M4A4)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 155;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 187;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 255;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 309;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 215;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 336;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 384;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 400;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 449;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 471;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 480;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 512;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 533;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 588;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 632;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 664;
										break;
									default:
										break;
									}
								}
								break;
								case 60: // M4A1
								{
									switch (M41S)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 60;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 430;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 77;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 254;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 189;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 301;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 217;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 257;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 321;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 326;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 360;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 383;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 440;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 445;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 497;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 548;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 587;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 631;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 644;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 663;
										break;
									default:
										break;
									}
								}
								break;
								case 9: // AWP
								{
									switch (AWP)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 174;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 344;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 84;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 30;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 51;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 72;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 181;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 259;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 395;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 212;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 227;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 251;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 279;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 424;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 446;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 451;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 475;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 525;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 640;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 662;
										break;

									default:
										break;
									}
								}
								break;
								case 61: // USP
								{
									switch (USPS)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 25;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 60;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 183;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 339;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 217;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 221;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 236;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 277;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 290;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 313;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 318;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 332;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 364;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 454;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 489;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 504;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 540;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 637;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 653;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 657;
										break;
									default:
										break;
									}
								}
								break;
								case 4: // Glock
								{
									switch (GLOCK)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 2;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 3;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 38;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 40;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 48;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 437;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 159;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 399;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 208;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 230;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 278;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 293;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 353;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 367;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 381;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 479;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 495;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 532;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 607;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 586;
										break;
									default:
										break;
									}
								}
								break;
								case 1: // Deagle
								{
									switch (DEAGLE)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 37;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 347;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 468;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 469;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 232;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 17;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 40;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 61;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 90;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 185;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 231;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 237;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 397;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 328;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 273;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 296;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 351;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 425;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 470;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 509;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 527;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 645;
										break;
									default:
										break;
									}
								}
								break;
								case 3: // Five Seven
								{
									switch (FIVE7)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 3;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 44;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 46;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 78;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 141;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 151;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 254;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 248;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 210;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 223;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 252;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 265;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 274;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 464;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 352;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 377;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 387;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 427;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 510;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 530;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 643;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 660;
										break;
									default:
										break;
									}
								}
								break;
								case 8: // AUG
								{
									switch (AUG)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 9;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 33;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 280;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 305;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 375;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 442;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 444;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 455;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 507;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 541;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 601;
										break;
									default:
										break;
									}
								}
								break;
								case 10: // Famas
								{
									switch (FAMAS)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 47;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 92;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 429;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 154;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 178;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 194;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 244;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 218;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 260;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 288;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 371;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 477;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 492;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 529;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 604;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 626;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 659;
										break;
									default:
										break;
									}
								}
								break;
								case 11: // G3SG1
								{
									switch (G3SG1)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 8;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 6;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 46;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 72;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 74;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 147;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 170;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 195;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 229;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 294;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 465;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 464;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 382;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 438;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 493;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 511;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 545;
										break;
									default:
										break;
									}
								}
								break;
								case 13: // Galil
								{
									switch (Galil)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 83;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 428;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 76;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 119;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 398;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 192;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 308;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 216;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 237;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 241;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 264;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 297;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 379;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 460;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 478;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 494;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 546;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 647;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 661;
										break;
									default:
										break;
									}
								}
								break;
								case 14: // M249
								{
									switch (M249)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 75;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 202;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 243;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 266;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 401;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 452;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 472;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 496;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 547;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 648;
										break;
									default:
										break;
									}
								}
								break;
								case 17: // Mac 10
								{
									switch (MAC10)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 101;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 3;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 32;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 17;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 38;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 433;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 98;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 157;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 188;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 337;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 246;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 284;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 310;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 333;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 343;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 372;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 402;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 498;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 534;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 651;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 665;
										break;
									default:
										break;
									}
								}
								break;
								case 19: // P90
								{
									switch (P90)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 342;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 20;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 100;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 67;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 111;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 124;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 156;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 234;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 169;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 175;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 182;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 244;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 228;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 283;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 311;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 335;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 359;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 486;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 516;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 636;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 669; //ayy lmao
										break;
									default:
										break;
									}
								}
								break;
								case 24: // UMP-45
								{
									switch (UMP45)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 37;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 15;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 17;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 436;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 70;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 93;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 169;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 175;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 193;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 392;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 281;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 333;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 362;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 441;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 488;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 556;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 652;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 672;
										break;
									default:
										break;
									}
								}
								break;
								case 25: // XM1014
								{
									switch (XM1014)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 166;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 238;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 42;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 96;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 95;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 135;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 151;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 169;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 205;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 240;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 251;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 393;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 320;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 314;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 348;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 370;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 407;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 505;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 521;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 557;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 654;
										break;
									default:
										break;
									}
								}
								break;
								case 26: // Bizon
								{
									switch (BIZON)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 13;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 164;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 25;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 70;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 148;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 149;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 159;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 171;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 203;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 224;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 236;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 267;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 306;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 323;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 349;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 376;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 457;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 459;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 508;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 526;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 542;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 641;
										break;
									default:
										break;
									}
								}
								break;
								case 27: // Mag 7
								{
									switch (MAG7)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 462;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 34;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 32;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 100;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 39;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 431;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 171;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 177;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 198;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 291;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 385;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 473;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 499;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 535;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 666;
										break;

									default:
										break;
									}
								}
								break;
								case 28: // Negev
								{
									switch (NEGEV)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 28;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 432;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 157;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 201;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 240;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 285;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 298;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 317;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 355;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 369;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 483;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 514;
										break;
									default:
										break;
									}
								}
								break;
								case 29: // Sawed Off
								{
									switch (SAWEDOFF)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 345;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 30;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 83;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 38;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 41;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 434;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 119;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 171;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 204;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 405;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 246;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 250;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 390;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 256;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 323;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 458;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 459;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 517;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 552;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 638;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 655;
										break;

									default:
										break;
									}
								}
								break;
								case 30: // Tec 9
								{
									switch (TEC9)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 101;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 2;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 463;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 17;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 36;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 439;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 159;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 179;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 248;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 206;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 216;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 242;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 272;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 289;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 303;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 374;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 459;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 520;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 539;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 555;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 614;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 671;
										break;
									default:
										break;
									}
								}
								break;
								case 32: // P2000
								{
									switch (P2000)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 104;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 32;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 21;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 25;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 36;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 485;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 38;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 71;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 95;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 184;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 211;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 338;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 246;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 275;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 327;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 346;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 357;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 389;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 442;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 443;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 515;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 550;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 591;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 667;
										break;
									default:
										break;
									}
								}
								break;
								case 33: // MP7
								{
									switch (MP7)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 2;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 102;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 5;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 28;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 11;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 15;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 22;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 36;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 141;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 235;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 245;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 209;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 213;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 250;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 354;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 365;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 423;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 442;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 481;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 500;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 536;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 649;
										break;
									default:
										break;
									}
								}
								break;
								case 34: // MP9
								{
									switch (MP9)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 482;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 33;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 100;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 39;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 61;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 148;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 141;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 199;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 329;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 262;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 366;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 368;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 386;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 403;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 448;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 549;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 609;
										break;
									default:
										break;
									}
								}
								break;
								case 35: // Nova
								{
									switch (NOVA)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 3;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 166;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 164;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 25;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 62;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 107;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 158;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 170;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 191;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 214;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 225;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 263;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 286;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 294;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 299;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 356;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 450;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 484;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 537;
										break;
									default:
										break;
									}
								}
								break;
								case 36: // P250
								{
									switch (P250)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 102;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 34;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 162;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 15;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 164;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 27;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 77;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 168;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 258;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 207;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 219;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 404;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 230;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 271;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 295;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 464;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 358;
										break;
									case 18:
										*pWeapon->FallbackPaintKit() = 373;
										break;
									case 19:
										*pWeapon->FallbackPaintKit() = 388;
										break;
									case 20:
										*pWeapon->FallbackPaintKit() = 426;
										break;
									case 21:
										*pWeapon->FallbackPaintKit() = 466;
										break;
									case 22:
										*pWeapon->FallbackPaintKit() = 467;
										break;
									case 23:
										*pWeapon->FallbackPaintKit() = 501;
										break;
									case 24:
										*pWeapon->FallbackPaintKit() = 551;
										break;
									case 25:
										*pWeapon->FallbackPaintKit() = 650;
										break;
									case 26:
										*pWeapon->FallbackPaintKit() = 668;
										break;

									default:
										break;
									}
								}
								break;
								case 38: // Scar 20
								{
									switch (SCAR20)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 165;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 100;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 46;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 70;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 116;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 157;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 196;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 232;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 391;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 298;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 312;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 406;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 453;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 502;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 518;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 597;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 642;
										break;
									default:
										break;
									}
								}
								break;
								case 39: // SG553
								{
									switch (SG553)
									{
									case 1:
										*pWeapon->FallbackPaintKit() = 39;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 98;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 410;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 347;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 287;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 298;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 363;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 378;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 487;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 519;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 553;
										break;
									default:
										break;
									}
								}
								break;
								case 40: // SSG08
								{
									switch (SSG08)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 26;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 60;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 96;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 99;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 157;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 200;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 222;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 233;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 253;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 304;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 319;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 361;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 503;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 538;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 554;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 624;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 670;
										break;

									default:
										break;
									}
								}
								break;
								case 64: // Revolver
								{
									switch (Magnum)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 12;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 522;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 595;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 523;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 199;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									default:
										break;
									}
								}
								break;
								case 63: // CZ-75
								{
									switch (CZ)
									{
									case 0:
										*pWeapon->FallbackPaintKit() = 270;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 1:
										*pWeapon->FallbackPaintKit() = 12;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 2:
										*pWeapon->FallbackPaintKit() = 476;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 3:
										*pWeapon->FallbackPaintKit() = 269;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 4:
										*pWeapon->FallbackPaintKit() = 643;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 5:
										*pWeapon->FallbackPaintKit() = 435;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 6:
										*pWeapon->FallbackPaintKit() = 350;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 7:
										*pWeapon->FallbackPaintKit() = 543;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 8:
										*pWeapon->FallbackPaintKit() = 268;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 9:
										*pWeapon->FallbackPaintKit() = 325;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 10:
										*pWeapon->FallbackPaintKit() = 602;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 11:
										*pWeapon->FallbackPaintKit() = 334;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 12:
										*pWeapon->FallbackPaintKit() = 622;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 13:
										*pWeapon->FallbackPaintKit() = 218;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 14:
										*pWeapon->FallbackPaintKit() = 297;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 15:
										*pWeapon->FallbackPaintKit() = 322;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 16:
										*pWeapon->FallbackPaintKit() = 453;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;
									case 17:
										*pWeapon->FallbackPaintKit() = 315;
										*pWeapon->FallbackWear() = .0000000000000001;
										break;

									default:
										break;
									}
								}
								break;
								default:
									break;
								}




								if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
								{
									auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);


									if (Model == 0) // Bayonet
									{
										*pWeapon->ModelIndex() = iBayonet; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iBayonet;
										*pWeapon->WorldModelIndex() = iBayonet + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500;
						

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 558; // Lore
										}
									}
									else if (Model == 1) // Bowie Knife
									{
										*pWeapon->ModelIndex() = iBowie; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iBowie;
										*pWeapon->WorldModelIndex() = iBowie + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 514;
								

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 558; // Lore
										}

									}
									else if (Model == 2) // Butterfly Knife
									{
										*pWeapon->ModelIndex() = iButterfly; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iButterfly;
										*pWeapon->WorldModelIndex() = iButterfly + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 515;
							

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 558; // Lore
										}

									}
									else if (Model == 3) // Falchion Knife
									{
										*pWeapon->ModelIndex() = iFalchion; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iFalchion;
										*pWeapon->WorldModelIndex() = iFalchion + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 512;
								

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 558; // Lore
										}
									}
									else if (Model == 4) // Flip Knife
									{
										*pWeapon->ModelIndex() = iFlip; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iFlip;
										*pWeapon->WorldModelIndex() = iFlip + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 505;
								

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 559; // Lore
										}

									}
									else if (Model == 5) // Gut Knife
									{
										*pWeapon->ModelIndex() = iGut; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iGut;
										*pWeapon->WorldModelIndex() = iGut + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 506;
							

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 560; // Lore
										}


									}
									else if (Model == 6) // Huntsman Knife
									{
										*pWeapon->ModelIndex() = iHuntsman; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iHuntsman;
										*pWeapon->WorldModelIndex() = iHuntsman + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 509;
							

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 559; // Lore
										}


									}
									else if (Model == 7) // Karambit
									{
										*pWeapon->ModelIndex() = iKarambit; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iKarambit;
										*pWeapon->WorldModelIndex() = iKarambit + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;
								

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 561; // Lore
										}

									}
									else if (Model == 8) // M9 Bayonet
									{
										*pWeapon->ModelIndex() = iM9Bayonet; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iM9Bayonet;
										*pWeapon->WorldModelIndex() = iM9Bayonet + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 508;
						

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 0; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 570; // Doppler Phase 4
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 568; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 562; // Lore
										}

									}


									else if (Model == 9) // Shadow Daggers
									{
										*pWeapon->ModelIndex() = iDagger; // m_nModelIndex
										*pWeapon->ViewModelIndex() = iDagger;
										*pWeapon->WorldModelIndex() = iDagger + 1;
										*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 516;
								

										int Skin = g_menu::window.Skins.KnifeSkin.GetIndex();

										if (Skin == 0)
										{
											*pWeapon->FallbackPaintKit() = 5; // Forest DDPAT
										}
										else if (Skin == 1)
										{
											*pWeapon->FallbackPaintKit() = 12; // Crimson Web
										}
										else if (Skin == 2)
										{
											*pWeapon->FallbackPaintKit() = 27; // Bone Mask
										}
										else if (Skin == 3)
										{
											*pWeapon->FallbackPaintKit() = 38; // Fade
										}
										else if (Skin == 4)
										{
											*pWeapon->FallbackPaintKit() = 40; // Night
										}
										else if (Skin == 5)
										{
											*pWeapon->FallbackPaintKit() = 42; // Blue Steel
										}
										else if (Skin == 6)
										{
											*pWeapon->FallbackPaintKit() = 43; // Stained
										}
										else if (Skin == 7)
										{
											*pWeapon->FallbackPaintKit() = 44; // Case Hardened
										}
										else if (Skin == 8)
										{
											*pWeapon->FallbackPaintKit() = 59; // Slaughter
										}
										else if (Skin == 9)
										{
											*pWeapon->FallbackPaintKit() = 72; // Safari Mesh
										}
										else if (Skin == 10)
										{
											*pWeapon->FallbackPaintKit() = 77; // Boreal Forest
										}
										else if (Skin == 11)
										{
											*pWeapon->FallbackPaintKit() = 98; // Ultraviolet
										}
										else if (Skin == 12)
										{
											*pWeapon->FallbackPaintKit() = 143; // Urban Masked
										}
										else if (Skin == 13)
										{
											*pWeapon->FallbackPaintKit() = 175; // Scorched
										}
										else if (Skin == 14)
										{
											*pWeapon->FallbackPaintKit() = 323; // Rust Coat
										}
										else if (Skin == 15)
										{
											*pWeapon->FallbackPaintKit() = 409; // Tiger Tooth
										}
										else if (Skin == 16)
										{
											*pWeapon->FallbackPaintKit() = 410; // Damascus Steel
										}
										else if (Skin == 17)
										{
											*pWeapon->FallbackPaintKit() = 411; // Damascus Steel
										}
										else if (Skin == 18)
										{
											*pWeapon->FallbackPaintKit() = 413; // Marble Fade
										}
										else if (Skin == 19)
										{
											*pWeapon->FallbackPaintKit() = 414; // Rust Coat
										}
										else if (Skin == 20)
										{
											*pWeapon->FallbackPaintKit() = 415; // Doppler Ruby
										}
										else if (Skin == 21)
										{
											*pWeapon->FallbackPaintKit() = 416; // Doppler Sapphire
										}
										else if (Skin == 22)
										{
											*pWeapon->FallbackPaintKit() = 417; // Doppler Blackpearl
										}
										else if (Skin == 23)
										{
											*pWeapon->FallbackPaintKit() = 418; // Doppler Phase 1
										}
										else if (Skin == 24)
										{
											*pWeapon->FallbackPaintKit() = 419; // Doppler Phase 2
										}
										else if (Skin == 25)
										{
											*pWeapon->FallbackPaintKit() = 420; // Doppler Phase 3
										}
										else if (Skin == 26)
										{
											*pWeapon->FallbackPaintKit() = 421; // Doppler Phase 4
										}
										else if (Skin == 27)
										{
											*pWeapon->FallbackPaintKit() = 569; // Gamma Doppler Phase1
										}
										else if (Skin == 28)
										{
											*pWeapon->FallbackPaintKit() = 570; // Gamma Doppler Phase2
										}
										else if (Skin == 29)
										{
											*pWeapon->FallbackPaintKit() = 571; // Gamma Doppler Phase3
										}
										else if (Skin == 30)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Phase4
										}
										else if (Skin == 31)
										{
											*pWeapon->FallbackPaintKit() = 568; // Gamma Doppler Emerald
										}
										else if (Skin == 32)
										{
											*pWeapon->FallbackPaintKit() = 561; // Lore
										}
									}
								}

								*pWeapon->OwnerXuidLow() = 0;
								*pWeapon->OwnerXuidHigh() = 0;
								*pWeapon->FallbackWear() = 0.001f;
								*pWeapon->m_AttributeManager()->m_Item()->ItemIDHigh() = 1;
							}
						}
				    }
			    }
			}
		}
	
		oFrameStageNotify(curStage);
}


void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
{

	IClientEntity* localplayer1 = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!localplayer1)
		return;

	if (!Interfaces::Engine->IsConnected())
		return;

	if (!Interfaces::Engine->IsInGame())
		return;

	if (localplayer1)
	{
		pSetup->fov += g_menu::window.VisualsTab.OtherFOV.GetValue();
	}
	currentfov = pSetup->fov;
	oOverrideView(ecx, edx, pSetup);
}

void GetViewModelFOV(float& fov)
{
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		if (!localplayer)
			return;


		if (g_menu::window.VisualsTab.Active.GetState())
			fov += g_menu::window.VisualsTab.Fov2.GetValue();
	}
}

float __stdcall GGetViewModelFOV()
{
	float fov = Hooks::VMTClientMode.GetMethod<oGetViewModelFOV>(35)();

	GetViewModelFOV(fov);

	return fov;
}

void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	static DWORD oRenderView = Hooks::VMTRenderView.GetOriginalFunction(6);

	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	__asm
	{
		PUSH whatToDraw
		PUSH nClearFlags
		PUSH hudViewSetup
		PUSH setup
		MOV ECX, ecx
		CALL oRenderView
	}
}