// Cruisin Brain Education Version
// Runs on a Maple Mini using STM32 addon for Arduino from stm32duino

// Required files
#include "pins.h"
#include "settings.h"


void setup() {

  // Setup code for Motor Controllers
  setupMotorController();

  // Stop motor(s)
  sendThrottleCommand(throttleBrake);

  // Setup non peripheral pins
  setupGeneralPins();

  // Setup Other peripherals
  setupOtherPeripherals();
  
}

void loop() {

  // Variable Declarations
  static int maxForward;
  static int minForward;
  static int maxReverse;
  static int minReverse;
  static int maxCruiseControl;
  static int minCruiseControl;
  
  static unsigned long forwardRampInterval;
  static unsigned long reverseRampInterval;
  static unsigned long brakeRampInterval;
  static unsigned long cruiseControlRampInterval;
  
  static int motionCommanded; // variable to store motion commands from momentary buttons
  static int motionCalculated; // variable to store motion calculated
  static unsigned long currentMillis; // variable to store currentMillis
  static int currentThrottle = throttleBrake; // Current throttle variable

  static boolean onDemandVarsCalced = false; // Have one time vars been calculated
  static boolean startupDebug = false; // Has startup DEBUG been printed?

  static unsigned long previousForwardRampMillis = 0; // the last time Forward speed was updated
  static unsigned long previousReverseRampMillis = 0; // the last time Reverse speed was updated
  static unsigned long previousBrakeRampMillis = 0; // will store last time brake throttle was updated
  static unsigned long previousCruiseControlRampMillis = 0; // the last time Cruise Control speed was updated
  
  static boolean cruiseControlOn = false; // 1 = true, 0 = false
  static unsigned long forwardFirstCommanded = false; // The first time Forward first commanded

  static boolean driveInhibited = true;
 
  static int cruiseControl_MC_forward_EnableDelayMs = cruiseControl_MC_forward_EnableDelaySec * 1000; // convert Seconds to Ms
  static int cruiseControl_MC_forward_DisableDelayMs = cruiseControl_MC_forward_DisableDelaySec * 1000; // convert Seconds to Ms
  // End Variable Declarations

  // Check if drive has been inhibited by switch, charger or panic button
  driveInhibited = digitalRead(inhibit_pin);
  while (driveInhibited == LOW) {
    sendThrottleCommand(throttleBrake); // Send brake to motorController
    #if defined(DEBUG)
      Serial.println("Drive Inhibited");
    #endif
  }

  // Calculate on demand variables
  if (onDemandVarsCalced == false) {
    calcForwardReverseValues(maxForward, minForward, maxReverse, minReverse);
    calcRampIntervals(forwardRampInterval, reverseRampInterval, brakeRampInterval, cruiseControlRampInterval);
    calcCruiseControlValues(maxCruiseControl, minCruiseControl);
    onDemandVarsCalced = true;
  }

  // Print Start DEBUG to Serial
  #if defined(DEBUG)
    if (startupDebug == false) {
      printStartupDebug();
      startupDebug = true;
    }
  #endif
  
  currentMillis = millis(); // Update currentMillis every loop
  
  getManualControl(motionCommanded); // Get the button combinations pressed (e.g. forward/reverse/brake)

  // Calculate if CruiseControl should be on
  if (enableCruiseControl == true) {
    calcCruiseControl(motionCommanded, cruiseControlOn, currentMillis, forwardFirstCommanded, previousForwardRampMillis, 
        cruiseControl_MC_forward_EnableDelayMs, cruiseControl_MC_forward_DisableDelaySec);
  }

  // Calculate motion from button states and other details
  calcMotion(motionCommanded, cruiseControlOn, currentThrottle, currentMillis, 
      maxForward, previousForwardRampMillis, forwardRampInterval,
      maxReverse, previousReverseRampMillis, reverseRampInterval,
      maxCruiseControl, previousCruiseControlRampMillis, cruiseControlRampInterval, forwardFirstCommanded,
      throttleBrake, previousBrakeRampMillis, brakeRampInterval);
  
}

// Gets Manual Motion Commanded
void getManualControl(int &motionCommanded) {

  boolean MC_forward = digitalRead(MC_forward_pin);
  boolean MC_reverseEnable = digitalRead(MC_reverseEnable_pin);

  // Brake
  if (MC_forward == HIGH && MC_reverseEnable == HIGH) {
    motionCommanded = 0;
  } 
  // Forward
  else if (MC_forward == LOW && MC_reverseEnable == HIGH) {
    motionCommanded = 1;
  } 
  // Reverse - also disables cruise control
  else if (MC_forward == LOW && MC_reverseEnable == LOW) {
    motionCommanded = 2;
  }
  // Cruise Control Disable
  else if (MC_forward == HIGH && MC_reverseEnable == LOW) {
    motionCommanded = 3;
  } 
  // If all else fails, then brake
  else {
    motionCommanded = 0;
  }
}

