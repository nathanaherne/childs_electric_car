// Get analog inputs for Manual Control
void getManualControlAnalog(int &MC_throttle, int &inputThrottle) {

  // Maple Mini pin mode change
  #if defined(BOARD_MAPLEMINI)
    pinMode(MC_throttle_pin, INPUT_ANALOG);
  #endif
  // Read throttle
  inputThrottle = analogRead(MC_throttle);
  
}

// Get any digital inputs for Manual Control
void getManualControlDigital(int MC_throttle, int MC_reverse, int MC_brake, int MC_cruiseControl,
        int debounceMax, boolean &inputThrottleDigital, int &inputThrottleDigitalIntegrator,
        boolean &inputReverseEnable, int &inputReverseEnableIntegrator, boolean &inputBrake, int &inputBrakeIntegrator,
        boolean &inputCruiseControl, int &inputCruiseControlIntegrator,
        unsigned long current_us, unsigned long manualControlDebounceInterval_us, unsigned long prev_manualControlDebounceInterval_us) {

  // Get Digital Values and debounce them
  if ((current_us - prev_manualControlDebounceInterval_us) >= manualControlDebounceInterval_us) {
    #if defined(BOARD_MAPLEMINI)
      pinMode(MC_throttle_pin, INPUT);
    #endif
    debounce(debounceMax, digitalRead(MC_throttle), inputThrottleDigital, inputThrottleDigitalIntegrator);
    debounce(debounceMax, digitalRead(MC_reverse), inputReverseEnable, inputReverseEnableIntegrator);
    debounce(debounceMax, digitalRead(MC_brake), inputBrake, inputBrakeIntegrator);
    debounce(debounceMax, digitalRead(MC_cruiseControl), inputCruiseControl, inputCruiseControlIntegrator);
    prev_manualControlDebounceInterval_us = current_us;
  }
  
}

// Calculate Manual Control Commands
void calcManualControlCommands(int &inputThrottle, boolean &inputThrottleDigital, int &forwardThrottle, int &reverseThrottle, boolean &inputReverseEnable,
        boolean &inputBrake, boolean &noMotionCommanded, boolean &inputCruiseControl, boolean &cruiseControlOn,
        int throttleDeadband_min, int throttleDeadband_max) {

  // Remove center deadband on throttle before doing calculations
  if (inputThrottle - throttleDeadband_min < analogReadMin) {
    inputThrottle = analogReadMin;
  }

  // manualControlFeature settings do not have brake set it to false
  if (manualControlFeature == 1 || manualControlFeature == 4) {
    inputBrake = false;
  }

  // Brake Commanded -> hard stop available using inhibit_drive_pin
  if (inputBrake == true) {
    inputBrake = inputBrake; // To be expected
    forwardThrottle = analogReadMin;
    reverseThrottle = analogReadMin;
    noMotionCommanded = false;
    return; // ok to return here
  }
  // Forward commanded (reverse NOT commanded)
  else if (inputThrottle > 0 && inputReverseEnable == false) {
    // Add throttleDeadband_max to inputThrottle and cap throttle to analogReadMax if above 
    if (inputThrottle + throttleDeadband_max >= analogReadMax) {
      forwardThrottle = analogReadMax;
    }
    else {
      forwardThrottle = inputThrottle;
    }
    reverseThrottle = analogReadMin;
    noMotionCommanded = false;
    inputBrake = false;
  }
  // Reverse commanded
  else if (inputThrottle > 0 && inputReverseEnable == true) {
    // Add throttleDeadband_max to inputThrottle and cap throttle to analogReadMax if above
    if (inputThrottle + throttleDeadband_max >= analogReadMax) {
      reverseThrottle = analogReadMax;
    } else {
      reverseThrottle = inputThrottle;
    }
    forwardThrottle = analogReadMin;
    noMotionCommanded = false;
    inputBrake = false;
  }
  
  // Catch where inputThrottle or reverseThrottle is not larger than throttleDeadband_min
  if (inputThrottle == analogReadMin && reverseThrottle == analogReadMin) {
    forwardThrottle = analogReadMin;
    reverseThrottle = analogReadMin;
    noMotionCommanded = true;
    inputBrake = false;
    return;
  }
}
