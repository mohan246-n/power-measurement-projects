/*
  Digital Ammeter — DC current with ACS712
  ------------------------------------------
  An ACS712 Hall-effect sensor measures the current flowing
  through its screw terminals and outputs an analogue voltage
  centred on 2.5 V (zero current). The Arduino converts that
  voltage back to current and shows it on a 16x2 I2C LCD.
  Zero is calibrated automatically at startup (no load).

  Wiring:
    ACS712 OUT : A0
    ACS712 VCC : 5V, ACS712 GND: GND
    Load current: supply+ - ACS712 IP+ / IP- - load - supply-
    LCD I2C    : SDA A4, SCL A5 (Uno)
  Range: DC only, low voltage. Never put mains through this
  basic build — see the README safety notes first.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---- Pin definitions ----
const int CURRENT_PIN = A0;  // ACS712 analogue output

// ---- Calibration ----
// ACS712 5 A version: 185 mV per amp, zero current = 2.5 V.
// For the 20 A version use 100, for the 30 A version use 66.
const float SENSITIVITY_MV_PER_A = 185.0;
const float ADC_REF              = 5.0;  // Arduino ADC reference (V)
const int   SAMPLES              = 20;   // Readings to average
const float ZERO_DEADBAND_A      = 0.05; // Hide noise around zero

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Change 0x27 to 0x3F if needed

float zeroVoltage = 2.5;  // Measured at startup with no load current

float readPinVoltage() {
  long total = 0;
  for (int i = 0; i < SAMPLES; i++) {
    total += analogRead(CURRENT_PIN);
    delay(2);
  }
  float average = total / (float)SAMPLES;
  return average * ADC_REF / 1023.0;
}

float readCurrent() {
  float voltage = readPinVoltage();
  float current = (voltage - zeroVoltage) /
                  (SENSITIVITY_MV_PER_A / 1000.0);
  if (abs(current) < ZERO_DEADBAND_A) current = 0.0;
  return current;
}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.print("Digital Ammeter");
  lcd.setCursor(0, 1);
  lcd.print("Calibrating zero");
  delay(500);

  // Zero calibration: keep the load OFF / disconnected for this.
  zeroVoltage = readPinVoltage();

  lcd.clear();
  Serial.print("Zero voltage calibrated: ");
  Serial.print(zeroVoltage, 3);
  Serial.println(" V");
  Serial.println("Digital Ammeter ready.");
}

void loop() {
  float current = readCurrent();

  // LCD output.
  lcd.setCursor(0, 0);
  lcd.print("Current:      ");
  lcd.setCursor(0, 1);
  lcd.print(current, 2);
  lcd.print(" A DC     ");

  // Serial output.
  Serial.print("Current: ");
  Serial.print(current, 2);
  Serial.println(" A");

  delay(500);
}
