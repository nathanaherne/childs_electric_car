// Required files
#include "settings.h"
#include "config.h"
#include "pins.h"


void setup() {

  // Setup motor controllers
  setupMotorControllers();
  
  // Stop motor(s)
  sendThrottleToMotorController(mot_throttleBrake);

  // Setup other peripherals
  setupPeripherals();

  // Setup any pins required
  setupPins();
  
}

void loop() {

  // Timing variables
  static unsigned long current_us;
  static unsigned long prev_current_us; // Used for loop timing

  prev_current_us = current_us;
  current_us = micros();

  // Variables to control timing of functions and updates to other variables
  static unsigned long manualControlDebounceInterval_us = (1000000 / debounceSampleFrequency_hz);
  static unsigned long prev_manualControlDebounceInterval_us;
  static unsigned long cruiseControlThrottleChangeInterval_us = 400000; // _us between cruise control throttle changes
  static unsigned long prev_cruiseControlThrottleChangeInterval_us;
  static unsigned long oledSerialInterval_us = 200000; // _us between sending data to OLED
  static unsigned long prev_oledSerialInterval_us;
  static unsigned long brakeLightInterval_us = 100000; // _us between checking brake light status
  static unsigned long prev_brakeLightInterval_us;
  static unsigned long headLightInterval_us = 100000; // _us between checking headlight status
  static unsigned long prev_headLightInterval_us;
  static unsigned long indicatorLightInterval_us = 100000; // _us between checking indicator status
  static unsigned long prev_indicatorLightInterval_us;
  static unsigned long currentThrottlePercentInterval_us = 100000; // _us between currentThrottlePercent calculations
  static unsigned long prev_currentThrottlePercentInterval_us;
  static unsigned long batteryInterval_us = 1000000; // _us between battery calculations
  static unsigned long prev_batteryInterval_us;
  static unsigned long debugPrintInterval_us = 100000; // _us between debug prints
  static unsigned long prev_debugPrintInterval_us;
  static unsigned long cruiseControlStatusChangeInterval_us = 500000; // _us between cruise control status changes
  static unsigned long prev_cruiseControlStatusChangeInterval_us; // last time Cruise Control status was updated
  static unsigned long onBoardLedInterval_us = 1000000; // us between onboard LED changes
  static unsigned long prev_onBoardLedInterval_us = 0; // last time LED was updated

  // Ramping intervals (in microseconds)
  static unsigned long forwardRampInterval;
  static unsigned long prev_forwardRampInterval = 0; // the last time Forward throttle was updated
  static unsigned long reverseRampInterval;
  static unsigned long prev_reverseRampInterval = 0; // the last time Reverse throttle was updated
  static unsigned long brakeRampInterval;
  static unsigned long prev_brakeRampInterval = 0; // will store last time Brake throttle was updated
  static unsigned long cruiseControlRampInterval;
  static unsigned long prev_cruiseControlRampInterval = 0; // the last time Cruise Control throttle was updated

  // System Variables
  static boolean driveInhibited; // Set by status of inhibit pin

  // Manual Input validation variables
  static boolean manualControlMapping = false; // Set the manual control mappings
  static boolean forwardError = true; // default error
  static boolean reverseError = true; // default error
  static boolean cruiseControlError = true; // default error

  // Motion Variables
  static boolean onDemandVarsCalced = false; // Only need to be calculated when settings are changed
  static int maxForward;
  static int minForward;
  static int maxReverse;
  static int minReverse;
  static int maxCruiseControl;
  static int minCruiseControl; // Throttle must be over this value for Cruise Control to be enabled

  // Throttle variables
  static int targetThrottle = mot_throttleBrake;
  static int currentThrottle = targetThrottle; // Tracks targetThrottle based on ramp intervals and Cruise Control
  static int currentThrottlePercent; // Value used for OLED display
  static int throttleDeadbandCenter; // calculated using throttleDeadband_min_Percent
  static int throttleDeadbandMax; // calculated using throttleDeadband_max_Percent
  static int driverDeadbandCenterHalf; // calculated using removeDriverDeadbandPercent (specific for each driver)

  // Control variables used by all input modes
  static int inputThrottle; // Throttle value
  static boolean inputThrottleDigital; // Digital forward command value -> used for Cruise Control on button
  static int inputThrottleDigitalIntegrator; // Used to debounce inputThrottleDigital
  static int forwardThrottle; // Calculated
  static boolean inputReverseEnable; // Digital value
  static int inputReverseEnableIntegrator; // Used to debounce inputReverseEnable
  static int reverseThrottle; // calculated based on inputThrottle and reverseEnable
  static boolean inputCruiseControl; // Digital command value
  static int inputCruiseControlIntegrator; // Used to debounce inputCruiseControl
  static boolean noMotionCommanded; // When no motion inputs are detected -> calculated value
  static boolean inputBrake; // Digital value
  static int inputBrakeIntegrator; // Used to debounce inputBrake

  static int debounceMax; // Used for debouncing button presses (seconds)

  // Variables for lights
  static boolean indicatorStatus = LOW;
  static boolean indicatorOnL = false;
  static boolean indicatorOnR = false;
  static boolean hazardOn = false;
  static boolean headLightOn = false;
  static boolean brakeLightOn = false;

  // Cruise Control Variables
  static boolean cruiseControlOn = false; // true/false
  static unsigned long cruiseControlEnabled_us = 0; // Time that cruiseControl was enabled
  static unsigned long throttleFirstCommanded_us = 0; // The time Throttle first commanded
  static boolean cruiseControlFirstEnabled = false;
  static unsigned long cruiseControl_MC_throttle_EnableDelay_us = cruiseControlForwardThrottleEnableDelay_Sec * 1000000; // convert Seconds to us
  static unsigned long cruiseControl_MC_throttle_DisableDelay_us = cruiseControlForwardThrottleDisableDelay_Sec * 1000000; // convert Seconds to us

  // Battery Variables
  static int batteryVoltage; // Battery voltage read from pin
  static float batteryPercent; // Calculated battery percent
  static int batteryCurrent; // battery current read from pin
  static float batteryMinVoltage; // calculated minVoltage
  static float batteryMaxVoltage; // calculated maxVoltage

  // Manual Control Variables, which are mapped to pins
  static int MC_throttle;
  static int MC_reverse;
  static int MC_cruiseControl;
  static int MC_brake;
  static int MC_horn; // Horn (digital)
  static int MC_indicL; // Indicator1 (digital)
  static int MC_indicR; // Indicator2 (digital)
  static int MC_head; // Headlights (digital)
  static int MC_hazard; // Other (analog)

  // OLED SHOWS STARTUP INFORMATION UNTIL IT GETS A VALID SIGNAL FROM THIS SOFTWARE

  // Validate manual input mappings
  if (manualControlMapping == false) {
    setManualControlMapping(MC_throttle, MC_reverse, MC_brake, MC_cruiseControl, MC_horn, MC_indicL, MC_indicR, MC_head, MC_hazard);
    manualControlMapping = true;
  }

  // Check if drive has been inhibited by switch, charger, panic button etc.
  if (inhibitFeature == 1) {
    driveInhibited = digitalRead(inhibit_pin);
    while (driveInhibited == LOW) {
      sendThrottleToMotorController(mot_throttleBrake); // Send brake to motorController
      if (oledFeature == 1) {
        oledSendData(batteryPercent, batteryVoltageWarningPercent, currentThrottlePercent, 
          batteryCurrent, cruiseControlOn, brakeLightOn, headLightOn, indicatorOnL, indicatorOnR,
          hazardOn, forwardError, reverseError, cruiseControlError, driveInhibited);
      }
      #if defined(DEBUG)
        Serial.println("Drive Inhibited");
      #endif
    }
  }

  // Calculate on demand variables
  if (onDemandVarsCalced == false) {
    calcDeadband(throttleDeadbandCenter, throttleDeadbandMax, driverDeadbandCenterHalf);
    calcForwardReverseValues(maxForward, minForward, maxReverse, minReverse, driverDeadbandCenterHalf);
    calcRampIntervals(forwardRampInterval, reverseRampInterval, brakeRampInterval, cruiseControlRampInterval);
    calcCruiseControlValues(maxCruiseControl, minCruiseControl);
    calcBatteryMinMax(batteryMinVoltage, batteryMaxVoltage);
    calcDebounceMax(debounceMax);
    onDemandVarsCalced = true;
  }

////////////////////////////////////////////////////////////////////////
// LIGHTS
  // Headlights -> only update per headLightInterval_us
  if ((current_us - prev_headLightInterval_us) >= headLightInterval_us) {
    getSetHeadLight(MC_head, headLightOn);
  }
  // Brake Lights -> only update per brakeLightInterval_us
  if ((current_us - prev_brakeLightInterval_us) >= brakeLightInterval_us) {
    getSetBrakeLight(currentThrottle, targetThrottle, brakeLightOn); 
  }
  // Set indicators if turned on
  if ((current_us - prev_brakeLightInterval_us) >= brakeLightInterval_us) {
    getSetIndicatorHazard(indicatorStatus, indicatorOnL, indicatorOnR, hazardOn); 
  }
////////////////////////////////////////////////////////////////////////

  // Manual Control inputs
  getManualControlAnalog(MC_throttle, inputThrottle);
  getManualControlDigital(MC_throttle, MC_reverse, MC_brake, MC_cruiseControl,
        debounceMax, inputThrottleDigital, inputThrottleDigitalIntegrator,
        inputReverseEnable, inputReverseEnableIntegrator, inputBrake, inputBrakeIntegrator,
        inputCruiseControl, inputCruiseControlIntegrator,
        current_us, manualControlDebounceInterval_us, prev_manualControlDebounceInterval_us);
  calcManualControlCommands(inputThrottle, inputThrottleDigital, forwardThrottle, reverseThrottle, inputReverseEnable, 
        inputBrake, noMotionCommanded, inputCruiseControl, cruiseControlOn, 
        throttleDeadbandCenter, throttleDeadbandMax);

////////////////////////////////////////////////////////////////////////
// CRUISE CONTROL
  // If Cruise Control enabled by throttle
  if (cruiseControlFeature == 1 && (current_us - prev_cruiseControlStatusChangeInterval_us) >= cruiseControlStatusChangeInterval_us) {
        cruiseControlThrottleEnable(targetThrottle, currentThrottle, forwardThrottle, reverseThrottle, inputReverseEnable,
          noMotionCommanded, inputBrake, cruiseControlOn,
          cruiseControlEnabled_us, throttleFirstCommanded_us, cruiseControl_MC_throttle_EnableDelay_us, 
          cruiseControl_MC_throttle_DisableDelay_us, current_us);
  }
  // If Cruise Control is enabled by its own button
  else if (cruiseControlFeature == 2 && (cruiseControlOn == true || inputCruiseControl == true || inputBrake == true)) {
        cruiseControlButton(currentThrottle, cruiseControlOn, minCruiseControl, cruiseControlFirstEnabled, inputCruiseControl, inputBrake,
        current_us, cruiseControlStatusChangeInterval_us, prev_cruiseControlStatusChangeInterval_us);
  }
////////////////////////////////////////////////////////////////////////

  // Calculate the Target throttle
  calcTargetThrottle(targetThrottle, currentThrottle, forwardThrottle, inputThrottleDigital, 
      reverseThrottle, inputReverseEnable, noMotionCommanded, inputBrake, 
      cruiseControlOn, cruiseControlFirstEnabled,
      maxForward, minForward, maxReverse, minReverse, maxCruiseControl,
      current_us, cruiseControlThrottleChangeInterval_us, prev_cruiseControlThrottleChangeInterval_us);

  // Calculate motion all inputs and settings
  calcMotion(targetThrottle, forwardThrottle, reverseThrottle, noMotionCommanded, inputBrake,
      cruiseControlOn, currentThrottle,
      maxForward, prev_forwardRampInterval, forwardRampInterval,
      maxReverse, prev_reverseRampInterval, reverseRampInterval,
      maxCruiseControl, prev_cruiseControlRampInterval, cruiseControlRampInterval,
      mot_throttleBrake, prev_brakeRampInterval, brakeRampInterval,
      driverDeadbandCenterHalf, current_us);

  // Check Battery Details
  if ((current_us - prev_batteryInterval_us) >= batteryInterval_us) {
    getBatteryDetails(batteryVoltage, batteryPercent, batteryMinVoltage, batteryMaxVoltage);
    prev_batteryInterval_us = current_us;
  }

  // Only send to OLED per interval
  if (oledFeature == 1 && (current_us - prev_oledSerialInterval_us) >= oledSerialInterval_us) {
      // Calculate currentThrottlePercent for OLED
      calcCurrentThrottlePercent(currentThrottle, currentThrottlePercent, mot_throttleBrake, minForward, maxForward);
      // Send data to OLED
      oledSendData(batteryPercent, batteryVoltageWarningPercent, currentThrottlePercent, 
            batteryCurrent, cruiseControlOn, brakeLightOn, headLightOn, indicatorOnL, indicatorOnR,
            hazardOn, forwardError, reverseError, cruiseControlError, driveInhibited);
    prev_oledSerialInterval_us = current_us;
    }

  // Turn the LED from off to on, or on to off
  #if defined(BOARD_MAPLEMINI)
    if (current_us - prev_onBoardLedInterval_us > onBoardLedInterval_us) {
      digitalWrite(onBoardLed_pin, !digitalRead(onBoardLed_pin));
      prev_onBoardLedInterval_us = current_us;
    }
  #endif

  #if defined(DEBUG)
    unsigned long looptime = current_us - prev_current_us;
    if ((current_us - prev_debugPrintInterval_us) >= debugPrintInterval_us) {
      // Calculate currentThrottlePercent for DEBUG
      calcCurrentThrottlePercent(currentThrottle, currentThrottlePercent, mot_throttleBrake, minForward, maxForward);
      if (cruiseControlFeature == 1) {
        if (throttleFirstCommanded_us > 0 && cruiseControlOn == false) {
          Serial.print("CC in: ");Serial.print(cruiseControl_MC_throttle_EnableDelay_us - (current_us - throttleFirstCommanded_us));Serial.print(" ");
        } else if (cruiseControlOn == true) {
          Serial.print("CC ON ");Serial.print(" ");
        } else {
          Serial.print("CC in: ");Serial.print(cruiseControl_MC_throttle_EnableDelay_us);Serial.print(" ");
        }
        Serial.print("CCO : ");Serial.print(cruiseControlOn);Serial.print(" ");
        Serial.print("FFCM : ");Serial.print(throttleFirstCommanded_us);Serial.print(" || ");
      }
      else {
        Serial.print("CCO : ");Serial.print(cruiseControlOn);Serial.print(" || ");
      }
      
      Serial.print("IT: ");Serial.print(inputThrottle);Serial.print(" ");
      Serial.print("ITD: ");Serial.print(inputThrottleDigital);Serial.print(" ");
      Serial.print("F: ");Serial.print(forwardThrottle);Serial.print(" ");
      Serial.print("R: ");Serial.print(reverseThrottle);Serial.print(" ");
      Serial.print("RE: ");Serial.print(inputReverseEnable);Serial.print(" ");
      Serial.print("NI: ");Serial.print(noMotionCommanded);Serial.print(" ");
      Serial.print("B: ");Serial.print(inputBrake);Serial.print(" ");
      Serial.print("CC : ");Serial.print(inputCruiseControl);Serial.print(" || ");
      Serial.print("Tar: ");Serial.print(targetThrottle);Serial.print(" ");
      Serial.print("Cur: ");Serial.print(currentThrottle);Serial.print(" ");
      Serial.print("CurP: ");Serial.print(currentThrottlePercent);Serial.print(" || ");
      
//      Serial.print("FRI: ");Serial.print(forwardRampInterval);Serial.print(" ");
//      Serial.print("RRI: ");Serial.print(reverseRampInterval);Serial.print(" ");
//      Serial.print("CCRI: ");Serial.print(cruiseControlRampInterval);Serial.print(" ");
//      Serial.print("BRI: ");Serial.print(brakeRampInterval);Serial.print(" || ");
      
      Serial.print("TDB: ");Serial.print(throttleDeadbandCenter);Serial.print(" ");
      Serial.print("DDB: ");Serial.print(removeDriverDeadbandPercent);Serial.print(" || ");

//      Serial.print("forwardRampInterval: ");Serial.print(forwardRampInterval);Serial.print(" ");
//      Serial.print("reverseRampInterval: ");Serial.print(reverseRampInterval);Serial.print(" ");
//      Serial.print("brakeRampInterval: ");Serial.print(brakeRampInterval);Serial.print(" ");
//      Serial.print("cruiseControlRampInterval: ");Serial.print(cruiseControlRampInterval);Serial.print(" || ");
      
      Serial.print("LT: ");Serial.print(looptime);Serial.print(" ");
      
      Serial.println();
      prev_debugPrintInterval_us = current_us;
    }
  #endif

} // end of loop function

