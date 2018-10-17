////////////////////////////////////////////////////////////////////////
// VEHICLE SPEED SETTINGS
boolean reverseMotorDirection = false; // Reverse motor direction -> true/false

// Speed and Acceleration settings
int maxForwardThrottlePercent = 70; // Maximum forward throttle (percentage)
int forwardRampPercent = 50; // Forward acceleration percent -> smaller value = slower acceleration

int maxReverseThrottlePercent = 40; // Maximum reverse throttle (percentage)
int reverseRampPercent = 50; // Reverse acceleration percent -> smaller value = slower acceleration

int brakeRampPercent = 80; // Brake acceleration percent -> smaller value = slower decceleration

// CRUISE CONTROL ONLY (i.e. cruiseControlFeature != 0)
int maxCruiseControlThrottlePercent = 75; // Maximum Cruise Control forward throttle (percentage)
int cruiseControlRampPercent = 50; // Cruise Control acceleration speed (milliseconds), smaller value = slower acceleration

// Only active when cruiseControlFeature = 1
int cruiseControlForwardThrottleEnableDelay_Sec = 30; // Seconds full throttle must be commanded for Cruise Control to activate
int cruiseControlForwardThrottleDisableDelay_Sec = 5; // Seconds after CruiseControl enabled before MC_throttle can disable it

// Only active when Cruise Control enabled by button
int minCruiseControlThrottleEnablePercent = 50; // Minimum throttle where Cruise Control can be enabled
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// MANUAL CONTROL
// 0. Disabled
// 1. Two button inputs -> Forward/Reverse
// 2. Three button inputs -> Forward/Reverse/Brake
// 3. Four button inputs -> Forward/Reverse/CruiseControl/Brake
// 4. One analog throttle input -> Forward/Reverse (cruiseControl not available)
const int manualControlFeature = 1;

// OTHER MANUAL INPUT OPTIONS
const int cruiseControlFeature = 1; //CRUISE CONTROL, 0 = disabled, 1 = enabled
const int inhibitFeature = 0; // INHIBIT -> Safety feature where input/button hard brakes vehicle, 0 = disabled, 1 = enabled
const int steeringWheelFeature = 0; // Rotation of steering wheel turns car -> Requires CruisinSteeringWheel -> powerSteeringFeature will be automatically enabled
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// REMOTE CONTROL (PPM) TODO
// Overrides all MANUAL CONTROL INPUTS (if MANUAL CONTROL enabled)
// 0. Disabled
// 1. Enabled
const int RcFeature = 1;
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// OLED FEATURE
// External OLED connected via serial connection
// 0 = disabled, 1 = enabled
boolean oledFeature = 0;
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// ELECTRIC BRAKE FEATURE TODO
// Electric brakes used on mobility scooters
// eBrake will be enabled whenever targetThrottle = brakeValue && currentThrottle == brakeValue
// eBrake will not be enabled during transitions from forward to reverse
// 0 = disabled, 1 = enabled
const int eBrakeFeature = 0;
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
// 1. BOARD_MAPLEMINI
// 2. BOARD_TEENSY3
// 3. BOARD_STM32F406VET6
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
// Enable or Disable debugging output - turn off when not being used, 
// Serial print on Arduino is blocking and slows code timings, which are important to smooth operation
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
  // Maple Mini
  #if defined(BOARD_MAPLEMINI)
    static int analogReadMin = 0;
    static int analogReadMax = 4095;
    static int analogReadMid = analogReadMax / 2;
  // Teensy3
  #elif defined(BOARD_TEENSY3)
    static int analogReadMin = 0;
    static int analogReadMax = 1023;
    static int analogReadMid = analogReadMax / 2;
  // STM32F406VET6
  #elif defined(BOARD_STM32F406VET6)
    static int analogReadMin = 0;
    static int analogReadMax = 4095;
    static int analogReadMid = analogReadMax / 2;
  #endif
////////////////////////////////////////////////////////////////////////
