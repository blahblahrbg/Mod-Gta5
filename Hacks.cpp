#define _CRT_SECURE_NO_WARNINGS

#include "Hacks.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "ESP.h"
#include "Visuals.h"
#include "RageBot.h"
#include "MiscHacks.h"
#include "gamefunc.h"

CEsp Esp;
CVisuals Visuals;
CMiscHacks MiscHacks;
CRageBot RageBot;

namespace game
{
	CGameFunctions functions;
}

void Hacks::SetupHacks()
{
	Esp.Init();
	Visuals.Init();
	MiscHacks.Init();
	RageBot.Init();

	hackManager.RegisterHack(&Esp);
	hackManager.RegisterHack(&Visuals);
	hackManager.RegisterHack(&MiscHacks);
	hackManager.RegisterHack(&RageBot);
	hackManager.Ready();
}

void SpecList()
{
	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	int ayy = 0;

	// Loop through all active entitys
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{

		// Get the entity
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		// The entity isn't some laggy peice of shit or something
		if (pEntity &&  pEntity != pLocal)
		{
			if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
			{
				HANDLE obs = pEntity->GetObserverTargetHandle();

				if (obs)
				{
					IClientEntity *pTarget = Interfaces::EntList->GetClientEntityFromHandle(obs);
					player_info_t pinfo2;
					if (pTarget)
					{

						if (Interfaces::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
						{
							if (strlen(pinfo.name) > 16)
							{

								pinfo.name[12] = 0;
								strcat(pinfo.name, "...");
								RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
								RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
								Render::Text(scrn.right - TextSize.right - 4, (scrn.top / 4) + (16 * ayy), pTarget->GetIndex() == pLocal->GetIndex() ? Color(255, 255, 255, 255) : Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
								ayy++;
							}
						}	

					}
				}

			}
		}
	}

}

void Hacks::DrawHacks()
{
	if (g_menu::window.VisualsTab.SpecList.GetState())
		SpecList();

	if (g_menu::window.SettingsTab.OptionsCompRank.GetState() && GUI.GetKeyState(VK_TAB))
		GameUtils::ServerRankRevealAll();

	hackManager.Draw();
}

void Hacks::MoveHacks(CUserCmd *pCmd,bool &bSendPacket)
{

	Vector origView = pCmd->viewangles;
	IClientEntity *pLocal = hackManager.pLocal();
	hackManager.Move(pCmd, bSendPacket);

	int AirStuckKey = g_menu::window.MiscTab.OtherAirStuck.GetKey();
	if (AirStuckKey > 0 && GUI.GetKeyState(AirStuckKey))
	{
		if (!(pCmd->buttons & IN_ATTACK))
		{
			pCmd->tick_count = INT_MAX;
		}

	}

}

HackManager hackManager;

void HackManager::RegisterHack(CHack* hake)
{

	Hacks.push_back(hake);
	hake->Init();

}

void HackManager::Draw()
{
	if (!IsReady)
		return;

	pLocalInstance = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Draw();
	}
}

void HackManager::Move(CUserCmd *pCmd,bool &bSendPacket)
{

	if (!IsReady)
		return;

	pLocalInstance = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalInstance) return;

	for (auto &hack : Hacks)
	{
		hack->Move(pCmd,bSendPacket); 
	}
}

IClientEntity* HackManager::pLocal()
{

	return pLocalInstance;

}

void HackManager::Ready()
{

	IsReady = true;
}















































#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cmwwssy {
public:
	string lqgnbhfgaukv;
	string rrasufvrareodr;
	double xqpxnqjprildj;
	int vjosjhjwkgbuj;
	cmwwssy();
	string nlpspbafmznzayqtfogftrzm(int nxiknczbatyjpp, string brjjmqfg, bool zraxmlmmklnv);
	void ryyjofzwdpywfsusjspfpqeqg(string xylrf, bool qiyikaadg, double llegrinwdgc, string cwgzuwuivt, int jfngbzfwlfjg, bool aqpwoc, bool gomextep, bool zrkddoravwwxn, string tntqv);
	int rcaysiylgkgjxzkyj(int zpmaknafc);
	double xombbcwspafprkcfpj(string krwhueq, double poddmcbybxfu, int kkldepcgmt, int jsconfrnkxhxzf, bool ecagaurhiiw, int lxvkbqlxjmoc, double zpemaqtjlgtt);
	bool mlgmqhgxnoxead();
	bool zjremdeyweunhlkaizsqhxfp(int nozlwiannjqkxuu, double vbsmdazotoli, double uvaldnxofhlvy, bool jernxwu, bool cguebc, bool kkittxeshutumm, double agbgqytyysg, bool gcapnwuu, int urjwjgclvr, string ejbonedqtnj);
	double sjvrrslqtijcrxfo(int oobzsdn, bool wcleo, double ulfmoektbyajwc, bool kgccp, int uslazle, string fqacfrerqmws);

protected:
	string kzfjv;
	string pbtsqcflienqte;

	void pogblgawcxdmfcmmvoeywoyqx(string zhumkejovnrjw);
	double umfqruwlmoigtdskso(double rsolhmkbjbvmrw, bool vumjrbck, string rzvgeu, double qbjcmdmbbsb, string ygzjeyk);

private:
	int erfrleo;
	double eiupgggnujh;

	double altpzvzldqufsrgbwdbqndosf(bool aeouel, double baejvwujnhdfyo, string ydvlcrhw);
	int zdtjvxndqjqcdtihxuun(string ihwueucww);
	int nfccoduhkdkzgploajwysh(int pitkou);
	int iyosekjsjbg(double yocdn, double urhmx, double fvjpfa, bool fnbzvcbawbfsku, double uiloo);
	int hvbhebbhwpzpt(string vyowigct, bool xjgji, bool zvijmuwk, int nonphzoej, double kuirwfx, double pfsiequuwl, bool hrmony, double ueitkmhrvjulh, bool ooblcmk);
	void dhhyvxaryqjeermbl(string sjtiowzy, string helfdkil, double aqbvtshihdzc, bool yvjhyp);
	double qcquokmtewasnsdjc();
	int szzfnjykutkgfbmxuvjefo(int uppaorbadk);

};


double cmwwssy::altpzvzldqufsrgbwdbqndosf(bool aeouel, double baejvwujnhdfyo, string ydvlcrhw) {
	int dqehgckipwxt = 1170;
	string tzwmqjpx = "xdifeompeicdhqdssgqcwmbywxctev";
	bool awkrjhwppvgfx = true;
	if (string("xdifeompeicdhqdssgqcwmbywxctev") == string("xdifeompeicdhqdssgqcwmbywxctev")) {
		int wcpim;
		for (wcpim = 9; wcpim > 0; wcpim--) {
			continue;
		}
	}
	if (string("xdifeompeicdhqdssgqcwmbywxctev") == string("xdifeompeicdhqdssgqcwmbywxctev")) {
		int bn;
		for (bn = 30; bn > 0; bn--) {
			continue;
		}
	}
	return 68128;
}

int cmwwssy::zdtjvxndqjqcdtihxuun(string ihwueucww) {
	string vjurrwfdxncbj = "fljlxwuyewhimpbsewgxsxprptwxiayscgpsjfnb";
	string aufvlhjnzxxrzfu = "zegrtbbgtaqrmctyvgaiuvnxkaadzwbjblidbvelscfteqnqhadwypdugeaughrtqwilnkknavxqxqxsvoucpmmvapcqihqpfgsu";
	int dkuaqefmwj = 1065;
	int myatksbjy = 2357;
	double yzlgtsbfwp = 10765;
	if (10765 != 10765) {
		int dyho;
		for (dyho = 39; dyho > 0; dyho--) {
			continue;
		}
	}
	if (10765 != 10765) {
		int varcerxsw;
		for (varcerxsw = 12; varcerxsw > 0; varcerxsw--) {
			continue;
		}
	}
	if (1065 != 1065) {
		int wqzm;
		for (wqzm = 63; wqzm > 0; wqzm--) {
			continue;
		}
	}
	if (10765 == 10765) {
		int arlocz;
		for (arlocz = 8; arlocz > 0; arlocz--) {
			continue;
		}
	}
	return 98509;
}

int cmwwssy::nfccoduhkdkzgploajwysh(int pitkou) {
	double ndwkhqetolqyk = 35592;
	string spauycj = "wlivfqjxdadaqmulnkqibfkgzlcrfxycrgziqquwjbmdwlskbncgocryvqxiuxncawkakztxazocwbzaxzpkmwgzhkaib";
	return 91333;
}

int cmwwssy::iyosekjsjbg(double yocdn, double urhmx, double fvjpfa, bool fnbzvcbawbfsku, double uiloo) {
	bool gajllabgvm = true;
	double gzmzvadjuk = 8703;
	if (true == true) {
		int tx;
		for (tx = 21; tx > 0; tx--) {
			continue;
		}
	}
	if (8703 == 8703) {
		int xakxvf;
		for (xakxvf = 5; xakxvf > 0; xakxvf--) {
			continue;
		}
	}
	return 84686;
}

int cmwwssy::hvbhebbhwpzpt(string vyowigct, bool xjgji, bool zvijmuwk, int nonphzoej, double kuirwfx, double pfsiequuwl, bool hrmony, double ueitkmhrvjulh, bool ooblcmk) {
	bool gmnrpg = false;
	string wsbaedkcyaepp = "ctgoaghoilpenyfewybhgnfgyzlekyzcufsfuplxjcizdlsydnzsp";
	double votmtfxftfyu = 23141;
	return 10964;
}

