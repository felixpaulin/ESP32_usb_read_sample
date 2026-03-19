#include <HardwareSerial.h>
#include <Arduino.h>
#include <ESP32Servo.h>


Servo sorterServo;

// ===== CONFIG =====
const int SERVO_PIN = 13;

// Change these angles to match your conveyor layout
// avoid 0 and 180 to avoid hitting mechanical limits of the servo. Adjust as needed for your setup.
const int IDLE_ANGLE = 10;     // 
const int BIN_1_ANGLE = 50;    // Straight / default
const int BIN_2_ANGLE = 110;   // Center
const int BIN_3_ANGLE = 155;  // Right
// ==================


unsigned long moveTime = 0;
bool waitingToReturn = false;


// LED pins
constexpr int RED_LED    = 25;
constexpr int GREEN_LED  = 26;
constexpr int YELLOW_LED = 27;

void lightLedOn(int pin) {
  digitalWrite(pin, HIGH);
}


void lightLedAllOff() {
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
}


void moveServoTo(int angle) {
  sorterServo.write(angle);
  moveTime = millis();
  waitingToReturn = true;
}
// SET THIS BACK 9600 BAUD, 115200 FOR DEBUGGING
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);

  sorterServo.setPeriodHertz(50);
  sorterServo.attach(SERVO_PIN, 500, 2400);

  // Start straight
  sorterServo.write(IDLE_ANGLE);

  Serial.println("ESP32 ready");
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();  // remove \r, spaces, etc.

    if (cmd == "BIN_1") {
      lightLedOn(RED_LED);
      moveServoTo(BIN_1_ANGLE);
    } 
    else if (cmd == "BIN_2") {
      lightLedOn(YELLOW_LED); 
      moveServoTo(BIN_2_ANGLE);
    } 
    else if (cmd == "BIN_3") {
      lightLedOn(GREEN_LED);
      moveServoTo(BIN_3_ANGLE);
    } 
    else {
      Serial.print("Unknown command: ");
      Serial.println(cmd);
    }
  }

  // ===== AUTO RETURN AFTER 6s =====
  if (waitingToReturn && millis() - moveTime >= 6000) {
    waitingToReturn = false;
    sorterServo.write(IDLE_ANGLE);
    lightLedAllOff();
    Serial.println("Returned to idle angle");
  }  
}

