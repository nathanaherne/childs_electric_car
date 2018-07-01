void setupMotorController() {
  
  #ifdef MCP4XXX
    pinMode (CS, OUTPUT);
    SPI.begin();
  #endif

  #if defined(MOT_SYREN50) || defined(MOT_SABER2x32)
    Serial1.begin(9600);
    ST.setTimeout(driverTimeout); // Controller must recieve commands every X milliseconds or it will stop motors
    ST.setRamping(0);
    ST.autobaud();
  #endif

  #if defined(MOT_SPARK)
    pinMode(mot_pwm1_pin, OUTPUT);
    spark1.attach(mot_pwm1_pin, throttleMin, throttleMax);
    spark2.attach(mot_pwm2_pin, throttleMin, throttleMax);
    spark3.attach(mot_pwm3_pin, throttleMin, throttleMax);
    spark4.attach(mot_pwm4_pin, throttleMin, throttleMax);
  #endif
  
}

void setupGeneralPins() {

  // Setup the input pins for the buttons
  pinMode(MC_forward_pin, INPUT);
  pinMode(MC_reverseEnable_pin, INPUT);
  
}

void setupOtherPeripherals() {

  
}