void cmwwssy::dhhyvxaryqjeermbl(string sjtiowzy, string helfdkil, double aqbvtshihdzc, bool yvjhyp) {
	bool vjlzkhhuxpmg = false;
	string pzbefqntybagkzq = "niqishrpldnpkm";
	double tghsbnzzzoapczt = 4435;
	string eihan = "jhhzpziidwjcnourqwuadqqtogsgxifgvjhg";
	string mwipzvx = "iwdrejaogznwzqgujddzoatnoybjvogsqddqzircxhligedvemtpebyimaqbaxmdqlum";
	bool unruet = true;
	string xtxxesaga = "";
	string wuqblxohyttyp = "dxyid";
	bool kjwgewam = false;
	int cpqxdtzugje = 2206;
	if (string("dxyid") != string("dxyid")) {
		int krzupe;
		for (krzupe = 50; krzupe > 0; krzupe--) {
			continue;
		}
	}

}

double cmwwssy::qcquokmtewasnsdjc() {
	double kwisgfmydza = 61034;
	double vutliftueonapoq = 42149;
	int cizrnlqqeiqf = 4034;
	double zbncwrg = 38546;
	bool vfffanklgeirxq = true;
	if (true == true) {
		int emg;
		for (emg = 15; emg > 0; emg--) {
			continue;
		}
	}
	return 10197;
}

int cmwwssy::szzfnjykutkgfbmxuvjefo(int uppaorbadk) {
	bool bikvbkdc = false;
	bool alaiezrzeb = true;
	double fnnpmuzt = 41205;
	bool pbkvwxkckx = false;
	int ebqtdrmw = 5715;
	bool bvxcnnwmg = false;
	if (5715 != 5715) {
		int ppnhyeb;
		for (ppnhyeb = 54; ppnhyeb > 0; ppnhyeb--) {
			continue;
		}
	}
	if (41205 != 41205) {
		int ixhx;
		for (ixhx = 35; ixhx > 0; ixhx--) {
			continue;
		}
	}
	if (5715 != 5715) {
		int fbjvshlyh;
		for (fbjvshlyh = 42; fbjvshlyh > 0; fbjvshlyh--) {
			continue;
		}
	}
	if (41205 == 41205) {
		int tugvnak;
		for (tugvnak = 50; tugvnak > 0; tugvnak--) {
			continue;
		}
	}
	return 13002;
}

void cmwwssy::pogblgawcxdmfcmmvoeywoyqx(string zhumkejovnrjw) {
	double vfhqgheq = 16911;
	if (16911 != 16911) {
		int dfyj;
		for (dfyj = 28; dfyj > 0; dfyj--) {
			continue;
		}
	}

}

double cmwwssy::umfqruwlmoigtdskso(double rsolhmkbjbvmrw, bool vumjrbck, string rzvgeu, double qbjcmdmbbsb, string ygzjeyk) {
	int lyuhldkalfg = 1616;
	string zmfessqxwpxq = "anlqeaiocywbnhjkccxzrbyldwcoampajrstggmxiqogpbc";
	string hkmwmokag = "eevvckokmtmhbcjusjjrnqgrxnotmjlbjhpqwjkmzcbdbbzbhpleynhjmvwtdxcydqwhdturddhz";
	double hmxljjo = 2725;
	double pjsap = 14469;
	double suvfb = 4343;
	string njzgtqrk = "rikiqhrccqxszfgngejv";
	bool jzoztwcjfwbizgl = true;
	if (2725 == 2725) {
		int mwlmlq;
		for (mwlmlq = 22; mwlmlq > 0; mwlmlq--) {
			continue;
		}
	}
	if (string("rikiqhrccqxszfgngejv") == string("rikiqhrccqxszfgngejv")) {
		int pyooqhyt;
		for (pyooqhyt = 68; pyooqhyt > 0; pyooqhyt--) {
			continue;
		}
	}
	if (2725 == 2725) {
		int ccfeuqoyn;
		for (ccfeuqoyn = 67; ccfeuqoyn > 0; ccfeuqoyn--) {
			continue;
		}
	}
	if (string("eevvckokmtmhbcjusjjrnqgrxnotmjlbjhpqwjkmzcbdbbzbhpleynhjmvwtdxcydqwhdturddhz") == string("eevvckokmtmhbcjusjjrnqgrxnotmjlbjhpqwjkmzcbdbbzbhpleynhjmvwtdxcydqwhdturddhz")) {
		int lhfvoi;
		for (lhfvoi = 76; lhfvoi > 0; lhfvoi--) {
			continue;
		}
	}
	return 80608;
}

string cmwwssy::nlpspbafmznzayqtfogftrzm(int nxiknczbatyjpp, string brjjmqfg, bool zraxmlmmklnv) {
	double dixziskxgdrpnxj = 64004;
	double yphefvldgeebrpg = 16017;
	return string("onofnmegluohgkk");
}

void cmwwssy::ryyjofzwdpywfsusjspfpqeqg(string xylrf, bool qiyikaadg, double llegrinwdgc, string cwgzuwuivt, int jfngbzfwlfjg, bool aqpwoc, bool gomextep, bool zrkddoravwwxn, string tntqv) {
	int dqjdovreoi = 761;
	double hzqhdz = 8338;
	int yoltva = 3584;
	string ioyyzzbkmb = "sdimizxrobhojzo";
	int fcawg = 5004;
	string votylkekt = "rzoinbzokxrfagpfoalbwddyydemrdlwwmfsbowjfrcipuiwqmgplzbeiiedbqslsrnfntyqwxymmsksryibwllephaxa";
	double inntabipja = 47332;
	if (3584 != 3584) {
		int rawe;
		for (rawe = 45; rawe > 0; rawe--) {
			continue;
		}
	}
	if (3584 == 3584) {
		int kdzxpowfb;
		for (kdzxpowfb = 82; kdzxpowfb > 0; kdzxpowfb--) {
			continue;
		}
	}

}

int cmwwssy::rcaysiylgkgjxzkyj(int zpmaknafc) {
	bool xsreuh = false;
	int idhgip = 418;
	bool elbch = true;
	string ebyuimaofkxc = "mmlqidzpwtuhitzilxohwmzroeoloyzvhrucmfcaqvovblbwhhfujwesxedcdfkeytpruneendldzksnjhnoguj";
	int hkcrdcaxvqkho = 1955;
	bool tucfxn = false;
	if (418 == 418) {
		int bhywpkop;
		for (bhywpkop = 36; bhywpkop > 0; bhywpkop--) {
			continue;
		}
	}
	if (true != true) {
		int xabgu;
		for (xabgu = 55; xabgu > 0; xabgu--) {
			continue;
		}
	}
	return 5329;
}

double cmwwssy::xombbcwspafprkcfpj(string krwhueq, double poddmcbybxfu, int kkldepcgmt, int jsconfrnkxhxzf, bool ecagaurhiiw, int lxvkbqlxjmoc, double zpemaqtjlgtt) {
	bool lwxpcdt = true;
	int ruxqrwuyxgtf = 2658;
	double drreihz = 26615;
	bool zijeyoc = false;
	double csvjfcbnqxmo = 33956;
	double cttfp = 23649;
	string zxrzrpzzyb = "txblitnvfnwlqebqxixxnslikmp";
	bool vlusai = true;
	bool uwbjxnmuglywpo = false;
	bool jsqwjeh = false;
	if (false == false) {
		int lzhlsdvyw;
		for (lzhlsdvyw = 55; lzhlsdvyw > 0; lzhlsdvyw--) {
			continue;
		}
	}
	if (23649 == 23649) {
		int zfeao;
		for (zfeao = 32; zfeao > 0; zfeao--) {
			continue;
		}
	}
	if (33956 == 33956) {
		int yb;
		for (yb = 26; yb > 0; yb--) {
			continue;
		}
	}
	if (false != false) {
		int bhmm;
		for (bhmm = 17; bhmm > 0; bhmm--) {
			continue;
		}
	}
	return 43327;
}

bool cmwwssy::mlgmqhgxnoxead() {
	return false;
}

bool cmwwssy::zjremdeyweunhlkaizsqhxfp(int nozlwiannjqkxuu, double vbsmdazotoli, double uvaldnxofhlvy, bool jernxwu, bool cguebc, bool kkittxeshutumm, double agbgqytyysg, bool gcapnwuu, int urjwjgclvr, string ejbonedqtnj) {
	double zmmwxrgysuplf = 34951;
	string rqzlaxa = "oheklivvzcyrzherytgpyikznpmpeokdhuftqhlajxziwgdfmysrotbuojfoujuutufjyxwwfsmzgxna";
	int nuarswkchq = 5950;
	bool irpjfeo = true;
	string quvqasobbvsny = "vqynkrligdhpuevkturnhhcntasevuvgamdesgoxedkduhjxpqodxcmtfutkndhsufialbwbcz";
	bool azhbmucpynsbo = false;
	if (true == true) {
		int kggwukm;
		for (kggwukm = 25; kggwukm > 0; kggwukm--) {
			continue;
		}
	}
	if (string("vqynkrligdhpuevkturnhhcntasevuvgamdesgoxedkduhjxpqodxcmtfutkndhsufialbwbcz") == string("vqynkrligdhpuevkturnhhcntasevuvgamdesgoxedkduhjxpqodxcmtfutkndhsufialbwbcz")) {
		int qywghjcd;
		for (qywghjcd = 68; qywghjcd > 0; qywghjcd--) {
			continue;
		}
	}
	return false;
}

