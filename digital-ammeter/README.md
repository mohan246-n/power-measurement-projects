# Digital Ammeter

A simple DC ammeter built with an Arduino, an ACS712 current
sensor, and a 16×2 LCD. It measures current in a low-voltage DC
circuit and displays the reading to two decimal places.

## How it works

The load current passes through the ACS712 module's screw
terminals — not through the Arduino. The sensor measures the
magnetic field from that current and outputs an analogue
voltage: 2.5 V at zero current, rising (or falling) by
185 mV per amp on the 5 A version. The Arduino reads that
voltage, subtracts the zero point, divides by the sensitivity,
and prints the current on the LCD and the Serial Monitor.
Readings are averaged over several samples, and the zero point
is measured at startup so small offsets do not show as a
false current.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| Current sensor | ACS712 module, 5 A version (185 mV/A) | 1 |
| LCD | 16×2 with I2C backpack | 1 |
| DC supply + load | e.g. 12 V supply and a lamp / motor to measure | 1 |
| Breadboard, jumper wires | — | — |

## Circuit diagram (text)

```
DC supply (+) ── ACS712 IP+ terminal
                 ACS712 IP- terminal ── Load (+) 
                 Load (-) ── DC supply (-)  (common GND)

ACS712 VCC ── Arduino 5V
ACS712 GND ── Arduino GND
ACS712 OUT ── Arduino A0

Arduino SDA (A4) ── LCD SDA
Arduino SCL (A5) ── LCD SCL
Arduino 5V / GND ── LCD VCC / GND
```

The ACS712 must be in series with the load, like a normal
ammeter. If the reading sign is reversed, swap IP+ and IP-.

## Wiring steps

1. Wire the ACS712 in series on the positive side of the load:
   supply + → IP+, IP- → load +, load − → supply −.
2. Connect the sensor: VCC → 5 V, GND → GND, OUT → A0.
3. Connect the LCD: SDA → A4, SCL → A5, VCC → 5 V, GND → GND.
4. Keep the load switched OFF, then upload
   `digital-ammeter.ino` — zero is calibrated at startup.
5. Switch the load on and compare the LCD with a multimeter
   in series.

## Calibration

1. With no load current, restart the Arduino. The startup
   reading is stored as the zero point (`zeroVoltage`).
2. Run a known current and measure it with a trusted
   multimeter in series with the load.
3. If the LCD differs, adjust `SENSITIVITY_MV_PER_A` in the
   code — 185 for the 5 A ACS712, 100 for 20 A, 66 for 30 A —
   and fine-tune it until the readings agree.
4. Small flicker around zero is normal sensor noise; the code
   hides anything below `ZERO_DEADBAND_A` (0.05 A).

## AC / DC note

This build is a **DC / simple version**: it averages the
sensor output, which is correct for steady DC current. It
does not calculate true RMS, so do not use it to report AC
current values — an AC waveform averages to about zero here.

## Safety notes

- **Low-voltage DC only.** Never run mains current through
  this basic breadboard build. ACS712 modules are sometimes
  advertised for AC use, but that needs an enclosed, properly
  insulated installation done by a qualified electrician.
- Do not exceed the sensor's rating (5 A on the 5 A version) —
  the terminals and tracks will overheat.
- Always switch the supply off before changing the series
  wiring, and check polarity with a multimeter first.
