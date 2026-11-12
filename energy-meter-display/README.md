# Energy Meter Display

A DC energy meter built with an Arduino and a 16×2 LCD.
It measures voltage and current at the same time, shows the
power in watts, and keeps a running total of energy used
in kilowatt-hours (kWh) — like a small, learning version of
an electricity meter, for low-voltage DC circuits.

## How it works

Voltage is measured with the same voltage divider as the
digital voltmeter project (30 kΩ + 7.5 kΩ, ratio 5:1), and
current is measured with an ACS712 sensor, as in the digital
ammeter project. Power is simply P = V × I. Energy is power
accumulated over time: on every loop the code uses `millis()`
to find how much time has passed, adds P × time to a running
total, and converts watt-hours to kWh. The LCD alternates
between a V / A / W screen and an energy screen.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Current sensor | ACS712 module, 5 A version (185 mV/A) | 1 |
| LCD | 16×2 with I2C backpack | 1 |
| Resistors | 30 kΩ (R1) and 7.5 kΩ (R2) — 1% tolerance preferred | 2 |
| DC supply + load | e.g. 12 V supply and a lamp / motor to measure | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
Supply (+) ──┬── 30k resistor (R1) ──┬── A0 (Arduino)
             │                        └── 7.5k resistor (R2) ── GND
             └── ACS712 IP+ terminal
                 ACS712 IP- terminal ── Load (+)
                 Load (-) ── Supply (-)  (common GND)

ACS712 VCC ── Arduino 5V    ACS712 GND ── Arduino GND
ACS712 OUT ── Arduino A1

Arduino SDA (A4) ── LCD SDA
Arduino SCL (A5) ── LCD SCL
Arduino 5V / GND ── LCD VCC / GND
```

## Wiring steps

1. Build the voltage divider: supply + → 30 kΩ → A0 node →
   7.5 kΩ → GND. Connect supply − to Arduino GND.
2. Wire the ACS712 in series with the load, as in the
   digital ammeter project, and its OUT pin to A1.
3. Connect the LCD: SDA → A4, SCL → A5, VCC → 5 V, GND → GND.
4. Keep the load OFF, upload `energy-meter-display.ino` —
   the ACS712 zero is calibrated at startup.
5. Switch the load on and check V and A against a multimeter.

## Calibration

1. Follow the voltmeter calibration for `DIVIDER_RATIO` and
   the ammeter calibration for `SENSITIVITY_MV_PER_A`.
2. Both calibrations affect the power and energy readings —
   a 5% error in each becomes about a 10% error in watts.
3. Energy total resets to zero whenever the Arduino is
   reset or loses power. It is not stored in EEPROM in this
   simple version.

## Safety notes

- **Low-voltage DC only, 0–25 V maximum.** Never connect
  this build to AC mains. It has no isolation and is not
  a certified energy meter — do not use it for billing.
- Do not exceed the ACS712 rating (5 A on the 5 A version).
- Always switch the supply off before changing wiring, and
  check every connection with a multimeter before power-up.
- Long energy runs mean a load left powered for hours —
  check the load, sensor, and wiring for heat regularly,
  and never leave it unattended.
