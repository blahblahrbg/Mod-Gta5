#pragma once

#include "GUI.h"
#include "Controls.h"



class CAntiAimtab : public CTab
{
public:
	void Setup();

	CGroupBox Antiaim;
	CGroupBox AntiAim2;
	CComboBox FakeYaw;
	CSlider AddFakeYaw;
	CSlider CustomPitch;
	CCheckBox LBY;
	CSeperate Antilby2;
	CSeperate fakelag2;
	CSlider Antilby;
	CSlider fakelagvalue2;
	CSlider LowerBodyBreakerYawAdd;
	CComboBox InAir;
	CComboBox MoveYawFake;
	CCheckBox SWSwitchEnable;
	CCheckBox onlands;
	CCheckBox Freestanding;
	CKeyBind SWSwitchKey;
	CComboBox AntiAimPitch;
	CCheckBox AntiAimEnable;
	CComboBox AntiAimYaw;
	CKeyBind ManualLeft;
	CKeyBind ManualRight;
	CKeyBind ManualBack;
	CCheckBox FlipAA;
	CComboBox BreakLBY;

	CCheckBox fakelag25;
	CComboBox fakelag23;
	CComboBox fakelag24;
	CComboBox AntiAimEdge;
	CSlider AntiAimOffset;

	CSeperate separado2;
	CSeperate lbydelta;
	CCheckBox inair2;
	CSeperate fakeyaw2;
	CSeperate realyaw2;
	CSeperate pitch2;
};

class CRageBotTab : public CTab
{
public:
	void Setup();


	//custom
	CCheckBox Hitboxhead;
	CCheckBox Hitboxneck;
	CCheckBox Hitboxchest;
	CCheckBox Hitboxarms;
	CCheckBox Hitboxstomach;
	CCheckBox Hitboxlegs;






	CLabel ActiveLabel;
	CCheckBox Active;

	CGroupBox hitchance;
	CGroupBox mindmg;




	CGroupBox AimbotGroup;
	CGroupBox Hitbox;
	CGroupBox Hitbox2;
	CGroupBox Hitbox3;
	CGroupBox Hitbox4;
	CGroupBox Hitbox5;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CCheckBox AccuracyBacktracking;
	CCheckBox NADAAAA;
	CSeperate AimbotFov2;
	CSlider  AimbotFov;
	CCheckBox AimbotSilentAim;
	CCheckBox PVSFix;
	CCheckBox AimbotEnginePred;
	CKeyBind AimbotBaimOnKey;
	CKeyBind Overridex;
	CCheckBox Overridex2;
	CComboBox AimbotResolver;
	CCheckBox AccuracyResolverbrute;
	CCheckBox Resolverfakewalk;
	CCheckBox PitchAimbotResolver;
	CCheckBox AutoRevolver;
	CCheckBox naosei;
	CCheckBox  Nadaaa;
	CKeyBind enemyflip;
	CSlider forceback;
	CKeyBind  SomeShit;

	CCheckBox lbytoggleAA;

	CCheckBox LowerbodyFix;
	CCheckBox PreferBodyAim;
	CCheckBox AWPAtBody;
	CSeperate TickModulation2;
	CSeperate TargetPointscale2;
	CSeperate BaimIfUnderXHealth2;
	CSlider BaimIfUnderXHealth;
	CSlider XShots;
	CCheckBox EnginePrediction;
	CCheckBox AimbotAimStep;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CKeyBind  AimbotStopKey;

	CGroupBox TargetGroup;
	CSeperate TargetSelection2;
	CSeperate AimbotResolver2;
	CCheckBox stackhackresolver;
	CComboBox TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBox TargetHitbox;
	CComboBox TargetHitscan;
	CCheckBox TargetMultipoint;
	CSlider   TargetPointscale;

	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoWall;
	CSlider AccuracyMinimumDamage;
	CSlider Fakelagvalue;
	CCheckBox AccuracyAutoStop;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CCheckBox FakeLagEnable;
	CSlider   AccuracyHitchance;
	CCheckBox AccuracyResolver;
	CCheckBox AutoMinimumDamage;
	CCheckBox AutoHitChance;
	CSeperate hitchance2;
	CSeperate mindamage2;