// If Cruise Control is enabled by button
void cruiseControlButton(int currentThrottle, boolean &cruiseControlOn, int minCruiseControl, boolean &cruiseControlFirstEnabled, boolean inputCruiseControl,
        boolean inputBrake, unsigned long current_us, unsigned int cruiseControlStatusChangeInterval_us, unsigned long &prev_cruiseControlStatusChangeInterval_us) {

    //If Cruise Control is ON
    if (cruiseControlOn == true) {
      // Disable IF -> Can disable at any time with inputBrake
      if (inputBrake == true) {
          cruiseControlOn = false;
          prev_cruiseControlStatusChangeInterval_us = current_us; // reset
      }
      // Restrict changing from on to off too quickly
      else if ((current_us - prev_cruiseControlStatusChangeInterval_us) >= cruiseControlStatusChangeInterval_us) {
        if (inputCruiseControl == true) {
            cruiseControlOn = false;
            prev_cruiseControlStatusChangeInterval_us = current_us; // reset
        }
        // Disable if
        else if (currentThrottle < minCruiseControl && reverseMotorDirection == false) {
            cruiseControlOn = false;
            prev_cruiseControlStatusChangeInterval_us = current_us; // reset
        }
        // Disable if
        else if (currentThrottle > minCruiseControl && reverseMotorDirection == true) {
            cruiseControlOn = false;
            prev_cruiseControlStatusChangeInterval_us = current_us; // reset
        }
      }
    }
    // If Cruise Control is OFF
    else if (cruiseControlOn == false) {
      // If CruiseControl button pushed
      if (inputCruiseControl == true && ((current_us - prev_cruiseControlStatusChangeInterval_us) >= cruiseControlStatusChangeInterval_us)) {
        // Enable if
        if (currentThrottle > minCruiseControl && reverseMotorDirection == false) {
            cruiseControlOn = true;
            cruiseControlFirstEnabled = true;
            prev_cruiseControlStatusChangeInterval_us = current_us;
        }
        // Enable if
        else if (currentThrottle < minCruiseControl && reverseMotorDirection == true) {
            cruiseControlOn = true;
            cruiseControlFirstEnabled = true;
            prev_cruiseControlStatusChangeInterval_us = current_us;
        }
    }
  }
}

