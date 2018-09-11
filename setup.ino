void setupMotorController() {
  
  #if defined(MCP4XXX)
    pinMode (CS, OUTPUT);
    SPI.begin();
  #endif

  #if defined(MOT_SYREN50) || defined(MOT_SABER2x32)
    Serial2.begin(9600);
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

  #if defined(OLED)
    Serial3.begin(9600);
  #endif
  
}

void setupGeneralPins() {

  // Input pins
  pinMode(MC_forward_pin, INPUT_ANALOG);
  pinMode(MC_reverseEnable_pin, INPUT);
  pinMode(MC_headlight_pin, INPUT);
  pinMode(inhibit_pin, INPUT);

  // Output pins
  pinMode(headLight_pin, OUTPUT);
  pinMode(brakeLight_pin, OUTPUT);
  pinMode(errorLed_pin, OUTPUT);
  pinMode(eBrake, OUTPUT);
  
}

void setupOtherPeripherals() {

  
}

