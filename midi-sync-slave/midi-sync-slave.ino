#include <TimerOne.h>

// Pins
int clockOutput1 = 6;
int clockOutput2 = 7;
int divisionInput1 = 2;
int divisionInput2 = 4;

bool isPlaying = false;

void setup() {
  Serial.begin(31250);
  pinMode(clockOutput1, OUTPUT);
  pinMode(clockOutput2, OUTPUT);
  pinMode(divisionInput1, INPUT);
  pinMode(divisionInput2, INPUT);
  updateDivisionMode();
}

bool switchState1 = false;
bool switchState2 = false;
int divisionMode = 0;

void updateDivisionMode() {
  switchState1 = digitalRead(divisionInput1);
  switchState2 = digitalRead(divisionInput2);
  if (switchState1 && switchState2) {
    // up - sixteenth
    divisionMode = 6;
  }
  else if (switchState2) {
    // middle - eigth
    divisionMode = 12;
  }
  else {
    // down - quarter
    divisionMode = 24;
  }
}

int midiClockCount = 0;

void processClock() {
  
  if (midiClockCount % divisionMode == 0) {
    digitalWrite(clockOutput1, HIGH);
  }
  else if (midiClockCount % divisionMode >= 1) {
    digitalWrite(clockOutput1, LOW);
  }
  // reset every whole
  midiClockCount = (midiClockCount + 1) % 96;
}

void startClock() {
  isPlaying = true;
  midiClockCount = 0;
}

void continueClock() {
  isPlaying = true;
}

void stopClock() {
  isPlaying = false;
  midiClockCount = 0;
  digitalWrite(clockOutput1, LOW);
  digitalWrite(clockOutput2, LOW);
}

byte data;

void loop() {
  while (Serial.available() > 0) {
    data = Serial.read();
    if (data == 250) startClock();
    else if (data == 252) stopClock();
    else if (data == 251) continueClock();
    else if ((data == 248) && isPlaying) processClock();
  }
  updateDivisionMode();
}


