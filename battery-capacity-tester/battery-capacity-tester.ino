/*
  Battery Capacity Tester — mAh from a timed discharge
  ------------------------------------------------------
  A charged low-voltage battery is discharged through a
  known power load. A relay / MOSFET switches the load ON
  and OFF (safe default: OFF). The Arduino measures battery
  voltage (divider) and load current (ACS712), integrates
  current over time with millis() to get mAh, logs to
  Serial, and stops the test at the cutoff voltage.

  Wiring:
    Battery+ : fuse - load - ACS712 - MOSFET/relay - batt-
    Battery+ : 30k - A0 - 7.5k - GND (voltage sense)
    ACS712 OUT: A1, VCC 5V, GND GND
    Load switch: D7 (relay module IN / MOSFET gate)
    Status LED : D13 (through a 220 ohm resistor to GND)
    LCD I2C    : SDA A4, SCL A5 (Uno)
  Low voltage ONLY: one 18650 cell or a small 12 V pack.
  Never test mains-connected or damaged batteries.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---- Pin definitions ----
const int VOLTAGE_PIN = A0;  // Battery voltage divider output
const int CURRENT_PIN = A1;  // ACS712 analogue output
const int LOAD_PIN    = 7;   // Relay / MOSFET load switch
const int STATUS_LED  = 13;  // Test-running indicator

// ---- Calibration / test settings ----
// Set CUTOFF for YOUR battery: one 18650 cell 3.0 V,
// 12 V lead-acid pack 10.8 V (see README).
const float CUTOFF_VOLTAGE       = 3.0;
const float DIVIDER_RATIO        = 5.0;  // (30k + 7.5k) / 7.5k
const float SENSITIVITY_MV_PER_A = 185.0; // ACS712 5 A version
const float ADC_REF              = 5.0;
const int   SAMPLES              = 10;

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to 0x3F if needed

float zeroVoltage = 2.5;   // ACS712 zero, calibrated at startup
float capacityMah = 0.0;
bool  testRunning = false;
bool  testDone    = false;
unsigned long lastMillis = 0;

float readPinVoltage(int pin) {
  long total = 0;
  for (int i = 0; i < SAMPLES; i++) { total += analogRead(pin); delay(2); }
  return (total / (float)SAMPLES) * ADC_REF / 1023.0;
}
float readBatteryVoltage() { return readPinVoltage(VOLTAGE_PIN) * DIVIDER_RATIO; }
float readCurrent() {
  float current = (readPinVoltage(CURRENT_PIN) - zeroVoltage) /
                  (SENSITIVITY_MV_PER_A / 1000.0);
  return abs(current) < 0.05 ? 0.0 : current;
}
void setLoad(bool on) {
  testRunning = on;
  digitalWrite(LOAD_PIN, on ? HIGH : LOW);
  digitalWrite(STATUS_LED, on ? HIGH : LOW);
}

void setup() {
  pinMode(LOAD_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  setLoad(false);  // Safe default: load OFF on startup.
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Capacity Tester");
  lcd.setCursor(0, 1);
  lcd.print("Load OFF - safe");
  delay(1000);
  zeroVoltage = readPinVoltage(CURRENT_PIN);  // No load current
  lastMillis = millis();
  if (readBatteryVoltage() > CUTOFF_VOLTAGE + 0.2) {
    setLoad(true);
    Serial.println("Test started. Time(s), V, A, mAh");
  } else {
    Serial.println("No battery / battery below cutoff. Load stays OFF.");
  }
}

void loop() {
  if (testDone) { delay(1000); return; }
  float voltage = readBatteryVoltage();
  float current = testRunning ? readCurrent() : 0.0;
  unsigned long now = millis();
  float hours = (now - lastMillis) / 3600000.0;  // mAh integration step
  if (testRunning) capacityMah += current * hours * 1000.0;
  lastMillis = now;
  if (testRunning && voltage <= CUTOFF_VOLTAGE) {
    setLoad(false);
    testDone = true;
    lcd.clear();
    lcd.print("Test complete");
    lcd.setCursor(0, 1);
    lcd.print(capacityMah, 0);
    lcd.print(" mAh");
    Serial.print("TEST COMPLETE. Capacity: ");
    Serial.print(capacityMah, 0);
    Serial.println(" mAh");
    return;
  }
  lcd.setCursor(0, 0);
  lcd.print("V:"); lcd.print(voltage, 2);
  lcd.print(" A:"); lcd.print(current, 2); lcd.print("  ");
  lcd.setCursor(0, 1);
  lcd.print(capacityMah, 0); lcd.print(" mAh      ");
  Serial.print(now / 1000); Serial.print(", ");
  Serial.print(voltage, 2); Serial.print(", ");
  Serial.print(current, 2); Serial.print(", ");
  Serial.println(capacityMah, 0);
  delay(1000);
}