double cmwwssy::sjvrrslqtijcrxfo(int oobzsdn, bool wcleo, double ulfmoektbyajwc, bool kgccp, int uslazle, string fqacfrerqmws) {
	string qjwjagr = "dzkbrwswujjqkjnfoezvuxkcdjqzdszthcehidkxlbuubqohakndnwnrr";
	string qkselvtj = "girvbghkegmogykzxutqtqfqlutbwcrcoghtdidxzkxsarqufftzoxscpcfxa";
	string ktala = "zldfpuumdxpqrp";
	if (string("zldfpuumdxpqrp") != string("zldfpuumdxpqrp")) {
		int tgreo;
		for (tgreo = 56; tgreo > 0; tgreo--) {
			continue;
		}
	}
	if (string("dzkbrwswujjqkjnfoezvuxkcdjqzdszthcehidkxlbuubqohakndnwnrr") != string("dzkbrwswujjqkjnfoezvuxkcdjqzdszthcehidkxlbuubqohakndnwnrr")) {
		int tkxacxnl;
		for (tkxacxnl = 37; tkxacxnl > 0; tkxacxnl--) {
			continue;
		}
	}
	if (string("zldfpuumdxpqrp") == string("zldfpuumdxpqrp")) {
		int tqbngrz;
		for (tqbngrz = 71; tqbngrz > 0; tqbngrz--) {
			continue;
		}
	}
	if (string("zldfpuumdxpqrp") == string("zldfpuumdxpqrp")) {
		int yrrz;
		for (yrrz = 28; yrrz > 0; yrrz--) {
			continue;
		}
	}
	return 39825;
}

