void getSetAllLights(int currentThrottle, int targetThrottle, boolean &brakeLightOn, boolean &headLightOn, 
          boolean &indicatorStatus, boolean &indicatorOnL, boolean &indicatorOnR, boolean &hazardOn,
          unsigned int brakeLightInterval_us, unsigned long prev_brakeLightInterval_us, 
          unsigned int headLightInterval_us, unsigned long prev_headLightInterval_us, 
          unsigned int indicatorLightInterval_us, unsigned long prev_indicatorLightInterval_us,
          unsigned long current_us) {

}

// Head lights
void getSetHeadLight(int MC_head, boolean &headLightOn) {
  // If headlights ON
  if (digitalRead(MC_head) == true) {
    digitalWrite(headLight_pin, HIGH);
    headLightOn = true;
  // Headlights must be OFF
  } else {
    digitalWrite(headLight_pin, LOW);
    headLightOn = false;
  }
}

// Brake Lights
void getSetBrakeLight(int currentThrottle, int targetThrottle, boolean &brakeLightOn) {
  
  if (reverseMotorDirection == false && targetThrottle < currentThrottle && currentThrottle > mot_throttleBrake) {
    digitalWrite(brakeLight_pin, HIGH);
    brakeLightOn = true;
  }
  else if (reverseMotorDirection == true && targetThrottle > currentThrottle && currentThrottle < mot_throttleBrake) {
    digitalWrite(brakeLight_pin, HIGH);
    brakeLightOn = true;
  }
  else if (currentThrottle == mot_throttleBrake && targetThrottle == mot_throttleBrake) {
    digitalWrite(brakeLight_pin, HIGH);
    brakeLightOn = true;
  }
  else {
    digitalWrite(brakeLight_pin, LOW);
    brakeLightOn = false;
  }
}

void getSetIndicatorHazard(boolean &indicatorStatus, boolean &indicatorOnL, boolean &indicatorOnR, boolean &hazardOn) {

  indicatorOnL = digitalRead(MC_indicL_pin);
  indicatorOnR = digitalRead(MC_indicR_pin);
  hazardOn = digitalRead(MC_hazard_pin);

  //Left indicator
  if ((indicatorOnL == true || hazardOn == true) && indicatorStatus == LOW) {
    digitalWrite(indicLightL_pin, HIGH);
    indicatorStatus = HIGH;
  }
  else if ((indicatorOnL == true || hazardOn == true) && indicatorStatus == HIGH) {
    digitalWrite(indicLightL_pin, LOW);
    indicatorStatus = LOW;
  } 
  else if (indicatorOnL == false || hazardOn == false){
    digitalWrite(indicLightL_pin, LOW);
  }

  //Right indicator & Hazard
  if ((indicatorOnR == true || hazardOn == true) && indicatorStatus == LOW) {
    digitalWrite(indicLightR_pin, HIGH);
    indicatorStatus = HIGH;
  }
  else if ((indicatorOnR == true || hazardOn == true) && indicatorStatus == HIGH) {
    digitalWrite(indicLightR_pin, LOW);
    indicatorStatus = LOW;
  } 
  else if (indicatorOnR == true || hazardOn == true){
    digitalWrite(indicLightR_pin, LOW);
  }
}
