#pragma once
#include "Gamemode.h"
class CodeDefuse :
	public Gamemode
{
public:
	CodeDefuse();
	~CodeDefuse();
	void Setup();
	void Loop();
	void setupCode();
	void setupCountdown();

protected:
	String disarmCode;
	String codeEntered;
	bool defused;

	unsigned long prevMillis;
};

