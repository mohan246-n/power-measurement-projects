# Power Measurement Projects

A collection of 5 practical, beginner-friendly Arduino-based power
measurement projects, built for learning, home use, and portfolio /
college work. Each project lives in its own folder with its own
README, working Arduino code (`.ino`), and a text circuit diagram.

## Projects

| # | Project | What it does |
|---|---------|--------------|
| 1 | [digital-voltmeter](digital-voltmeter/) | Measures 0–25 V DC with a voltage divider and shows the reading on a 16×2 LCD. |
| 2 | [digital-ammeter](digital-ammeter/) | Measures DC current with an ACS712 sensor and shows the reading on a 16×2 LCD. |
| 3 | [energy-meter-display](energy-meter-display/) | Measures voltage and current together, shows power (W) and accumulated energy (kWh) on a 16×2 LCD. |
| 4 | [battery-capacity-tester](battery-capacity-tester/) | Discharges a low-voltage battery through a known load, logs current over time, and calculates its real capacity in mAh. |
| 5 | [ac-voltage-detector](ac-voltage-detector/) | Detects AC voltage with an isolated ZMPT101B-style module and warns with a buzzer and LED. |

## How to use

1. Pick a project folder and read its `README.md` first — it lists the
   exact components, wiring, and safety notes for that project.
2. Open the `.ino` file in the Arduino IDE (or Arduino CLI / VS Code
   with the Arduino extension).
3. Select your board (Arduino Uno unless the project says otherwise) and
   the correct port, then upload.
4. Wire the circuit exactly as described in the project README.
   **Do not power the circuit until you have double-checked the wiring.**
5. Open the Serial Monitor at 9600 baud (where the project uses it)
   to see readings and status messages.

## Common components and tools

- Arduino Uno (all 5 projects)
- Breadboard, jumper wires, USB cable
- 16×2 LCD with I2C backpack (projects 1, 2, 3)
- Resistors: 30 kΩ, 7.5 kΩ for voltage dividers (see each project)
- ACS712 current sensor module — 5 A version (projects 2, 3)
- ZMPT101B AC voltage sensor module, isolated (project 5)
- Relay module / MOSFET, load resistor, buzzer, LEDs (projects 4, 5)
- Multimeter (for calibration — essential for every project here),
  wire stripper, screwdriver, soldering iron (optional)

## ⚠️ Safety warnings — read before building anything

- **Mains voltage (230 V AC in India / 110–240 V AC elsewhere) can kill.**
  Only project 5 goes anywhere near AC, and it does so only through an
  enclosed, isolated sensor module. Never connect mains directly to
  an Arduino, a breadboard, or a voltage divider.
- The voltage-divider and ACS712 projects (1, 2, 3) in this repo are
  for **low-voltage DC only** (25 V DC maximum). Never use them on
  AC mains — they are not isolated and not rated for it.
- If you are not confident working near mains wiring, get a qualified
  electrician to wire the AC side of project 5, or test it only on a
  low-voltage AC source such as a 12 V transformer secondary.
- Never touch sensor terminals or load wires while the circuit is
  powered. Always switch off / unplug before changing wiring.
- Check every connection with a multimeter before first power-up.
  Reversed polarity on the battery project can damage the Arduino,
  the load, and the battery.
- The battery capacity tester (project 4) is for **single low-voltage
  batteries / packs only** (e.g. one 18650 cell or a 12 V pack, as
  described there). Use a fuse in series, never short a battery, and
  never leave a discharging battery unattended.
- These are learning / indication instruments, not certified meters.
  Do not use their readings for billing, protection, or safety
  decisions.

## Repo layout

```
power-measurement-projects/
├── README.md
├── LICENSE
├── .gitignore
├── digital-voltmeter/
├── digital-ammeter/
├── energy-meter-display/
├── battery-capacity-tester/
└── ac-voltage-detector/
```

## License

MIT — see [LICENSE](LICENSE).
