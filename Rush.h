#pragma once

#ifndef __Gamemode_Rush_h__
#define __Gamemode_Rush_h__

class Rush : public Gamemode {
	public:
		Rush();
		~Rush();
		void Setup();
		void Loop();

	protected:
		int bases;
		void setupBases();
		void setupCountdown();
};

#endif