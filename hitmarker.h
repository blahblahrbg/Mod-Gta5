#pragma once


#include <Windows.h>
#pragma comment(lib, "Winmm.lib")

#include "MiscClasses.h"
#include "Interfaces.h"
#include "Menu.h"

#define g_pGameEventManager Interfaces::EventManager

class hitmarker
{
	class player_hurt_listener
		: public IGameEventListener2
	{
	public:
		void start()
		{
			if (!g_pGameEventManager->AddListener(this, "player_hurt", false)) {
				throw std::exception("Failed to register the event");
			}
		}
		void stop()
		{
			g_pGameEventManager->RemoveListener(this);
		}
		void FireGameEvent(IGameEvent *event) override
		{
			hitmarker::singleton()->on_fire_event(event);
		}
		int GetEventDebugID(void) override
		{
			return 0x2A;
		}
	};
public:
	static hitmarker* singleton()
	{
		static hitmarker* instance = new hitmarker;
		return instance;
	}

	void initialize()
	{
		_listener.start();
	}
	void on_fire_event(IGameEvent* event)
	{
		if (!strcmp(event->GetName(), "player_hurt")) {
			int attacker = event->GetInt("attacker");
			if (Interfaces::Engine->GetPlayerForUserID(attacker) == Interfaces::Engine->GetLocalPlayer() && g_menu::window.SettingsTab.OtherHitmarker.GetState())
			{
				_flHurtTime = Interfaces::Globals->curtime;
			}
			if (Interfaces::Engine->GetPlayerForUserID(attacker) == Interfaces::Engine->GetLocalPlayer() && g_menu::window.SettingsTab.OtherHitmarker.GetState())
			{
				PlaySoundA(_soundFileName, NULL, SND_ASYNC);
			}
		}
	}

	void on_paint()
	{
		auto curtime = Interfaces::Globals->curtime;
		auto lineSize = 8;
		if (_flHurtTime + 0.25f >= curtime)
		{
			int screenSizeX, screenCenterX;
			int screenSizeY, screenCenterY;
			Interfaces::Engine->GetScreenSize(screenSizeX, screenSizeY);

			screenCenterX = screenSizeX / 2;
			screenCenterY = screenSizeY / 2;

#define g_pSurface Interfaces::Surface
			g_pSurface->DrawSetColor(200, 200, 200, 255);
			g_pSurface->DrawLine(screenCenterX - lineSize, screenCenterY - lineSize, screenCenterX - (lineSize / 4), screenCenterY - (lineSize / 4));
			g_pSurface->DrawLine(screenCenterX - lineSize, screenCenterY + lineSize, screenCenterX - (lineSize / 4), screenCenterY + (lineSize / 4));
			g_pSurface->DrawLine(screenCenterX + lineSize, screenCenterY + lineSize, screenCenterX + (lineSize / 4), screenCenterY + (lineSize / 4));
			g_pSurface->DrawLine(screenCenterX + lineSize, screenCenterY - lineSize, screenCenterX + (lineSize / 4), screenCenterY - (lineSize / 4));
		}
	}

private:
	player_hurt_listener    _listener;
	const char*             _soundFileName = "csgo\\sound\\hitsound.wav";
	float                   _flHurtTime;
};

