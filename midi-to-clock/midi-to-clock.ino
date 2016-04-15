// midi-to-clock

// Pins
int clockOutput1 = 6;
int clockOutput2 = 7;
int divisionSwitch1 = 2;
int divisionSwitch2 = 4;

bool isPlaying = false;

int clockDivision = 0;

bool divisionSwitch1State = false;
bool divisionSwitch2State = false;

void updateDivisionMode() {
  divisionSwitch1State = digitalRead(divisionSwitch1);
  divisionSwitch2State = digitalRead(divisionSwitch2);
  if (divisionSwitch1State && divisionSwitch2State) clockDivision = 6; // up - sixteenth
  else if (divisionSwitch2State) clockDivision = 12; // middle - eigth
  else clockDivision = 24; // down - quarter
}

int midiClockCount = 0;
unsigned long lastClock1Millis = 0;
unsigned long lastClock2Millis = 0;

bool clockOutput1State = false;
bool clockOutput2State = false;

void processClock() {
  if (midiClockCount % clockDivision == 0) {
    digitalWrite(clockOutput1, HIGH);
    clockOutput1State = true;
    lastClock2Millis = millis();
  }

  if (midiClockCount % 24 == 0) {
    digitalWrite(clockOutput2, HIGH);
    clockOutput2State = true;
    lastClock2Millis = millis();
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
  if (currentMillis - lastClock1Millis >= 5 && clockOutput1State) {
    digitalWrite(clockOutput1, LOW);
    clockOutput1State = false;
  }
  if (currentMillis - lastClock2Millis >= 5 && clockOutput2State) {
    digitalWrite(clockOutput2, LOW);
    clockOutput2State = false;
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

