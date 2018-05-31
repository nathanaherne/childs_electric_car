// Settings File
#include "settings.h"


void setup() {

  #ifdef MCP4XXX
    pinMode (CS, OUTPUT);
    SPI.begin();
  #endif

  #if defined(MOT_SYREN50) || defined(MOT_SABER2x32)
    pinMode(SwSerialTxPin, OUTPUT);
    SabertoothSWSerial.begin(9600);
    ST.setTimeout(motorDriverTimeout); // Controller must recieve commands every X milliseconds or it will stop motors
    ST.setRamping(0);
    ST.autobaud();
  #endif

  #if defined(MOT_SPARK)
    pinMode(spark1_pin, OUTPUT);
    spark1.attach(spark1_pin, throttleMin, throttleMax);
  #endif

  // Stop motor(s)
  sendThrottleCommand(throttleBrake);

  // Setup the input pins for the buttons
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  
}

void loop() {

  // Variable Declarations
  static int motionCommanded; // variable to store motion commands from momentary buttons
  static int motionCalculated; // variable to store motion calculated
  static unsigned long currentMillis; // variable to store currentMillis
  static int currentThrottle = throttleBrake; // Current throttle variable

  static boolean oneTimeVars = false; // Have one time vars been calculated
  static boolean startupDebug = false; // Has startup debug been printed?

  static unsigned long previousForwardRampMillis = 0; // the last time Forward speed was updated
  static unsigned long previousReverseRampMillis = 0; // the last time Reverse speed was updated
  static unsigned long previousBrakeRampMillis = 0; // will store last time brake throttle was updated

  static boolean cruiseControlOn = 0; // 1 = true, 0 = false
  static unsigned long cruiseControlMillis = 0; // the first time Forward first commanded
  static unsigned long previousCruiseControlRampMillis = 0; // the last time Cruise Control speed was updated
  static int cruiseControlWaitMs = cruiseControlWaitSec * 1000; // convert Seconds to Ms
  static int cruiseControlRightButtonDisableDelayMs = cruiseControlRightButtonDisableDelaySec * 1000; // convert Seconds to Ms
  // End Variable Declarations

  // Calculate one time variables
  if (oneTimeVars == false) {
    calcOneTimeVars();
    oneTimeVars = true;
  }

  // Print Start Debug to Serial
  #if defined(Debug)
    if (startupDebug == false) {
      printStartupDebug();
      startupDebug = true;
    }
  #endif
  
  currentMillis = millis(); // Update currentMillis every loop
  
  getButtons(motionCommanded); // Get the button combinations pressed (e.g. forward/reverse/brake)

  // Calculate if CruiseControl should be on
  if (enableCruiseControl) {
    calcCruiseControl(motionCommanded, cruiseControlOn, currentMillis, cruiseControlMillis, previousForwardRampMillis, 
        cruiseControlWaitMs, cruiseControlRightButtonDisableDelayMs);
  }

  // Calculate motion from button states and other details
  calcMotion(motionCommanded, cruiseControlOn, currentThrottle, currentMillis, 
      maxForward, previousForwardRampMillis, forwardRampInterval,
      maxReverse, previousReverseRampMillis, reverseRampInterval,
      maxCruiseControl, previousCruiseControlRampMillis, cruiseControlRampInterval, cruiseControlMillis,
      throttleBrake, previousBrakeRampMillis, brakeRampInterval);
  
}

// Returns button states
int getButtons(int &motion) {

  boolean rightButtonValue = digitalRead(rightButton);
  boolean leftButtonValue = digitalRead(leftButton);

  // Brake
  if (rightButtonValue == HIGH && leftButtonValue == HIGH) {
    motion = 0;
  } 
  // Forward
  else if (rightButtonValue == LOW && leftButtonValue == HIGH) {
    motion = 1;
  } 
  // Reverse - also disables cruise control
  else if (rightButtonValue == LOW && leftButtonValue == LOW) {
    motion = 2;
  }
  // Cruise Control Disable
  else if (rightButtonValue == HIGH && leftButtonValue == LOW) {
    motion = 3;
  } 
  // If all else fails, then brake
  else {
    motion = 0;
  }
}

