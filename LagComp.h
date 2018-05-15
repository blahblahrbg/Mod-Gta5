#include "Hooks.h"
#include "Hacks.h"
#include "ESP.h"
#include "Entities.h"
#include "SDK.h"

inline Vector AngleVector(Vector meme)
{
	auto sy = sin(meme.y / 180.f * static_cast<float>(PI));
	auto cy = cos(meme.y / 180.f * static_cast<float>(PI));

	auto sp = sin(meme.x / 180.f * static_cast<float>(PI));
	auto cp = cos(meme.x / 180.f* static_cast<float>(PI));

	return Vector(cp*cy, cp*sy, -sp);
}

inline float DistPointToLine(Vector Point, Vector LineOrigin, Vector Dir)
{
	auto PointDir = Point - LineOrigin;

	auto TempOffset = PointDir.Dot(Dir) / (Dir.x*Dir.x + Dir.y*Dir.y + Dir.z*Dir.z);
	if (TempOffset < 0.000001f)
		return FLT_MAX;

	auto PerpendicularPoint = LineOrigin + (Dir * TempOffset);

	return (Point - PerpendicularPoint).Length();
}

struct lbyRecords
{
	int tick_count;
	float lby;
	Vector headPosition;
};
struct backtrackData
{
	float simtime;
	Vector hitboxPos;
};

class BackTrack
{
	int latest_tick;
	bool IsTickValid(int tick);
	bool Lowerbody(int tick);
	void UpdateRecord(int i);
	bool RunLBYBackTrack(int i, CInput::CUserCmd * cmd, IClientEntity * pEntity);
public:
	lbyRecords records[64];
	bool RunLBYBackTrack(int i, CInput::CUserCmd* cmd, Vector& aimPoint);
	void Update(int tick_count);
	void legitBackTrack(CUserCmd* cmd, IClientEntity* pLocal);
	void ragebacktrack(CUserCmd * cmd, IClientEntity * pLocal);
};

extern backtrackData headPositions[64][12];

extern BackTrack* backtracking;

