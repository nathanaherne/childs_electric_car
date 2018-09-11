////////////////////////////////////////////////////////////////////////
// Calculate the currentThrottle percent
void calcCurrentThrottlePercent(int currentThrottle, int &currentThrottlePercent, int mot_throttleBrake, int minForward, int maxForward) {
  currentThrottlePercent = mapfloat(currentThrottle, minForward, maxForward, 0, 100);
  if (reverseMotorDirection == true && currentThrottle <= mot_throttleBrake && currentThrottlePercent < 0) {
    currentThrottlePercent = 0;
  }
  else if (reverseMotorDirection == false && currentThrottle >= mot_throttleBrake && currentThrottlePercent < 0) {
    currentThrottlePercent = 0;
  }
}

////////////////////////////////////////////////////////////////////////
// Function for Mapping float values
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

////////////////////////////////////////////////////////////////////////
// Debounce Digital Inputs
void debounce(int debounceMax, boolean pinValue, boolean &calcedPinValue, int &integrator) {

  /* Step 1 */
  if (!(pinValue)) {
    if (integrator > 0) {
      integrator--;
    }
  }
  else if (integrator < debounceMax) {
    integrator++;
  }

  /* Step 2 */
  if (integrator == 0) {
    calcedPinValue = false;
  }
  else if (integrator >= debounceMax) {
    calcedPinValue = true;
    integrator = 0;
  }
}

void sendThrottleToMotorController(int currentThrottle){
  // Send throttle command to Syren50
  #if defined(MOT_SYREN50)
    ST.motor(1, currentThrottle);
  #endif

  #if defined(MOT_SABER2x32)
    ST.motor(1, currentThrottle);
    ST.motor(2, -currentThrottle);
  #endif

  #if defined(MOT_SPARK)
    #if defined(BOARD_TEENSY3)
      motor1.write(currentThrottle); 
    #elif defined(BOARD_MAPLEMINI)
      int pwmThrottle = mapfloat(currentThrottle, 0, pwmPeriod_us, pwmDutyMin, pwmDutyMax);
      pwmWrite(mot_pwm1_pin, pwmThrottle);
    #endif
  #endif

}

////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
void oledSendData(int batteryPercent, int batteryVoltageWarningPercent, int currentThrottlePercent, 
        int batteryCurrent, boolean cruiseControlOn, boolean brakeLightOn, boolean headLightOn, boolean indicatorOnL, boolean indicatorOnR,
        boolean hazardOn, boolean forwardError, boolean reverseError, boolean cruiseControlError, boolean driveInhibited) {

    int OledData[] = {batteryPercent, batteryVoltageWarningPercent, currentThrottlePercent, batteryCurrent, currentWarningAmps, cruiseControlOn, 
            brakeLightOn, headLightOn, indicatorOnL, indicatorOnR, hazardOn, forwardError, reverseError, cruiseControlError, driveInhibited};
  
    int sizeOfArray = sizeof(OledData) / sizeof(OledData[0]);
  
    //Serial.print("sizeOfArray: ");Serial.println(sizeOfArray);
  
    for (int i = 0; i < sizeOfArray; i++){
      if (i == 0) {
        Serial3.print("<");//Serial.print("<");
      }
      Serial3.print(OledData[i]);//Serial.print(OledData[i]);
      if (i == (sizeOfArray - 1)) {
        Serial3.print(">");//Serial.print(">");
      } else {
        Serial3.print(",");//Serial.print(",");
      }
    }
    //Serial.println();
}

