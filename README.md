# LEGAL STUFF

You use this software and instructions at your own risk - if you break something, hurt someone or something or anything else happens, I am not responsible.

<br>

-------------------------------------------------------------------------------------------------------
# OVERVIEW

This software was initially developed for Arduino to convert a mobility scooter into an electric car for a child. The electric cars and software have excited so many parents and children that I created [Cruisin Co](https://cruisin.co). [Cruisin Co](https://cruisin.co) has detailed instructions on how to build an electric car for children (and adults) as well as other information you might find of interest. 

We are working on creating a [Cruisin Brain](https://cruisin.co/cruisin-brain/cruisinBrain/) that you can purchase so you do not need to make the hardware to run the Cruisin Brain software.

The Cruisin Brain is a combination of this software and hardware, to create a full featured onboard computer.

# FEATURES

This software was developed to take input from two switches (buttons) (named Right and Left) and translate button pushes into motion.

- Pressing the Right button, results in the car moving forwards
- Pressing both the Right and Left buttons, results in the car moving reverse
- Holding the Right button for longer than 30 seconds, results in Cruise Control being enabled (car moves forward without input)
- When Cruise Control is enabled pressing the Left (at any time) or Right (5 seconds after Cruise Control is enabled) button disables Cruise Control and the car will brake
- Pressing no buttons results in the car stopping (unless Cruise Control is enabled)

## WARNING ON CRUISE CONTROL
This software has a feature called Cruise Control (enabled by default, can be disabled) which results in the Cruisin Brain holding full throttle without input after Forward has been commanded a set amount of time (normally 30 seconds but can be changed). Cruise Control was implemented 
because the children I created these cars for (7 families in our neighborhood so far) complained that pushing the button for long periods of time hurt their finger (certainly a first world problem). Cruise Control can have an unintended side effect (which has happened to one of the very young children) where the child falls off the car while Cruise Control is enabled, which results in the car continuing on its merry way without a driver.

<br>

-------------------------------------------------------------------------------------------------------

# MOTOR CONTROLLERS SUPPORTED

This software was developed to interface with the following motor controllers:

1. [Dimension Engineering Syren 50](https://www.dimensionengineering.com/products/syren50)
2. [Dimension Engineering Sabertooth 2x32](https://www.dimensionengineering.com/products/sabertooth2x32)
3. [Rev Robotics Spark](http://www.revrobotics.com/rev-11-1200/)

**I have tried for several months to get the motor controllers that come with mobility scooters and electric wheelchairs to work with the Cruisin Brain and I have been unsuccessful. I got a couple of controllers to work but after a while, they simply stopped working for no apparent reason. If you wish to use this software you will need to buy one of the motor controllers listed above**

### Syren50

Mobility scooters can have one motor with a physical differential or two motors, one motor per wheel. The Syren50 will work with both types. To use the Syren50 with one motor, just wire it as per the manual. To use the Syren50 with two motors, you should wire the motors in series so that you get an electronic differential.

NOTE: To use the Syren50 with a mobility scooter you will need to
1. remove the electronic brake attached to the end of the motor (unless you implement control of the brake using a separate piece of hardware). 
2. Cut off all proprietary plugs and connect the correct wires directly to the motor controller as per the Syren50's manual.

### Sabertooth2x32

The Sabertooth2x32 has the ability to control two electric motors (among a number of other features). This software has been designed to support the Sabertooth2x32. The main issue with using the Sabertooth2x32 with two rear motors is there will be no differential. I do not think it is a great idea to use the Sabertooth2x32 for two motors, even though it is possible. The Sabertooth2x32 is useful for people who would like to use it to control an electronic brake, though at this time, I have yet to implement or test this functionality.

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

If using Spark Motor controller

1. Digital pin 9 for signal wire between Arduino Nano and Spark
2. Positive and negative from spark to positive and negative on Arduino Nano

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

You can also configure acceleration (for Forward/Reverse/CruiseControl) and deceleration (braking)

<br>

-------------------------------------------------------------------------------------------------------

# TECHNICAL NOTES

The software has been written with the intention of it being readable more than technically code efficient.
