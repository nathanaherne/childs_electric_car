////////////////////////////////////////////////////////////////////////
// VEHICLE SPEED SETTINGS
boolean reverseMotorDirection = false; // Reverse motor direction -> true/false

// Speed and Acceleration settings
int maxForwardThrottlePercent = 65; // Maximum forward throttle (percentage)
int forwardRampPercent = 50; // Forward acceleration percent -> smaller value = slower acceleration

int maxReverseThrottlePercent = 40; // Maximum reverse throttle (percentage)
int reverseRampPercent = 50; // Reverse acceleration percent -> smaller value = slower acceleration

int brakeRampPercent = 10; // Brake acceleration percent -> smaller value = slower acceleration
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// CRUISE CONTROL FEATURE
// Enable/Disable Cruise Control
// 0. Cruise Control NOT enabled
// 1. Cruise Control enabled by throttle
// 2. Cruise Control enabled by its own input (button)
int cruiseControlFeature = 1;

// Only active if Cruise Control is enabled (i.e. cruiseControlFeature != 0)
int maxCruiseControlThrottlePercent = 70; // Maximum Cruise Control forward throttle (percentage)
int cruiseControlRampPercent = 50; // Cruise Control acceleration speed (milliseconds), smaller value = slower acceleration

// Only active when cruiseControlFeature = 1
int cruiseControlForwardThrottleEnableDelay_Sec = 30; // Seconds full throttle must be commanded for Cruise Control to activate
int cruiseControlForwardThrottleDisableDelay_Sec = 5; // Seconds after CruiseControl enabled before MC_throttle can disable it

// Only active when cruiseControlFeature = 2
int minCruiseControlThrottleEnablePercent = 50; // Throttle percent above which Cruise Control can be enabled by button
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// OLED FEATURE
// External OLED connected via serial connection
// 0 = disabled, 1 = enabled
boolean oledFeature = 0;
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// INPUT CONTROL OPTIONS
// Options are:
// 1. Manual inputs - Inputs are manual
// 2. Steering wheel - Rotation of steering wheel turns car -> requires Cruisin IMU -> powerSteeringFeature will be automatically enabled, other inputs are the same as manual
// 3. Remote control (RC) - overrides all manual inputs -> powerSteeringFeature will be automatically enabled and PPM or PWM must be selected below
// 4. Manual and RC
// 5. Steering and RC
// If you enable INPUT_MANUAL or INPUT_STEERING and INPUT_RC -> INPUT_RC overrides INPUT_MANUAL or INPUT_STEERING
const int inputControlFeature = 1;

//INPUT_RC
// You will need to enable the power steering mode of your choice to control steering remotely.
// Only enabled if inputControlFeature = 3, 4 or 5
// 1. PPM
// 2. PWM
const int RcFeature = 1;
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// THROTTLE FEATURE
// 1. Forward/Reverse controlled by throttle and reverseEnable
// 2. Forward/Reverse controlled by joystick
const int throttleFeature = 1;
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// INHIBIT FEATURE
// Safety feature where input/button disables drive
// 0 = disabled, 1 = enabled
const int inhibitFeature = 0;
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Battery & current
const int batteryType = 2; // SLA = 1, LiPo = 2, LiFeP04 = 3
const int batteryCapacity = 10000; // battery capacity in milliamp hours

const int lipoCellCount = 6;

const int batteryVoltageWarningPercent = 20; // voltage at which it shows a warning
const int currentWarningAmps = 20;

// System settings
const float lipoCellVoltMax = 4.2;
const float lipoCellVoltMin = 3.7;
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// BOARD OPTIONS
// 1. BOARD_PROMINI
// 2. BOARD_MAPLEMINI
// 3. BOARD_TEENSY3
// 4. BOARD_STM32F406VET6
#define BOARD_MAPLEMINI
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// MOTOR CONTROLLER OPTIONS
// Options are:
// 1. MOT_SYREN50 - Dimension Engineering Syren50 motor controller
// 2. MOT_SABER2x32 - Dimension Engineering Sabertoo2th 2x32 motor controller
// 3. MOT_SPARK - Rev Robotics Spark Motor Controller
#define MOT_SPARK
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Enable or Disable debugging output - turn off when not being used, Serial print on Arduino is blocking and slows code
#define DEBUG
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Throttle Settings
const int throttleDeadband_min_Percent = 5; // Removes this percentage of throttle around min throttle
const int throttleDeadband_max_Percent = 5; // Removes this percentage of throttle around max throttle
const int throttleRange_ms = 25500; // Max milliseconds between full forward and full reverse, this effects ramping for all throttle control modes
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Button Debounce settings
const int debounceSampleFrequency_hz = 1000; // Frequency that digital inputs checked
const int debounce_ms = 20; // Number of Milliseconds to check for
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Hardware PWM settings
static int pwmPeriod_us = 20000; // PWM period in microseconds
volatile int pwmDutyMin = 0; // Here for completeness
volatile int pwmDutyMax; // Calculated in setup
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// Analog Read settings
  // Arduino
  #if defined(BOARD_PROMINI)
    static int analogReadMin = 0;
    static int analogReadMax = 1023;
  // Maple Mini
  #elif defined(BOARD_MAPLEMINI)
    static int analogReadMin = 0;
    static int analogReadMax = 4095;
  // Teensy3
  #elif defined(BOARD_TEENSY3)
    static int analogReadMin = 0;
    static int analogReadMax = 1023;
  // STM32F406VET6
  #elif defined(BOARD_STM32F406VET6)
    static int analogReadMin = 0;
    static int analogReadMax = 4095;
  #endif
////////////////////////////////////////////////////////////////////////