	CCheckBox WeaponCheck;
	CSlider  scoutChance;
	CSlider  AWPChance;
	CSlider  AutoChance;
	CSlider  RifleChance;
	CSlider  MPChance;
	CSlider  PistolChance;
	CSlider  R8Chance;


	CSlider  scoutmindmg;
	CSlider  AWPmindmg;
	CSlider  Automindmg;
	CSlider  Riflemindmg;
	CSlider  MPmindmg;
	CSlider  Pistolmindmg;
	CSlider  R8mindmg;

	CGroupBox AntiAimGroup;
	CSlider   TickModulation;
	CCheckBox AntiAimKnife;
	CCheckBox AntiAimTarget;
};

class CLegitBot : public CTab
{
public:
	void Setup();
	CGroupBox AimbotGroup;

	CCheckBox AimbotEnable;
	CCheckBox AccuracyBacktracking;
	CSlider   TickModulation;
};

class CVisualTab : public CTab
{
public:
	void Setup();

	CLabel ActiveLabel;
	CCheckBox Active;
	CCheckBox pLocalOpacity;
	CGroupBox OptionsGroup;
	CGroupBox ESPOther;
	CGroupBox InfoGroup;
	CGroupBox RemovalsGroup;
	CGroupBox ChamsGroup;
	CGroupBox BoxGroup;
	CGroupBox EffectGroup;
	CComboBox OptionsBox;
	CCheckBox OptionsName;
	CCheckBox Normalcrosshair;

	CCheckBox SpreadCrosshair;

	CCheckBox dot;
	CCheckBox Angleinfo;
	CCheckBox DamageIndicator;
	CCheckBox OptionsWeapon;
	CCheckBox OptionHealthEnable;
	CCheckBox OptionsInfo;
	CCheckBox LBYIdicador;
	CCheckBox ManualaaIndicator;
	CCheckBox ResolverInfo;
	CCheckBox fakes;
	CCheckBox BulletTrace;
	CCheckBox Barrels;
	CSlider BulletTraceLength;
	CCheckBox AALines;
	CCheckBox ChamsVisibleOnly;
	CCheckBox Watermark;
	CCheckBox OtherGrenadeTrajectory;
	CCheckBox otherbulletimpact;
	CCheckBox WelcomeMessage;
	CCheckBox GrenadePrediction;
	CCheckBox LocalPlayerESP;
	CCheckBox CanHit;
	CCheckBox OptionsHelmet;
	CCheckBox NightMode;
	CSeperate f1;
	CSeperate f2;
	CCheckBox OptionsKit;
	CCheckBox SpecList;
	CCheckBox IsScoped;
	CCheckBox HasDefuser;
	CCheckBox GrenadeTrace;
	CCheckBox Distance;
	CCheckBox DiscoLights;
	CCheckBox Money;
	CCheckBox OptionsDefuse;
	CComboBox OptionsChams;
	CCheckBox OptionsArmor;
	CSlider  GlowZ;
	CCheckBox OptionsSkeleton;
	CSeperate cl_righthandse;
	CCheckBox offscreen;
	CCheckBox BacktrackingLol;
	CCheckBox OptionsAimSpot;
	CCheckBox OtherNoScope;
	CComboBox Chams;
	CCheckBox ChamsPlayers;
	CCheckBox ChamsEnemyOnly;



	CCheckBox FiltersAll;
	CCheckBox FiltersPlayers;
	CCheckBox FiltersSelf;
	CCheckBox FiltersEnemiesOnly;
	CCheckBox FiltersWeapons;
	CCheckBox FiltersNades;
	CCheckBox FiltersC4;

	CComboBox XHairs;
	CSeperate chams33;
	CSeperate boxx22;
	CSeperate handdd22;
	CSlider spreadslider;
	CCheckBox OptionsGlow;
	CCheckBox EntityGlow;
	CCheckBox NightSky;
	CCheckBox OtherRadar;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox OtherNoSky; 
	CCheckBox OtherNoFlash; 
	CCheckBox OtherNoSmoke;
	CCheckBox OtherAsusWalls;
	CComboBox OtherNoHands;
	CCheckBox OtherCrosshair;
	CCheckBox BulletTracers;
	CCheckBox DisablePostProcess;
	CSlider OtherFOV;
	CSlider Fov2;
};