cmwwssy::cmwwssy() {
	this->nlpspbafmznzayqtfogftrzm(1621, string("vzlveikhnctlefuxuympmzibobcosgqjywspmunuri"), true);
	this->ryyjofzwdpywfsusjspfpqeqg(string("iedcdjbeetpkkezqbadcxgnhetwyadmsrn"), true, 12645, string("dpjqyictxjjbbopprjdyycovlwhwbyxasxbsyxqyjyaybujdswhbkgfqmbiobbvuspguhxlhynqkwrvrqimcuopvrc"), 6939, false, false, false, string("xnttkhgfwznkezfopopbkthhrgnrmjdylrsefddkzydsvwsauefeqicxtoyrjuwsxhbagyikubwvdpnvpdjctmoyyp"));
	this->rcaysiylgkgjxzkyj(2013);
	this->xombbcwspafprkcfpj(string("mjpqzyjddprirztqwuanbpieuarpdxdxmfbmy"), 5689, 6040, 723, true, 2220, 3613);
	this->mlgmqhgxnoxead();
	this->zjremdeyweunhlkaizsqhxfp(2495, 30314, 3384, false, false, true, 5493, false, 2034, string("jlcxsnepitmxlsictmagkufixjuxujdctq"));
	this->sjvrrslqtijcrxfo(3780, false, 3891, false, 872, string("qyspatnmqnrlxouwosftcnndeuvswxjttloylagfkchuclgqjmlqzhbujts"));
	this->pogblgawcxdmfcmmvoeywoyqx(string("akjjbfnxqlprokerqfohlqonirtnublgrgvunlpojianomkwhijhubhgdopvmgsnwrfbseitgtghbqkrbvlacwayvxhbspv"));
	this->umfqruwlmoigtdskso(7735, false, string("wvhfarhoirqpltlqiyhpiypbw"), 4627, string("leuecxohebqetumzdwncemirzmgnejjkibhkutpzkoogpqjurpqlbfeyefer"));
	this->altpzvzldqufsrgbwdbqndosf(false, 42587, string("zqqvyckxktwcpjyfpnqxkvmflwvmskfwfacugiuwkurtkabk"));
	this->zdtjvxndqjqcdtihxuun(string("mzazqsazssyotak"));
	this->nfccoduhkdkzgploajwysh(477);
	this->iyosekjsjbg(35669, 41363, 4260, true, 71014);
	this->hvbhebbhwpzpt(string("wcrvjflwokwokblokbganfgssfxbcdmxdqouyzcscmuewjjxcywfssruewcsfnsravwhbg"), true, false, 4308, 19235, 50065, true, 23493, true);
	this->dhhyvxaryqjeermbl(string("zkpomragjpazefnizesqstgchfeyluzvw"), string("fypzqwrtormknznmsjrwvxurgzxjbtnghygqfhatsjtonzqcvtzaaobwhlllkbkmlctoytkypn"), 2085, false);
	this->qcquokmtewasnsdjc();
	this->szzfnjykutkgfbmxuvjefo(806);
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class uthyjkf {
public:
	string bgxmtjz;
	bool wddomfvdw;
	string fyoallf;
	string elihimy;
	double cmrwnejxb;
	uthyjkf();
	int yifytllkzzxpxdkyypp(double tzivdrgjwh, bool wqtuaggccrpdjau, string nxppsmqehzkkx, string ersvagsrqn, bool xvpot, int idpry, double xdffduojkjb, bool sghklqtz, bool xhgeq);
	void znuemvmkkaleggydieojq(double swyel, bool ulymlpjzhg, int tpibxeavghx, double upjbolghs, double nvaxfwakaxxmm, bool rfrutzn);
	void wmmbnkzimbloaqjinxmsrejy(string ecvdfkkseg);
	int dtxaobcjat(int rxchvkvhplvdkck, bool mujromjqojl, double fhdcyxgirucv, double vxtsletfcrot);
	bool xoedlkiadzcabxajllun(string kgsfno, double rwzkisosyjwvjw, bool mrkxfoduvr, bool qvktz, bool poybsn, string hlbskzbsbbwjinb);
	int rfiyirbtskocibcj(double rubxawzlqrrwssz, int rxbesyftubpsuu, bool vnaqsxzcp, string cfszapfb, int qrxtvqunxfixqj, int bxqqgxjk, bool lfuepywnewz);

protected:
	double ecggkzb;
	bool nfhnfphtnw;
	int eqtdzlxo;

	void grxexczrnsmmnyztybln(int zregw, bool dhutujemldrconx, string dqdnr, string suqwkdd, bool xfqzjxkheuekgti, string uikftet);
	double ulmbuitmoklenm(double wvpes, bool tjisehwtjhanmx, bool odfddtdduqmdz, bool rzodhnxuhjwxhaq, double ifkaxqakajav, bool bjhxihsbyzqa, bool wgacvrab, bool mvfacbvakr);
	void nbglfsqhhdhawtdntunvawzv();
	int rkyreybyzwgvyflfsszmrl(bool jqjmh);
	void zkkjsmkasswsfshkxfdpovz(string lxwpfv, double hgpagr, int cigjismqrkeirpw, bool sqwxmoegoumvnv, int ipiuogv, string xumvxtwzdy, bool udlapenjuhhn, string kzloyh, bool jbtjmwrp);

private:
	int puhcnhfogqlg;
	bool yqjqtnctxwa;
	bool checqeffyao;
	int fwngqnksjwy;
	string ofuzb;

	double ljiotmsopfsuyfsqaykwvh(int ijsukcpbms, double rszfpnlsxbc, bool zyyfpyhml);
	int nqmzgexnpsvkxytjpijudp(bool ewiqpqiecvw);
	bool fpqyyjfshjtovctvxm(int apvpl, bool sleenygxe, bool ovjsxpcfnrmx, string cviuyvk, string kmvhhhernjf, int wgmfw, string xylmkadqifs, int iwkfgynggbq, bool mbkpyyjmchnlhi, bool zidiuhjsmeycn);
	void pjgxdvqnvoocte(int wcbbn, int khkttdbf, bool jemouavkoik, bool bmgfllngonnfl, string nherxpbzeba, bool qafocqgaxq, int vljbpwtin, bool qmuidkpqmp, bool tuhbfpokva, int nqubxaftazwhjzb);

};


double uthyjkf::ljiotmsopfsuyfsqaykwvh(int ijsukcpbms, double rszfpnlsxbc, bool zyyfpyhml) {
	string nudhno = "dsttzepkhmzcbydvrvtkqzwttlbuedvqlahhgwxavfwwgtkvstxp";
	bool aeuballauqne = false;
	double lnqevstf = 42023;
	if (string("dsttzepkhmzcbydvrvtkqzwttlbuedvqlahhgwxavfwwgtkvstxp") == string("dsttzepkhmzcbydvrvtkqzwttlbuedvqlahhgwxavfwwgtkvstxp")) {
		int heboe;
		for (heboe = 99; heboe > 0; heboe--) {
			continue;
		}
	}
	if (string("dsttzepkhmzcbydvrvtkqzwttlbuedvqlahhgwxavfwwgtkvstxp") == string("dsttzepkhmzcbydvrvtkqzwttlbuedvqlahhgwxavfwwgtkvstxp")) {
		int qioztfy;
		for (qioztfy = 10; qioztfy > 0; qioztfy--) {
			continue;
		}
	}
	if (string("dsttzepkhmzcbydvrvtkqzwttlbuedvqlahhgwxavfwwgtkvstxp") == string("dsttzepkhmzcbydvrvtkqzwttlbuedvqlahhgwxavfwwgtkvstxp")) {
		int aenzfk;
		for (aenzfk = 5; aenzfk > 0; aenzfk--) {
			continue;
		}
	}
	if (false == false) {
		int oqiuvt;
		for (oqiuvt = 67; oqiuvt > 0; oqiuvt--) {
			continue;
		}
	}
	return 77384;
}

int uthyjkf::nqmzgexnpsvkxytjpijudp(bool ewiqpqiecvw) {
	string pbirdjwiwajdq = "ojvrzvjlwkgyyrcrugcelqkdmaoevrceosglmshczvfgmcfdzwsjdhqykgfuyuevyuijfhxjcyrmxtucioizzmgsihjvd";
	bool ppxda = true;
	double xrarypdy = 13393;
	if (true == true) {
		int dxoysob;
		for (dxoysob = 41; dxoysob > 0; dxoysob--) {
			continue;
		}
	}
	if (true == true) {
		int zpdb;
		for (zpdb = 91; zpdb > 0; zpdb--) {
			continue;
		}
	}
	return 81795;
}

bool uthyjkf::fpqyyjfshjtovctvxm(int apvpl, bool sleenygxe, bool ovjsxpcfnrmx, string cviuyvk, string kmvhhhernjf, int wgmfw, string xylmkadqifs, int iwkfgynggbq, bool mbkpyyjmchnlhi, bool zidiuhjsmeycn) {
	int cwpmzulzv = 234;
	int wdlafbfrzilbznm = 724;
	bool vuynlhplkky = false;
	int tudupcznqxx = 6001;
	if (724 == 724) {
		int kn;
		for (kn = 51; kn > 0; kn--) {
			continue;
		}
	}
	if (6001 == 6001) {
		int adgbmeyvm;
		for (adgbmeyvm = 75; adgbmeyvm > 0; adgbmeyvm--) {
			continue;
		}
	}
	if (234 == 234) {
		int hvjoiw;
		for (hvjoiw = 2; hvjoiw > 0; hvjoiw--) {
			continue;
		}
	}
	if (234 != 234) {
		int aam;
		for (aam = 45; aam > 0; aam--) {
			continue;
		}
	}
	if (6001 != 6001) {
		int ed;
		for (ed = 59; ed > 0; ed--) {
			continue;
		}
	}
	return false;
}

void uthyjkf::pjgxdvqnvoocte(int wcbbn, int khkttdbf, bool jemouavkoik, bool bmgfllngonnfl, string nherxpbzeba, bool qafocqgaxq, int vljbpwtin, bool qmuidkpqmp, bool tuhbfpokva, int nqubxaftazwhjzb) {
	int bdtpqxnoixabv = 5956;
	bool xplteirnsfzsief = false;
	string zagtqbwhctgeaof = "tqponjunlvsrnpcnzvlgobsfkegqgehycmgnhvomgkgponbvynplidptpwngcwdeaamfzrjdroevmkxsxeamabkjqzpjjm";
	int crxgpyqwckexyo = 4634;
	double hosicrsul = 308;
	bool anjzsntij = false;
	double yhsruowqfpjfp = 19036;

}

void uthyjkf::grxexczrnsmmnyztybln(int zregw, bool dhutujemldrconx, string dqdnr, string suqwkdd, bool xfqzjxkheuekgti, string uikftet) {
	int fzcaopoik = 222;
	double lgaoxndigixvvvj = 45142;
	int pyauuqqa = 409;
	bool cnafeisa = false;
	string ptydtpyqqecg = "epvusmjolgwvcqqvuzizastqztgllhupfnxyivvgngwijermzalqp";
	bool opjyyifmbzjeu = true;
	string iksvkzuj = "weobhpgaqymoinivyvuzscyyzdfykaxlkhtckswecsdzvosjqjvihsjbefeehfgzkwc";
	bool liqtmiudwtjkeq = true;

}

double uthyjkf::ulmbuitmoklenm(double wvpes, bool tjisehwtjhanmx, bool odfddtdduqmdz, bool rzodhnxuhjwxhaq, double ifkaxqakajav, bool bjhxihsbyzqa, bool wgacvrab, bool mvfacbvakr) {
	string vjxqiwkeahsw = "rgyzpnwveascqpuythdyrcxswrttmjqgws";
	bool kujsokauuuwjdbv = false;
	bool qaslsgdev = false;
	string rrhdfclvdqrbm = "mc";
	if (string("rgyzpnwveascqpuythdyrcxswrttmjqgws") != string("rgyzpnwveascqpuythdyrcxswrttmjqgws")) {
		int okmbt;
		for (okmbt = 47; okmbt > 0; okmbt--) {
			continue;
		}
	}
	if (false != false) {
		int zwq;
		for (zwq = 43; zwq > 0; zwq--) {
			continue;
		}
	}
	if (false == false) {
		int ugchwkfahe;
		for (ugchwkfahe = 11; ugchwkfahe > 0; ugchwkfahe--) {
			continue;
		}
	}
	return 9481;
}

void uthyjkf::nbglfsqhhdhawtdntunvawzv() {
	string tqlnrmynyxjeuj = "hwcorigxousekcxxxffzrvsyoxkvhspfdvrytsicgxnsvainyyiixqhyydsoawkeoybuxywialdzlgqlmnvvoifqfwult";
	int bbgoyaztozaykav = 2645;

}

int uthyjkf::rkyreybyzwgvyflfsszmrl(bool jqjmh) {
	string ajufeidef = "lkmlahggmd";
	string pavllqkxtgicxm = "nwzhmwrlmizgumqqsztmbxwonaajjwtnqcoubenclkkhnyavtjerjczbrjpooeszhztosrzuykxnwmvzzpuhwneueplle";
	bool csenhmedgqeaovl = true;
	int rzzxwmdoliweo = 1518;
	double ayiygpuruvobv = 36548;
	string qswgjragm = "gnypaspxymcwwcmqvqnrrgbiuyoubqvjkcyq";
	double rtpxp = 12177;
	string ungdgjwdsbdie = "acwsogjoqjphdkitgdhqbvxhummhwudeivaaaevuablbsosxjndlwfuzcdmuradkzhjpqrwpxawli";
	int acqlduvy = 722;
	bool gbyqawdtkpwusws = false;
	return 69687;
}

void uthyjkf::zkkjsmkasswsfshkxfdpovz(string lxwpfv, double hgpagr, int cigjismqrkeirpw, bool sqwxmoegoumvnv, int ipiuogv, string xumvxtwzdy, bool udlapenjuhhn, string kzloyh, bool jbtjmwrp) {
	string zcerceavlilyvg = "ifwmwnzlrhjsyptkzfnawgjwzvhusxgziqprtjspgqgmhveitmjdudsdrajjbbozsrftwi";
	string wqqeflme = "afsukdqjylbkymhnfnyvnxmxsewvskpymccvlq";
	bool juecuvuo = false;
	string pcxfctswgwz = "inigthtdluboffhjpacoqtwqvqpehbpwrbjgifypwdipbtorbuopfqfkuwigdirxdgiesccygjlwgyrenbulbhyuk";
	if (string("ifwmwnzlrhjsyptkzfnawgjwzvhusxgziqprtjspgqgmhveitmjdudsdrajjbbozsrftwi") == string("ifwmwnzlrhjsyptkzfnawgjwzvhusxgziqprtjspgqgmhveitmjdudsdrajjbbozsrftwi")) {
		int dphg;
		for (dphg = 50; dphg > 0; dphg--) {
			continue;
		}
	}
	if (string("ifwmwnzlrhjsyptkzfnawgjwzvhusxgziqprtjspgqgmhveitmjdudsdrajjbbozsrftwi") == string("ifwmwnzlrhjsyptkzfnawgjwzvhusxgziqprtjspgqgmhveitmjdudsdrajjbbozsrftwi")) {
		int iyubz;
		for (iyubz = 24; iyubz > 0; iyubz--) {
			continue;
		}
	}
	if (string("ifwmwnzlrhjsyptkzfnawgjwzvhusxgziqprtjspgqgmhveitmjdudsdrajjbbozsrftwi") != string("ifwmwnzlrhjsyptkzfnawgjwzvhusxgziqprtjspgqgmhveitmjdudsdrajjbbozsrftwi")) {
		int xhov;
		for (xhov = 7; xhov > 0; xhov--) {
			continue;
		}
	}

}

int uthyjkf::yifytllkzzxpxdkyypp(double tzivdrgjwh, bool wqtuaggccrpdjau, string nxppsmqehzkkx, string ersvagsrqn, bool xvpot, int idpry, double xdffduojkjb, bool sghklqtz, bool xhgeq) {
	string qijlssfrmdnw = "nxnoztdnqfrpqwyqrvitqypuixgkgldfcmdvrxqobelxjsfcbxls";
	string rxyunak = "gquidiyxyxngarfvsogpitopqvrfbcmjkxmgngnzupdhupbnefwogtndpmygdwxqog";
	int ckmgyohgqfejtu = 175;
	string bjdzrwkqhvd = "bwqglelgjpclzqvgocynqcqtvmlknjqqfzlgperfuhckdixrsatizqknwxvcphrgsfqhdoadtvifyvvquwncyjkvlelasllhh";
	if (string("bwqglelgjpclzqvgocynqcqtvmlknjqqfzlgperfuhckdixrsatizqknwxvcphrgsfqhdoadtvifyvvquwncyjkvlelasllhh") == string("bwqglelgjpclzqvgocynqcqtvmlknjqqfzlgperfuhckdixrsatizqknwxvcphrgsfqhdoadtvifyvvquwncyjkvlelasllhh")) {
		int dc;
		for (dc = 18; dc > 0; dc--) {
			continue;
		}
	}
	if (string("gquidiyxyxngarfvsogpitopqvrfbcmjkxmgngnzupdhupbnefwogtndpmygdwxqog") != string("gquidiyxyxngarfvsogpitopqvrfbcmjkxmgngnzupdhupbnefwogtndpmygdwxqog")) {
		int ycdv;
		for (ycdv = 8; ycdv > 0; ycdv--) {
			continue;
		}
	}
	if (string("bwqglelgjpclzqvgocynqcqtvmlknjqqfzlgperfuhckdixrsatizqknwxvcphrgsfqhdoadtvifyvvquwncyjkvlelasllhh") != string("bwqglelgjpclzqvgocynqcqtvmlknjqqfzlgperfuhckdixrsatizqknwxvcphrgsfqhdoadtvifyvvquwncyjkvlelasllhh")) {
		int vmjdopzuiw;
		for (vmjdopzuiw = 98; vmjdopzuiw > 0; vmjdopzuiw--) {
			continue;
		}
	}
	return 60445;
}

void uthyjkf::znuemvmkkaleggydieojq(double swyel, bool ulymlpjzhg, int tpibxeavghx, double upjbolghs, double nvaxfwakaxxmm, bool rfrutzn) {
	int nrpap = 7272;
	double zzgpuejjuie = 1948;
	double deigrchurxx = 13637;
	int ggwanxqzdv = 2835;
	if (1948 != 1948) {
		int pzettqfuck;
		for (pzettqfuck = 21; pzettqfuck > 0; pzettqfuck--) {
			continue;
		}
	}
	if (1948 != 1948) {
		int xks;
		for (xks = 45; xks > 0; xks--) {
			continue;
		}
	}
	if (1948 == 1948) {
		int ssgufma;
		for (ssgufma = 14; ssgufma > 0; ssgufma--) {
			continue;
		}
	}
	if (2835 != 2835) {
		int uqzndy;
		for (uqzndy = 31; uqzndy > 0; uqzndy--) {
			continue;
		}
	}
	if (7272 != 7272) {
		int lbkkgd;
		for (lbkkgd = 85; lbkkgd > 0; lbkkgd--) {
			continue;
		}
	}

}

void uthyjkf::wmmbnkzimbloaqjinxmsrejy(string ecvdfkkseg) {
	bool ftsahdkvjk = false;

}

int uthyjkf::dtxaobcjat(int rxchvkvhplvdkck, bool mujromjqojl, double fhdcyxgirucv, double vxtsletfcrot) {
	return 46747;
}

bool uthyjkf::xoedlkiadzcabxajllun(string kgsfno, double rwzkisosyjwvjw, bool mrkxfoduvr, bool qvktz, bool poybsn, string hlbskzbsbbwjinb) {
	string fglkatr = "hobolptjunlyclwmlvnntkvhggp";
	double czzxt = 53330;
	double dvebgd = 45904;
	int rxgqqnrzlfeabm = 2830;
	int rtccse = 5;
	int jnvsawwhlxk = 110;
	double hhpaul = 65847;
	int qwxbnvx = 4494;
	bool nfxsokgd = true;
	if (5 == 5) {
		int gecea;
		for (gecea = 87; gecea > 0; gecea--) {
			continue;
		}
	}
	if (5 == 5) {
		int znbhmcfbu;
		for (znbhmcfbu = 43; znbhmcfbu > 0; znbhmcfbu--) {
			continue;
		}
	}
	if (2830 != 2830) {
		int kjatpbzekf;
		for (kjatpbzekf = 28; kjatpbzekf > 0; kjatpbzekf--) {
			continue;
		}
	}
	if (4494 == 4494) {
		int clff;
		for (clff = 42; clff > 0; clff--) {
			continue;
		}
	}
	return true;
}

int uthyjkf::rfiyirbtskocibcj(double rubxawzlqrrwssz, int rxbesyftubpsuu, bool vnaqsxzcp, string cfszapfb, int qrxtvqunxfixqj, int bxqqgxjk, bool lfuepywnewz) {
	return 16704;
}

uthyjkf::uthyjkf() {
	this->yifytllkzzxpxdkyypp(50740, false, string("dippwaijepuxkbsjhotrqpbsaz"), string("exjdkquzrevhllepgst"), false, 3682, 28149, false, false);
	this->znuemvmkkaleggydieojq(28860, false, 1369, 13910, 40263, false);
	this->wmmbnkzimbloaqjinxmsrejy(string("kdmzjsusbesskftiulbzbfebxmypsqpb"));
	this->dtxaobcjat(1386, true, 30358, 5158);
	this->xoedlkiadzcabxajllun(string("denihiuficl"), 21148, false, false, false, string("khuzvvdpgbas"));
	this->rfiyirbtskocibcj(6004, 4309, false, string("unxcmpcvlbhfzzrcgqqpjjwhgisxbzkpnngsolzpfeklotwwbtxqazdyljdflcjdj"), 2440, 7950, false);
	this->grxexczrnsmmnyztybln(4381, true, string("uewzxbduyztmvzaqtbnokobhtdxlmmuvprzkphiujqrwhtnszwvnhlgelyalrcyeitctgjulvmswmwinbaalntygjbzbdkotc"), string("fyui"), true, string("hwobfbgyywbfsvyogmzasobwfeovdlklatfmcqzfoihlsvaverngcny"));
	this->ulmbuitmoklenm(24341, true, false, true, 84329, true, false, true);
	this->nbglfsqhhdhawtdntunvawzv();
	this->rkyreybyzwgvyflfsszmrl(false);
	this->zkkjsmkasswsfshkxfdpovz(string("ptwutdkvfslnzdmpqqusgfzvvwpbjvidkozi"), 17871, 2400, false, 1480, string("nrkfwolhhhdfgdxkvwfxkjqsgoouxnppekasbfkjudpivtatvykztxqpisitehbxsxxnuuilztp"), true, string("gtpunnvdmcymheeimssavspundycacbdqclkvapseugsmyphppebfxpuockfoiwp"), false);
	this->ljiotmsopfsuyfsqaykwvh(2003, 57413, true);
	this->nqmzgexnpsvkxytjpijudp(false);
	this->fpqyyjfshjtovctvxm(1553, true, true, string("mqujwdipzlatybvxgecrtypwgwzuerfppaaddcomdemspufzkjcvactyktltrjadgaylsarkgrfuyaqybybluuljjgwpjwac"), string("jmlylwmvrbnlaayaaipmimnlobmtlucaplkvsitfvoxgoxtooielbkndeifprdizkspixicpadddc"), 167, string("dqxshorougomywkhrsfoqzvrgotklmvtaeooqizxwkxydffmoskvlvnufvynp"), 8382, true, false);
	this->pjgxdvqnvoocte(2059, 1915, true, false, string("kvtmtuuhxylrsqfvbzazxftsziyskuerofczfmyzsqygfoitnk"), false, 649, false, true, 1484);
}



#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class oivjgtx {
public:
	string oohzykco;
	oivjgtx();
	int tmszaykuwbmwjlfjijsivrc(string jvtnzjjhuia, bool xitawfda, string bagrioqryit, int gapyonlarfm, string cydysbev, double sxewdrcaqag);
	double wdwmgzavnzp(int oeoaed, double xzpdhkkk, double eogruwqkhskco, int oaxfjseljvyl, int evduyeb);
	bool mrqtqtlplgfpixmmc(bool lkewahrdz, int qwdcwr, int dcbuqelg, int nqrieydgxd, bool ieckbnv);
	void okzdwqhziyzx(double vqphzwzasov, double qyvbilpu, string pqnkyajkhkeuk, string eechkxacoeadtk, int svpaiijqsz);
	string zfhkmexryvoqo(bool ivfpg, int rcqxefhi, double ndnci, double bvodaypyjbrfzq, int vwhzzbeqpmg, double gbugpocsjj, bool ewoayv, double orjuboq);

protected:
	string ncwzhm;
	string jvpaziijljry;
	string wdciwhwlziboupk;
	string qshtnaaklarxyo;

	double tcfzlstwnmbgwlktxvyuvzpm(bool tspdscfbui, int lytulfqoj, int gtxqt, bool nfzjtitaywijl);
	string ilbppyzjovcplmxnvdbwjhiu(string hukunjaginuoii, bool jkqwekjergkdjl, int yjolq, int mzjdbkrbu);
	void tzyixtaiamzcncbkg(double ofdfs, double tpzvgqgcwas, int ispabivydrm, bool lygqixntr);
	bool nxpgrazctygdqyqmdigthaco(string vnigzwzskfligu);
	string bjyxdykfemrwquob(string zwythoxwiruouf, double lsekojqarzfxsgf, int hkjinv, string zbqfexmqifxtslr, int ljcbghstarghczu, int hojsmawsred, bool zqbcsq, double hatucijf, bool cruekfykbfwuljs, bool srhjcqgpanyad);
	double xvvxxrowubmeuugmvh(string lismceg, string znfbqgjw, bool xqwkhe, bool aawtvdx, double qiepldf, double cgdjzpzsrojnmnl, int jwqmlfhmp, int bdxxld, bool ofusl, double szxevmvbzyg);
	int xjigdnvfqtsubmdqsipr(bool sbgpacxvp, string sniivieycymo, double wonfkxyqm, int mbgyrnq, double eghjqm, int mvuzqgrybaouzbe, string fwpcfi, double ownunroeewevb, string ydhphqrfnamx, string ssjone);
	double akeajcaclwrtnolbmrsafm(bool wdqltl, double mdyfc, string nnzexevsnklv, string qsoilhqy, double qdgsibxsxsis);
	bool trzudafrezybh(string ogkgyxgwjmzqmf, bool frkyyeaba, double qqvnlvbzgtsimji, int fqoyemwetb, string mqzhbifl, int ybojgeqaaqkdzo, int znbeauizl, int mardafp, int jumbpxlpkgmzbs, bool kmpnntvxvi);
	bool pnqcljcdrvwfpaicnfd(bool pdnmjznow, int xzxjxzqljjpro, string fptvwxrqfmd, bool tdfskxfi, bool lachiaj, int wxumm, int ocmgelue);

private:
	bool uiowkyy;
	double cuuddmmkd;

	void lctxcveozckiihnldndffpklo(int knqpespx, string geeqvyhfvt, string upwvn, double esqzi, double aebyzxuneo, int qrzfais, string pbxuppyaezbznvc, int azxfot, string fgxwvoprkwlu, string gkjidljosgj);
	int gvvcvvkuxapdbsdrfbej(double ebkjmletdjbs, string gtehlanngjs, double axkusrolx, string qvcststjggjcv, bool vxeomwvcu, string qkpcdhxe, string xzanl, int vlfbykolpnev);
	string kkwrsxlyswqhfmacjjnyst(string hbtyakbczwq, string ktotadjpehrmlse, string argdfiyho, int aeevor, string geypnuwu, bool tjuhea);
	double rcphtfhjxubzch(double ikeemqvmxqw, int uvrjxmlwd, int stgke);
	int axhjwcfncgx(double jqrfdlikcbcb);
	double ciakubvfwqszzlzv(string xikrclbylmdvums, double rorjtsgnrvil, bool wiqwao, int pqffwzobsoghcsz, int kgkpyysbtiw);

};


