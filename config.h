////////////////////////////////////////////////////////////////////////
// SYREN50 MOTOR CONTROLLER
#if defined(MOT_SYREN50)
  #include <Sabertooth.h>
  
  Sabertooth ST(128, Serial1); // Address 128, and use SWSerial as the serial port.

  // Throttle Values for Syren50
  const int mot_throttleMin = -127;  // Minimum throttle value for motor controller
  const int mot_throttleBrake = 0; // Brake throttle value for motor controller
  const int mot_throttleMax = 127; // Maximum throttle value for motor controller

  const int driverTimeout_us = 500; // Number of milliseconds 
  const int removeDriverDeadbandPercent = 0; // Remove motor driver deadband -> remove percentage of throttle around mot_throttleBrake
#endif

////////////////////////////////////////////////////////////////////////
// SABERTOOTH2x32 MOTOR CONTROLLER
#if defined(MOT_SABER2x32)
  #include <USBSabertooth.h>

  USBSabertoothSerial C(Serial1);
  USBSabertooth ST(C, 128);

  // Throttle values for Sabertooth2x32
  const int mot_throttleMin = -2047;  // Minimum throttle value for motor controller
  const int mot_throttleBrake = 0; // Brake throttle value for motor controller
  const int mot_throttleMax = 2047; // Maximum throttle value for motor controller
  
  const int driverTimeout_us = 500; // Number of milliseconds before Sabertooth2x32 will stop motors.
  const int removeDriverDeadbandPercent = 0; // Remove motor driver deadband -> remove percentage of throttle around mot_throttleBrake
#endif

////////////////////////////////////////////////////////////////////////
// SPARK MOTOR CONTROLLER
#if defined(MOT_SPARK)
  // Teensy3
  #if defined(BOARD_TEENSY3)
    #include <PWMServo.h>
  
    PWMServo motor1;
  
    // Forward/Reverse values for Spark Motor Controller
    const int mot_throttleMin = 1000;  // Minimum throttle value for motor controller
    const int mot_throttleBrake = 1500; // Brake throttle value for motor controller
    const int mot_throttleMax = 2000; // Maximum throttle value for motor controller
  
    const int removeDriverDeadbandPercent = 10; // Remove motor driver deadband -> remove percentage of throttle around mot_throttleBrake

  // Maple Mini
  #elif defined(BOARD_MAPLEMINI)
    HardwareTimer pwmTimer2(2); //Setup timer object
    
    // Forward/Reverse/Brake values for Spark Motor Controller
    const int mot_throttleMin = 1000;  // Minimum throttle value for motor controller
    const int mot_throttleBrake = 1500; // Brake throttle value for motor controller
    const int mot_throttleMax = 2000; // Maximum throttle value for motor controller
    
    const int removeDriverDeadbandPercent = 10; // Remove motor driver deadband -> remove percentage of throttle around mot_throttleBrake
  #endif
#endif
////////////////////////////////////////////////////////////////////////
