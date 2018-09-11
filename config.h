////////////////////////////////////////////////////////////////////////
// MCP4XXX 5V DIGITAL POTENTIOMETER VIA SPI
#if defined(MCP4XXX)

  // Interface with digital potentiometer via SPI
  #include <SPI.h>
  byte address = 0x00;
  // SPI pins
  const int CS = 10; // (SS) or Chip Select,
  // 11 (MOSI)
  // 12 (MISO)
  // 13 (SCK)

  // Pot output Values
  const int throttleMin = 1; // Potentiometer minimum or Reverse value
  const int throttleBrake = 128; // Potentiometer Brake value
  const int throttleMax = 255; // Potentiometer Max or Forward value

#endif
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// SYREN50 MOTOR CONTROLLER
#if defined(MOT_SYREN50)
  #include <Sabertooth.h>
  
  Sabertooth ST(128, Serial2); // Address 128, and use SWSerial as the serial port.

  // Throttle Values for Syren50
  const int throttleMin = -127;  // minimum or Reverse value
  const int throttleBrake = 0; // Brake value
  const int throttleMax = 127; // maximum or Forward value

  const int driverTimeout = 500; //Number of milliseconds before Sabertooth2x32 will stop motors.
  const int driverDeadband = 0; //Full deadband around throttleBrake -> If motor driver has deadband this amount will be removed
#endif

////////////////////////////////////////////////////////////////////////
// SABERTOOTH2x32 MOTOR CONTROLLER
#if defined(MOT_SABER2x32)
  #include <USBSabertooth.h>

  USBSabertoothSerial C(Serial2);
  USBSabertooth ST(C, 128);

  // Throttle values for Sabertooth2x32
  const int throttleMin = -2047;  // minimum or Reverse value
  const int throttleBrake = 0; // Brake value
  const int throttleMax = 2047; // maximum or Forward value
  
  const int driverTimeout = 500; //Number of milliseconds before Sabertooth2x32 will stop motors.
  const int driverDeadband = 0; //Full deadband around throttleBrake -> If motor driver has deadband this amount will be removed
#endif

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// SPARK MOTOR CONTROLLER
#if defined(MOT_SPARK)
  #include <Servo.h> 

  // Throttle values for Spark
  const int throttleMin = 1000;  // minimum or Reverse value
  const int throttleBrake = 1500; // Brake value
  const int throttleMax = 2000; // maximum or Forward value
  
  const int driverDeadband = 23; //Full deadband around throttleBrake -> If motor driver has deadband this amount will be removed

  Servo spark1;
  Servo spark2;
  Servo spark3;
  Servo spark4;
#endif
////////////////////////////////////////////////////////////////////////
