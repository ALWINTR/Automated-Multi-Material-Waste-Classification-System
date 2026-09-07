/*
 * ============================================================================
 * Automated Smart Waste Segregation System (ULN2003 + 28BYJ-48 Stepper)
 * ============================================================================
 * Microcontroller: Arduino Uno
 * 
 * Waste Categories & Bin Mapping:
 *   1. Metallic Waste          -> Bin 1 (0 degrees)
 *   2. Wet / Organic Waste     -> Bin 2 (120 degrees)
 *   3. Dry / Non-metallic Waste -> Bin 3 (240 degrees)
 * 
 * Sensors:
 *   - IR Sensor: Object presence detection
 *   - Inductive Proximity Sensor: Metal detection (LJ12A3-4-Z/BX)
 *   - Raindrop / Moisture Sensor: Wet waste detection
 * 
 * Actuators:
 *   - 28BYJ-48 Stepper Motor + ULN2003 Driver: Rotates 3-compartment carousel
 *   - Servo Motor (SG90 / MG995): Flap / Trapdoor to drop waste
 * 
 * Power:
 *   - 9V 2A DC Adapter -> Inductive Proximity VCC (Brown) & Buck Converter IN+
 *   - Buck Converter (LM2596) -> 5.0V DC to Arduino Uno (5V pin), ULN2003 (+), 
 *     Servo (VCC), IR Sensor, and Rain Sensor
 *   - Common Ground connected across all components
 * ============================================================================
 */

#include <Servo.h>

// ==================== PIN DEFINITIONS ====================

// Sensors
const int PIN_IR_SENSOR        = 2;   // IR Obstacle Sensor DO pin
const int PIN_INDUCTIVE_SENSOR = 3;   // Inductive Sensor (NPN with internal pullup)
const int PIN_RAIN_SENSOR_DIG  = 4;   // Rain Sensor Digital Output (DO)
const int PIN_RAIN_SENSOR_ANA  = A0;  // Rain Sensor Analog Output (AO)

// ULN2003 Stepper Motor Driver (28BYJ-48 5V)
const int PIN_ULN_IN1          = 5;
const int PIN_ULN_IN2          = 6;
const int PIN_ULN_IN3          = 7;
const int PIN_ULN_IN4          = 8;

// Servo Motor (Trapdoor Flap)
const int PIN_SERVO            = 9;   // Servo PWM control pin

// Status Indicator LEDs & Buzzer
const int PIN_LED_METAL        = 10;
const int PIN_LED_WET          = 11;
const int PIN_LED_DRY          = 12;
const int PIN_BUZZER           = 13;

// ==================== CONFIGURATION & CONSTANTS ====================

// Servo Trapdoor Positions (Degrees)
const int SERVO_CLOSED_ANGLE   = 0;    // Flap closed (holds waste for sensing)
const int SERVO_OPEN_ANGLE     = 90;   // Flap open (drops waste into bin)

// 28BYJ-48 Stepper Motor Configuration (2048 steps per 360° revolution in 4-step mode)
const int STEPS_PER_REV        = 2048; 
const int STEP_DELAY_MS        = 3;    // Milliseconds between steps (speed control)

// Bin Positions in Steps (3-bin system: 0°, 120°, 240°)
const int BIN_POS_METAL        = 0;                              // 0°   -> 0 steps
const int BIN_POS_WET          = (long)(STEPS_PER_REV * 120) / 360; // 120° -> ~683 steps
const int BIN_POS_DRY          = (long)(STEPS_PER_REV * 240) / 360; // 240° -> ~1365 steps

// Rain sensor analog threshold (0-1023: lower value = more moisture)
const int RAIN_ANALOG_THRESHOLD = 700; 

// Sensor Active State Definitions
#define IR_DETECTED          LOW
#define METAL_DETECTED       LOW
#define WET_DETECTED         LOW

// ==================== GLOBAL OBJECTS & STATE ====================

