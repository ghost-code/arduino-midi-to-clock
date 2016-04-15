// midi-to-clock

// Pins
int clockOutput1 = 6;
int clockOutput2 = 7;
int divisionSwitch1 = 2;
int divisionSwitch2 = 4;

bool isPlaying = false;

int clockDivision = 0;

bool divisionSwitchState1 = false;
bool divisionSwitchState2 = false;

void updateDivisionMode() {
  divisionSwitchState1 = digitalRead(divisionSwitch1);
  divisionSwitchState2 = digitalRead(divisionSwitch2);
  if (divisionSwitchState1 && divisionSwitchState2) clockDivision = 6; // up - sixteenth
  else if (divisionSwitchState2) clockDivision = 12; // middle - eigth
  else clockDivision = 24; // down - quarter
}

int midiClockCount = 0;
unsigned long lastClockMillis1 = 0;
unsigned long lastClockMillis2 = 0;

void processClock() {
  if (midiClockCount % clockDivision == 0) {
    digitalWrite(clockOutput1, HIGH);
	lastClockMillis1 = millis();
  }
  // reset clock count every whole
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
}

byte data;

void processMIDI() {
  while (Serial.available() > 0) {
    data = Serial.read();
    if (data == 250) startClock();
    else if (data == 252) stopClock();
    else if (data == 251) continueClock();
    else if ((data == 248) && isPlaying) processClock();
  }
}

unsigned long currentMillis = 0;

void updateClockOutputs() {
	currentMillis = millis();
	if (currentMillis - lastClockMillis1 >= 10) {
		pinMode(clockOutput1, LOW);
	}
	if (currentMillis - lastClockMillis2 >= 10) {
		pinMode(clockOutput2, LOW);
	}
}

void setup() {
  Serial.begin(31250);
  pinMode(clockOutput1, OUTPUT);
  pinMode(clockOutput2, OUTPUT);
  pinMode(divisionSwitch1, INPUT);
  pinMode(divisionSwitch2, INPUT);
}

void loop() {
  updateDivisionMode();
  processMIDI();
  updateClockOutputs();
}


