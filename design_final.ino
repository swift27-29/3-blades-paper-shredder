#include "HX711.h"

// Constants for weight conversion and calibration
#define calibration_factor 4037.7
#define zero_factor 30047

#define LOADCELL_DOUT_PIN A0
#define LOADCELL_SCK_PIN A1

HX711 scale;

// Seven-segment BCD pins
#define BCD_A 2
#define BCD_B 3
#define BCD_C 4
#define BCD_D 7

// Averaging array
float avgRead[10];
int avgIndex = 0;

// Motor control pins
#define BUTTON_MOTOR1 12
#define BUTTON_MOTOR2 A2
#define BUTTON_MOTOR3 A5
#define RPWM_PIN 5
#define LPWM_PIN 6
#define RPWM_PIN2 A4
#define LPWM_PIN2 A3
#define RPWM_PIN3 11
#define LPWM_PIN3 10

// Buzzer pin
#define BUZZER_PIN 8
#define BUZZER_TIME 3000   // buzzer sound duration (3 seconds)
#define RESET_TIME 10000   // total wait before reset (10 seconds)

// Motor flags
bool motorRunning = false;
bool secondMotorRunning = false;
bool thirdMotorRunning = false;

// Variable to track if the 9kg weight was reached and waiting for reset
bool waitingForReset = false;
unsigned long resetStartTime = 0;

// Buzzer control
bool buzzerOn = false;

// Function to average weight readings
float averageWeight() {
  float scaleRead = scale.get_units(); // already in kilograms
  avgRead[avgIndex] = scaleRead;
  avgIndex = (avgIndex + 1) % 10;

  float total = 0;
  for (int i = 0; i < 10; i++) {
    total += avgRead[i];
  }

  return total / 10.0;
}

// Display digit on 7-segment using BCD
void displayDigit(int digit) {
  digitalWrite(BCD_A, (digit & 0x01));
  digitalWrite(BCD_B, (digit & 0x02) >> 1);
  digitalWrite(BCD_C, (digit & 0x04) >> 2);
  digitalWrite(BCD_D, (digit & 0x08) >> 3);
}

void setup() {
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor);
  scale.tare();

  pinMode(BCD_A, OUTPUT);
  pinMode(BCD_B, OUTPUT);
  pinMode(BCD_C, OUTPUT);
  pinMode(BCD_D, OUTPUT);

  pinMode(BUTTON_MOTOR1, INPUT_PULLUP);
  pinMode(BUTTON_MOTOR2, INPUT_PULLUP);
  pinMode(BUTTON_MOTOR3, INPUT_PULLUP);

  pinMode(RPWM_PIN, OUTPUT);
  pinMode(LPWM_PIN, OUTPUT);
  pinMode(RPWM_PIN2, OUTPUT);
  pinMode(LPWM_PIN2, OUTPUT);
  pinMode(RPWM_PIN3, OUTPUT);
  pinMode(LPWM_PIN3, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // Turn everything off initially
  digitalWrite(RPWM_PIN, LOW);
  digitalWrite(LPWM_PIN, LOW);
  digitalWrite(RPWM_PIN2, LOW);
  digitalWrite(LPWM_PIN2, LOW);
  digitalWrite(RPWM_PIN3, LOW);
  digitalWrite(LPWM_PIN3, LOW);

  delay(100);
  Serial.println("Scale Initialized.");
}

void loop() {
  // --- SAMPLE weight ---
  float weight = averageWeight();
  if (weight < 0) weight = 0;
  float weightKg = weight;
  if (weightKg > 9.0) weightKg = 9.0;

  // --- ROUND properly ---
  weightKg = round(weightKg);

  // --- PRINT weight ---
  Serial.print("Weight (kg): ");
  Serial.println(weightKg);

  // --- DISPLAY on 7-segment ---
  displayDigit((int)weightKg);
  delay(200);

  // --- CHECK weight range ---
  if (weightKg >= 5.0 && weightKg <= 9.0 && !waitingForReset) {
    Serial.println("Weight reached 5-9kg!");
    waitingForReset = true;
    resetStartTime = millis();
    buzzerOn = true;
    digitalWrite(BUZZER_PIN, HIGH);
  }

  if (waitingForReset) {
    unsigned long elapsed = millis() - resetStartTime;

    if (buzzerOn && elapsed >= BUZZER_TIME) {
      digitalWrite(BUZZER_PIN, LOW);
      buzzerOn = false;
    }

    if (elapsed >= RESET_TIME) {
      Serial.println("Resetting weight to 0.");
      scale.tare();
      waitingForReset = false;
      buzzerOn = false;
      digitalWrite(BUZZER_PIN, LOW);
    }
  }

  // --- BUTTONS and MOTOR SELECTION ---
  int button1State = digitalRead(BUTTON_MOTOR1); // HIGH = released
  int button2State = digitalRead(BUTTON_MOTOR2);
  int button3State = digitalRead(BUTTON_MOTOR3);

  int releasedCount = 0;
  if (button1State == HIGH) releasedCount++;
  if (button2State == HIGH) releasedCount++;
  if (button3State == HIGH) releasedCount++;

  // STOP ALL motors if two or more are released
  if (releasedCount >= 2) {
    motorRunning = false;
    secondMotorRunning = false;
    thirdMotorRunning = false;
    Serial.println("Two or more buttons released — stopping all motors");
  } else {
    // If only one button is released, run only its motor
    if (button1State == HIGH && button2State == LOW && button3State == LOW) {
      motorRunning = true;
      secondMotorRunning = false;
      thirdMotorRunning = false;
      Serial.println("Motor 1 running");
    } else if (button2State == HIGH && button1State == LOW && button3State == LOW) {
      motorRunning = false;
      secondMotorRunning = true;
      thirdMotorRunning = false;
      Serial.println("Motor 2 running");
    } else if (button3State == HIGH && button1State == LOW && button2State == LOW) {
      motorRunning = false;
      secondMotorRunning = false;
      thirdMotorRunning = true;
      Serial.println("Motor 3 running");
    } else {
      // No valid case, stop all
      motorRunning = false;
      secondMotorRunning = false;
      thirdMotorRunning = fa