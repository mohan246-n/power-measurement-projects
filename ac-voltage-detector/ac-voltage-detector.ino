/*
  AC Voltage Detector — isolated module, buzzer + LED
  ------------------------------------------------------
  A ZMPT101B-style AC voltage sensor module measures AC
  voltage through its own isolation transformer and outputs
  a small AC signal centred on 2.5 V. The Arduino samples
  that signal many times, measures its peak deviation from
  the centre, and if it crosses the threshold it warns with
  a buzzer and an LED. Buzzer and LED are OFF on startup.

  Wiring:
    ZMPT101B OUT : A0
    ZMPT101B VCC : 5V, GND: GND
    Buzzer       : D8 (active buzzer, + to D8, - to GND)
    Alarm LED    : D13 (through a 220 ohm resistor to GND)
    Module AC in : to the AC source ONLY as the module
                   manual describes, in an enclosure.
  NEVER connect mains directly to the Arduino, a divider,
  or a breadboard. The module's isolation is the only thing
  between mains and the Arduino — see the README.
*/

// ---- Pin definitions ----
const int AC_PIN     = A0;  // ZMPT101B module analogue output
const int BUZZER_PIN = 8;   // Buzzer
const int ALARM_LED  = 13;  // Alarm LED

// ---- Thresholds ----
// The module output sits at ~2.5 V with no AC. Its peak
// swing grows with the AC voltage. Calibrate AC_THRESHOLD
// from Serial readings (see README) for your module/source.
const float CENTRE_VOLTAGE   = 2.5;  // Module output at zero AC
const float AC_THRESHOLD     = 0.30; // Peak swing (V) = AC present
const float CLEAR_THRESHOLD  = 0.20; // Below this = AC clear
const float ADC_REF          = 5.0;  // Arduino ADC reference (V)
const int   SAMPLE_COUNT     = 200;  // Fast samples per reading

bool acPresent = false;

float readPeakSwing() {
  float highest = CENTRE_VOLTAGE;
  float lowest  = CENTRE_VOLTAGE;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    float voltage = analogRead(AC_PIN) * ADC_REF / 1023.0;
    if (voltage > highest) highest = voltage;
    if (voltage < lowest)  lowest  = voltage;
  }
  // Peak deviation from the centre, whichever side is larger.
  float up   = highest - CENTRE_VOLTAGE;
  float down = CENTRE_VOLTAGE - lowest;
  return up > down ? up : down;
}

void setAlarm(bool on) {
  acPresent = on;
  digitalWrite(BUZZER_PIN, on ? HIGH : LOW);
  digitalWrite(ALARM_LED, on ? HIGH : LOW);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(ALARM_LED, OUTPUT);

  // Safe defaults: buzzer and LED OFF on startup.
  setAlarm(false);

  Serial.begin(9600);
  Serial.println("AC Voltage Detector ready.");
  Serial.println("Warning: mains only via the isolated module.");
}

void loop() {
  float swing = readPeakSwing();

  // Hysteresis: ON above AC_THRESHOLD, OFF below CLEAR.
  if (!acPresent && swing > AC_THRESHOLD) {
    setAlarm(true);
    Serial.println("AC VOLTAGE DETECTED");
  } else if (acPresent && swing < CLEAR_THRESHOLD) {
    setAlarm(false);
    Serial.println("AC clear.");
  }

  Serial.print("Peak swing: ");
  Serial.print(swing, 2);
  Serial.print(" V  AC: ");
  Serial.println(acPresent ? "DETECTED" : "none");

  delay(250);
}