// Cruise Control enabled by Throttle
void cruiseControlThrottleEnable(int targetThrottle, int currentThrottle, int inputThrottle, int reverseThrottle, boolean inputReverseEnable, 
          boolean noMotionCommanded, boolean inputBrake, boolean &cruiseControlOn,
          unsigned long &cruiseControlEnabled_us, unsigned long &throttleFirstCommanded_us, unsigned int cruiseControl_MC_throttle_EnableDelay_us, 
          unsigned int cruiseControl_MC_throttle_DisableDelay_us, unsigned long current_us) {

    // Check that targetThrottle is positive and inputThrottle > 0
    // targetThrottle can be negative when reverseMotorDirection = true
    if (abs(targetThrottle) > 0 && inputThrottle > 0) {
      // Only set throttleFirstCommanded_us the first time inputThrottle > 0
      if (throttleFirstCommanded_us == 0) {
          throttleFirstCommanded_us = current_us;
      }
       // Is throttleFirstCommanded_us >= cruiseControlWait_us
      else if (throttleFirstCommanded_us > 0 && cruiseControlOn == false && (current_us - throttleFirstCommanded_us >= cruiseControl_MC_throttle_EnableDelay_us)) {
        cruiseControlOn = true; // Enabled Cruise Control
        cruiseControlEnabled_us = current_us; // Set the time when Cruise Control was enabled
      }
      // Disable Cruise Control if cruiseControlOn, Forward commanded (see above) and cruiseControl_MC_throttle_DisableDelay_us has elapsed
      else if (cruiseControlOn == true && (current_us - cruiseControlEnabled_us) >= cruiseControl_MC_throttle_DisableDelay_us) {
        cruiseControlOn = false; // Disable Cruise Control
        throttleFirstCommanded_us = 0; // Reset to zero
        cruiseControlEnabled_us = 0; // Reset to zero
      }
    }
    // Disable Cruise Control when Brake or Reverse is commanded
    else if (cruiseControlOn == true && (inputBrake == true || reverseThrottle > 0 || inputReverseEnable == true)) {
      cruiseControlOn = false; // Disable Cruise Control
      throttleFirstCommanded_us = 0; // Reset to zero
      cruiseControlEnabled_us = 0; // Reset to zero
    }
    else if (targetThrottle == mot_throttleBrake || reverseThrottle > 0 || inputBrake == true || inputReverseEnable == true) {
      cruiseControlOn = false; // Disable Cruise Control
      throttleFirstCommanded_us = 0; // Reset to zero
    }

}

