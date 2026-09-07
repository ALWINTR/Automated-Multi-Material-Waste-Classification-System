# Step-by-Step Implementation Guide: Smart Waste Segregation System

---

## Phase 1: Preparation & Pre-Assembly Verification

### 1.1 Tools & Materials Checklist
* **Tools**: Multimeter, small flathead screwdriver (for trimpots), soldering iron / wire stripper, hot glue gun / mounting screws, USB Type-B cable for Arduino.
* **Materials**: Arduino Uno, 12V 2A–3A DC Power Supply, LM2596 Buck Converter, NEMA 17 Stepper + A4988 Driver (with heatsink), SG90 / MG995 Servo, LJ12A3-4-Z/BX Inductive Proximity Sensor, IR Obstacle Sensor, Raindrop Sensor Module, 100µF electrolytic capacitor, jumper wires, breadboard / PCB.

---

## Phase 2: Power Rail Setup & Calibration (CRITICAL FIRST STEP)

> [!WARNING]
> Never connect the Arduino, Servo, or sensors to the Buck Converter until its output voltage is measured and set to 5.0V.

1. **Connect 12V Input**:
   - Connect the 12V power supply positive (+) to LM2596 `IN+`.
   - Connect the 12V power supply negative (-) to LM2596 `IN-`.
2. **Tune to Exactly 5.0V**:
   - Power ON the 12V adapter.
   - Place multimeter probes on `OUT+` and `OUT-` (DC Voltage mode).
   - Turn the brass adjustment screw on the blue potentiometer counter-clockwise until the meter reads **5.00V ± 0.05V**.
3. **Power OFF** the supply before continuing to wiring.

---

## Phase 3: Mechanical Assembly & Sensor Positioning

1. **Upper Sensing Platform / Chute**:
   - Fabricate an inclined slide or rectangular platform with a hinged trapdoor flap.
   - **IR Obstacle Sensor**: Mount horizontally at the top entrance of the chute.
   - **Inductive Proximity Sensor**: Mount underneath/flush with the floor of the trapdoor flap (max 4mm sensing distance for standard LJ12A3-4-Z/BX).
   - **Raindrop Sensor Grid**: Fix onto the surface of the trapdoor flap so wet objects make direct contact with the nickel traces.
2. **Servo Motor Flap Linkage**:
   - Fix the servo horn to the pivot hinge of the trapdoor flap.
   - Ensure `0°` keeps the flap completely closed, and `90°` swings it wide open to allow waste to fall freely.
3. **Lower Rotary Bin Carousel**:
   - Divide a round container or base plate into 3 equal 120° compartments (Bin 1: Metal, Bin 2: Wet, Bin 3: Dry).
   - Fasten the center of the carousel directly to the NEMA 17 stepper motor shaft with a 5mm mounting hub.
   - Position the carousel directly below the servo trapdoor.

---

## Phase 4: Circuit Wiring & A4988 Driver Tuning

### 4.1 Tune Stepper Driver Current Limit ($V_{\text{ref}}$)
* Formula for A4988: $V_{\text{ref}} = I_{\text{max}} \times 8 \times R_{\text{sense}}$ (usually $R_{\text{sense}} = 0.1\,\Omega$).
* For a 1.0A motor: Set $V_{\text{ref}} \approx 0.6\text{V} - 0.7\text{V}$ between the driver potentiometer wiper and GND using your multimeter.

### 4.2 Wiring Steps
1. **Common Ground**: Connect Arduino `GND`, 12V supply `GND`, LM2596 `OUT-`, and A4988 `GND` to a single common ground bus.
2. **Sensors**:
   - IR Sensor: `VCC` -> 5V, `GND` -> GND, `OUT` -> Arduino `D2`.
   - Inductive Sensor: `Brown` -> +12V, `Blue` -> GND, `Black` -> Arduino `D3`.
   - Raindrop Sensor: `VCC` -> 5V, `GND` -> GND, `DO` -> Arduino `D4`, `AO` -> Arduino `A0`.
3. **Actuators**:
   - Servo: `Red` -> 5V (Buck Converter), `Brown` -> GND, `Signal (Yellow)` -> Arduino `D9`.
   - A4988: `VMOT` & `GND` -> 12V (add 100µF capacitor across them), `VDD` & `GND` -> 5V / GND, `STEP` -> `D6`, `DIR` -> `D7`, `ENABLE` -> `D8`.
   - Short `RESET` and `SLEEP` pins together on the A4988.
   - Connect stepper 4-pin motor coil plug to `1A, 1B, 2A, 2B`.

---

## Phase 5: Software Upload & Calibration

1. Open **Arduino IDE**.
2. Connect Arduino Uno via USB.
3. Open `waste_segregation.ino`.
4. Select Board: **Arduino Uno** and your COM Port.
5. Click **Upload**.
6. Open **Serial Monitor** at **9600 baud**.

---

## Phase 6: Subsystem Testing & Tuning

### Test 1: IR Sensor Trigger
* Place an object on the chute.
* Verify the onboard LED on the IR module lights up and Serial Monitor outputs `[!] Object Detected`.
* Adjust the IR onboard potentiometer if it triggers prematurely or fails to detect.

### Test 2: Inductive Proximity (Metal)
* Place a metal can or steel bolt on the flap.
* Check that Serial Monitor displays `Inductive Sensor (Metal): YES` and category `METALLIC WASTE`.

### Test 3: Rain Sensor (Wet)
* Place a damp tissue or wet sponge on the sensor grid.
* Verify Serial Monitor displays `Rain Sensor: YES` and category `WET / ORGANIC WASTE`.
* Adjust the blue potentiometer on the rain module until the digital LED switches accurately.

### Test 4: Actuator Motion
* Verify the stepper rotates smoothly to $0^\circ$, $120^\circ$, or $240^\circ$ corresponding to the classified waste.
* Verify the servo opens the trapdoor for 1.2 seconds and returns to $0^\circ$.

---

## Phase 7: End-to-End Validation Run

| Test Waste Item | Expected Detection | Stepper Target | Servo Action |
| :--- | :--- | :--- | :--- |
| **Aluminum Soda Can** | Metal: YES | Bin 1 (0°) | Drops item into Bin 1 |
| **Wet Vegetable / Fruit Peel** | Metal: NO, Wet: YES | Bin 2 (120°) | Drops item into Bin 2 |
| **Dry Plastic Bottle / Paper** | Metal: NO, Wet: NO | Bin 3 (240°) | Drops item into Bin 3 |
