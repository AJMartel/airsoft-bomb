#include "Bomb.h"

char Gamemode::gameModes[2][14] = { {"Defuse"}, {"Defuse (code)"} }; // , { "Missie 2" } };

Gamemode::Gamemode()
{
}


Gamemode::~Gamemode()
{
}

void Gamemode::Setup()
{

}

void Gamemode::Init()
{
	clearLCD();
	myLCD->print(printf("== %-14s ==", this->name));

	myLCD->setCursor(0, 3);
	myLCD->print("[*] Clear | Conf [#]");
}

void Gamemode::Loop()
{
	clearLCD();
	myLCD->print("=== ERROR ===");
	myLCD->setCursor(0, 1);
	myLCD->print("No gamemode set");
	
}

void Gamemode::updateDisplay(int gameModeShowing)
{
	myLCD->setCursor(0, 1);
	myLCD->print("<                  >");
	myLCD->setCursor(2, 1);
	myLCD->print(Gamemode::gameModes[gameModeShowing]);
}

void Gamemode::SelectionMenu()
{
	myLCD->clear();
	myLCD->home();
	myLCD->print("Select game mode:");

	myLCD->setCursor(0, 2);
	myLCD->print("[1] Prev    Next [3]");

	myLCD->setCursor(0, 3);
	myLCD->print("     [#] Confirm");
	int gameModeSelected = 0;
	int gameModeShowing = 0;
	Gamemode::updateDisplay(gameModeShowing);
	char key;
	while (gameModeSelected < 1)
	{
		key = NO_KEY;

		while (key == NO_KEY) {
			key = getKey();
			switch (key)
			{
			case '#':
				gameModeSelected = gameModeShowing + 1;
				break;

			case '1':
				gameModeShowing = (gameModeShowing > 0 ? gameModeShowing - 1 : Gamemode::GAMEMODES_MAX - 1);
				indicateInput();
				Gamemode::updateDisplay(gameModeShowing);
				break;

			case '3':
				gameModeShowing = (gameModeShowing < Gamemode::GAMEMODES_MAX - 1 ? gameModeShowing + 1 : 0);
				indicateInput();
				Gamemode::updateDisplay(gameModeShowing);
				break;
			default:
				break;
			}
			delay(100);
		}
		if (gameModeSelected < 1) {
			key = NO_KEY;
		}
		delay(100);
	}

	Gamemode::Select(gameModeSelected);
	indicateSelect();
}


void Gamemode::Select(int id)
{
	switch (id)
	{
	case GAMEMODE_DEFUSE:
		currentGameMode = new Defuse();
		break;
	case GAMEMODE_CODEDEFUSE:
		currentGameMode = new CodeDefuse();
		break;
	}
}
