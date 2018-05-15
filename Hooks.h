#pragma once

extern bool DoUnload;

#include "Utilities.h"
#include "Interfaces.h"
extern float lastmovelby[64];
extern bool antiAimSide;

namespace resolvokek
{
	extern int Shots;
	extern int missedshots;
	extern float RealAngle;
	extern float FakeAngle;
	extern bool shouldflip;
	extern bool ySwitch;
	extern float NextTime;
	extern int resolvemode;
	extern float fakeAngle;
	extern float OldSimulationTime[65];
	extern bool error;
}
Vector CalcAngle69(Vector dst, Vector src);/**/
namespace Hooks
{
	void Initialise();
	void DrawBeamd(Vector src, Vector end, Color color);
	void UndoHooks();


	extern Utilities::Memory::VMTManager VMTPanel; 
	extern Utilities::Memory::VMTManager VMTClient; 
	extern Utilities::Memory::VMTManager VMTClientMode;
	extern Utilities::Memory::VMTManager VMTModelRender;
	extern Utilities::Memory::VMTManager VMTPrediction; 
	extern Utilities::Memory::VMTManager VMTRenderView;
	extern Utilities::Memory::VMTManager VMTEventManager;
	extern Utilities::Memory::VMTManager VMTCreateMove;
};

namespace bigboi
{
	extern int indicator;
}

namespace Resolver
{
	extern bool didhitHS;
}
extern bool warmup;
extern int hittedLogHits[65];
extern int missedLogHits[65];
extern int shotsfired[65];
extern bool flipAA;