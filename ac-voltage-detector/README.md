# AC Voltage Detector

An AC voltage detector built with an Arduino, an isolated
ZMPT101B-style voltage sensor module, a buzzer, and an LED.
When AC voltage is present at the module input, the buzzer
sounds and the LED lights — a warning indicator, in the
same style as a non-contact voltage tester pen.

## How it works

The ZMPT101B module contains a small voltage transformer.
That transformer is what isolates the mains side from the
Arduino side — the Arduino never touches mains. The module
outputs a small AC signal centred on 2.5 V, whose size grows
with the AC voltage at its input. The Arduino samples the
output many times in quick succession, finds the peak swing
away from 2.5 V, and compares it with a threshold. Above
the threshold the buzzer and LED turn on; a lower clear
threshold turns them off again, so the alarm does not
flicker at the switching point.

## Components

| Component | Value / spec | Qty |
|-----------|--------------|-----|
| Arduino Uno | — | 1 |
| AC voltage sensor | ZMPT101B module (isolated, with on-board op-amp) | 1 |
| Buzzer | Active buzzer, 5 V | 1 |
| LED + resistor | LED with 220 Ω resistor | 1 |
| Enclosure | Insulated box for the module and AC connections | 1 |
| Breadboard, jumper wires | For the low-voltage side only | — |

## Circuit diagram (text)

```
AC source ── ZMPT101B module AC input terminals
             (inside an insulated enclosure ONLY)

ZMPT101B VCC ── Arduino 5V
ZMPT101B GND ── Arduino GND
ZMPT101B OUT ── Arduino A0

Arduino D8  ── Buzzer (+)     Buzzer (-) ── GND
Arduino D13 ── 220 Ω ── Alarm LED ── GND

NO connection of any kind between the AC side and the
Arduino / breadboard, except through the module itself.
```

## Wiring steps

1. **Build and test the low-voltage side first, with no AC
   connected:** module VCC → 5 V, GND → GND, OUT → A0,
   buzzer → D8, LED → D13 through 220 Ω.
2. Upload `ac-voltage-detector.ino` and watch Serial — the
   peak swing should sit close to 0.00 V with no AC input.
3. Only then, with everything unplugged, wire the AC source
   to the module's AC input terminals exactly as the module
   manual describes, inside the insulated enclosure.
4. Close the enclosure before applying AC power. Test first
   on a low-voltage AC source (e.g. a 12 V transformer
   secondary) if one is available.

## Calibration

1. With no AC at the module, note the peak swing on Serial.
2. Apply a known AC voltage and note the new peak swing.
3. Set `AC_THRESHOLD` in the code about halfway between
   the two readings (default 0.30 V). `CLEAR_THRESHOLD`
   should stay a little below it.
4. The module has a small trim potentiometer for gain —
   adjust it so a full mains input does not clip the
   output at 0 V / 5 V. This is an indication project,
   not a calibrated AC voltmeter.

## Safety notes

- **Mains voltage can kill.** Mains must only ever reach
  the input terminals of the isolated sensor module,
  inside an enclosure. Never connect mains directly to
  an Arduino, a voltage divider, or a breadboard — the
  other projects in this repo are DC only for this reason.
- If you are not fully confident with mains wiring, get a
  qualified electrician to wire the module's AC side, or
  build and demonstrate the project on low-voltage AC only.
- Never open the enclosure or touch the module's AC
  terminals while it is powered. Unplug at the wall first.
- This is a warning / indication device, not a safety
  tester. Never use it to prove a circuit is dead before
  touching it — use a certified tester and lock-out rules.
