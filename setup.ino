////////////////////////////////////////////////////////////////////////
// Code that is run solely within the Setup routine
////////////////////////////////////////////////////////////////////////

void setupMotorControllers() {

    // Start the Serial interface for the Syren50
  #if defined(MOT_SYREN50)
    Serial1.begin(9600);
    ST.setTimeout(driverTimeout_us); // Controller must receive commands every driverTimeout_us or it will stop motors
    ST.setRamping(0); //Disable ramping as it is done within this software
    //ST.autobaud(); // Send Autobaud command
  #endif

  // Start the Serial interface for the Sabertooth2x32
  #if defined(MOT_SABER2x32)
    Serial1.begin(9600);
    ST.setTimeout(driverTimeout_us); // Controller must receive commands every X milliseconds or it will stop motors
    ST.setRamping(0); //Disable ramping as it is done within this software
  #endif

  // Setup pins for Spark Motor Controller and different boards
  #if defined(MOT_SPARK)
    #if defined(BOARD_TEENSY3)
      motor1.attach(mot_pwm1_pin, mot_ThrottleMin, mot_ThrottleMax);
    #elif defined(BOARD_MAPLEMINI)
      pwmTimer2.pause();
      pwmDutyMax = pwmTimer2.setPeriod(pwmPeriod_us);
      pwmTimer2.refresh();
      pwmTimer2.resume();
    #endif
  #endif
  
}

void setupPeripherals() {

  #if defined(DEBUG)
    Serial.begin(9600);
  #endif

  // Setup serial port for OLED -> data sent to Cruisin OLED board
  if (oledFeature == 1) {
    Serial3.begin(9600);
  }
  
}

// Setup any pins
void setupPins() {

  if (inhibitFeature == 1) {
    pinMode(inhibit_pin, INPUT);
  }
  
  // Setup non configurable pins
  pinMode(headLight_pin, OUTPUT);
  pinMode(indicLightL_pin, OUTPUT);
  pinMode(indicLightR_pin, OUTPUT);
  pinMode(brakeLight_pin, OUTPUT);
  pinMode(onBoardLed_pin, OUTPUT);

  // Setup the pins for manual control
  if (inputControlFeature == 1) {
    #if defined(BOARD_PROMINI)
      pinMode(MC_throttle_pin, INPUT);
      pinMode(MC_reverse_pin, INPUT);
      pinMode(MC_cruiseControl_pin, INPUT);
      pinMode(MC_brake_pin, INPUT);
      pinMode(MC_horn_pin, INPUT);
      pinMode(MC_indicL_pin, INPUT);
      pinMode(MC_indicR_pin, INPUT);
      pinMode(MC_head_pin, INPUT);
      pinMode(MC_hazard_pin, INPUT);
    #elif defined(BOARD_MAPLEMINI)
      pinMode(MC_throttle_pin, INPUT_ANALOG);
      pinMode(MC_reverse_pin, INPUT);
      pinMode(MC_cruiseControl_pin, INPUT);
      pinMode(MC_brake_pin, INPUT);
      pinMode(MC_horn_pin, INPUT);
      pinMode(MC_indicL_pin, INPUT);
      pinMode(MC_indicR_pin, INPUT);
      pinMode(MC_head_pin, INPUT);
      pinMode(MC_hazard_pin, INPUT);
    #elif defined(BOARD_TEENSY3)
      pinMode(MC_throttle_pin, INPUT);
      pinMode(MC_reverse_pin, INPUT);
      pinMode(MC_cruiseControl_pin, INPUT);
      pinMode(MC_brake_pin, INPUT);
      pinMode(MC_horn_pin, INPUT);
      pinMode(MC_indicL_pin, INPUT);
      pinMode(MC_indicR_pin, INPUT);
      pinMode(MC_head_pin, INPUT);
      pinMode(MC_hazard_pin, INPUT);
    #elif defined(BOARD_STM32F406VET6)
      pinMode(MC_throttle_pin, INPUT);
      pinMode(MC_reverse_pin, INPUT);
      pinMode(MC_cruiseControl_pin, INPUT);
      pinMode(MC_brake_pin, INPUT);
      pinMode(MC_horn_pin, INPUT);
      pinMode(MC_indicL_pin, INPUT);
      pinMode(MC_indicR_pin, INPUT);
      pinMode(MC_head_pin, INPUT);
      pinMode(MC_hazard_pin, INPUT);
    #endif
  }

  // Enable pins for Spark Motor Controller
  #if defined(MOT_SPARK)
    #if defined(BOARD_PROMINI)
      pinMode(mot_pwm1_pin, OUTPUT);
    #elif defined(BOARD_MAPLEMINI)
      pinMode(mot_pwm1_pin, PWM);
      pinMode(mot_pwm2_pin, PWM);
      pinMode(mot_pwm3_pin, PWM);
      pinMode(mot_pwm4_pin, PWM);
    #elif defined(BOARD_TEENSY3)
      pinMode(mot_pwm1_pin, OUTPUT);
      pinMode(mot_pwm2_pin, OUTPUT);
      pinMode(mot_pwm3_pin, OUTPUT);
      pinMode(mot_pwm4_pin, OUTPUT);
    #elif defined(BOARD_STM32F406VET6)
      pinMode(mot_pwm1_pin, OUTPUT);
      pinMode(mot_pwm2_pin, OUTPUT);
      pinMode(mot_pwm3_pin, OUTPUT);
      pinMode(mot_pwm4_pin, OUTPUT);
    #endif
  #endif

}