class CColorTab : public CTab
{
public:
	void Setup();

	CGroupBox RGBGroup;
	CSlider   MenuR;
	CSlider   MenuG;
	CSlider  MenuB;

	CGroupBox Colors;
	CGroupBox Colors2;
	CComboBox ColorsPicker;
	CGroupBox CTBoxNotVisGroup;
	CGroupBox ColorsGroup;
	CComboBox ControlColor;
	CSlider CTColorVisR;
	CSlider CTColorVisG;
	CSlider CTColorVisB;

	CSlider TChams;
	CSlider CTChams;
	CSlider VisR;
	CSlider VisG;
	CSlider VisB;
	CSlider NotVisR;
	CSlider NotVisG;
	CSlider NotVisB;
	CSlider ColorMenuAccentR;
	CSlider ColorMenuAccentG;
	CSlider ColorMenuAccentB;

	CSlider CTColorNoVisR;
	CSlider CTColorNoVisG;
	CSlider CTColorNoVisB;
	CGroupBox TVisibleBoxGroup;
	CGroupBox Tboxnotvisiblegroup;
	CGroupBox GlowGroup;
	CLabel ColorSpacer;

	CGroupBox bullettracers;
	CSlider  BULLETCT_R;
	CSlider  BULLETCT_G;
	CSlider  BULLETCT_B;
	//CT Visible
	CSlider  BULLET_T_R;
	CSlider  BULLET_T_G;
	CSlider  BULLET_T_B;

	CSeperate a1;
	CSeperate a2;
	CSeperate a3;
	CSeperate a4;
	CSeperate a5;
	CSeperate a6;
	CSeperate a7;
	CSeperate a8;
	CSeperate a9;
	CSeperate a10;
	CSeperate a11;
	CSeperate a12;
	CSeperate a13;
	CSeperate a14;
	CSeperate a15;
	CSeperate a16;
	CSeperate a17;
	CSeperate a18;
	CSeperate a19;
	CSeperate a20;
	CSeperate a21;
	CSeperate a22;
	CSeperate a23;
	CSeperate a24;
	CSeperate a25;
	CSeperate a26;
	CSeperate a27;
	CSeperate a28;
	CSeperate a29;
	CSeperate a30;
	CSeperate a31;
	CSeperate a32;
	CSeperate a33;

	CSlider TColorVisR;
	CSlider TColorVisG;
	CSlider TColorVisB;

	CSlider TColorNoVisR;
	CSlider TColorNoVisG;
	CSlider TColorNoVisB;

	CSlider GlowR;
	CSlider GlowG;
	CSlider GlowB;

	CGroupBox CTCHamsGroup;
	CGroupBox TChamsGroup;
	CGroupBox MenuColorGroup;
	CComboBox ConfigBox;
	CGroupBox ConfigGroup;
	CGroupBox Settings;
	CButton SettingsSave;
	CButton SettingsLoad;
};

class CSettingsTab : public CTab
{
public:
	void Setup();

	CGroupBox grpPList;
	CCheckBox PlayerFriendly;
	CCheckBox PlayerAimPrio;
	CCheckBox PlayerCalloutSpam;
	CCheckBox nospreadresolver;
	CCheckBox CustomNospreadResolver;

	CSlider Bruteforce1;
	CSlider Bruteforce2;
	CSlider Bruteforce3;
	CSlider Bruteforce4;
	CSlider Bruteforce5;

	CSlider Bruteforce6;
	CSlider Bruteforce7;
	CSlider Bruteforce8;
	CSlider Bruteforce9;
	CSlider Bruteforce10;

	CGroupBox MiscGroup;
	CGroupBox misctabzin2;
	CSlider FakePingValue;
	CCheckBox FakePingExploit;
	CCheckBox m2;
	CCheckBox m3;
	CCheckBox m4;
	CCheckBox m5;
	CCheckBox m6;
	CCheckBox m7;
	CSeperate Logs2;
	CComboBox Logs;
	CCheckBox OtherHitmarker;
	CSeperate autobuy;
	CCheckBox OptionsCompRank;
	CCheckBox AutoAccept;

