# Automated Multi Material Waste Classification System

[![GitHub Repository](https://img.shields.io/badge/GitHub-Repository-00f0ff?style=for-the-badge&logo=github&logoColor=white)](https://github.com/ALWINTR/Automated-Multi-Material-Waste-Classification-System)
[![Developer](https://img.shields.io/badge/Developer-Alwin_T_R-0284c7?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/alwintr)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)

An automated industrial-grade waste classification station that separates incoming waste into metallic and non-metallic categories using precision stepper motor rotary indexing, inductive proximity sensing, optical drop detection, and servo-actuated sorting flaps.

---

## System Architecture and Mechanics

The system operates as a closed-loop classification station:
1. **Intake and Drop Sensing**: The optical infrared sensor detects the arrival of an item in the inspection chute.
2. **Material Classification**: The inductive proximity sensor tests the material. High Eddy-current dampening indicates metallic content; absence designates non-metallic waste.
3. **Carousel Indexing**: A stepper motor rotates the carousel to position the correct bin compartment directly beneath the chute.
4. **Sorting Actuation**: A high-torque micro-servo actuates the trapdoor flap to drop the material into the designated compartment.

```
       +-------------------------------+
       |   Incoming Waste Insertion    |
       +---------------+---------------+
                       |
                       v
       +-------------------------------+
       |  Optical IR Drop Detection    |
       +---------------+---------------+
                       | Item Present
                       v
       +-------------------------------+
       |   Inductive Metal Sensing     |
       +-------+---------------+-------+
               |               |
      [Metal Detected]   [Non-Metallic]
               |               |
               v               v
       +-------------------------------+
       |  Stepper Carousel Indexing    |
       |  (Rotate to Designated Bin)   |
       +---------------+---------------+
                       |
                       v
       +-------------------------------+
       |  Servo Trapdoor Flap Eject    |
       +-------------------------------+
```

---

## Hardware Bill of Materials (BOM)

| Component | Specification | Functional Role |
| :--- | :--- | :--- |
| **Microcontroller** | Arduino Uno (ATmega328P, 16MHz) | Master classification logic and motion sequencing |
| **Stepper Motor** | NEMA 17 / 28BYJ-48 Geared Stepper | Precision rotary indexing of sorting bins |
| **Stepper Driver** | A4988 / ULN2003 Driver Module | Microstepping coil excitation and current regulation |
| **Inductive Proximity Sensor** | LJ12A3-4-Z/BX Probe | High-frequency Eddy current metal detection |
| **Optical Drop Sensor** | TCRT5000 / FC-51 IR Module | Immediate detection of item arrival in hopper |
| **Sorting Diverter Servo** | TowerPro SG90 9g Micro Servo | Rapid mechanical trapdoor ejection flap |

---

## Circuit Pinout Table

| Module Signal | Arduino Uno Pin | Direction | Description |
| :--- | :--- | :--- | :--- |
| **Stepper Phase A (IN1)** | Digital Pin D8 | Output | Coil phase 1 drive signal |
| **Stepper Phase B (IN2)** | Digital Pin D9 | Output | Coil phase 2 drive signal |
| **Stepper Phase C (IN3)** | Digital Pin D10 | Output | Coil phase 3 drive signal |
| **Stepper Phase D (IN4)** | Digital Pin D11 | Output | Coil phase 4 drive signal |
| **Servo PWM Signal** | Digital Pin D6 | PWM Output | Trapdoor open and close actuation |
| **IR Drop Sensor** | Digital Pin D2 | Input (Pull-up) | External trigger for item arrival |
| **Inductive Metal Sensor** | Digital Pin D3 | Input | Active LOW when metal is detected |
| **Status LED (Metallic)** | Digital Pin D4 | Output | Visual indicator for metal classification |
| **Status LED (Non-Metal)**| Digital Pin D5 | Output | Visual indicator for non-metal classification |

---

## Firmware State Machine

The firmware (`waste_segregation.ino`) implements a non-blocking finite state machine (FSM):
- `STATE_IDLE`: Continuously polls the infrared sensor while holding the carousel at home index.
- `STATE_INSPECT`: Samples the inductive sensor over 200ms to eliminate false negatives.
- `STATE_INDEX_CAROUSEL`: Drives the stepper motor smoothly with acceleration profiling.
- `STATE_DUMP`: Opens the servo trapdoor for 1200ms and verifies complete clearance.
- `STATE_RESET`: Re-indexes the carousel back to baseline readiness.

---

## Author

**Alwin T R** - Robotics and Automation Engineer  
- LinkedIn: [linkedin.com/in/alwintr](https://www.linkedin.com/in/alwintr)  
- Portfolio: [alwintr.github.io](https://alwintr.github.io)  
- GitHub: [github.com/ALWINTR](https://github.com/ALWINTR)

---

## License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
