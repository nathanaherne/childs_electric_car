#if defined(Debug)
void printStartupDebug() {
  // Configure serial port to send debugging information
  Serial.begin(9600);
  // Print some information to the Serial Port
  Serial.println();
  Serial.print("******************************************************************************************");Serial.println();
    if (reverseMotorDirection) {
      Serial.print("Motor Direction Reversed: ");Serial.print("True");Serial.println();
    } else {
      Serial.print("Motor Direction Reversed: ");Serial.print("False");Serial.println();
    }
    if (enableCruiseControl) {
      Serial.print("Cruise Control Enabled: ");Serial.print("True");Serial.println();
    } else {
      Serial.print("Cruise Control Enabled: ");Serial.print("False");Serial.println();
    }
  Serial.print("--------------------------------------------");Serial.println();
  Serial.print("Maximum Forward Throttle Percent: ");Serial.print(maxForwardPercent);Serial.print("%");Serial.println();
  Serial.print("Maximum Forward Throttle Output: ");Serial.print(maxForward);Serial.println();
  Serial.print("Forward Ramp Percent: ");Serial.print(forwardRampPercent);Serial.print("%");Serial.println();
  Serial.print("Forward Ramp Inverval: ");Serial.print(forwardRampInterval);Serial.print(" milliseconds");Serial.println();
  Serial.print("--------------------------------------------");Serial.println();
  Serial.print("Maximum Reverse Throttle Percent: ");Serial.print(maxReversePercent);Serial.print("%");Serial.println();
  Serial.print("Maximum Reverse Throttle Output: ");Serial.print(maxReverse);Serial.println();
  Serial.print("Reverse Ramp Percent: ");Serial.print(reverseRampPercent);Serial.print("%");Serial.println();
  Serial.print("Reverse Ramp Inverval: ");Serial.print(reverseRampInterval);Serial.print(" milliseconds");Serial.println();
  Serial.print("--------------------------------------------");Serial.println();
  Serial.print("Maximum Cruise Control Throttle Percent: ");Serial.print(maxCruiseControlPercent);Serial.print("%");Serial.println();
  Serial.print("Maximum Cruise Control Throttle Output: ");Serial.print(maxCruiseControl);Serial.println();
  Serial.print("Cruise Control Ramp Percent: ");Serial.print(cruiseControlRampPercent);Serial.print("%");Serial.println();
  Serial.print("CruiseControl Ramp Inverval: ");Serial.print(cruiseControlRampInterval);Serial.print(" milliseconds");Serial.println();
  Serial.print("--------------------------------------------");Serial.println();
  Serial.print("Brake Value: ");Serial.print(throttleBrake);Serial.println();
  Serial.print("Brake Ramp Percent: ");Serial.print(brakeRampPercent);Serial.print("%");Serial.println();
  Serial.print("Brake Ramp Interval: ");Serial.print(brakeRampInterval);Serial.print(" milliseconds");Serial.println();
  Serial.print("******************************************************************************************");Serial.println();
  delay(500);
}
#endif
