#include <Bounce2.h>
#include <AnalogMatrixKeypad.h>
#include <Wire.h>
#include <FastIO.h>
#include <I2CIO.h>
#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal_SR.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR3W.h>
#include <Key.h>
#include <TimeLib.h>
#include <Time.h>
#include <Tone.h>
#include <stdarg.h>

#include "Bomb.h"
#include "Gamemode.h"
#include "Rush.h"
#include "Defuse.h"

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // 0x27 is the I2C bus address for an unmodified backpack

boolean switch_LED_State = false;

char *lnBuffer;

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

Tone *myTone = new Tone();
LCD *myLCD = &lcd;
AnalogMatrixKeypad AnMatrixKeypad(analogPinKeyboard);

const long secMillis = 0; // store last time for second add
unsigned long currentMillis;


char *gameModes[] = {
	"Rush"
};
int gameModeMax = 1;

Gamemode* currentGameMode = NULL;

Bounce debounceMSwitchRed = Bounce();
Bounce debounceMSwitchGreen = Bounce();
Bounce debounceButtonRed = Bounce();
Bounce debounceButtonGreen = Bounce();
Bounce debounceKeySwitch = Bounce();

int ledPin[] = { ledRed, ledYellow, ledGreen, ledBtnGreen, ledBtnRed };
unsigned long ledTimer[] = { 0, 0, 0, 0, 0 };
const int ledArraySize = sizeof ledPin;
unsigned long lcdTimer = 0;

void setup()
{
	for (int i = 0; i < 5; i++)
	{
		pinMode(ledPin[i], OUTPUT);
		ledOff(ledPin[i]);
	}
	
	pinMode(mSwitchGreen, INPUT);
	pinMode(mSwitchRed, INPUT);
	pinMode(btnGreen, INPUT);
	pinMode(btnRed, INPUT);
	pinMode(keySwitch, INPUT);

	debounceMSwitchGreen.attach(mSwitchGreen);
	debounceMSwitchGreen.interval(DEBOUNCE);
	debounceMSwitchRed.attach(mSwitchRed);
	debounceMSwitchRed.interval(DEBOUNCE);
	debounceButtonRed.attach(btnRed);
	debounceButtonRed.interval(DEBOUNCE);
	debounceButtonGreen.attach(btnGreen);
	debounceButtonGreen.interval(DEBOUNCE);
	debounceKeySwitch.attach(keySwitch);
	debounceKeySwitch.interval(DEBOUNCE);

	Serial.begin(9600);
	
	myLCD->begin(20, 4);
	myTone->begin(10);
	 
	clearLCD();
	myLCD->backlight();
	myLCD->print("S.H.A.T Bomb");
	myLCD->setCursor(0, 1);
	myLCD->print("v 1.0.0");
	myLCD->setCursor(0, 2);
	myLCD->print(NO_KEY);
	myLCD->setCursor(2, 2);
	myLCD->print(0);
	
	for (int i = 0; i< 3; i++)
	{
		myLCD->noBacklight();
		delay(250);
		myLCD->backlight();
		delay(250);
	}
	myLCD->backlight();
	
	myLCD->home();

	delay(1000);

	//setGMPassword();
	//delay(3000);

	Gamemode::SelectionMenu();

	currentGameMode->Setup();

	clearLine(1);
	clearLine(2);
	clearLine(3);
}

void loop()
{
	debounceMSwitchGreen.update();
	debounceMSwitchRed.update();
	debounceButtonRed.update();
	debounceButtonGreen.update();
	debounceKeySwitch.update();

	currentMillis = millis();
	for (int i = 0; i < ledArraySize; i++) {
		if (ledTimer[i] > 0 && ledTimer[i] < currentMillis)
		{
			ledOff(ledPin[i]);
			ledTimer[i] = 0;
		}
	}
	if (lcdTimer > 0 && lcdTimer < currentMillis) {
		myLCD->backlight();
	}

	currentGameMode->Loop();
}

