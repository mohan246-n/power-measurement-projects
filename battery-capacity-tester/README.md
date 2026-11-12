# Battery Capacity Tester

A battery capacity tester built with an Arduino and a 16×2
LCD. It discharges a single low-voltage battery through a
known load, measures the current the whole time, and adds
it up to give the battery's real capacity in mAh — the
number that tells you whether an old cell is still good.

## How it works

A relay module (or a logic-level MOSFET) connects the load
to the battery — OFF on startup, so nothing discharges until
the code decides to start. Battery voltage is read through
a voltage divider (30 kΩ + 7.5 kΩ, ratio 5:1) and discharge
current is read with an ACS712 sensor. On every loop the
code uses `millis()` to integrate current over time:
mAh = current (A) × time (hours) × 1000. Voltage, current,
and mAh so far are shown on the LCD and logged to Serial
once a second, so the log can be copied into a spreadsheet.
When the battery falls to the cutoff voltage, the load is
switched OFF and the final capacity stays on the LCD.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Current sensor | ACS712 module, 5 A version (185 mV/A) | 1 |
| Load switch | 5 V relay module, or logic-level MOSFET (e.g. IRLZ44N) | 1 |
| Load | Power resistor, e.g. 10 Ω / 10 W for a single 18650 cell | 1 |
| LCD | 16×2 with I2C backpack | 1 |
| Resistors | 30 kΩ and 7.5 kΩ (divider), 220 Ω (LED) | 3 |
| Fuse + holder | 2–5 A, in series with the battery | 1 |
| Battery under test | One 18650 cell, or a small 12 V pack (see below) | 1 |
| Breadboard, jumper wires, LED | — | — |

## Circuit diagram (text)

```
Battery (+) ── Fuse ── Load resistor ── ACS712 IP+ / IP- ──┐
                                                            │
Battery (+) ── 30k resistor (R1) ──┬── A0 (Arduino)    Relay / MOSFET
                                    └── 7.5k (R2) ── GND    (switched by D7)
Battery (-) ─────────────────────── GND (common) ◄──────────┘

ACS712 VCC ── 5V    GND ── GND    OUT ── A1 (Arduino)
Arduino D7 ── Relay IN (or MOSFET gate via 100 Ω)
Arduino D13 ── 220 Ω ── Status LED ── GND
Arduino SDA (A4) / SCL (A5) ── LCD SDA / SCL, 5V / GND ── LCD
```

Size the load so the discharge current is sensible for the
battery — about 0.5–1 A for a single 18650 cell.

## Wiring steps

1. Build the voltage divider from battery + to A0, and
   connect battery − to Arduino GND.
2. Wire battery + → fuse → load → ACS712 → relay/MOSFET →
   battery −, so D7 controls the whole discharge path.
3. Connect ACS712 OUT → A1, relay IN → D7, LED → D13.
4. Connect the LCD: SDA → A4, SCL → A5, VCC → 5 V, GND → GND.
5. Set `CUTOFF_VOLTAGE` in the code for your battery (see
   below), upload, then connect a fully charged battery.

## Calibration

1. Set the cutoff before testing: **3.0 V for one 18650
   lithium cell, 10.8 V for a 12 V lead-acid pack.** Going
   below these damages the battery.
2. Calibrate the divider (`DIVIDER_RATIO`) and the ACS712
   (`SENSITIVITY_MV_PER_A`) against a multimeter, as in the
   voltmeter and ammeter projects — capacity is only as
   accurate as the current reading.
3. Cross-check the load current once with a multimeter in
   series at the start of the test.

## Safety notes

- **Low voltage only.** Test one cell or one small pack at
  a time. Never test a damaged, swollen, or hot battery —
  stop the test and isolate it if it heats up.
- The load resistor gets hot by design. Keep it clear of
  plastic, paper, and fingers, and allow for cooling.
- Use the series fuse, never short the battery, and never
  leave a discharging battery unattended.
- Discharging lithium cells below their cutoff damages
  them permanently and can make them unsafe to recharge.
