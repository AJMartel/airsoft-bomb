#include "Bomb.h"

Defuse::Defuse()
{
	this->id = Gamemode::GAMEMODE_DEFUSE;
	this->name = "Defuse";
	this->countdown = 0;
	this->prevMillis = 0;
	this->keyState = LOW;
	this->switchState = LOW;
	this->btnState = LOW;
	this->firstHeld = 0;
	this->prevBtnState = LOW;
	this->prevKeyState = LOW;
	this->prevSwitchState = LOW;
	this->defused = false;
}


Defuse::~Defuse()
{
}

void Defuse::Setup()
{
	Gamemode::Init();

	this->setupCountdown();
	delay(2000);

	clearLCD();
	myLCD->print("Time:        ["+ displayTime(countdown) +"]");

	waitForGameStart();

	myLCD->setCursor(0, 2);
	myLCD->print("====== LOCKED ======");
	myLCD->setCursor(0, 3);
	myLCD->print("====== LOCKED ======");
}

void Defuse::setupCountdown()
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

	myLCD->setCursor(0, 2);
	myLCD->noCursor();
	myLCD->print("== Countdown set  ==");
}

void Defuse::Loop()
{
	int currentKeyState = digitalRead(keySwitch);
	int currentSwitchState = debounceMSwitchGreen.read();
	int currentBtnState = debounceButtonGreen.read();

	if (defused == false) {
		if ((unsigned long)(currentMillis - prevMillis) >= sInterval) {
			countdown--;
			prevMillis = millis();

			myLCD->setCursor(14, 0);
			myLCD->print(displayTime(countdown));

			blinkLed(ledRed);
		}

		if (currentKeyState != keyState) {
			myLCD->setCursor(0, 1);
			if (currentKeyState == HIGH) {
				myLCD->print("===== UNLOCKED =====");
			}
			else {
				myLCD->print("====== LOCKED ======");
				switchState = LOW;
			}
			keyState = currentKeyState;
		}

		if (keyState == HIGH) {
			if (currentSwitchState != switchState) {
				myLCD->setCursor(0, 2);
				if (currentSwitchState == HIGH) {
					myLCD->print("= READY TO DISARM  =");
					myLCD->setCursor(0, 3);
					myLCD->print("Awaiting input...");
					ledOn(ledBtnGreen);
				}
				else {
					myLCD->print("====== LOCKED ======");
					myLCD->setCursor(0, 3);
					myLCD->print("====== LOCKED ======");
					ledOff(ledBtnGreen);
				}
				switchState = currentSwitchState;
			}

			if (currentBtnState != btnState) {
				btnState = currentBtnState;
				if (btnState == LOW) {
					firstHeld = 0;
					myLCD->setCursor(0, 3);
					myLCD->print(printf("%-20s", "Awaiting disarm.."));
				}
			}

			if (switchState == HIGH && btnState == HIGH) {
				if (firstHeld == 0) {
					firstHeld = millis();
					myLCD->setCursor(0, 3);
					myLCD->print(printf("%-20s", "Disarming..."));
				}

				millisHeld = (millis() - firstHeld);
				secsHeld = millisHeld / 1000;
				if (!defused) {
					myLCD->setCursor(13, 3);
					myLCD->print(printf("%2d", 30 - secsHeld));
				}

				if (secsHeld > prevSecsHeld && secsHeld < 31) {
					blinkLed(ledGreen);
					playTone(NOTE_C6, 200);
				}
				if (secsHeld == 30) {
					clearLine(1);
					clearLine(3);
					myLCD->setCursor(0, 2);
					myLCD->print("   Bomb  disarmed   ");
					defused = true;

					missionEnd();
				}

				prevSecsHeld = secsHeld;
			}
			if (btnState == LOW && prevBtnState != btnState) {
				firstHeld = 0;
				myLCD->setCursor(0, 3);
				myLCD->print("= READY TO DISARM  =");
			}
		}
	}
}