Servo trapdoorServo;
int currentStepperPosition = 0; // Tracks current step position (0 to 2047)
int stepIndex = 0;              // Current index in 4-step sequence (0 to 3)

// 4-step drive sequence for 28BYJ-48
const int stepLookup[4][4] = {
  {HIGH, LOW,  LOW,  LOW},
  {LOW,  HIGH, LOW,  LOW},
  {LOW,  LOW,  HIGH, LOW},
  {LOW,  LOW,  LOW,  HIGH}
};

enum WasteType {
  NONE,
  METAL,
  WET,
  DRY
};

// ==================== SETUP ====================

void setup() {
  Serial.begin(9600);
  Serial.println(F("================================================"));
  Serial.println(F("Smart Waste Segregation (ULN2003 + 28BYJ-48)"));
  Serial.println(F("================================================"));

  // Initialize Sensor Pins
  pinMode(PIN_IR_SENSOR, INPUT);
  pinMode(PIN_INDUCTIVE_SENSOR, INPUT_PULLUP);
  pinMode(PIN_RAIN_SENSOR_DIG, INPUT);
  pinMode(PIN_RAIN_SENSOR_ANA, INPUT);

  // Initialize ULN2003 Pins
  pinMode(PIN_ULN_IN1, OUTPUT);
  pinMode(PIN_ULN_IN2, OUTPUT);
  pinMode(PIN_ULN_IN3, OUTPUT);
  pinMode(PIN_ULN_IN4, OUTPUT);
  disableStepperCoils(); // Power off coils initially to save power/heat

  // Initialize Status Indicators
  pinMode(PIN_LED_METAL, OUTPUT);
  pinMode(PIN_LED_WET, OUTPUT);
  pinMode(PIN_LED_DRY, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // Initialize Servo to closed position
  trapdoorServo.attach(PIN_SERVO);
  trapdoorServo.write(SERVO_CLOSED_ANGLE);
  delay(500);

  // Ready signal (short beep)
  beep(100);
  Serial.println(F("System Ready. Place waste on sensing platform..."));
}

// ==================== MAIN LOOP ====================

void loop() {
  // Step 1: Check if an object is present on the sensing chute
  if (digitalRead(PIN_IR_SENSOR) == IR_DETECTED) {
    Serial.println(F("\n[!] Object Detected by IR Sensor. Starting Classification..."));
    beep(50);
    
    // Allow waste item to settle on the sensing plate
    delay(1000); 

    // Step 2: Classify the waste
    WasteType detectedType = classifyWaste();

    // Step 3: Move Stepper to corresponding bin and drop waste
    routeAndDispense(detectedType);

    // Step 4: Wait for chute clearance
    delay(1500);
    clearIndicators();
    Serial.println(F("Ready for next item."));
  }

  delay(100); // Polling interval
}

// ==================== SENSING & CLASSIFICATION ====================

WasteType classifyWaste() {
  // Read Inductive Sensor (Metal)
  bool isMetal = (digitalRead(PIN_INDUCTIVE_SENSOR) == METAL_DETECTED);

  // Read Rain Sensor (Moisture / Wet)
  int rainAnalogVal = analogRead(PIN_RAIN_SENSOR_ANA);
  bool isWet = (digitalRead(PIN_RAIN_SENSOR_DIG) == WET_DETECTED) || (rainAnalogVal < RAIN_ANALOG_THRESHOLD);

  Serial.print(F("Inductive Sensor (Metal): "));
  Serial.println(isMetal ? F("YES") : F("NO"));
  Serial.print(F("Rain Sensor (Wetness AO="));
  Serial.print(rainAnalogVal);
  Serial.print(F("): "));
  Serial.println(isWet ? F("YES") : F("NO"));

  // Classification Hierarchy:
  // 1. Metal takes precedence
  if (isMetal) {
    Serial.println(F(">>> CATEGORY: METALLIC WASTE <<<"));
    digitalWrite(PIN_LED_METAL, HIGH);
    return METAL;
  }
  // 2. Wet / Organic
  else if (isWet) {
    Serial.println(F(">>> CATEGORY: WET / ORGANIC WASTE <<<"));
    digitalWrite(PIN_LED_WET, HIGH);
    return WET;
  }
  // 3. Dry / Plastic / Paper (Object detected, but neither metal nor wet)
  else {
    Serial.println(F(">>> CATEGORY: DRY / PLASTIC WASTE <<<"));
    digitalWrite(PIN_LED_DRY, HIGH);
    return DRY;
  }
}

// ==================== ACTUATION & ROUTING ====================

void routeAndDispense(WasteType type) {
  int targetPosition = currentStepperPosition;

  switch (type) {
    case METAL:
      targetPosition = BIN_POS_METAL;
      break;
    case WET:
      targetPosition = BIN_POS_WET;
      break;
    case DRY:
      targetPosition = BIN_POS_DRY;
      break;
    default:
      return;
  }

  // 1. Rotate Stepper to target bin compartment
  if (targetPosition != currentStepperPosition) {
    Serial.print(F("Rotating carousel to Target Step: "));
    Serial.println(targetPosition);
    moveStepperTo(targetPosition);
  } else {
    Serial.println(F("Carousel already aligned at target bin."));
  }

  // Allow mechanical vibration to dampen
  delay(400);

  // 2. Open Servo Trapdoor to drop the waste
  Serial.println(F("Opening Trapdoor Flap..."));
  trapdoorServo.write(SERVO_OPEN_ANGLE);
  delay(1200); // Wait for item to fall through

  // 3. Close Servo Trapdoor
  Serial.println(F("Closing Trapdoor Flap..."));
  trapdoorServo.write(SERVO_CLOSED_ANGLE);
  delay(600);
}

// ==================== ULN2003 STEPPER MOTOR DRIVER ====================

void moveStepperTo(int targetPos) {
  // Calculate shortest path on circular ring (0 to STEPS_PER_REV - 1)
  int diff = targetPos - currentStepperPosition;

  // Normalize step difference for shortest circular path
  if (diff > (STEPS_PER_REV / 2)) {
    diff -= STEPS_PER_REV;
  } else if (diff < -(STEPS_PER_REV / 2)) {
    diff += STEPS_PER_REV;
  }

  int direction = (diff >= 0) ? 1 : -1;
  int stepsToMove = abs(diff);

  for (int i = 0; i < stepsToMove; i++) {
    // Step forward or backward through the 4-phase sequence
    stepIndex = (stepIndex + direction + 4) % 4;
    
    digitalWrite(PIN_ULN_IN1, stepLookup[stepIndex][0]);
    digitalWrite(PIN_ULN_IN2, stepLookup[stepIndex][1]);
    digitalWrite(PIN_ULN_IN3, stepLookup[stepIndex][2]);
    digitalWrite(PIN_ULN_IN4, stepLookup[stepIndex][3]);
    
    delay(STEP_DELAY_MS);
  }

  currentStepperPosition = targetPos;
  
  // Disable coils after movement to keep motor cool and save power
  disableStepperCoils();
}

void disableStepperCoils() {
  digitalWrite(PIN_ULN_IN1, LOW);
  digitalWrite(PIN_ULN_IN2, LOW);
  digitalWrite(PIN_ULN_IN3, LOW);
  digitalWrite(PIN_ULN_IN4, LOW);
}

// ==================== UTILITY FUNCTIONS ====================

void beep(int durationMs) {
  digitalWrite(PIN_BUZZER, HIGH);
  delay(durationMs);
  digitalWrite(PIN_BUZZER, LOW);
}

void clearIndicators() {
  digitalWrite(PIN_LED_METAL, LOW);
  digitalWrite(PIN_LED_WET, LOW);
  digitalWrite(PIN_LED_DRY, LOW);
}