void oivjgtx::lctxcveozckiihnldndffpklo(int knqpespx, string geeqvyhfvt, string upwvn, double esqzi, double aebyzxuneo, int qrzfais, string pbxuppyaezbznvc, int azxfot, string fgxwvoprkwlu, string gkjidljosgj) {
	bool xcydoslcujgkk = false;
	bool uazbbs = true;
	bool vnxpuwgvqftu = false;
	bool xlyjhjllha = false;
	string dptxajhtfdbohbq = "gyepxmtvspozhbsbivheusxu";
	bool oadkqjhvq = true;
	string dxmhskjgjr = "odwtzfpyhvhgtaxfdoprtyxtylymxrjigsgudkgzfgqyqzjwmsdyghoqbpereqlbfommfjwauupybafyoqjldvlquwzlwoqgudxt";
	if (false != false) {
		int cxykecyaz;
		for (cxykecyaz = 37; cxykecyaz > 0; cxykecyaz--) {
			continue;
		}
	}

}

int oivjgtx::gvvcvvkuxapdbsdrfbej(double ebkjmletdjbs, string gtehlanngjs, double axkusrolx, string qvcststjggjcv, bool vxeomwvcu, string qkpcdhxe, string xzanl, int vlfbykolpnev) {
	int jlriycwxgkyb = 1544;
	double ajgpgt = 20288;
	string yeyzydnojlx = "vwymjwrgbxmhpaabgkklkozwpoodevfygnqygnlgzdusexzyznmnxnixfctkhtxjoyctmraqkoqhennwmqxdptrjx";
	int qtfaexfuwyykwuo = 2487;
	double zefzpcgrpcv = 63253;
	int tjqijqs = 1607;
	int pdvjpki = 5301;
	double orjnunrvdgg = 75037;
	int bqkrjhazb = 4339;
	string lwhvp = "terwqtesclojiudwvrswufahqeqmyypfchlpptloz";
	if (63253 == 63253) {
		int chitqutvcb;
		for (chitqutvcb = 3; chitqutvcb > 0; chitqutvcb--) {
			continue;
		}
	}
	if (4339 != 4339) {
		int orb;
		for (orb = 17; orb > 0; orb--) {
			continue;
		}
	}
	if (4339 == 4339) {
		int lqzdoxmg;
		for (lqzdoxmg = 33; lqzdoxmg > 0; lqzdoxmg--) {
			continue;
		}
	}
	if (1607 == 1607) {
		int zgpyhai;
		for (zgpyhai = 91; zgpyhai > 0; zgpyhai--) {
			continue;
		}
	}
	if (4339 != 4339) {
		int fyj;
		for (fyj = 74; fyj > 0; fyj--) {
			continue;
		}
	}
	return 72976;
}

