/*
  Energy Meter Display — V, A, W and kWh (DC)
  ---------------------------------------------
  Combines the voltmeter and ammeter projects: a voltage
  divider measures the supply voltage, an ACS712 measures
  the load current, and the Arduino calculates power and
  accumulates energy over time with millis() integration.
  Voltage, current, power and kWh are shown on a 16x2 I2C
  LCD (alternating screens) and on the Serial Monitor.

  Wiring:
    Voltage input+ : 30k - A0 - 7.5k - GND
    ACS712 OUT     : A1, VCC 5V, GND GND
    Load current   : supply+ - ACS712 IP+ / IP- - load
    LCD I2C        : SDA A4, SCL A5 (Uno)
  Range: low-voltage DC only (0-25 V). Never connect to
  AC mains — this build is not isolated.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---- Pin definitions ----
const int VOLTAGE_PIN = A0;  // Voltage divider output
const int CURRENT_PIN = A1;  // ACS712 analogue output

// ---- Calibration ----
const float DIVIDER_RATIO        = 5.0;  // (30k + 7.5k) / 7.5k
const float SENSITIVITY_MV_PER_A = 185.0; // ACS712 5 A version
const float ADC_REF              = 5.0;  // Arduino ADC reference (V)
const int   SAMPLES              = 10;   // Readings to average

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to 0x3F if needed

float zeroVoltage  = 2.5;   // ACS712 zero, calibrated at startup
float energyKwh    = 0.0;   // Accumulated energy
unsigned long lastMillis = 0;

float readPinVoltage(int pin) {
  long total = 0;
  for (int i = 0; i < SAMPLES; i++) {
    total += analogRead(pin);
    delay(2);
  }
  float average = total / (float)SAMPLES;
  return average * ADC_REF / 1023.0;
}

float readVoltage() {
  return readPinVoltage(VOLTAGE_PIN) * DIVIDER_RATIO;
}

float readCurrent() {
  float voltage = readPinVoltage(CURRENT_PIN);
  float current = (voltage - zeroVoltage) /
                  (SENSITIVITY_MV_PER_A / 1000.0);
  if (abs(current) < 0.05) current = 0.0;
  return current;
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Energy Meter");
  lcd.setCursor(0, 1);
  lcd.print("Calibrating zero");
  delay(500);

  // Zero calibration: keep the load OFF for this.
  zeroVoltage = readPinVoltage(CURRENT_PIN);
  lastMillis = millis();

  lcd.clear();
  Serial.println("Energy Meter ready.");
}

void loop() {
  float voltage = readVoltage();
  float current = readCurrent();
  float power   = voltage * current;  // Watts

  // Energy integration: E (kWh) += P (kW) x time (hours).
  unsigned long now = millis();
  float hours = (now - lastMillis) / 3600000.0;
  energyKwh += (power / 1000.0) * hours;
  lastMillis = now;

  // LCD screen 1: voltage and current.
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(voltage, 1);
  lcd.print("  A:");
  lcd.print(current, 2);
  lcd.print("   ");
  // LCD screen 1, line 2: power.
  lcd.setCursor(0, 1);
  lcd.print("P:");
  lcd.print(power, 1);
  lcd.print(" W       ");
  delay(1500);

  // LCD screen 2: accumulated energy.
  lcd.setCursor(0, 0);
  lcd.print("Energy:         ");
  lcd.setCursor(0, 1);
  lcd.print(energyKwh, 4);
  lcd.print(" kWh     ");

  // Serial output.
  Serial.print("V: ");  Serial.print(voltage, 2);
  Serial.print(" V  A: "); Serial.print(current, 2);
  Serial.print(" A  P: "); Serial.print(power, 1);
  Serial.print(" W  E: "); Serial.print(energyKwh, 4);
  Serial.println(" kWh");

  delay(1500);
}