void calcCruiseControl(int motionCommanded, boolean &cruiseControlOn, unsigned long currentMillis, unsigned long previousForwardRampMillis, 
          unsigned long &cruiseControlMillis, int cruiseControlWaitMs, int cruiseControlRightButtonDisableDelayMs) {

  // Check to see if Forward has been commanded for more than CruiseControlWaitMs
  if (motionCommanded == 1 && !cruiseControlOn && (cruiseControlMillis != 0 && currentMillis - cruiseControlMillis >= cruiseControlWaitMs)) {
    cruiseControlOn = 1;
    
    #if defined(Debug)
      Serial.print("CRUISE CONTROL ENABLED");Serial.print("   ");
    #endif
  }
  // Disable Cruise Control if Brake or Reverse is commanded
  else if (cruiseControlOn && (motionCommanded == 2 || motionCommanded == 3)) {
    cruiseControlOn = 0;
    cruiseControlMillis = 0;
    
    #if defined(Debug)
      Serial.print("CRUISE CONTROL DISABLED");Serial.print("   ");
    #endif
  }
  // Disable Cruise Control if cruiseControlOn, Forward commanded and cruiseControlRightButtonDisableDelayMs has elapsed
  else if (cruiseControlOn && motionCommanded == 1 && currentMillis - (cruiseControlMillis + cruiseControlWaitMs) >= cruiseControlRightButtonDisableDelayMs) {
    cruiseControlOn = 0;
    cruiseControlMillis = 0;
    
    #if defined(Debug)
      Serial.print("CRUISE CONTROL DISABLED");Serial.print("   ");
    #endif
  }

}

