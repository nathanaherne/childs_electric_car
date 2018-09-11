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
// Debugging
// Turn on/off debugging output - turn off when not being used as it slows the software down
//#define DEBUG
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
// OTHER VARIABLES
////////////////////////////////////////////////////////////////////////
const int throttleRangeMs = 25500; // Max milliseconds between full forward and full reverse -> effects all RampIntervals
