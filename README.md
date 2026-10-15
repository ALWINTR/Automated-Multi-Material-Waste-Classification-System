# Automated Multi Material Waste Classification System

<div align="center">

[![GitHub Repository](https://img.shields.io/badge/GitHub-Repository-00f0ff?style=for-the-badge&logo=github&logoColor=white)](https://github.com/ALWINTR/Automated-Multi-Material-Waste-Classification-System)
[![Developer](https://img.shields.io/badge/Developer-Alwin_T_R-0284c7?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/alwintr)
[![Portfolio](https://img.shields.io/badge/Portfolio-alwintr.github.io-00f0ff?style=for-the-badge&logo=googlechrome&logoColor=black)](https://alwintr.github.io)
[![License](https://img.shields.io/badge/License-MIT-10b981?style=for-the-badge)](LICENSE)

</div>

> Closed-loop industrial waste classification station utilizing precision NEMA 17 stepper carousel indexing, inductive metal detection, infrared drop verification, and high-speed servo diversion flaps.

---

## 📌 Executive Overview

Closed-loop industrial waste classification station utilizing precision NEMA 17 stepper carousel indexing, inductive metal detection, infrared drop verification, and high-speed servo diversion flaps. This repository contains production-ready firmware implementations, hardware wiring pinouts, signal conditioning routines, and real-time telemetry pipelines engineered for reliable physical operation.

- **Primary Domain**: Industrial Automation
- **Microcontroller & Hardware Core**: Arduino / Stepper
- **Specialization**: Industrial Automation
- **Live Specs & BOM Viewer**: [alwintr.github.io](https://alwintr.github.io)

---

## 📐 System Architecture & Signal Flow

```
[ Feed Hopper ] ===> [ TCRT5000 IR Drop Beam ]
                             | (Drop Trigger Interrupt)
                             v
+--------------------------------------------------------------+
|                     Arduino Uno ATmega328P                   |
+--------------------------------------------------------------+
        |                                        |
        | Step/Dir Pulses (1/16)                 | Dynamic Angle PWM
        v                                        v
+-----------------------+              +-----------------------+
|  A4988 Stepper Driver |              | SG90 Micro Servo Gate |
+-----------+-----------+              +-----------+-----------+
            |                                      |
            v                                      v
  [ NEMA 17 Carousel (90 deg) ]        [ Metal / Non-Metal Bins ]
            ^
            |
  [ Inductive Proximity Sensor ]
```

---

## 📋 Hardware Bill of Materials (BOM)

| Component Name | Operating Voltage | Interface / Functionality |
| :--- | :--- | :--- |
| **Arduino Uno R3** | `5V DC` | Master state machine controller |
| **NEMA 17 Stepper (42BYGHW)** | `12V / 1.5A` | 200 steps/rev, 1.8 deg step angle |
| **A4988 Driver Carrier** | `8-35V VMOT` | 1/16 microstepping interpolation |
| **LJ12A3-4-Z/BX Inductive Sensor** | `6-12V (Opto-isolated)` | 4mm detection range |
| **TowerPro SG90 Servo** | `5V DC` | 180 deg diverter gate |

---

## 🔌 Circuit Pinout & Microcontroller Interconnect

| MCU Pin / Channel | Peripheral Pin | Signal Function / Bus |
| :--- | :--- | :--- |
| `Arduino Pin 3` | `A4988 STEP Pin` | Timer PWM Pulse Train |
| `Arduino Pin 4` | `A4988 DIR Pin` | Carousel Rotation Direction |
| `Arduino Pin 2` | `TCRT5000 IR Out` | Drop Beam Interrupt INT0 |
| `Arduino Pin 7` | `Inductive Sensor Signal` | Metal Verification Input |
| `Arduino Pin 9` | `SG90 Servo PWM` | Hardware Timer PWM Output |

---

## ⚙️ Firmware Architecture & Control Logic

Deterministic state machine coordinating feed detection, 90-degree rotary indexing with trapezoidal acceleration, sensor settling delays, and servo deflection timing.

### Key Firmware Capabilities:
- **Zero-Blocking Architecture**: Real-time non-blocking scheduling eliminating delay() deadlocks.
- **Hardware Interrupt Servicing**: Microsecond-precision edge-triggered pulse detection and safety shutoffs.
- **Diagnostic Telemetry**: Continuous serial datastreaming and state-machine transition logging.

---

## 📁 Repository Structure

```
Automated-Multi-Material-Waste-Classification-System/
├── .gitignore
├── IMPLEMENTATION_GUIDE.md
├── LICENSE
├── waste_segregation.ino
└── README.md
```

---

## 🚀 Build & Deployment Instructions

### 1. Hardware Assembly
- Verify common system ground across all microcontrollers, drivers, and peripheral sensor boards.
- Ensure dedicated power supply rails match the operating voltage specified in the Hardware Bill of Materials.

### 2. Firmware Compilation & Flashing
```bash
# Clone the repository
git clone https://github.com/ALWINTR/Automated-Multi-Material-Waste-Classification-System.git

# Navigate to the project directory
cd Automated-Multi-Material-Waste-Classification-System
```

- Open the project in **Arduino IDE**, **PlatformIO**, or **ESP-IDF**.
- Select the target board configuration and COM port.
- Compile and upload firmware directly to the microcontroller.

---

## 👨‍💻 Author

**Alwin T R** — Robotics & Automation Engineer  
- 💼 **LinkedIn**: [linkedin.com/in/alwintr](https://www.linkedin.com/in/alwintr)  
- 🌐 **Interactive Portfolio**: [alwintr.github.io](https://alwintr.github.io)  
- 🐙 **GitHub**: [github.com/ALWINTR](https://github.com/ALWINTR)  
- ✉️ **Direct Email**: [alwintr2003@gmail.com](mailto:alwintr2003@gmail.com)

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.
