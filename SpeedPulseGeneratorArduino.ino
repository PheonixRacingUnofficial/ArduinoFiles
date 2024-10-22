// Define constants
const float wheelDiameterInches = 16.0; // Change this to your wheel diameter
const float pi = 3.14159; // Pi constant
const int pulsesPerRotation = 16; // Number of pulses per rotation
const int outputPulsePin = 9; // Pin to send pulses (NANO SPECIFIC!!!)

// Variables
float userMPH = 0.0; // Variable to store user input for MPH
float pulseIntervalMicros = 0.0; // Time between pulses in microseconds

void setup() {
  // Initialize serial communication for input/output
  Serial.begin(9600);
  pinMode(outputPulsePin, OUTPUT); // Set the output pin to send pulses
  Serial.println("Enter speed in MPH:"); // Prompt user to enter speed
}

void loop() {
  // Check if data is available in the serial buffer
  if (Serial.available() > 0) {
    // Read the input from the serial buffer and convert it to a float (MPH)
    userMPH = Serial.parseFloat();

    // If the user entered a valid speed, process the input
    if (userMPH > 0) {
      // Calculate pulse interval based on user-defined speed
      pulseIntervalMicros = calculatePulseInterval(userMPH);
      
      // Print the interval and calculated pulses for debugging
      Serial.print("Pulse interval (microseconds): ");
      Serial.println(pulseIntervalMicros);
      Serial.print("Generating pulses for ");
      Serial.print(userMPH);
      Serial.println(" MPH");

      // Generate pulses based on the calculated interval
      generatePulses(pulseIntervalMicros);
    }
  }
}

float calculatePulseInterval(float mph) {
  // Calculate wheel circumference in inches
  float wheelCircumferenceInches = pi * wheelDiameterInches;

  // Convert MPH to inches per minute
  float inchesPerMinute = (mph * 63360) / 60.0; // Convert mph to inches per minute

  // Calculate rotations per minute (RPM)
  float rpm = inchesPerMinute / wheelCircumferenceInches;

  // Calculate pulses per minute
  float pulsesPerMinute = rpm * pulsesPerRotation;

  // Calculate time between pulses (in microseconds)
  float pulseInterval = (60.0 / pulsesPerMinute) * 1000000.0; // Time between pulses in microseconds

  return pulseInterval;
}

void generatePulses(float intervalMicros) {
  // Continuously generate pulses with the specified interval
  while (true) {
    // Send a 5V pulse
    digitalWrite(outputPulsePin, HIGH);
    delayMicroseconds(100); // Short pulse duration

    // Bring the pin low to complete the pulse
    digitalWrite(outputPulsePin, LOW);
    
    // Wait for the specified time before the next pulse
    delayMicroseconds(intervalMicros - 100); // Subtract pulse duration
  }
}
