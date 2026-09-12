# ♻️ Automated Smart Waste Segregation System (Multi-Sensor Carousel Classification)

[![GitHub Repository](https://img.shields.io/badge/GitHub-Repository-00f0ff?style=for-the-badge&logo=github&logoColor=white)](https://github.com/ALWINTR/waste-segregation-system)
[![Developer](https://img.shields.io/badge/Developer-Alwin_T_R-0284c7?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/alwintr)
[![Platform](https://img.shields.io/badge/Platform-Arduino_Uno_%26_Stepper-38bdf8?style=for-the-badge&logo=arduino&logoColor=white)](https://github.com/ALWINTR)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)

An industrial automated waste classification station that automatically detects, categorizes, and sorts incoming waste into metallic and non-metallic / dry categories using precision stepper motor indexing, inductive proximity sensing, optical drop detection, and servo-actuated sorting flaps.

---

## 📌 System Architecture & Mechanics

The system operates as a closed-loop automated classification station:
1. **Intake & Drop Sensing**: The optical IR sensor detects the insertion of an object into the inspection chute.
2. **Material Classification**: The **inductive proximity sensor** interrogates the material. High Eddy-current dampening confirms metallic composition; absence flags non-metallic waste.
3. **Carousel Indexing**: A **stepper motor (28BYJ-48 / NEMA 17)** indexes the rotary carousel to align the correct destination bin beneath the sorting chute.
4. **Sorting Actuation**: A high-speed **SG90 micro-servo** actuates the trapdoor flap to drop the material into the target compartment.

```
       ┌───────────────────────────────┐
       │   Incoming Waste Insertion    │
       └───────────────┬───────────────┘
                       │
                       ▼
       ┌───────────────────────────────┐
       │  Optical IR Drop Detection    │
       └───────────────┬───────────────┘
                       │ Item Present
                       ▼
       ┌───────────────────────────────┐
       │   Inductive Metal Sensing     │
       └───────┬───────────────┬───────┘
               │               │
      [Metal Detected]   [Non-Metallic]
               │               │
               ▼               ▼
       ┌───────────────────────────────┐
       │  Stepper Carousel Indexing    │
       │  (Rotate to Designated Bin)   │
       └───────────────┬───────────────┘
                       │
                       ▼
       ┌───────────────────────────────┐
       │  Servo Trapdoor Flap Eject    │
       └───────────────────────────────┘
```

---

## ⚙️ Hardware Components & Specifications

| Component | Part / Model | Operating Specs | Function |
| :--- | :--- | :--- | :--- |
| **Microcontroller** | Arduino Uno (ATmega328P) | 16MHz, 5V Logic | Master state machine controller |
| **Stepper Motor** | 28BYJ-48 / NEMA 17 | 5V / 12V Unipolar / Bipolar | Rotary carousel positioning (64:1 gear) |
| **Stepper Driver** | ULN2003 / A4988 Module | Darlington Array / 1/16 Microstepping | High-current coil phase energization |
| **Metal Sensor** | LJ12A3-4-Z/BX / Inductive Probe | 6-36V (Regulated to 5V) | High-frequency Eddy current sensing |
| **Drop Sensor** | TCRT5000 / FC-51 Optical IR | 5V Analog/Digital | Instantaneous object entry detection |
| **Sorting Servo** | TowerPro SG90 9g Servo | 50Hz PWM (0.5ms - 2.5ms) | Rapid mechanical sorting trapdoor |

---

## 🔌 Circuit Pinout Table

| Module Signal | Arduino Uno Pin | Direction | Description |
| :--- | :--- | :--- | :--- |
| **Stepper IN1 (Coil A)** | Digital Pin 8 | Output | Motor phase 1 energize |
| **Stepper IN2 (Coil B)** | Digital Pin 9 | Output | Motor phase 2 energize |
| **Stepper IN3 (Coil C)** | Digital Pin 10 | Output | Motor phase 3 energize |
| **Stepper IN4 (Coil D)** | Digital Pin 11 | Output | Motor phase 4 energize |
| **Sorting Servo PWM** | Digital Pin 6 | PWM Output | Trapdoor open/close (0° / 90°) |
| **IR Object Drop Sensor** | Digital Pin 2 | Input (Pull-up) | External trigger for item arrival |
| **Inductive Metal Sensor** | Digital Pin 3 | Input | Active LOW when metal detected |
| **Status LED (Metal)** | Digital Pin 4 | Output | Blue indicator for metallic waste |
| **Status LED (General)** | Digital Pin 5 | Output | Green indicator for non-metal waste |

---

## 🧠 Software Implementation & Code Structure

The Arduino firmware (`waste_segregation.ino`) implements a non-blocking finite state machine (FSM):
- `STATE_IDLE`: Polling IR sensor while holding carousel at home position.
- `STATE_INSPECT`: Sampling inductive sensor readings over 200ms to eliminate false negatives.
- `STATE_INDEX_CAROUSEL`: Stepping the motor smoothly using half-step 8-phase sequences to prevent vibration.
- `STATE_DUMP`: Opening servo trapdoor for 1200ms and verifying item clearance.
- `STATE_RESET`: Returning carousel to baseline home index.

---

## 🚀 Getting Started

1. Clone repository:
   ```bash
   git clone https://github.com/ALWINTR/waste-segregation-system.git
   ```
2. Open `waste_segregation.ino` in Arduino IDE.
3. Verify board selection is set to **Arduino Uno**.
4. Compile and upload to board.
5. Review `IMPLEMENTATION_GUIDE.md` for full mechanical 3D printing and mounting details.

---

## 👨‍💻 Author

**Alwin T R** — Robotics & Automation Engineer  
- 💼 LinkedIn: [linkedin.com/in/alwintr](https://www.linkedin.com/in/alwintr)  
- 🌌 Portfolio: [alwintr.github.io](https://alwintr.github.io)  
- 💻 GitHub: [github.com/ALWINTR](https://github.com/ALWINTR)

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.
