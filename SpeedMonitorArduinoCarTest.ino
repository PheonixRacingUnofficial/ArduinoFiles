// Define constants
const float wheelDiameterInches = 21.6; // Need to get accurate wheel diameter
const float pi = 3.14159; // Can be increased shouldn't need to
const float motorRatio = 1; // May be unused
const float upperVoltageThreshold = 4.0;
const float lowerVoltageThreshold = 1.0;

// Define pin functions
int analogPin = A0;
int inputPulsePin = 2; // Needs to be D2 on a NANO Board to read
int outputPin = 11; // May need to be more or less depending on display format

// Global variables
volatile unsigned long lastPulseTime = 0; // Stores time of the last pulse
volatile unsigned long pulseInterval = 0; // Time between two pulses
unsigned long lastHigh = 0;
unsigned long lastLow = 0;
bool high = 0;

float voltage = 0.0;

void setup() {
  Serial.begin(9600); // Sending input back to Ardunio IDE for testing, comment out to avoid errors when not connected
  // Serial.print("Hello World"); // Confirm input back to IDE
  Serial.println("[Arduino] Established Connection to Raspberry Pi");

  attachInterrupt(digitalPinToInterrupt(inputPulsePin), checkFlipLow, FALLING);
  attachInterrupt(digitalPinToInterrupt(inputPulsePin), checkFlipHigh, RISING); // Interrupt on rising edge
  
}

void loop() {

  // Serial.print("Last high: ");
  // Serial.println(lastHigh);

  // Serial.print("Last low: ");
  // Serial.println(lastLow);

  // Serial.print("Calculated Interval: |");
  // Serial.print(lastLow);
  // Serial.print(" - ");
  // Serial.print(lastHigh);
  // Serial.print("| = ");
  // Serial.println(abs(lastLow - lastHigh));

  // Serial.print("Interval: ");
  // Serial.println(pulseInterval);

  if (pulseInterval > 0) {
  
    float rpm = pulseToRPM();
    Serial.print("RPM: ");
    Serial.println(rpm);

    float mph = RPMToMPH(rpm);
    Serial.print("MPH: ");
    Serial.println(mph);

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

void checkFlipHigh() {
  Serial.println("Rising Interupt");
  if (high != 1) {
    high = !high;
    lastHigh = micros();
    pulseInterval = abs(lastHigh - lastLow);
  }
}

void checkFlipLow() {
  Serial.println("Falling Interupt");

  if (high != 0) {
    high = !high;
    lastLow = micros();
    pulseInterval = abs(lastLow - lastHigh);
  }

}

void pulseISR() {
  unsigned long currentTime = micros(); // Get current time in microseconds
  pulseInterval = currentTime - lastPulseTime; // Time between current and last pulse
  lastPulseTime = currentTime; // Update last pulse time
}
