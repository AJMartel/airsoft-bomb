#ifndef __Bomb_h__
#define __Bomb_h__

	#include <Arduino.h>
	#include <stdarg.h>
	#include <LiquidCrystal_I2C.h>
	#include <Tone.h>
	#include <Keypad.h>
	#include <Key.h>
	#include <Gamemode.h>
	#include <Bounce2.h>
	#include <AnalogMatrixKeypad.h>

	#define pound 14
	#define SW_IO_PIN 10

	#define LED_ON		0
	#define LED_OFF		1

	#define NO_KEY KEY_NOT_PRESSED
	#define DEBOUNCE 5

	#define analogPinKeyboard 3

	#define ledRed 2
	#define ledYellow 3
	#define ledGreen 4

	#define mSwitchRed 9
	#define btnRed 8
	#define keySwitch 5
	#define btnGreen 6
	#define mSwitchGreen 7

	#define buzzer 10

	#define ledBtnRed 11
	#define ledBtnGreen 12

	#define sInterval 1000

	//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

	extern LCD *myLCD;
	extern Tone *myTone;
	extern Gamemode *currentGameMode;

	extern char *lnBuffer;

	extern Bounce debounceMSwitchRed;
	extern Bounce debounceMSwitchGreen;
	extern Bounce debounceButtonRed;
	extern Bounce debounceButtonGreen;
	extern Bounce debounceKeySwitch;

	extern unsigned long currentMillis;

	char getKey();
	void playTone(int note, int time);
	int keyToInt(char key);
	int ledIndex(int led);
	void ledOn(int led);
	void ledOff(int led);
	void blinkLed(int led, int time);
	void blinkLed(int led);
	void blinkLCD(int time);
	void blinkLCD();
	void indicateInput();
	void indicateCancel();
	void indicateSelect();
	void indicateError();
	void clearLCD();
	void clearLine(int ln);
	int setCountdownTime(char value[4]);
	int getNumber(int lcdLine, int lcdPos, int maxLength);
	int waitForSingleKeyInput(char message[20]);
	String displayTime(int time);
	void missionEnd();
	void waitForGameStart();

	#include "Gamemode.h"
	#include "Rush.h"
	#include "Defuse.h"
	#include "CodeDefuse.h"

#endif
