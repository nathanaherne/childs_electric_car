# DISCLAIMER

You use this software and instructions at your own risk - if you break something, hurt someone or something or anything else happens
that is entirely on you.

<br>

-------------------------------------------------------------------------------------------------------
# OVERVIEW

This software was developed for Arduino to convert a mobility scooter into an electric car for a child.

This software was developed to take input from two momentary switches (buttons) (named Right and Left) and translate button pushes into motion.

- Pressing the Right button, results in the car moving forwards
- Pressing both the Right and Left buttons, results in the car moving reverse
- Holding the Right button for longer than 30 seconds, results in CruiseControl being enabled (car moves forward without input)
- When Cruise Control is enabled pressing the Left (at any time) or Right (5 seconds after Cruise Control is enabled) button disables Cruise Control and the car will brake
- Pressing no buttons results in the car stopping (unless Cruise Control is enabled)

## WARNING ON CRUISE CONTROL
This software has a feature called Cruise Control (enabled by default can be disabled) which results in the Arduino holding full throttle without input after Forward has been commanded a set amount of time (default 30 seconds but can be changed). Cruise Control was implemented 
because the children I created these cars for (7 families in our neighborhood so far) complained that pushing the button for long periods of time hurt their finger (I believe they pushed harder thinking they would go faster). Cruise Control can have an unintended side effect (which has happened to one of the very young children) where the child falls off the car while Cruise Control is enabled, which results in the car continuing on its merry way without a driver.

## WARNING ON USING MOBILITY SCOOTERS AS KIDS CARS
Mobility scooters are designed to carry large adults (up to 200kg), this has the benefit of one car being able to transport several children at once.
The negative of this is that these cars are very strong and have the ability to push a small child over or even drive over them, which I have been unable to solve programatically. 

<br>

-------------------------------------------------------------------------------------------------------

# MOTOR CONTROLLERS SUPPORTED

This software was developed to interface with several motor controllers. IF you can justify the extra money, I recommend using a General Purpose Motor Controller as it makes the whole process much simpler.

## Mobility scooter motor controllers that work with digital potentiometers

I have tested 7 controllers so far and the following controllers work with digital potentiometers

1. Dynamic Controls R-Series

### NOTE:
If you find the motor controller from your donor mobility scooter will not work with a digital potentiometer, you can use the wigwag from the mobility scooter to control forward/reverse.

### Motor scooter controllers usually have a features you do not see in general motor controllers:

1. Potentiometer control of speed (5v, 12v)
2. Electronic braking system
3. Features only available via the propriatary programmer
4. Requires a 24v battery, a lower voltage battery will result in the contoller not working.

The input for speed control on these controllers expect a voltage range between 0-5v, where 0v is full reverse, 2.5v is stop and 5v is full forward. I have seen some chinese controllers that require 0-12v, which creates problems for modern microcontrollers and digital potentiometers (most are maximum of 5v).

## Mobility scooter motor controllers that DO NOT work with digital potentiometers

1. Dyanmic Controls Rhino - Blue in color (does not work)
2. PG Drives Solo - Black and Silver versions (intermittantly)

In my experience, Dynamic Controls Support are easier to work with than PG Drives (owned by Curtiss Wright) Support. Both companies Support people respond very quickly but PG Drives seem reluctant to provide much technical information about their product. Dynamic Controls on the other hand post full technical manuals on the internet for their contollers.

### NOTE:
1. When using any mobility scooter controller, make sure you keep all the original electronics from your donor mobility scooter. This will allow you to get status indications from the controllers and determine the wiring the controller expects to see.
2. Having a multimeter will help immensely to work out expected wiring when using a mobility scooter motor controller.

## General purpose motor controllers

1. Dimension Engineering Syren50 (single motor control)
2. Dimension Engineering Sabertooth2x32 (dual motor control)

General purpose motor controllers are much easier to use in these applications because everything can be done in code and they have extensive technical manuals.

### Syren50

Mobility scooters can have one motor with a physical differential or two motors, one motor per wheel. The Syren50 will work with both types. To use the Syren50 with one motor, just wire it as per the manual. To use the Syren50 with two motors, you should wire the motors in series so that you get an electronic differential.

NOTE: To use the Syren50 with a mobility scooter you will need to
1. remove the electronic brake attached to the end of the motor (unless you implement control of the brake using a separate piece of hardware). 
2. Cut off all propriatary plugs and connect the correct wires directly to the motor controller as per the Syren50's manual.

### Sabertooth2x32

The Sabertooth2x32 has the ability to control two electric motors (among a number of other features). This software has been designed to support the Sabertooth2x32. The main isssue with using the Sabertooth2x32 with two rear motors is there will be no differential. I do not think it is a great idea to use the Sabertooth2x32 for two motors, even though it is possible. The Sabertooth2x32 is useful for people who would like to use it to control an electronic brake, though at this time, I have yet to implement or test this functionality.

The Sabertooth 2x32 has the ability to engage/disengage the electronic brake so you can leave it attached if you wish.

<br>

-------------------------------------------------------------------------------------------------------

# WIRING

The software expects the following wiring connections (the Arduino pins can be changed in settings.h):

1. Right button - digital pin 5 and Gnd on Arduino board (pushing the button grounds the pin)
2. Left button - digital pin 7 and Gnd on Arduino board (pushing the button grounds the pin)

If using Syren50 or Sabertooth2x32

1. Serial Tx (digital pin 11 on Arduino board) to S1 on motor controller
2. 5v and Gnd on Arduino to 5v and 0v on motor controller (the Arduino is powered by the motor controller)

If using digital poteniometer
1. Digital pin 10 on Arduino board to (SS) or Chip Select pin on potentiometer chip,
2. Digital pin 11 (or MOSI) on Arduino board to MOSI pin on potentiometer chip
3. Digital pin 12 (or MISO) on Arduino board to MISO pin on potentiometer chip
4. Digital pin 13 (or SCK) on Arduino board to SCK pin on potentiometer chip

You will need to wire potentiometer HIGH, potentiometer wiper and potentiometer LOW from the controller to
the corresponding pins on the digital potentiometer.

<br>

-------------------------------------------------------------------------------------------------------

# CONFIGURATION

You can configure this software in the settings.h file.

The top of the file provides a number of configurable options for the software, including 

1. Motor Controller options
2. Reverse motor direction (change forward motion to reverse motion)
3. Maximum Forward speed
4. Maximum Reverse speed
5. Maximum CruiseControl speed
6. Turn on/off debugging output

You can also configure acceleration (for Forward/Reverse/Cruisecontrol) and deceleration (braking)

<br>

-------------------------------------------------------------------------------------------------------

# TECHNICAL NOTES

The software has been written with the intention of it being readable more than technically code efficient.