// Calculate the target throttle
void calcTargetThrottle(int &targetThrottle, int currentThrottle, int forwardThrottle, boolean inputThrottleDigital, 
      int reverseThrottle, boolean inputReverseEnable, boolean noMotionCommanded, boolean inputBrake, 
      boolean cruiseControlOn, boolean &cruiseControlFirstEnabled,
      int maxForward, int minForward, int maxReverse, int minReverse, int maxCruiseControl,
      unsigned long current_us, unsigned int cruiseControlThrottleChangeInterval_us, unsigned long &prev_cruiseControlThrottleChangeInterval_us) {

  // If cruise control is on
  if (cruiseControlOn == true) {
    // If Cruise Control is enabled by Forward
    if (cruiseControlFeature == 1) {
      targetThrottle = maxCruiseControl;
    }
    // If Cruise Control enabled by switch
    else if (cruiseControlFeature == 2) {
      // First time Cruise Control enabled
      if (cruiseControlFirstEnabled == true) {
        targetThrottle = currentThrottle;
        // Make sure targetThrottle is never higher than maxCruiseControl
        if (targetThrottle > maxCruiseControl && reverseMotorDirection == false) {
          targetThrottle = maxCruiseControl;
        }
        // Make sure targetThrottle is never higher than maxCruiseControl
        else if (targetThrottle < maxCruiseControl && reverseMotorDirection == true) {
          targetThrottle = maxCruiseControl;
        }
        cruiseControlFirstEnabled = false;
      }
      else if (inputThrottleDigital == true && reverseMotorDirection == false) {
        if ((current_us - prev_cruiseControlThrottleChangeInterval_us) >= cruiseControlThrottleChangeInterval_us) {
          targetThrottle++;
          prev_cruiseControlThrottleChangeInterval_us = current_us;
        }
        // Limit targetThrottle to maxCruiseControl
        if (targetThrottle > maxCruiseControl) {
          targetThrottle = maxCruiseControl;
        }
      }
      else if (inputThrottleDigital == true && reverseMotorDirection == true) {
        if ((current_us - prev_cruiseControlThrottleChangeInterval_us) >= cruiseControlThrottleChangeInterval_us) {
          targetThrottle--;
          prev_cruiseControlThrottleChangeInterval_us = current_us;
        }
        // Limit targetThrottle to maxCruiseControl
        if (targetThrottle < maxCruiseControl) {
          targetThrottle = maxCruiseControl;
        }
      }
      else if (inputReverseEnable == true && reverseMotorDirection == false) {
        if ((current_us - prev_cruiseControlThrottleChangeInterval_us) >= cruiseControlThrottleChangeInterval_us) {
          targetThrottle--;
          prev_cruiseControlThrottleChangeInterval_us = current_us;
        }
      }
      else if (inputReverseEnable == true && reverseMotorDirection == true) {
        if ((current_us - prev_cruiseControlThrottleChangeInterval_us) >= cruiseControlThrottleChangeInterval_us) {
          targetThrottle++;
          prev_cruiseControlThrottleChangeInterval_us = current_us;
        }
      }
    }
  }
  // If forward commanded
  else if (forwardThrottle > 0 && cruiseControlOn == false) {
    targetThrottle = mapfloat(forwardThrottle, analogReadMin, analogReadMax, minForward, maxForward);
  } 
  // If reverse commanded
  else if (reverseThrottle > 0 && cruiseControlOn == false) {
    targetThrottle = mapfloat(reverseThrottle, analogReadMin, analogReadMax, minReverse, maxReverse);
  }
  // If no motion commanded
  else if (noMotionCommanded == true) {
    targetThrottle = mot_throttleBrake;
  }
  // If brake commanded
  else if (inputBrake == true) {
    targetThrottle = mot_throttleBrake;
  }
}

