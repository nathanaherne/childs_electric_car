// Manual Control Input pins
const int MC_throttle_pin = PB0; // Throttle (analog & digital)
const int MC_reverseEnable_pin = PB2; // Reverse Enable
const int MC_cruiseControl_pin = PB6; // Cruise Control Enable (digital)
const int MC_brake_pin = PA7; // Cruise Control Enable (digital)
const int MC_headlight_pin = PB5; // Headlights (digital)
const int MC_indicL_pin = PA14; // Indicator1 (digital)
const int MC_indicR_pin = PB3; // Indicator2 (digital)

const int inhibit_pin = PA6; // pin that inhibits drive OR emergency shutdown

// Output pins
const int headLight_pin = PC14;
const int brakeLight_pin = PA14;
const int errorLed_pin = PA13;
const int eBrake = PA8;

// PWM pins for PWM motor controllers
const int mot_pwm1_pin = PA15; // pin1, pwm2/1
const int mot_pwm2_pin = PA1; // pin2, pwm2/2
const int mot_pwm3_pin = PA2; // pin2, pwm2/3
const int mot_pwm4_pin = PA3; // pin2, pwm2/4

// Serial port for Serial motor controllers
const int mot_Serial1Rx_pin = PA10;
const int mot_Serial1Tx_pin = PA9;

// Serial port for Cruisin Brain OLED
const int oled_Serial3Rx_pin = PB11;
const int oled_Serial3Tx_pin = PB10;

// Input pins for voltage and current monitoring
const int voltage_pin = PA4;
const int current_pin = PA5;

//I2C
const int i2c_sda_pin = PB7;
const int i2c_scl_pin = PB6;
