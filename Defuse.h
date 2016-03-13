#pragma once

#ifndef __Gamemode_Defuse_h__
#define __Gamemode_Defuse_h__

class Defuse : public Gamemode
{
	public:
		Defuse();
		~Defuse();
		void Setup();
		void Loop();
	protected:
		void setupCountdown();
		unsigned long prevMillis;
		int keyState;
		int switchState;
		int btnState;
		int prevKeyState;
		int prevSwitchState;
		int prevBtnState;
		unsigned long btnTimer;
		unsigned long millisHeld;
		unsigned long secsHeld;
		unsigned long prevSecsHeld;
		unsigned long firstHeld;
		bool defused;
};

#endif