void playTone(int note, int time) {
	myTone->play(note, time);
}
int ledIndex(int led) {
	for (int i = 0; i < ledArraySize; i++) {
		if (ledPin[i] == led)
			return i;
	}
	return -1;
}
void ledOn(int led) {
	digitalWrite(led, LOW);
}
void ledOff(int led) {
	digitalWrite(led, HIGH);
}
void blinkLed(int led, int time) {
	ledOn(led);
	ledTimer[ledIndex(led)] = millis() + time;
}
void blinkLed(int led) {
	blinkLed(led, 100);
}
void blinkLCD(int time) {
	myLCD->noBacklight();
	lcdTimer = millis() + time;
}
void blinkLCD() {
	blinkLCD(100);
}
void indicateInput() {
	playTone(NOTE_C6, 200);
	blinkLed(ledYellow);
}
void indicateCancel() {
	playTone(NOTE_C4, 200);
	blinkLed(ledYellow);
}
void indicateError() {
	playTone(NOTE_C6, 200);
	blinkLed(ledRed);
	playTone(NOTE_C4, 200);
}
void indicateSelect() {
	//blinkLCD(100);
	playTone(NOTE_B6, 200);
	blinkLed(ledGreen);
}
void clearLCD() {
	myLCD->clear();
	myLCD->home();
}
void clearLine(int ln) {
	myLCD->setCursor(0, ln);
	myLCD->print("                    ");
}
int keyToInt(char key) {
	if ((key != '*') && (key != '#'))
	{
		return key - 48;
	}
}
int waitForSingleKeyInput(char message[20]) {
	int inputValue = 0;
	int returnValue = 0;
	while (returnValue < 1)
	{
		char key = getKey();
		if (key != NO_KEY)
		{
			if (key == '0') {
				indicateError();
			}
			else if (key == '*') {
				inputValue = 0;
				myLCD->setCursor(18, 2);
				myLCD->print(" ");
				indicateCancel();
			}
			else if (key == '#') {
				if (inputValue > 0) {
					returnValue = inputValue;
					myLCD->setCursor(0, 2);
					myLCD->print("== Base no. set   ==");
					myLCD->noCursor();
					indicateSelect();
				}
				else {
					indicateError();
				}
			}
			else {
				indicateInput();
				inputValue = keyToInt(key);
				myLCD->setCursor(18, 2);
				myLCD->print(inputValue);
			}
		}
	}
	return returnValue;
}

int getNumber(int lcdLine, int lcdPos, int maxLength)
{
	int num = 0;
	int length = 0;
	char key = getKey();
	while (key != '#')
	{
		switch (key)
		{
		case NO_KEY:
			break;

		case '*':
			num = 0;
			length = 0;
			myLCD->setCursor(lcdPos, lcdLine);
			myLCD->cursor();
			for (int i = 0; i < maxLength; i++)
			{
				myLCD->print(" ");
			}
			indicateCancel();
			break;

		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			if (length < maxLength)
			{
				num = num * 10 + (key = '0');
				myLCD->setCursor(lcdPos + length, lcdLine);
				myLCD->print(key);
				myLCD->cursor();
				length++;
				indicateInput();
			}
			break;

		default:
			break;
		}

		key = getKey();
	}
	indicateSelect();
	return num;
}

String displayTime(int time)
{
	int minutes = floor(time / 60);
	int seconds = (time - (minutes * 60));
	return String(minutes) + ":" + (seconds < 10 ? "0" : "") + String(seconds);
}

void loopRush() {
	myLCD->setCursor(10,2);
	myLCD->print(millis()/1000);
}

char getKey()
{
	return AnMatrixKeypad.readKey();
}

void missionEnd()
{
	int leds[3] = { ledGreen, ledYellow, ledRed };
	int notes[3] = { NOTE_C4, NOTE_C5, NOTE_C6 };

	ledOn(ledGreen);
	ledOff(ledYellow);
	ledOff(ledRed);
	int c = 0;
	int p = 0;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			ledOff(leds[p]);
			ledOn(leds[c]);
			playTone(notes[c], 200);
			c = (c == 2 ? 0 : c + 1);
			p = (c == 0 ? 2 : c - 1);
			delay(250);
		}
	}
}

void waitForGameStart()
{
	ledOn(ledBtnRed);
	myLCD->setCursor(0, 1);
	myLCD->print(printf("%-20s", " "));
	myLCD->setCursor(0, 2);
	myLCD->print(printf("%-20s", "Press red button"));
	myLCD->setCursor(0, 3);
	myLCD->print(printf("%-20s", "to start"));
	while ((int)debounceButtonRed.read() == LOW) {
		delay(100);
	}

	clearLine(1);
	clearLine(2);
	clearLine(3);
}