// Calculate motion based on inputs/outputs and other states
void calcMotion(int targetThrottle, int forwardThrottle, int reverseThrottle, boolean noMotionCommanded, boolean inputBrake,
      boolean cruiseControlOn, int &currentThrottle,
      int maxForward, unsigned long &prev_forwardRampInterval, unsigned int forwardRampInterval,
      int maxReverse, unsigned long &prev_reverseRampInterval, unsigned int reverseRampInterval,
      int maxCruiseControl, unsigned long &prev_cruiseControlRampInterval, unsigned int cruiseControlRampInterval,
      int mot_throttleBrake, unsigned long &prev_brakeRampInterval, unsigned int brakeRampInterval,
      int driverDeadbandCenterHalf, unsigned long current_us) {

    // Forward
    if (forwardThrottle > 0 && cruiseControlOn == false) {
      // Forward/Reverse is normal
      if (reverseMotorDirection == false) {
        // currentThrottle is forward value and currentThrottle is forward value -> use forwardRampInterval
        if (currentThrottle < targetThrottle && currentThrottle >= mot_throttleBrake) {
            increaseThrottle(prev_forwardRampInterval, forwardRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        // currentThrottle is reverse value but targetThrottle is forward value -> use brakeRampInterval
        else if (currentThrottle < targetThrottle && currentThrottle < mot_throttleBrake) {
            increaseThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        // Catch scenarios when the currentThrottle is above targetThrottle -> this is usually when switching out of CruiseControl
        else if (currentThrottle > targetThrottle) {
            reduceThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        else if (currentThrottle == targetThrottle) {
            sendThrottleToMotorController(currentThrottle);
        }
      }
      // Forward/Reverse is reversed
      else if (reverseMotorDirection == true) {
        // targetThrottle is forward value and currentThrottle is forward value -> use forwardRampInterval
        if (currentThrottle > targetThrottle && currentThrottle <= mot_throttleBrake) {
            reduceThrottle(prev_forwardRampInterval, forwardRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        // currentThrottle is reverse value and targetThrottle is forward value -> use brakeRampInterval
        else if (currentThrottle > targetThrottle && currentThrottle > mot_throttleBrake) {
            reduceThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        // When the currentThrottle is less than targetThrottle -> this is usually when switching out of CruiseControl
        else if (currentThrottle < targetThrottle) {
            increaseThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        else if (currentThrottle == targetThrottle) {
            sendThrottleToMotorController(currentThrottle);
        }
      }
    }
    // Reverse
    else if (reverseThrottle > 0 && cruiseControlOn == false) {
      // Forward/Reverse is normal
      if (reverseMotorDirection == false) {
        // targetThrottle is reverse value and currentThrottle is reverse value -> use reverseRampInterval
        if (currentThrottle > targetThrottle && currentThrottle <= mot_throttleBrake) {
            reduceThrottle(prev_reverseRampInterval, reverseRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        // targetThrottle is forward value and currentThrottle is reverse value -> use reverseRampInterval
        if (currentThrottle > targetThrottle && currentThrottle > mot_throttleBrake) {
            reduceThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        // Catch scenarios when the currentThrottle is less than targetThrottle
        else if (currentThrottle < targetThrottle) {
            increaseThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        else if (currentThrottle == targetThrottle) {
            sendThrottleToMotorController(currentThrottle);
        }
      }
      // Forward/Reverse is reverse
      else if (reverseMotorDirection == true) {
        // targetThrottle is reverse value and currentThrottle is reverse value -> use reverseRampInterval
        if (currentThrottle < targetThrottle && currentThrottle >= mot_throttleBrake) {
            increaseThrottle(prev_reverseRampInterval, reverseRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        // targetThrottle is reverse value and currentThrottle is forward value -> use brakeRampInterval
        if (currentThrottle < targetThrottle && currentThrottle < mot_throttleBrake) {
            increaseThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        // Catch scenarios when the currentThrottle is larger than targetThrottle
        else if (currentThrottle > targetThrottle) {
            reduceThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        else if (currentThrottle == targetThrottle) {
            sendThrottleToMotorController(currentThrottle);
        }
      }
    }
    // Cruise Control
    else if (cruiseControlOn == true) {
      if (currentThrottle > targetThrottle) {
          reduceThrottle(prev_cruiseControlRampInterval, cruiseControlRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
      }
      else if (currentThrottle < targetThrottle) {
          increaseThrottle(prev_cruiseControlRampInterval, cruiseControlRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
      }
      else if (currentThrottle == targetThrottle) {
          sendThrottleToMotorController(currentThrottle);
      }
    }
    // Brake
    else if (noMotionCommanded == true || inputBrake == true)  {
        if (currentThrottle > targetThrottle) {
            reduceThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        else if (currentThrottle < targetThrottle) {
            increaseThrottle(prev_brakeRampInterval, brakeRampInterval, currentThrottle, targetThrottle, driverDeadbandCenterHalf, current_us);
        }
        else if (currentThrottle == targetThrottle) {
            sendThrottleToMotorController(targetThrottle);
        }
    } 
}

// Increase throttle
void increaseThrottle(unsigned long &prev_us, unsigned int interval_us, int &currentThrottle, int targetThrottle, int driverDeadbandCenterHalf, unsigned long current_us) {

  if (current_us - prev_us >= interval_us) {
    // Remove Driver deadband if removedriverDeadbandPercent is set
    if (currentThrottle > (mot_throttleBrake - driverDeadbandCenterHalf) && currentThrottle < (mot_throttleBrake + driverDeadbandCenterHalf) && targetThrottle > mot_throttleBrake) {
        currentThrottle = (mot_throttleBrake + driverDeadbandCenterHalf);
    }
    // Increase currentThrottle by 1
    for (;currentThrottle <= targetThrottle;) {
      currentThrottle++;
      prev_us = current_us;
      break;
      }
  }
  // Send throttle command even if there is no update -> in essence a keepalive for the motor controller
  sendThrottleToMotorController(currentThrottle);
}

// Reduce throttle
void reduceThrottle(unsigned long &prev_us, unsigned int interval_us, int &currentThrottle, int targetThrottle, int driverDeadbandCenterHalf, unsigned long current_us) {

  if (current_us - prev_us >= interval_us) {
    // Remove Driver deadband if removeDriverDeadbandPercent is set
    if (currentThrottle > (mot_throttleBrake - driverDeadbandCenterHalf) && currentThrottle < (mot_throttleBrake + driverDeadbandCenterHalf) && targetThrottle < mot_throttleBrake) {
        currentThrottle = (mot_throttleBrake - driverDeadbandCenterHalf);
    }
    // Reduce currentThrottle by 1
    for (;currentThrottle >= targetThrottle;) {
      currentThrottle--;
      prev_us = current_us;
      break;
      }
  }
  // Send throttle command even if there is no update -> in essence a keepalive for the motor controller
  sendThrottleToMotorController(currentThrottle);

}
