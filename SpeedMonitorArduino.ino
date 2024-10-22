// Define constants
const float wheelDiameterInches = 16.0; // Need to get accurate wheel diameter
const float pi = 3.14159; // Can be increased shouldn't need to
const float motorRatio = 1; // May be unused

// Define pin functions
int inputPulsePin = 2; // Needs to be D2 on a NANO Board
int outputPin = 11; // May need to be more or less depending on display format

// Global variables
volatile unsigned long lastPulseTime = 0; // Stores time of the last pulse
volatile unsigned long pulseInterval = 0; // Time between two pulses

void setup() {
  Serial.begin(9600); // Sending input back to Ardunio IDE for testing, comment out to avoid errors when not connected
  // Serial.print("Hello World"); // Confirm input back to IDE

  attachInterrupt(digitalPinToInterrupt(inputPulsePin), pulseISR, RISING); // Interrupt on rising edge
}

void loop() {
  if (pulseInterval > 0) {
  
  float rpm = pulseToRPM();
  Serial.print("RPM: ");
  Serial.println(rpm);

  float mph = RPMToMPH(rpm);
  Serial.print("MPH: ");
  Serial.println(mph);
  
  delay(500);

  }

}

float pulseToRPM() {
  float timePerRotation = (pulseInterval * 16.0) / 1000000.0;
  float rpm = 60.0 / timePerRotation;
  return rpm;
}

float RPMToMPH(float rpm) {
  float wheelCircumferenceInches = pi * wheelDiameterInches;
  float inchesPerMinute = rpm * wheelCircumferenceInches;
  float mph = (inchesPerMinute * 60) / (63360);
  return mph;
}

void pulseISR() {
  unsigned long currentTime = micros(); // Get current time in microseconds
  pulseInterval = currentTime - lastPulseTime; // Time between current and last pulse
  lastPulseTime = currentTime; // Update last pulse time
}
