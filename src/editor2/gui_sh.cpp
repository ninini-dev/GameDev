#include <editor/editor.h>
#include <editor/gui_misc.h>
#include <editor/gui_sh.h>
#include <own/shooter.h>
#include <editor/gui_enm.h>

namespace gui_sh {
	bool open = false;
	int tabSel = -1;
	int openContext = -1;

	void TopMenuBar() {

		TabResultTag topTabResult = TopBar();
	}

	const char* textBtAspect[] = {
		"LEAF", "EMPTY_CIRCLE", "CIRCLE", "CAPSULE", "KUNAI", "OFUDA", "BULLET", "B_CAPSULE",
		"START", "GLOW", "RING", "B_TINY", "TINY", "FLASH", "BULLET_ASPECT_COUNT"
	};
	const char* textBtColor[] = {
		"BLACK", "D_RED", "RED", "D_MAGENTA", "MAGENTA", "D_BLUE", "BLUE", "D_CYAN", "CYAN",
		"D_GREEN", "GREEN", "LIME", "D_YELLOW", "YELLOW", "GOLD", "WHITE", "BULLET_COLOR_COUNT"
	};

	void ContentMenu() {
		if (tabSel == -1)return;

		ShooterParams& sh = shooterList[tabSel];
		Nico::Bullet& bt = sh.bullet;

		SeparatorText("Shooter");
		InputInt("Bullets Amount", reinterpret_cast<int*>(&sh.aB));
		InputInt("Arrays Amount", reinterpret_cast<int*>(&sh.aA));
		InputFloat("Delay Between Bullet", &sh.dBtwB);
		InputFloat("Delay At Begin", &sh.dAtB);
		InputFloat("Spread Between Array", &sh.sBtwA);
		InputFloat("Rate", &sh.rate);
		SeparatorText("Bullet");
		Combo("Aspect", reinterpret_cast<int*>(&bt.aspect), textBtAspect, IM_ARRAYSIZE(textBtAspect));
		Combo("Color", reinterpret_cast<int*>(&bt.color), textBtColor, IM_ARRAYSIZE(textBtColor));
		InputFloat("Speed", &bt.speed);
	}
}
using namespace gui_sh;
void OpenShWindow() { gui_sh::open = true; }
void ShWindow() {
	if (!gui_sh::open)return;

	if (Begin("Shooter Editor",&gui_sh::open, ImGuiWindowFlags_MenuBar)) {
		gui_sh::TopMenuBar();

		auto temp = TopBar2(shooterList, gui_sh::tabSel, GetShooters());
		if (temp != -1)openContext = temp;
		if (openContext != -1) if (BeginPopupContextWindow("popup"))
		{
			if (Selectable("Select")) {
				GetShooters().flip(openContext);
				UpdateDrop(openContext);
				openContext = -1;
			}
			EndPopup();
		}
		gui_sh::ContentMenu();

		End();
	}
}