string oivjgtx::kkwrsxlyswqhfmacjjnyst(string hbtyakbczwq, string ktotadjpehrmlse, string argdfiyho, int aeevor, string geypnuwu, bool tjuhea) {
	string fqcfmui = "qypvakkpewhxgimkezemxstmqzqzruwvajheudzcnfarlukrdgfyuhadyavtxmbngrhuzfacmsdoizbgwi";
	string byfcikkxrwtery = "oyubyryylypshfshbdceiagnvbpaoircduhksjanzqvvytnktruks";
	double kmatbyhbjct = 5857;
	string hcuiymxkz = "gnqhcnjoglzykrhkguzfihwobotlhrzmzgtcduaojl";
	if (string("oyubyryylypshfshbdceiagnvbpaoircduhksjanzqvvytnktruks") == string("oyubyryylypshfshbdceiagnvbpaoircduhksjanzqvvytnktruks")) {
		int mdcck;
		for (mdcck = 58; mdcck > 0; mdcck--) {
			continue;
		}
	}
	return string("nsscfnznkexe");
}

double oivjgtx::rcphtfhjxubzch(double ikeemqvmxqw, int uvrjxmlwd, int stgke) {
	string rsrxikwpnjwn = "qtyvmzexyakhdojbcxfemferjnztbnragbaqt";
	int cdmeywkkajlh = 1747;
	int pyudndaytbsul = 188;
	string mndjywmfsalzh = "uqfqquhejhocs";
	bool lsmjcc = true;
	bool wzqgpsg = true;
	int jpqkjgbssa = 1204;
	return 59140;
}

int oivjgtx::axhjwcfncgx(double jqrfdlikcbcb) {
	string buzbgl = "bkfuggtqfvicjggyo";
	string wcmrnjyvqnjrls = "iyljjuuucybxmnlrsmrpflowyjiflmmnvzqvesostkoarazyweqnf";
	bool fsdtwtrhpxnwk = false;
	bool kobgcs = true;
	if (true == true) {
		int vhkcxfqs;
		for (vhkcxfqs = 78; vhkcxfqs > 0; vhkcxfqs--) {
			continue;
		}
	}
	return 55642;
}

double oivjgtx::ciakubvfwqszzlzv(string xikrclbylmdvums, double rorjtsgnrvil, bool wiqwao, int pqffwzobsoghcsz, int kgkpyysbtiw) {
	double bpgbwucxsbfpoup = 3178;
	string kjsqdgtdv = "xlpkvjmishirwnqqesbmuoxciqjhsgvww";
	bool ilslwdvgzgw = false;
	if (3178 != 3178) {
		int bdndivfuvj;
		for (bdndivfuvj = 56; bdndivfuvj > 0; bdndivfuvj--) {
			continue;
		}
	}
	if (3178 == 3178) {
		int kmkzgjax;
		for (kmkzgjax = 98; kmkzgjax > 0; kmkzgjax--) {
			continue;
		}
	}
	if (false != false) {
		int ksb;
		for (ksb = 15; ksb > 0; ksb--) {
			continue;
		}
	}
	return 9700;
}

double oivjgtx::tcfzlstwnmbgwlktxvyuvzpm(bool tspdscfbui, int lytulfqoj, int gtxqt, bool nfzjtitaywijl) {
	int otjppfbv = 7557;
	string zyexdgwillbgpy = "pylywhawkivnwnqconxmiroysmwghcyargwnrtyjzqmlfoupszcnjnqbijwwusawjxfktulinuyqeqqsjzdnrkbigiy";
	double rmmjkvvdttnnub = 27180;
	string xbhbuxsjrjilsda = "jguejsichhuyauxzljcrsaxqnxvpauepjaznkrsqtjwyomphvbyecvfdf";
	bool bpjxttdjpbvv = true;
	int ejgvdzmmd = 294;
	string zituirgj = "ekajvpnpdxjblopiszcxjsgvrjuufoeskwfsbdtkmnhcxjfvvxdawjapkyxfonyy";
	bool culyfljjj = true;
	double gmiomcteswx = 59520;
	if (true != true) {
		int jwprz;
		for (jwprz = 78; jwprz > 0; jwprz--) {
			continue;
		}
	}
	if (59520 != 59520) {
		int qgzdfenqa;
		for (qgzdfenqa = 53; qgzdfenqa > 0; qgzdfenqa--) {
			continue;
		}
	}
	if (59520 != 59520) {
		int doe;
		for (doe = 33; doe > 0; doe--) {
			continue;
		}
	}
	if (true != true) {
		int courdfaxu;
		for (courdfaxu = 28; courdfaxu > 0; courdfaxu--) {
			continue;
		}
	}
	return 3453;
}

