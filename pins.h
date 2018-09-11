////////////////////////////////////////////////////////////////////////
// PIN DEFINITIONS

const int NOT_A_PIN = -1;

////////////////////////////////////////////////////////////////////////
// Manual control input pins
#if defined(BOARD_ARDUINO)
  const int MC_throttle_pin = A0; // Throttle (analog)
  const int MC_reverse_pin = A2; // Reverse Enable (digital)
  const int MC_cruiseControl_pin = 2; // Cruise Control Enable (digital)
  const int MC_brake_pin = A6; // Brake (digital)
  const int MC_indicL_pin = A6; // Indicator1 (digital)
  const int MC_indicR_pin = A6; // Indicator2 (digital)
  const int MC_head_pin = A6; // Headlights (digital)
  const int MC_hazard_pin = A6; // Hazard lights (digital)
  const int MC_horn_pin = A6; // Horn (digital)
#elif defined(BOARD_TEENSY3)
  const int MC_throttle_pin = A0; // Throttle (analog)
  const int MC_reverse_pin = A2; // Reverse Enable (digital)
  const int MC_cruiseControl_pin = 2; // Cruise Control Enable (digital)
  const int MC_brake_pin = A6; // Brake (digital)
  const int MC_indicL_pin = A6; // Indicator1 (digital)
  const int MC_indicR_pin = A6; // Indicator2 (digital)
  const int MC_head_pin = A6; // Headlights (digital)
  const int MC_hazard_pin = A6; // Hazard lights (digital)
  const int MC_horn_pin = A6; // Horn (digital)
#elif defined(BOARD_MAPLEMINI)
  const int MC_throttle_pin = PB0; // Throttle (analog)
  const int MC_reverse_pin = PB2; // Reverse Enable (digital)
  const int MC_cruiseControl_pin = PB6; // Cruise Control Enable (digital)
  const int MC_brake_pin = PA7; // Brake (digital)
  const int MC_indicL_pin = PA14; // Indicator1 (digital)
  const int MC_indicR_pin = PB3; // Indicator2 (digital)
  const int MC_head_pin = PB5; // Headlights (digital)
  const int MC_hazard_pin = PA8; // Hazard lights (digital)
  const int MC_horn_pin = PA13; // Horn (digital)
#elif defined(BOARD_STM32F406VET6)
  const int MC_throttle_pin = PB0; // Throttle (analog)
  const int MC_reverse_pin = PC13; // Reverse Enable (digital)
  const int MC_cruiseControl_pin = PA8; // Cruise Control Enable (digital)
  const int MC_brake_pin = PC14; // Brake (digital)
  const int MC_indicL_pin = PB4; // Indicator1 (digital)
  const int MC_indicR_pin = PB3; // Indicator2 (digital)
  const int MC_head_pin = PC15; // Headlights (digital)
  const int MC_hazard_pin = PA14; // Hazard lights (digital)
  const int MC_horn_pin = PB5; // Horn (digital)
#endif
////////////////////////////////////////////////////////////////////////
// PWM PINS FOR MOTOR CONTROLLERS
#if defined(MOT_SPARK)
  #if defined(BOARD_MAPLEMINI)
    const int mot_pwm1_pin = PA0; // pin1
    const int mot_pwm2_pin = PA1; // pin2
    const int mot_pwm3_pin = PA2; // pin2
    const int mot_pwm4_pin = PA3; // pin2
  #elif defined(BOARD_STM32F406VET6)
    const int mot_pwm1_pin = 4; // pin1
    const int mot_pwm2_pin = 11; // pin2
    const int mot_pwm3_pin = 11; // pin2
    const int mot_pwm4_pin = 11; // pin2
  #endif
#endif

////////////////////////////////////////////////////////////////////////
// INHIBIT PIN -> low equals no drive
#if defined(BOARD_MAPLEMINI)
  int inhibit_pin = PA6;
  
#elif defined(BOARD_STM32F406VET6)
  int inhibit_pin = PE2;
#endif
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// BATTERY AND CURRENT
#if defined(BOARD_MAPLEMINI)
  const int batteryVoltage_pin = PA0;
  
#elif defined(BOARD_STM32F406VET6)
  const int batteryVoltage_pin = PC0;
  const int batteryCurrent_pin = PC1;
#endif
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// OTHER PERIPHERALS AND PINS
#if defined(BOARD_MAPLEMINI)
  const int headLight_pin = NOT_A_PIN;
  const int indicLightL_pin = NOT_A_PIN;
  const int indicLightR_pin = 13;
  const int brakeLight_pin = NOT_A_PIN;
  const int errorLed_pin = NOT_A_PIN;
  const int onBoardLed_pin = PB1;

#elif defined(BOARD_STM32F406VET6)
  int headLight_pin = NOT_A_PIN;
  int indicLightL_pin = NOT_A_PIN;
  int indicLightR_pin = 13;
  int brakeLight_pin = NOT_A_PIN;
  int errorLed_pin = NOT_A_PIN;
#endif
////////////////////////////////////////////////////////////////////////
