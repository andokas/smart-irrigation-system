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
int dryThreshold = 700;   // Value above = dry (needs water)
int wetThreshold = 400;   // Value below = wet

void setup() {
  // Initialize servo
  pump.attach(pumpPin);
  pump.write(0); // Initial position (pump off)
  
  // Configure LED pins as outputs
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  // Turn off all LEDs at startup
  digitalWrite(greenLED, LOW);
  digitalWrite(yellowLED, LOW);
  digitalWrite(redLED, LOW);
}

void loop() {
  // Read moisture sensor value
  moistureValue = analogRead(moistureSensor);
  
  // Evaluate moisture level and act accordingly
  if (moistureValue > dryThreshold) {
    // DRY SOIL - Activate irrigation
    
    // Turn on red LED
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
    
    // Activate pump (servo)
    pump.write(90);  // Turn servo (simulate pump ON)
    delay(2000);     // Water for 2 seconds
    pump.write(0);   // Turn off pump
    
  } else if (moistureValue > wetThreshold) {
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
  
  // Wait 1 second before next reading
  delay(1000);
}