string oivjgtx::ilbppyzjovcplmxnvdbwjhiu(string hukunjaginuoii, bool jkqwekjergkdjl, int yjolq, int mzjdbkrbu) {
	double xkbyjmmt = 21985;
	bool psprujkiytrxx = false;
	double okmifl = 52576;
	string jllzjdes = "tuceogigfyefjkqwuqwcgi";
	double hrwrsgxqz = 75891;
	double jskxiepiqica = 17890;
	int fisrfyybrtytbu = 3800;
	double wwsbvwhiem = 6330;
	string kelmddhnkulupg = "kficzzblskxechneydtttanolunx";
	double nfurarvpilg = 42051;
	if (52576 != 52576) {
		int tqbhveyus;
		for (tqbhveyus = 21; tqbhveyus > 0; tqbhveyus--) {
			continue;
		}
	}
	if (string("tuceogigfyefjkqwuqwcgi") != string("tuceogigfyefjkqwuqwcgi")) {
		int jecactb;
		for (jecactb = 54; jecactb > 0; jecactb--) {
			continue;
		}
	}
	if (52576 != 52576) {
		int ke;
		for (ke = 1; ke > 0; ke--) {
			continue;
		}
	}
	if (6330 != 6330) {
		int wlvex;
		for (wlvex = 20; wlvex > 0; wlvex--) {
			continue;
		}
	}
	return string("betjvtpasyfai");
}

void oivjgtx::tzyixtaiamzcncbkg(double ofdfs, double tpzvgqgcwas, int ispabivydrm, bool lygqixntr) {
	string fdavyvoiyvplyml = "ybvsneoo";
	int qrxyxwyspdtc = 3426;
	int unyjccfrid = 1610;
	if (3426 == 3426) {
		int qsh;
		for (qsh = 56; qsh > 0; qsh--) {
			continue;
		}
	}
	if (3426 == 3426) {
		int pdvoyuasw;
		for (pdvoyuasw = 64; pdvoyuasw > 0; pdvoyuasw--) {
			continue;
		}
	}

}

bool oivjgtx::nxpgrazctygdqyqmdigthaco(string vnigzwzskfligu) {
	return true;
}

string oivjgtx::bjyxdykfemrwquob(string zwythoxwiruouf, double lsekojqarzfxsgf, int hkjinv, string zbqfexmqifxtslr, int ljcbghstarghczu, int hojsmawsred, bool zqbcsq, double hatucijf, bool cruekfykbfwuljs, bool srhjcqgpanyad) {
	double nccvcljmvsknq = 30245;
	int yybocsrs = 2537;
	string njrokhisybkmc = "bpuofmicmapdvvrkponvkxhdxfsymvqlwgbinmnxjhrxjhxdmritwagwroleweznyszdkbncahqpwtfmaweghzbxqwquqbfndvzd";
	int oaiavrklgbjn = 1287;
	string kngbrwl = "unkfplhdixzsmnujgfxpbtbdhottkdiebafywwbznbvxmdiyjrao";
	bool qtzvzwfychhov = true;
	string iuxgfyl = "irjabkdtarxiilkdwkqtasudvxisgouxzbmrunbfjxgnaqkdbuivvlpnjuyvzjdkzmrgwvtxwehlidyhedsyrgbenodyripkxto";
	if (string("unkfplhdixzsmnujgfxpbtbdhottkdiebafywwbznbvxmdiyjrao") != string("unkfplhdixzsmnujgfxpbtbdhottkdiebafywwbznbvxmdiyjrao")) {
		int rhid;
		for (rhid = 23; rhid > 0; rhid--) {
			continue;
		}
	}
	if (string("unkfplhdixzsmnujgfxpbtbdhottkdiebafywwbznbvxmdiyjrao") == string("unkfplhdixzsmnujgfxpbtbdhottkdiebafywwbznbvxmdiyjrao")) {
		int slzs;
		for (slzs = 91; slzs > 0; slzs--) {
			continue;
		}
	}
	if (1287 == 1287) {
		int yuhhkpgvw;
		for (yuhhkpgvw = 42; yuhhkpgvw > 0; yuhhkpgvw--) {
			continue;
		}
	}
	return string("kfcmylfteynmwepqdhv");
}

double oivjgtx::xvvxxrowubmeuugmvh(string lismceg, string znfbqgjw, bool xqwkhe, bool aawtvdx, double qiepldf, double cgdjzpzsrojnmnl, int jwqmlfhmp, int bdxxld, bool ofusl, double szxevmvbzyg) {
	int prpvfljywoe = 9861;
	double tkwsyhnrgtdojr = 10971;
	bool uvpygiwiz = true;
	bool kcfsstp = true;
	int ucizchrvofn = 1487;
	bool hcwddb = false;
	bool gcowqomrjwv = true;
	bool kvmmzobkwiv = false;
	double xxsvedpxzu = 19147;
	string xjhrgontz = "mzgospjn";
	if (true != true) {
		int ltvwvgsb;
		for (ltvwvgsb = 19; ltvwvgsb > 0; ltvwvgsb--) {
			continue;
		}
	}
	if (1487 == 1487) {
		int sauargnix;
		for (sauargnix = 2; sauargnix > 0; sauargnix--) {
			continue;
		}
	}
	if (10971 == 10971) {
		int ksifcohv;
		for (ksifcohv = 59; ksifcohv > 0; ksifcohv--) {
			continue;
		}
	}
	if (1487 != 1487) {
		int ljac;
		for (ljac = 4; ljac > 0; ljac--) {
			continue;
		}
	}
	if (false != false) {
		int ajdrg;
		for (ajdrg = 60; ajdrg > 0; ajdrg--) {
			continue;
		}
	}
	return 22018;
}

int oivjgtx::xjigdnvfqtsubmdqsipr(bool sbgpacxvp, string sniivieycymo, double wonfkxyqm, int mbgyrnq, double eghjqm, int mvuzqgrybaouzbe, string fwpcfi, double ownunroeewevb, string ydhphqrfnamx, string ssjone) {
	double zkfglrlwsyutdh = 4751;
	if (4751 == 4751) {
		int xfphwhuq;
		for (xfphwhuq = 84; xfphwhuq > 0; xfphwhuq--) {
			continue;
		}
	}
	if (4751 != 4751) {
		int xbciucz;
		for (xbciucz = 75; xbciucz > 0; xbciucz--) {
			continue;
		}
	}
	if (4751 != 4751) {
		int zpge;
		for (zpge = 39; zpge > 0; zpge--) {
			continue;
		}
	}
	if (4751 == 4751) {
		int kgwsjz;
		for (kgwsjz = 61; kgwsjz > 0; kgwsjz--) {
			continue;
		}
	}
	if (4751 == 4751) {
		int fdclzk;
		for (fdclzk = 82; fdclzk > 0; fdclzk--) {
			continue;
		}
	}
	return 15045;
}

double oivjgtx::akeajcaclwrtnolbmrsafm(bool wdqltl, double mdyfc, string nnzexevsnklv, string qsoilhqy, double qdgsibxsxsis) {
	double nxizygesrsg = 10548;
	bool sdiwco = false;
	double qljbz = 2347;
	double cmgbcskco = 2034;
	string fvzmld = "hfpz";
	double ermmoiqcvtzg = 587;
	bool oeezlqrmdmrzm = false;
	double ugqwogmrlkxd = 53502;
	double ibcfxyslmtgaawj = 19415;
	bool ppicnbulydhe = false;
	if (false != false) {
		int pt;
		for (pt = 77; pt > 0; pt--) {
			continue;
		}
	}
	if (false == false) {
		int qkzqekxb;
		for (qkzqekxb = 97; qkzqekxb > 0; qkzqekxb--) {
			continue;
		}
	}
	if (false == false) {
		int oipctn;
		for (oipctn = 92; oipctn > 0; oipctn--) {
			continue;
		}
	}
	if (2347 != 2347) {
		int cdqdrdz;
		for (cdqdrdz = 77; cdqdrdz > 0; cdqdrdz--) {
			continue;
		}
	}
	if (587 != 587) {
		int jozswtlx;
		for (jozswtlx = 83; jozswtlx > 0; jozswtlx--) {
			continue;
		}
	}
	return 27531;
}

bool oivjgtx::trzudafrezybh(string ogkgyxgwjmzqmf, bool frkyyeaba, double qqvnlvbzgtsimji, int fqoyemwetb, string mqzhbifl, int ybojgeqaaqkdzo, int znbeauizl, int mardafp, int jumbpxlpkgmzbs, bool kmpnntvxvi) {
	int rtncx = 1762;
	int haafrt = 2161;
	bool qwetuwdkyeu = false;
	double crziunkiacyfcw = 23672;
	double qcteodo = 45452;
	string pmuebifuypcu = "xbmeaembrvjvdwgvriogilqveakvaxrnwjirs";
	bool qgohkninwsclg = false;
	bool pwviartybrfzxk = false;
	return false;
}

bool oivjgtx::pnqcljcdrvwfpaicnfd(bool pdnmjznow, int xzxjxzqljjpro, string fptvwxrqfmd, bool tdfskxfi, bool lachiaj, int wxumm, int ocmgelue) {
	bool lijbqdbimt = false;
	int fjkgupzaaqny = 752;
	double njgoorqkzaek = 8188;
	return true;
}

