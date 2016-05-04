// midi-to-clock

// Pins
#define CLOCK_OUTPUT_1 9
#define DIVISION_SWITCH_1 7
#define DIVISION_SWITCH_2 8

bool isPlaying = false;

uint8_t clockDivision = 24;

bool divisionSwitch1State = false;
bool divisionSwitch2State = false;

void updateDivisionMode() {
  divisionSwitch1State = digitalRead(DIVISION_SWITCH_1);
  divisionSwitch2State = digitalRead(DIVISION_SWITCH_2);
  if (divisionSwitch1State && divisionSwitch2State) clockDivision = 6; // up - sixteenth
  else if (divisionSwitch1State || divisionSwitch2State) clockDivision = 12; // middle - eigth
  else clockDivision = 24; // down - quarter
}

uint8_t midiClockCount = 0;
unsigned long lastClock1Millis = 0;

bool clockOutput1State = false;

void processClock() {
  if (midiClockCount % clockDivision == 0) {
    digitalWrite(CLOCK_OUTPUT_1, HIGH);
    clockOutput1State = true;
    lastClock1Millis = millis();
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

void updateClockOutput() {
  currentMillis = millis();
  if (currentMillis - lastClock1Millis >= 5 && clockOutput1State) {
    digitalWrite(CLOCK_OUTPUT_1, LOW);
    clockOutput1State = false;
  }
}

void setup() {
  Serial.begin(31250);
  pinMode(CLOCK_OUTPUT_1, OUTPUT);
  pinMode(DIVISION_SWITCH_1, INPUT);
  pinMode(DIVISION_SWITCH_2, INPUT);
}

void loop() {
  updateDivisionMode();
  processMIDI();
  updateClockOutput();
}

