////////////////////////////////////////////////////////////////////////
// MOTOR CONTROLLER OPTIONS
// Options are:
// 1. MOT_MCP4XXX -> Mobility scooter controller or controller that requires an MCP4XXX family digital potentiometer
// 3. MOT_SYREN50 -> Dimension Engineering MOT_SYREN50 motor controller
// 4. MOT_SABER2x32 -> Dimension Engineering Sabertooth 2x32 motor controller
// 5. MOT_SPARK -> Spark Motor controller
#define MOT_SPARK
////////////////////////////////////////////////////////////////////////

// Reverse motor direction
boolean reverseMotorDirection = true; // true is reverse motor direction, false is do not reverse motor direction

// Enable/Disable Cruise Control
boolean enableCruiseControl = true; // True is enabled, false is disabled

// Speed and Acceleration settings
int maxForwardPercent = 67; // Maximum forward throttle (percentage)
int forwardRampPercent = 50; // Forward acceleration percent -> larger value = slower acceleration

int maxReversePercent = 60; // Maximum reverse throttle (percentage)
int reverseRampPercent = 50; // Reverse acceleration percent -> larger value = slower acceleration

int brakeRampPercent = 25; // Brake acceleration percent -> larger value = slower acceleration

int minCruiseControlPercent = 50; // Throttle percent above which Cruise Control can be enabled by button
int maxCruiseControlPercent = 70; // Maximum Cruise Control forward throttle (percentage)
int cruiseControlRampPercent = 50; // Cruise Control acceleration speed (milliseconds), larger value = slower acceleration

// Only active when MC_cruise_control = MC_forward
int cruiseControl_MC_forward_EnableDelaySec = 30; // Seconds MC_forward must be true for Cruise Control to activate
int cruiseControl_MC_forward_DisableDelaySec = 5; // Seconds after CruiseControl enabled before MC_forward can disable it

////////////////////////////////////////////////////////////////////////
// DO NOT CHANGE THE BELOW SETTINGS UNLESS YOU KNOW WHAT YOU ARE DOING
////////////////////////////////////////////////////////////////////////

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
  
  Sabertooth ST(128, Serial1); // Address 128, and use SWSerial as the serial port.

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

  USBSabertoothSerial C(Serial1);
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

  const int mot_pwm1_pin = PC15; // pin1
  const int mot_pwm2_pin = PA1; // pin2
  const int mot_pwm3_pin = PA2; // pin2
  const int mot_pwm4_pin = PA3; // pin2

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

////////////////////////////////////////////////////////////////////////
// Debugging
// Turn on/off debugging output - turn off when not being used as it slows the software down
//#define DEBUG
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// OTHER VARIABLES
////////////////////////////////////////////////////////////////////////
const int throttleRangeMs = 25500; // Max milliseconds between full forward and full reverse -> effects all RampIntervals