void calcMotion(int motionCommanded, boolean &cruiseControlOn, int &currentThrottle, unsigned long &currentMillis, 
      int maxForward, unsigned long &previousForwardRampMillis, int forwardRampInterval,
      int maxReverse, unsigned long &previousReverseRampMillis, int reverseRampInterval,
      int maxCruiseControlForward, unsigned long &previousCruiseControlRampMillis, int cruiseControlForwardRampInterval, unsigned long &cruiseControlMillis,
      int throttleBrake, unsigned long &previousBrakeRampMillis, int brakeRampInterval) {

  // Forward commanded and reverseMotorDirection = false PLUS
  // cruiseControlOn = false -> forward is different to CruiseControl 
  // currentThrottle >= throttleBrake -> when going from reverse to forward, brakeRampInterval unsed until currentThrottle = throttleBrake 
  // currentThrottle <= maxForward -> if forward commanded BEFORE throttle is below maxForward then vehicle must be braked first
  if (motionCommanded == 1 && !cruiseControlOn && reverseMotorDirection == false && currentThrottle >= throttleBrake && currentThrottle <= maxForward + 1) {
    #if defined(Debug)
    Serial.print("Forward - ");
    #endif
    increaseThrottle(currentMillis, previousForwardRampMillis, forwardRampInterval, currentThrottle, maxForward);
    // If the first time Forward commanded since last CruiseControlMillis reset update CruiseControlMillis
    if (cruiseControlMillis == 0) {
      cruiseControlMillis = millis();
      Serial.print("Cruise Control Millis: ");Serial.print(cruiseControlMillis);Serial.print("   ");
    }
  }
  // Forward commanded and reverseMotorDirection = true PLUS
  // cruiseControlOn = false -> forward is different to CruiseControl 
  // currentThrottle >= throttleBrake -> when going from reverse to forward, brakeRampInterval unsed until currentThrottle = throttleBrake 
  // currentThrottle <= maxForward -> if forward commanded BEFORE throttle is below maxForward then vehicle must be braked first
  else if (motionCommanded == 1 && cruiseControlOn == false && reverseMotorDirection == true && currentThrottle <= throttleBrake && currentThrottle >= maxForward - 1) {
    #if defined(Debug)
    Serial.print("Forward - ");
    #endif
    reduceThrottle(currentMillis, previousForwardRampMillis, forwardRampInterval, currentThrottle, maxForward);
  }
  // Reverse commanded and reverseMotorDirection = false
  else if (motionCommanded == 2  && reverseMotorDirection == false && currentThrottle <= throttleBrake) {
    #if defined(Debug)
    Serial.print("Reverse - ");
    #endif
    reduceThrottle(currentMillis, previousReverseRampMillis, reverseRampInterval, currentThrottle, maxReverse);
    cruiseControlMillis = 0; // reset CruiseControlMillis counter
  }
  // Reverse commanded and reverseMotorDirection = true  
  else if (motionCommanded == 2  && reverseMotorDirection == true && currentThrottle >= throttleBrake) {
    #if defined(Debug)
    Serial.print("Reverse - ");
    #endif
    increaseThrottle(currentMillis, previousReverseRampMillis, reverseRampInterval, currentThrottle, maxReverse);
    cruiseControlMillis = 0; // reset CruiseControlMillis counter
  }
  // cruiseControlOn = true and reverseMotorDirection = false
  else if (cruiseControlOn == true && reverseMotorDirection == false) {
    #if defined(Debug)
    Serial.print("Cruise Control - ");
    #endif
    increaseThrottle(currentMillis, previousCruiseControlRampMillis, cruiseControlForwardRampInterval, currentThrottle, maxCruiseControlForward);
  }
  // cruiseControlOn = true and reverseMotorDirection = true
  else if (cruiseControlOn == true && reverseMotorDirection == true) {
    #if defined(Debug)
    Serial.print("Cruise Control - ");
    #endif
    reduceThrottle(currentMillis, previousCruiseControlRampMillis, cruiseControlForwardRampInterval, currentThrottle, maxCruiseControlForward);
  }
  // Brake from Positive Throttle
  else if (!cruiseControlOn && currentThrottle > throttleBrake) {
    #if defined(Debug)
    Serial.print("Brake FP - ");
    #endif
    reduceThrottle(currentMillis, previousBrakeRampMillis, brakeRampInterval, currentThrottle, throttleBrake);
    cruiseControlMillis = 0; // reset CruiseControlMillis counter
  }
  // Brake from Negative Throttle
  else if (!cruiseControlOn && currentThrottle < throttleBrake) {
    #if defined(Debug)
    Serial.print("Brake FN - ");
    #endif
    increaseThrottle(currentMillis, previousBrakeRampMillis, brakeRampInterval, currentThrottle, throttleBrake);
    cruiseControlMillis = 0; // reset CruiseControlMillis counter
  }
  // Brake
  else if ((motionCommanded == 0 || motionCommanded == 3) && currentThrottle == throttleBrake) {
    //sendThrottleCommand(throttleBrake); // Send throttleBrake to controller anyway as a keepalive for controllers that need it
    cruiseControlMillis = 0;  // reset CruiseControlMillis counter
    
    #if defined(Debug)
    Serial.print("Brake - ");Serial.print("throttle: ");Serial.print(currentThrottle);Serial.println();
    #endif
  }
  
}

void increaseThrottle(unsigned long currentMillis, unsigned long &previousMillis, int interval, int &currentThrottle, int throttleTarget) {

  if (currentMillis - previousMillis >= interval) {
    for (currentThrottle; currentThrottle <= throttleTarget;) {
      currentThrottle++;
      previousMillis = currentMillis;
      break;
      }
  }
  // Send throttle command even if there is no update -> in essence a keepalive for the motor controller
  sendThrottleCommand(currentThrottle);
  
  #if defined(Debug)
    Serial.print("throttle: ");Serial.print(currentThrottle);Serial.println();
  #endif

}

void reduceThrottle(unsigned long currentMillis, unsigned long &previousMillis, int interval, int &currentThrottle, int throttleTarget) {

  if (currentMillis - previousMillis >= interval) {
    for (currentThrottle; currentThrottle >= throttleTarget;) {
      currentThrottle--;
      previousMillis = currentMillis;
      break;
      }
  }
  // Send throttle command even if there is no update -> in essence a keepalive for the motor controller
  sendThrottleCommand(currentThrottle);
  
  #if defined(Debug)
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

