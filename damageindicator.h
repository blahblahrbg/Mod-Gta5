#pragma once
#include "Entities.h"
#include "Interfaces.h"

struct DamageIndicator_t {
	int iDamage;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	IClientEntity * Player;
	Vector Position;
};

class DamageIndicators {
public:
	std::vector<DamageIndicator_t> data;
	void paint();
};

extern DamageIndicators damage_indicators;