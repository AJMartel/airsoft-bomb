#include <stdarg.h>
#include "Bomb.h"

Rush::Rush()
{
	this->id = Gamemode::GAMEMODE_RUSH;
	this->name = "Rush";
	this->countdown = 0;
	this->bases = 0;
}


Rush::~Rush()
{
}

void Rush::Setup()
{
	Gamemode::Init();

	this->setupBases();
	delay(2000);

	this->setupCountdown();
	delay(2000);

	clearLCD();
	myLCD->print("Starting rush game");
	myLCD->setCursor(0, 1);
	//myLCD->print(printf("Bases [%1d] Time [%3d]", this->bases, this->countdown));

	delay(5000);
}

void Rush::Loop()
{
	clearLCD();
	myLCD->println("== RUSH ==");
	//myLCD->print(printf("Bases down     [%d]", this->bases));
}

void Rush::setupBases()
{
	clearLine(1);
	clearLine(2);

	myLCD->setCursor(0, 1);
	myLCD->print("Number of bases:");
	myLCD->setCursor(0, 2);
	myLCD->print("  (1-9)          [ ]");

	//this->bases = waitForSingleKeyInput("== Base no. set   ==");
}

void Rush::setupCountdown()
{
	int countdownTimeMinutes = 0;

	clearLine(1);
	clearLine(2);

	myLCD->setCursor(0, 1);
	myLCD->print("Bomb timer:");
	myLCD->setCursor(0, 2);
	myLCD->print("in minutes     [   ]");

	countdownTimeMinutes = getNumber(16, 2, 2);
	this->countdown = countdownTimeMinutes * 60;

	myLCD->setCursor(0, 2);
	myLCD->noCursor();
	myLCD->print("== Countdown set  ==");
}