void calcCruiseControl(int motionCommanded, boolean &cruiseControlOn, unsigned long currentMillis, unsigned long previousForwardRampMillis, 
          unsigned long &forwardFirstCommanded, int cruiseControl_MC_forward_EnableDelayMs, int cruiseControl_MC_forward_DisableDelaySec) {

  // Check to see if Forward has been commanded for more than cruiseControl_MC_forward_EnableDelayMs
  if (motionCommanded == 1 && cruiseControlOn == false && (forwardFirstCommanded != 0 && currentMillis - forwardFirstCommanded >= cruiseControl_MC_forward_EnableDelayMs)) {
    cruiseControlOn = 1;
    
    #if defined(DEBUG)
      Serial.print("CRUISE CONTROL ENABLED");Serial.print("   ");
    #endif
  }
  // Disable Cruise Control if Brake or Reverse is commanded
  else if (cruiseControlOn && (motionCommanded == 2 || motionCommanded == 3)) {
    cruiseControlOn = 0;
    forwardFirstCommanded = 0;
    
    #if defined(DEBUG)
      Serial.print("CRUISE CONTROL DISABLED");Serial.print("   ");
    #endif
  }
  // Disable Cruise Control if cruiseControlOn, Forward commanded and cruiseControl_MC_forward_DisableDelaySec has elapsed
  else if (cruiseControlOn && motionCommanded == 1 && currentMillis - (forwardFirstCommanded + cruiseControl_MC_forward_EnableDelayMs) >= cruiseControl_MC_forward_DisableDelaySec) {
    cruiseControlOn = 0;
    forwardFirstCommanded = 0;
    
    #if defined(DEBUG)
      Serial.print("CRUISE CONTROL DISABLED");Serial.print("   ");
    #endif
  }

}

void calcMotion(int motionCommanded, boolean &cruiseControlOn, int &currentThrottle, unsigned long &currentMillis, 
      int maxForward, unsigned long &previousForwardRampMillis, int forwardRampInterval,
      int maxReverse, unsigned long &previousReverseRampMillis, int reverseRampInterval,
      int maxCruiseControlForward, unsigned long &previousCruiseControlRampMillis, int cruiseControlForwardRampInterval, unsigned long &forwardFirstCommanded,
      int throttleBrake, unsigned long &previousBrakeRampMillis, int brakeRampInterval) {

  // Forward commanded and reverseMotorDirection = false PLUS
  // cruiseControlOn = false -> forward is different to CruiseControl 
  // currentThrottle >= throttleBrake -> when going from reverse to forward, brakeRampInterval unsed until currentThrottle = throttleBrake 
  // currentThrottle <= maxForward -> if forward commanded BEFORE throttle is below maxForward then vehicle must be braked first
  if (motionCommanded == 1 && !cruiseControlOn && reverseMotorDirection == false && currentThrottle >= throttleBrake && currentThrottle <= maxForward + 1) {
    #if defined(DEBUG)
    Serial.print("Forward - ");
    #endif
    increaseThrottle(currentMillis, previousForwardRampMillis, forwardRampInterval, currentThrottle, maxForward);
    // If the first time Forward commanded since last CruiseControlMillis reset update CruiseControlMillis
    if (forwardFirstCommanded == 0) {
      forwardFirstCommanded = millis();
      Serial.print("Cruise Control Millis: ");Serial.print(forwardFirstCommanded);Serial.print("   ");
    }
  }
  // Forward commanded and reverseMotorDirection = true PLUS
  // cruiseControlOn = false -> forward is different to CruiseControl 
  // currentThrottle >= throttleBrake -> when going from reverse to forward, brakeRampInterval unsed until currentThrottle = throttleBrake 
  // currentThrottle <= maxForward -> if forward commanded BEFORE throttle is below maxForward then vehicle must be braked first
  else if (motionCommanded == 1 && cruiseControlOn == false && reverseMotorDirection == true && currentThrottle <= throttleBrake && currentThrottle >= maxForward - 1) {
    #if defined(DEBUG)
    Serial.print("Forward - ");
    #endif
    reduceThrottle(currentMillis, previousForwardRampMillis, forwardRampInterval, currentThrottle, maxForward);
  }
  // Reverse commanded and reverseMotorDirection = false
  else if (motionCommanded == 2  && reverseMotorDirection == false && currentThrottle <= throttleBrake) {
    #if defined(DEBUG)
    Serial.print("Reverse - ");
    #endif
    reduceThrottle(currentMillis, previousReverseRampMillis, reverseRampInterval, currentThrottle, maxReverse);
    forwardFirstCommanded = 0; // reset CruiseControlMillis counter
  }
  // Reverse commanded and reverseMotorDirection = true  
  else if (motionCommanded == 2  && reverseMotorDirection == true && currentThrottle >= throttleBrake) {
    #if defined(DEBUG)
    Serial.print("Reverse - ");
    #endif
    increaseThrottle(currentMillis, previousReverseRampMillis, reverseRampInterval, currentThrottle, maxReverse);
    forwardFirstCommanded = 0; // reset CruiseControlMillis counter
  }
  // cruiseControlOn = true and reverseMotorDirection = false
  else if (cruiseControlOn == true && reverseMotorDirection == false) {
    #if defined(DEBUG)
    Serial.print("Cruise Control - ");
    #endif
    increaseThrottle(currentMillis, previousCruiseControlRampMillis, cruiseControlForwardRampInterval, currentThrottle, maxCruiseControlForward);
  }
  // cruiseControlOn = true and reverseMotorDirection = true
  else if (cruiseControlOn == true && reverseMotorDirection == true) {
    #if defined(DEBUG)
    Serial.print("Cruise Control - ");
    #endif
    reduceThrottle(currentMillis, previousCruiseControlRampMillis, cruiseControlForwardRampInterval, currentThrottle, maxCruiseControlForward);
  }
  // Brake from Positive Throttle
  else if (!cruiseControlOn && currentThrottle > throttleBrake) {
    #if defined(DEBUG)
    Serial.print("Brake FP - ");
    #endif
    reduceThrottle(currentMillis, previousBrakeRampMillis, brakeRampInterval, currentThrottle, throttleBrake);
    forwardFirstCommanded = 0; // reset CruiseControlMillis counter
  }
  // Brake from Negative Throttle
  else if (!cruiseControlOn && currentThrottle < throttleBrake) {
    #if defined(DEBUG)
    Serial.print("Brake FN - ");
    #endif
    increaseThrottle(currentMillis, previousBrakeRampMillis, brakeRampInterval, currentThrottle, throttleBrake);
    forwardFirstCommanded = 0; // reset CruiseControlMillis counter
  }
  // Brake
  else if ((motionCommanded == 0 || motionCommanded == 3) && currentThrottle == throttleBrake) {
    //sendThrottleCommand(throttleBrake); // Send throttleBrake to controller anyway as a keepalive for controllers that need it
    forwardFirstCommanded = 0;  // reset CruiseControlMillis counter
    
    #if defined(DEBUG)
    Serial.print("Brake - ");Serial.print("throttle: ");Serial.print(currentThrottle);Serial.println();
    #endif
  }
  
}

