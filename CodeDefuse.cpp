#include "Bomb.h"

CodeDefuse::CodeDefuse()
{
	id = Gamemode::GAMEMODE_CODEDEFUSE;
	name = "Code Defuse";
	countdown = 0;
	disarmCode = "";
	codeEntered = "";
	defused = false;
	prevMillis = 0;
}

CodeDefuse::~CodeDefuse()
{
}

void CodeDefuse::Setup()
{
	Gamemode::Init();

	this->setupCode();
	delay(2000);

	this->setupCountdown();
	delay(2000);

	clearLCD();
	myLCD->print("Time:        [" + displayTime(countdown) + "]");

	waitForGameStart();

	myLCD->setCursor(0, 1);
	myLCD->print("Enter code: [      ]");
	myLCD->setCursor(13, 1);
	myLCD->cursor();
}

void CodeDefuse::setupCountdown()
{
	int countdownTimeMinutes;
	clearLine(1);
	clearLine(2);

	myLCD->setCursor(0, 1);
	myLCD->print("Bomb timer:");
	myLCD->setCursor(0, 2);
	myLCD->print("in minutes      [  ]");

	countdownTimeMinutes = getNumber(17, 2, 2);
	this->countdown = countdownTimeMinutes * 60;

	clearLine(1);
	clearLine(2);
	myLCD->setCursor(0, 2);
	myLCD->noCursor();
	myLCD->print("== Countdown set  ==");
}

void CodeDefuse::setupCode()
{
	clearLine(1);
	clearLine(2);

	myLCD->setCursor(0, 1);
	myLCD->print(printf("%-20s", "Set defuse code:"));
	myLCD->setCursor(0, 2);
	myLCD->print("            [      ]");
	myLCD->setCursor(13, 2);
	myLCD->cursor();

	char key = getKey();
	bool codeSet = false;
	while (codeSet == false)
	{
		switch (key)
		{
		case '*':
			disarmCode = "";
			indicateCancel();
			myLCD->setCursor(13, 2);
			myLCD->print("      ");
			myLCD->setCursor(13, 2);
			myLCD->cursor();
			break;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		case 'A': case 'B': case 'C': case 'D':
			if (disarmCode.length() < 6)
			{
				this->disarmCode += key;
				indicateInput();
				myLCD->cursor();
			}
			else
			{
				indicateError();
			}
			break;

		case '#':
			if (disarmCode.length() >= 4) {
				codeSet = true;
			}
		}

		delay(100);
		key = getKey();
	}

	indicateSelect();
	clearLine(1);
	clearLine(2);
	myLCD->noCursor();
	myLCD->setCursor(0, 2);
	myLCD->print("== Disarm code set =");
}

void CodeDefuse::Loop()
{
	if (defused == false) {
		char key = getKey();
		bool codeSet = false;

		if ((unsigned long)(currentMillis - prevMillis) >= sInterval) {
			countdown--;
			prevMillis = millis();

			myLCD->setCursor(14, 0);
			myLCD->print(displayTime(countdown));

			blinkLed(ledRed);
			myLCD->setCursor(13 + codeEntered.length(), 1);
			myLCD->cursor();
		}

		while (codeSet == false)
		{
			switch (key)
			{
			case '*':
				codeEntered = "";
				indicateCancel();
				myLCD->setCursor(13, 1);
				myLCD->print("      ");
				myLCD->setCursor(13, 1);
				myLCD->cursor();
				break;

			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			case 'A': case 'B': case 'C': case 'D':
				if (codeEntered.length() < 6)
				{
					this->codeEntered += key;
					indicateInput();
					myLCD->cursor();
				}
				else
				{
					indicateError();
				}
				break;

			case '#':
				if (codeEntered == disarmCode) {
					clearLine(1);
					clearLine(3);
					myLCD->setCursor(0, 2);
					myLCD->print("   Bomb  disarmed   ");
					defused = true;

					missionEnd();
				}
				else {
					codeEntered = "";
					countdown -= 60;
					indicateError();
					
					myLCD->setCursor(0, 2);
					myLCD->print("Invalid code");
					myLCD->setCursor(0, 3);
					myLCD->print("60s penalty");
					
					myLCD->setCursor(13, 1);
					myLCD->print("      ");
					myLCD->setCursor(13, 1);
					myLCD->cursor();
				}
			}
			key = getKey();
		}
	}
}