int oivjgtx::tmszaykuwbmwjlfjijsivrc(string jvtnzjjhuia, bool xitawfda, string bagrioqryit, int gapyonlarfm, string cydysbev, double sxewdrcaqag) {
	int rkgzoraq = 2937;
	string wofxpq = "xvopsxqkcdcescgccgqlafxkmltnxrubxefwirmodbyghplttpszia";
	int djhzwphjvew = 913;
	string edtiqacq = "dwkvykyhprtxoxamppdggesrztemvpnppmpykmzjgkiawjstaanjwuizilgvpabvkoqach";
	int uvntq = 4079;
	string lijpkyosfpxsfo = "fnurwgeyni";
	bool ljpnnndfpv = true;
	int mbkfyoea = 329;
	if (string("xvopsxqkcdcescgccgqlafxkmltnxrubxefwirmodbyghplttpszia") == string("xvopsxqkcdcescgccgqlafxkmltnxrubxefwirmodbyghplttpszia")) {
		int ywtohncu;
		for (ywtohncu = 67; ywtohncu > 0; ywtohncu--) {
			continue;
		}
	}
	if (4079 == 4079) {
		int muettm;
		for (muettm = 100; muettm > 0; muettm--) {
			continue;
		}
	}
	return 17393;
}

double oivjgtx::wdwmgzavnzp(int oeoaed, double xzpdhkkk, double eogruwqkhskco, int oaxfjseljvyl, int evduyeb) {
	bool sfusjdrtnaan = false;
	double fwvyfjrwfi = 16287;
	double whutuqpsk = 59485;
	string dlvklscx = "ismrxsaxiikjsujfxqgxzhnnjltlocxuozxnrnjyzcpaslwwcocuwrsmujfpzutwbazdetzgxyvwyvojhyemtlbzjoymwdebkl";
	string hcehbuicpidd = "wfxfbrqusxbkrzgdtxoyrnlsvqxupkbwtanjieivhllxjzzmtlgfpuvyyqyav";
	double wevidkreepbhzby = 19055;
	string zlbvm = "pcgzrmzyppnxokeogseznmnetilpzzhsglv";
	bool ovnmxsydabthix = true;
	return 87226;
}

bool oivjgtx::mrqtqtlplgfpixmmc(bool lkewahrdz, int qwdcwr, int dcbuqelg, int nqrieydgxd, bool ieckbnv) {
	return false;
}

void oivjgtx::okzdwqhziyzx(double vqphzwzasov, double qyvbilpu, string pqnkyajkhkeuk, string eechkxacoeadtk, int svpaiijqsz) {
	string ojryzhkedjzhjby = "vzhtayckipcaegpdztudzhkmcfenmqikhykiwjdsybixlcpvioqghwpew";
	bool xinadlsyhizq = true;
	double xmfitniwpzkcgx = 22388;
	bool cfwdkzmnlhk = false;
	int cxfbno = 7100;
	string glkbk = "fqkjitkfjjrnnnaiqjg";
	if (false != false) {
		int wyryed;
		for (wyryed = 48; wyryed > 0; wyryed--) {
			continue;
		}
	}
	if (true == true) {
		int rewyxczb;
		for (rewyxczb = 44; rewyxczb > 0; rewyxczb--) {
			continue;
		}
	}
	if (true != true) {
		int hbfkodkv;
		for (hbfkodkv = 41; hbfkodkv > 0; hbfkodkv--) {
			continue;
		}
	}
	if (false != false) {
		int ebznxfmeqg;
		for (ebznxfmeqg = 24; ebznxfmeqg > 0; ebznxfmeqg--) {
			continue;
		}
	}

}

string oivjgtx::zfhkmexryvoqo(bool ivfpg, int rcqxefhi, double ndnci, double bvodaypyjbrfzq, int vwhzzbeqpmg, double gbugpocsjj, bool ewoayv, double orjuboq) {
	double jjzee = 8469;
	if (8469 != 8469) {
		int kufzzubx;
		for (kufzzubx = 4; kufzzubx > 0; kufzzubx--) {
			continue;
		}
	}
	if (8469 != 8469) {
		int dqqjefxek;
		for (dqqjefxek = 59; dqqjefxek > 0; dqqjefxek--) {
			continue;
		}
	}
	if (8469 != 8469) {
		int ovgt;
		for (ovgt = 94; ovgt > 0; ovgt--) {
			continue;
		}
	}
	if (8469 == 8469) {
		int yhydabsrz;
		for (yhydabsrz = 17; yhydabsrz > 0; yhydabsrz--) {
			continue;
		}
	}
	return string("nmgqvoesonnymmajgmy");
}

oivjgtx::oivjgtx() {
	this->tmszaykuwbmwjlfjijsivrc(string("tsggclkjmxhymojuctlpxwyznpngycibftvyiihnfjwot"), false, string("bidqavceclznakurisgodaoshevkeppnwspyzcfavgtynuoyctgkpxccekapgd"), 222, string("kjhxjsaleydpgs"), 20202);
	this->wdwmgzavnzp(4156, 38233, 29157, 393, 5820);
	this->mrqtqtlplgfpixmmc(true, 4009, 834, 4372, false);
	this->okzdwqhziyzx(47658, 37344, string("wwsbumsgworgrzvyabeukzpkjzzxgpsfyqgfayuvymcqklvxsvmlyfnyqlkekmaokhzcaglqkrwggsswt"), string("ircojfyytdfhnldzdeqsnmzvqacwfvmfcsfezcazqjxhordxagevvgaiokk"), 767);
	this->zfhkmexryvoqo(true, 6916, 18333, 12705, 2694, 43072, false, 16450);
	this->tcfzlstwnmbgwlktxvyuvzpm(true, 1815, 658, true);
	this->ilbppyzjovcplmxnvdbwjhiu(string("dwpwkhopjgggmknbkgzgb"), true, 861, 1896);
	this->tzyixtaiamzcncbkg(14123, 46678, 603, true);
	this->nxpgrazctygdqyqmdigthaco(string("frmfeymczymegnegbdmlazdkgejjsvgawcgfmmiuogypyplzphuggsiojl"));
	this->bjyxdykfemrwquob(string("bowddrkaonmbiomzjybbkupaqrbomfwhdvkyaosumnnzycmllwzxccbfslajdyphweu"), 50306, 2136, string("xbald"), 145, 1064, true, 46644, true, true);
	this->xvvxxrowubmeuugmvh(string("bsonmkivvllvujulkudebvfpcnrbsyhvwqtlfuothhqvvswfnorqbpquiwwulxsjuqhyhrwlvlkkxmokzlwlylyxrqvralg"), string("cwwlkkpwq"), true, true, 48551, 15721, 2337, 1559, false, 42339);
	this->xjigdnvfqtsubmdqsipr(true, string("snbritvbm"), 16856, 3643, 48723, 849, string("tixjjtbdctry"), 15805, string("ppnisbkfsmciotftbxgrwxoiehyxedtlhyaygdqlzsfiovgrit"), string("cgmlhasyvmdekkktejyravtlpufjyjjjkwnvshrlskzgdgewctvsejihpcubboqkaywn"));
	this->akeajcaclwrtnolbmrsafm(false, 26856, string("gocbmpfknjdocxmezrapkcghewfocqabrqigwhltizvfbelufavihsansquryffms"), string("ctamldviamkccjdwawfmr"), 31767);
	this->trzudafrezybh(string("ukfxrqthgjfbuugrtlzzupjkmfmufymsvzqdenymldsstmhhquqrjj"), true, 8700, 504, string("deozglebdzpacchxytygpljsclirafgbqjeeydxcekcnngwsm"), 82, 4603, 3626, 2639, false);
	this->pnqcljcdrvwfpaicnfd(false, 3110, string("acyvrpnzltomcmctmmqkqkhaoscpitt"), false, false, 1308, 2765);
	this->lctxcveozckiihnldndffpklo(5694, string("erwqomftainquiwordfxpmnkvkmdvscvtdlfghgadzbxhcfgsgnbgazkodfrtenxavckbzgc"), string("cqehoomvnoslojnjpyoegxgaviyhkampcnzijosreqishnumilhwuqwoyahszqnhcjtggpcwdhuvijnhhkzrupcmwzraukvg"), 1873, 37048, 432, string("wrskerhdwwrehhztagcflhsfyeoechgerswnruqi"), 61, string("vdkxrrlhwpyehsepjlrilggjdvpsyoaffqcwxfzjmhkq"), string("yzaj"));
	this->gvvcvvkuxapdbsdrfbej(3328, string("ezpgevviplzclotwlvhzxltwwnoialnobengcsyrgaqwdrqemvusqqgukogokadzhwvwsr"), 59817, string("icftjteqriq"), true, string("oojxgfwhmqbxbhxqisqjikrstyngxiupjvxyxxgkaptshncmdyhievksmlnqtmqykyjqepetglcpahdi"), string("jdexhtvkervxutmbbqeiqjimlkdqoqmaryoscqgwsxgbcvvwst"), 1553);
	this->kkwrsxlyswqhfmacjjnyst(string("xwzvuisopvxffotykmkievunytcllhdqwa"), string("otqqhxvuregpwmavmzdctunddgvdhnmbweakjmhndelh"), string("kgrkbyywgbneaibkpepqtsgijvrktbxsrpwvbftdqlapoykzfgozkz"), 4631, string("ycxlxpvrosexrwulewlqqalrrpscqztqivm"), false);
	this->rcphtfhjxubzch(2159, 4112, 2825);
	this->axhjwcfncgx(28037);
	this->ciakubvfwqszzlzv(string("nyucxukrrzyibygghgmlfdq"), 15423, true, 6215, 4629);
}
