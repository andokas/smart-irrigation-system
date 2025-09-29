#include <Servo.h>

// Create servo object
Servo pump;

// Define pins
int moistureSensor = A0;  // Analog pin for moisture sensor
int greenLED = 13;        // Green LED - wet soil
int yellowLED = 12;       // Yellow LED - medium moisture
int redLED = 11;          // Red LED - dry soil
int pumpPin = 9;          // Servo pin (pump)

// System variables
int moistureValue = 0;
int wetThreshold = 700;   // Value above = wet
int dryThreshold = 400;   // Value below = dry (needs water)

// Temperature sensor and manual mode
int tempSensor = A1;        // Analog pin for temperature sensor (TMP36)
int buzzerPin = 8;          // Buzzer pin for temperature alerts
int buttonPin = 7;          // Manual watering button
int blueLED = 6;            // Blue LED - manual mode indicator
int tempValue = 0;
float temperature = 0.0;
float tempThreshold = 30.0; // Temperature alert threshold (Celsius)
bool buttonPressed = false;
unsigned long lastTempAlert = 0;
unsigned long tempAlertInterval = 3000; // Alert every 3 seconds

void setup() {
  
  // Initialize servo
  pump.attach(pumpPin);
  pump.write(0); // Initial position (pump off)
  
  // Configure pins as inputs and outputs
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Turn off buzzer and all LEDs at startup
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(buzzerPin, LOW);
  
  // Wait for button to stabilize and set initial state
  delay(2000);
}

void loop() {
  
  // Read moisture sensor value
  moistureValue = analogRead(moistureSensor);
  
  // Read temperature sensor
  tempValue = analogRead(tempSensor);
  float voltage = (tempValue * 5.0) / 1024.0;
  temperature = (voltage - 0.5) * 100.0;
  
  // Check manual button
  if (digitalRead(buttonPin) == LOW && !buttonPressed) {
    buttonPressed = true;
    manualWatering();
    delay(500);
  }
  if (digitalRead(buttonPin) == HIGH) {
    buttonPressed = false;
  }
  
  // Check temperature alert
  if (temperature > tempThreshold) {
    unsigned long currentTime = millis();
    if (currentTime - lastTempAlert > tempAlertInterval) {
      temperatureAlarm();
      lastTempAlert = currentTime;
    }
  }
  
  // Adjust thresholds based on temperature
  int adjustedDryThreshold = dryThreshold;
  int adjustedWetThreshold = wetThreshold;
  
  if (temperature > 25.0) {
    adjustedDryThreshold = dryThreshold + 100; // Water earlier when hot
    adjustedWetThreshold = wetThreshold + 50;
  }
  
  // Evaluate moisture level and act accordingly
  if (moistureValue < adjustedDryThreshold) {
    // DRY SOIL - Activate irrigation
    
    // Turn on red LED
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    
    // Activate pump (servo)
    pump.write(90);  // Turn servo (simulate pump ON)
    delay(2000);     // Water for 2 seconds
    pump.write(0);   // Turn off pump
    
  } else if (moistureValue < adjustedWetThreshold) {
    // MEDIUM MOISTURE
    
    // Turn on yellow LED
    digitalWrite(yellowLED, HIGH);
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    
    // Pump off
    pump.write(0);
    
  } else {
    // WET SOIL - No watering needed
    
    // Turn on green LED
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
    
    // Pump off
    pump.write(0);
  }
  
  // Wait 500 millis before next reading
  delay(500);
}

// Manual watering function
void manualWatering() {
  digitalWrite(blueLED, HIGH);
  
  pump.write(90);
  delay(2000);
  pump.write(0);

  digitalWrite(blueLED, LOW);
}

// Temperature alarm function
void temperatureAlarm() {
  for (int i = 0; i < 3; i++) {
    tone(buzzerPin, 1000, 200);
    delay(250);
    tone(buzzerPin, 1500, 200);
    delay(250);
  }
}