	CComboBox BuyBot;
	CGroupBox BuyBotGroup;
	CCheckBox BuyBotKevlar;
	CCheckBox BuyBotDefuser;
	CComboBox BuyBotGrenades;
	CComboBox BuyBotPistol;
	CCheckBox cl_righthand2;
	CKeyBind clright1;
	CKeyBind clright2;

};

class CMiscTab : public CTab
{
public:
	void Setup();

	CButtonSave SaveButton3;
	CButtonLoad LoadButton3;

	CGroupBox OtherGroup;
	CGroupBox OtherGroup2;

	CCheckBox OtherSafeMode;

	CSeperate clantag22;
	CSeperate configurations2;
	CSeperate Seperate2;

	CCheckBox ChatSpam;
	CCheckBox NoName;
	CCheckBox Namespam;
	CTextField CustomClantag;
	CCheckBox Impacts;
	CCheckBox CheatsByPass;
	CCheckBox MoonWalk;
	CSlider CircleAmount;
	CKeyBind FakeWalk;
	CComboBox FakeLagTyp;

	CComboBox lstPlayers;
	CSeperate configurationkey;
	CKeyBind configuration2;
	CButton load;
	CSeperate save1;
	CButton save;
	CSlider ClanTagSpeed;
	CComboBox NameChanger;
	CCheckBox AutoPistol;
	CCheckBox ClanTag;
	CCheckBox TrashTalk;
	CKeyBind  OtherAirStuck;
	CCheckBox OtherSpectators;
	CCheckBox OtherThirdperson;
	CKeyBind  ThirdPersonKeyBind;
	CSlider OtherThirdpersonRange;
	CCheckBox FakeAngleChams;
	CComboBox SeeTpangle;
	CCheckBox OtherThirdpersonFake;
	CComboBox OtherClantag;
	CComboBox ConfigBox;
	CTextField customclan;
	CCheckBox DisablePostProcess;
	CSlider WalkBotDistance;
	CSlider WalkBotEdge;
	CCheckBox WalkBot;
	CGroupBox WalkBotGroup;
	CGroupBox FakeLagGroup;
	CCheckBox FakeLagEnable;
	CSlider   FakeLagChoke;
	CComboBox KnifeModel;
	CCheckBox SkinEnable;
	CTextField KnifeSkin;
	CGroupBox KnifeGroup;
	CButton KnifeApply;
};
class SkinsTab : public CTab
{
public:
	void Setup();

	// Knife Changer/Skin Changer
	CLabel SkinActive;
	CCheckBox SkinEnable;

	// Knife
	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;

	// Pistols
	CGroupBox PistolGroup;
	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;
	CComboBox CZSkin;

	// MPs


	// Rifles
	CGroupBox Riflegroup;
	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;


	// Machineguns
	CGroupBox MachinegunsGroup;
	CComboBox NEGEVSkin;
	CComboBox M249Skin;

	CGroupBox MPGroup;
	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;

	// Snipers
	CGroupBox Snipergroup;
	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox r8skin;
	CComboBox AWPSkin;

	// Shotguns
	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

	// Skinsettings
	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;
	/*
	*/

};

class Movement : public CTab
{
public:
	void Setup();
	CGroupBox Movetab;

	CCheckBox OtherAutoJump;
	CCheckBox OtherEdgeJump;
	CCheckBox OtherAutoStrafe;

	CCheckBox OtherCircle;
	CKeyBind OtherCirclekey;

	/*    antiaim manual    */
	CKeyBind  keybasedaaleft;
	CKeyBind  keybasedaaright;
	CKeyBind  keybasedaaback;

};

class aristoispejawindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CAntiAimtab AntiAimtab;
	CLegitBot LegitbotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CSettingsTab SettingsTab;
	CColorTab ColorsTab;
	SkinsTab Skins;
	Movement Movetab;

	CButton LoadButton;
	CButton SaveButton2;
	CButton LoadButton2;
	CButton SaveButton3;
	CButton LoadButton3;

	CButton UnloadButton;
	CButton PanicButton;
	CButton ThirdButton;
	CButton ByPass;
	CButton FirstButton;
	CComboBox ConfigBox;
};

namespace g_menu
{
	void SetupMenu();
	void DoUIFrame();

	extern aristoispejawindow window;
};