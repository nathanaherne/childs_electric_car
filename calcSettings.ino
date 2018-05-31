void calcOneTimeVars(){
  
  // Calculations for maxForward
  if (reverseMotorDirection) {
    maxForward = throttleBrake - (((throttleBrake - throttleMin) * maxForwardPercent) / 100); // Based on maxForwardPercent & Throttle Output Value settings
  } else if (!reverseMotorDirection) {
    maxForward = throttleBrake + (((throttleMax - throttleBrake) * maxForwardPercent) / 100); // Based on maxForwardPercent & Throttle Output Value settings
  } else {
    maxForward = throttleBrake;
  }
  
  // Calculation for maxReverse
  if (reverseMotorDirection) {
    maxReverse = throttleBrake + (((throttleMax - throttleBrake) * maxReversePercent) / 100); // Based on maxReversePercent & Throttle Output Value settings
  } else if (!reverseMotorDirection) {
    maxReverse = throttleBrake - (((throttleBrake - throttleMin) * maxReversePercent) / 100); // Based on maxReversePercent & Throttle Output Value settings
  } else {
    maxReverse = throttleBrake;
  }
  
  // Calculation for maxCCForward
  if (reverseMotorDirection) {
    maxCruiseControl = throttleBrake - (((throttleBrake - throttleMin) * maxCruiseControlPercent) / 100); //Based on maxCruiseControlPercent & Throttle Output Value settings
  } else if (!reverseMotorDirection) {
    maxCruiseControl = throttleBrake + (((throttleMax - throttleBrake) * maxCruiseControlPercent) / 100); // Based on maxCruiseControlPercent & Throttle Output Value settings
  } else {
    maxCruiseControl = throttleBrake;
  }
  
  forwardRampInterval = (forwardRampPercent * (throttleRangeMs / (throttleMax - throttleMin)) / 100); // milliseconds between Forward updates
  reverseRampInterval = (reverseRampPercent * (throttleRangeMs / (throttleMax - throttleMin)) / 100); // milliseconds between Reverse updates
  brakeRampInterval =  (brakeRampPercent * (throttleRangeMs / (throttleMax - throttleMin)) / 100); // milliseconds between Reverse updates
  cruiseControlRampInterval = (cruiseControlRampPercent * (throttleRangeMs / (throttleMax - throttleMin)) / 100); // milliseconds between Reverse updates
  
}
