void setManualControlMapping(int &MC_throttle, int &MC_reverse, int &MC_brake, int &MC_cruiseControl,
      int &MC_horn, int &MC_indicL, int &MC_indicR, int &MC_head, int &MC_hazard){

  MC_throttle = MC_throttle_pin;
  MC_reverse = MC_reverse_pin;
  
  // No Cruise Control
  if (cruiseControlFeature == 0) {
    MC_cruiseControl;
  }
  // Cruise Control enabled by throttle
  else if (cruiseControlFeature == 1) {
    MC_cruiseControl = MC_throttle_pin;
  }
  // Cruise Control enabled by button
  else if (cruiseControlFeature == 2) {
    MC_cruiseControl = MC_cruiseControl_pin;
  }
  else {
    MC_cruiseControl;
  }
  MC_brake = MC_brake_pin;
  
  MC_horn = MC_horn_pin; // Horn (digital)
  MC_indicL = MC_indicL_pin; // Indicator1 (digital)
  MC_indicR = MC_indicR_pin; // Indicator2 (digital)
  MC_head = MC_head_pin; // Headlights (digital)
  MC_hazard = MC_hazard_pin; // Other (analog)
  
}

// Calculate Forward/Reverse values
void calcForwardReverseValues(int &maxForward, int &minForward, int &maxReverse, int &minReverse, int driverDeadbandCenterHalf){
  
  // Calculations for maxForward
  if (reverseMotorDirection == true) {
    maxForward = mot_throttleBrake - (((mot_throttleBrake - mot_throttleMin) * maxForwardThrottlePercent) / 100); // Based on maxForwardThrottlePercent & Throttle Output Value settings
    minForward = mot_throttleBrake - driverDeadbandCenterHalf;
  } else if (reverseMotorDirection == false) {
    maxForward = mot_throttleBrake + (((mot_throttleMax - mot_throttleBrake) * maxForwardThrottlePercent) / 100); // Based on maxForwardThrottlePercent & Throttle Output Value settings
    minForward = mot_throttleBrake + driverDeadbandCenterHalf;
  } else {
    maxForward = mot_throttleBrake;
  }
  
  // Calculation for maxReverse
  if (reverseMotorDirection == true) {
    maxReverse = mot_throttleBrake + (((mot_throttleMax - mot_throttleBrake) * maxReverseThrottlePercent) / 100); // Based on maxReverseThrottlePercent & Throttle Output Value settings
    minReverse = mot_throttleBrake + driverDeadbandCenterHalf;
  } else if (reverseMotorDirection == false) {
    maxReverse = mot_throttleBrake - (((mot_throttleBrake - mot_throttleMin) * maxReverseThrottlePercent) / 100); // Based on maxReverseThrottlePercent & Throttle Output Value settings
    minReverse = mot_throttleBrake - driverDeadbandCenterHalf;
  } else {
    maxReverse = mot_throttleBrake;
  }

//  Serial.print("minForward: ");Serial.print(minForward);Serial.print(" ");
//  Serial.print("maxForward: ");Serial.print(maxForward);Serial.print(" ");
//  Serial.print("minReverse: ");Serial.print(minReverse);Serial.print(" ");
//  Serial.print("maxReverse: ");Serial.print(maxReverse);Serial.print(" ");
//  Serial.println();
}

// Calculate Cruise Control throttle values
void calcCruiseControlValues(int &maxCruiseControl, int &minCruiseControl){

  // Calculation for maxCruiseControl forward throttle && cruiseControlMinThrottle
  if (reverseMotorDirection == true) {
    maxCruiseControl = mot_throttleBrake - (((mot_throttleBrake - mot_throttleMin) * maxCruiseControlThrottlePercent) / 100); //Based on maxCruiseControlThrottlePercent & Throttle Output Value settings
    minCruiseControl = mot_throttleBrake - (((mot_throttleBrake - maxCruiseControl) * minCruiseControlThrottleEnablePercent) / 100);
  } else if (reverseMotorDirection == false) {
    maxCruiseControl = mot_throttleBrake + (((mot_throttleMax - mot_throttleBrake) * maxCruiseControlThrottlePercent) / 100); // Based on maxCruiseControlThrottlePercent & Throttle Output Value settings
    minCruiseControl = mot_throttleBrake + (((maxCruiseControl - mot_throttleBrake) * minCruiseControlThrottleEnablePercent) / 100);
  } else {
    maxCruiseControl = mot_throttleBrake;
  }

//  Serial.print("minCruiseControl: ");Serial.print(minCruiseControl);Serial.print(" ");
//  Serial.print("maxCruiseControl: ");Serial.print(maxCruiseControl);Serial.print(" ");
//  Serial.println();

  
}

// Calculations for ramp intervals
void calcRampIntervals(unsigned long &forwardRampInterval, unsigned long &reverseRampInterval, unsigned long &brakeRampInterval, unsigned long &cruiseControlRampInterval) {
  
  forwardRampInterval = 1000 * float(float(1 - ((float)forwardRampPercent / (float)100)) * float(throttleRange_ms / (mot_throttleMax - mot_throttleMin))); // us between Forward currentThrottle updates
  
  reverseRampInterval = 1000 * float(float(1 - ((float)reverseRampPercent / (float)100)) * float(throttleRange_ms / (mot_throttleMax - mot_throttleMin))); // us between Reverse currentThrottle updates

  brakeRampInterval =  1000 * float(float(1 - ((float)brakeRampPercent / (float)100)) * float(throttleRange_ms / (mot_throttleMax - mot_throttleMin))); // us between Reverse currentThrottle updates
  
  cruiseControlRampInterval = 1000 * float(float(1 - ((float)cruiseControlRampPercent / (float)100)) * float(throttleRange_ms / (mot_throttleMax - mot_throttleMin))); // us between Cruise Control currentThrottle updates

//  Serial.print("forwardRampInterval: ");Serial.print(forwardRampInterval);Serial.print(" ");
//  Serial.print("reverseRampInterval: ");Serial.print(reverseRampInterval);Serial.print(" ");
//  Serial.print("brakeRampInterval: ");Serial.print(brakeRampInterval);Serial.print(" ");
//  Serial.print("cruiseControlRampInterval: ");Serial.print(cruiseControlRampInterval);Serial.print(" ");
//  Serial.println();
  
}

void calcDeadband(int &throttleDeadbandCenter, int &throttleDeadbandMax, int &driverDeadbandCenterHalf) {

  throttleDeadbandCenter = (analogReadMax - analogReadMin) * throttleDeadband_min_Percent / 100;

  throttleDeadbandMax = (analogReadMax - analogReadMin) * throttleDeadband_max_Percent / 100;

  driverDeadbandCenterHalf = round((((mot_throttleMax - mot_throttleMin) * (removeDriverDeadbandPercent / 100)) / 2) - 0.5); // round down

  //Serial.print("driverDeadband: ");Serial.print(driverDeadbandCenterHalf);Serial.println();
}

void calcDebounceMax(int &debounceMax) {

  debounceMax = int(debounce_ms * debounceSampleFrequency_hz) / 1000; // debounceMax is in seconds

  //Serial.print("debounceMax: ");Serial.print(debounceMax);Serial.println();
  
}