void increaseThrottle(unsigned long currentMillis, unsigned long &previousMillis, int interval, int &currentThrottle, int throttleTarget) {

  if (currentMillis - previousMillis >= interval) {
    for (; currentThrottle <= throttleTarget;) {
      currentThrottle++;
      previousMillis = currentMillis;
      break;
      }
  }
  // Send throttle command even if there is no update -> in essence a keepalive for the motor controller
  sendThrottleCommand(currentThrottle);
  
  #if defined(DEBUG)
    Serial.print("throttle: ");Serial.print(currentThrottle);Serial.println();
  #endif

}

void reduceThrottle(unsigned long currentMillis, unsigned long &previousMillis, int interval, int &currentThrottle, int throttleTarget) {

  if (currentMillis - previousMillis >= interval) {
    for (;currentThrottle >= throttleTarget;) {
      currentThrottle--;
      previousMillis = currentMillis;
      break;
      }
  }
  // Send throttle command even if there is no update -> in essence a keepalive for the motor controller
  sendThrottleCommand(currentThrottle);
  
  #if defined(DEBUG)
    Serial.print("throttle: ");Serial.print(currentThrottle);Serial.println();
  #endif

}

// Send Throttle Command to the motor controller
void sendThrottleCommand(int currentThrottle) {

  #if defined(MCP4XXX)
    MOT_MCP4XXX_sendThrottle(currentThrottle);
  #endif
  
  #if defined(MOT_SYREN50)
    MOT_SYREN50_sendThrottle(currentThrottle);
  #endif
  
  #if defined(MOT_SABER2x32)
    MOT_SABER2x32_sendThrottle(currentThrottle);
  #endif
  
  #if defined(MOT_SPARK)
    MOT_SPARK_sendThrottle(currentThrottle);
  #endif
}

#ifdef MOT_MCP4XXX
// Code to control a MCP4151 digital potentiometer via SPI
void MOT_MCP4XXX_sendThrottle(int currentThrottle) {
  digitalWrite(CS, LOW);
  SPI.transfer(address);
  SPI.transfer(currentThrottle);
  digitalWrite(CS, HIGH);
}
#endif

#if defined(MOT_SYREN50)
// Code to control a Dimension Engineering motor controller
void MOT_SYREN50_sendThrottle(int currentThrottle){

  ST.motor(1, currentThrottle);
  
}
#endif

#if defined(MOT_SABER2x32)
// Code to control a Dimension Engineering motor controller
void MOT_SABER2x32_sendThrottle(int currentThrottle){

  ST.motor(1, currentThrottle);
  ST.motor(2, -currentThrottle);
  
}
#endif

#if defined(MOT_SPARK)
  // Send throttle command to Spark Motor Controllers
  void MOT_SPARK_sendThrottle(int currentThrottle){
    spark1.writeMicroseconds(currentThrottle); 
  }
#endif

