# Automated Smart Waste Segregation System (ULN2003 + 28BYJ-48)

A complete project guide for sorting waste into **Metallic**, **Wet/Organic**, and **Dry/Plastic** categories using an **Arduino Uno**, **28BYJ-48 Stepper Motor + ULN2003 Driver**, **Servo Motor**, **IR Sensor**, **Inductive Proximity Sensor**, **Raindrop/Moisture Sensor**, and **LM2596 Buck Converter**.

---

## 1. Power Distribution Scheme (All Actuators on 5V)

Using the **ULN2003 driver + 28BYJ-48 5V Stepper Motor** simplifies the circuit: **the Stepper Motor, Servo Motor, and Arduino can now all run directly from the 5V Buck Converter rail!**

* **Main Adapter**: 9V 2A DC Adapter
* **9V Direct Rail**:
  * LM2596 Buck Converter `IN+` / `IN-`
  * Inductive Proximity Sensor `Brown (VCC)`
* **Buck Converter (LM2596)**:
  * Adjust potentiometer to output **5.0V DC**.
* **5.0V Regulated Bus**:
  * Powers Arduino Uno `5V` pin
  * Powers ULN2003 Stepper Driver `+` pin (5V)
  * Powers Servo Motor `VCC` (Red wire)
  * Powers IR Sensor & Rain Sensor `VCC`
* **Common Ground**:
  * Connect all GND lines together (9V adapter GND, Buck OUT-, Arduino GND, ULN2003 `-` pin, Sensor GNDs).

---

## 2. Complete Pinout & Wiring Connections

### A. Sensors to Arduino Uno
| Sensor / Module | Sensor Pin | Arduino Pin / Power Source | Notes |
| :--- | :--- | :--- | :--- |
| **IR Obstacle Sensor** | VCC / GND | 5V Bus / Common GND | Presence detection |
| | OUT / DO | **Digital Pin 2** | Low on detection |
| **Inductive Proximity** | Brown (VCC) | **+9V DC Rail** (Adapter) | Operates on 6V-36V |
| *(LJ12A3-4-Z/BX - NPN NO)* | Blue (GND) | Common GND | |
| | Black (Signal) | **Digital Pin 3** | Arduino uses `INPUT_PULLUP` |
| **Raindrop Sensor** | VCC / GND | 5V Bus / Common GND | Moisture detection |
| | DO | **Digital Pin 4** | Digital threshold |
| | AO | **Analog Pin A0** | Analog moisture value |

### B. Actuators to Arduino Uno
| Actuator / Driver | Module Pin | Arduino Pin / Power | Notes |
| :--- | :--- | :--- | :--- |
| **ULN2003 Stepper Driver** | `+` (VCC) | **5V Bus** (Buck Converter) | Powers 28BYJ-48 motor |
| | `-` (GND) | Common GND | |
| | `IN1` | **Digital Pin 5** | Phase A |
| | `IN2` | **Digital Pin 6** | Phase B |
| | `IN3` | **Digital Pin 7** | Phase C |
| | `IN4` | **Digital Pin 8** | Phase D |
| **Servo Motor (SG90 / MG995)**| Red (VCC) | **5V Bus** (Buck Converter) | Do not power from Uno 5V |
| | Brown / Black | Common GND | |
| | Orange / Yellow | **Digital Pin 9** | PWM angle control |
| **Status Indicators** | LEDs & Buzzer | **D10 (Metal), D11 (Wet), D12 (Dry), D13 (Buzzer)** | Visual/audio feedback |

---

## 3. Stepper Angle Mapping (28BYJ-48)

* In 4-step sequence, 1 full rotation ($360^\circ$) = **2048 steps**.
* **Bin 1 (Metal)**: $0^\circ \rightarrow \mathbf{0\text{ steps}}$
* **Bin 2 (Wet)**: $120^\circ \rightarrow \frac{2048 \times 120}{360} \approx \mathbf{683\text{ steps}}$
* **Bin 3 (Dry)**: $240^\circ \rightarrow \frac{2048 \times 240}{360} \approx \mathbf{1365\text{ steps}}$
