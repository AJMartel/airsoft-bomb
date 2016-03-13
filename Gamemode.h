#pragma once

#ifndef __Gamemode_h__
#define __Gamemode_h__

class Gamemode
{
	protected:
		int  id   = 0;
		char *name = "undefined";
		int countdown = 0;

	public:
		Gamemode();
		virtual ~Gamemode();
		static void Select(int id);
		static void SelectionMenu();
		static void updateDisplay(int gameModeShowing);
		virtual void Setup();
		virtual void Loop();
		void Init();

		static const int GAMEMODES_MAX = 3;

		static const int GAMEMODE_DEFUSE = 1;
		static const int GAMEMODE_CODEDEFUSE = 2;
		static const int GAMEMODE_RUSH = 3;
		static char gameModes[2][14];
};

#endif