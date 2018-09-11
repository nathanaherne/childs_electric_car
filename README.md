## Cruisin Brain - Education Version

This is the Cruisin Brain (education version) software. The Cruisin Brain was developed for the Cruisin Electric Car, so that it was awesome, educational and safe. Detailed instructions on how to build the Cruisin Electric car are [available here](https://cruisin.co/diy-cruisin-car/buildOverview/).

Currently you need to build your own Cruisin Brain hardware. We are working on creating [Cruisin Brain](https://cruisin.co/cruisin-brain/cruisinBrain/) hardware that you can purchase so you do not need to make the hardware yourself.

<br>

## Features

This software was developed to take input from two switches (buttons) (named Forward and Reverse) and translate button pushes into motion.

- Pressing the Forward button, results in the car moving Forwards
- Pressing both the Forward and Reverse buttons, results in the car moving in Reverse
- Holding the Right button for longer than 30 seconds, results in Cruise Control being enabled (car moves Forward without input)
- When Cruise Control is enabled pressing the Reverse (at any time) or Forward (5 seconds after Cruise Control is enabled) button disables Cruise Control and the car will brake
- Pressing no buttons results in the car stopping (unless Cruise Control is enabled)

<br>

## Motor Controllers supported

This software was developed to interface with the following motor controllers:

1. [Dimension Engineering Syren 50](https://www.dimensionengineering.com/products/syren50)
2. [Dimension Engineering Sabertooth 2x32](https://www.dimensionengineering.com/products/sabertooth2x32)
3. [Rev Robotics Spark](http://www.revrobotics.com/rev-11-1200/)

**I have tried for several months to get the motor controllers that come with mobility scooters and electric wheelchairs to work with the Cruisin Brain and I have been unsuccessful. I got a couple of controllers to work but after a while, they simply stopped working for no apparent reason. If you wish to use this software you will need to buy one of the motor controllers listed above**

### Syren50

Your car may have one motor with a physical differential or two motors, one motor per wheel. The Syren50 will work with both types. To use the Syren50 with one motor, just wire it as per the manual. To use the Syren50 with two motors, you should wire the motors in series so that you get an electronic differential, though speed will be halved.

NOTE: You will need to remove the electronic brakes from the motors or implement control of the electronic brakes yourself.

### Sabertooth2x32

The Sabertooth2x32 has the ability to control two electric motors (among a number of other features). This software has been designed to support the Sabertooth2x32. The main issue with using the Sabertooth2x32 with two rear motors is there will be no differential, the rear wheels will be locked, making steering difficult.

### Rev Robotics Spark

Your car may have one motor with a physical differential or two motors, one motor per wheel. The Rev Robotics Spark will work with both types. To use the Rev Robotics Spark with one motor, just wire it as per the manual. To use the Spark with two motors, you should wire the motors in series so that you get an electronic differential, though speed will be halved.

NOTE: You will need to remove the electronic brakes from the motors or implement control of the electronic brakes yourself.

<br>

## Wiring

The software expects the following wiring connections:

1. Forward button - pin PB0 and VCC, with 5k pull down resistor
2. Reverse button - pin PC13 and VCC, with 5k pull down resistor
3. Inhibit pin - pin PB2, with 5K pull up resistor

### Syren50 or Sabertooth2x32

1. Serial1 TX on Maple Mini to S1 on motor controller
2. 5v and Gnd on Maple Mini to 5v and 0v on motor controller - the Maple Mini is powered by the motor controller

### Spark Motor controller

1. Digital pin 9 for signal wire between Maple Mini and Spark
2. You will need to power the Maple Mini with a separate 5v power supply

<br>

## Configuration

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

## LEGAL STUFF

You use this software and instructions at your own risk - if you break something, hurt someone or something or anything else happens, I am not responsible.

<br>

## WARNINGS

### CRUISE CONTROL
This software has a feature called Cruise Control (enabled by default, can be disabled) which results in the Cruisin Brain holding full throttle without input after Forward has been commanded a set amount of time (normally 30 seconds but can be changed). Cruise Control was implemented 
because the children I created these cars for (7 families in our neighborhood so far) complained that pushing the button for long periods of time hurt their finger (certainly a first world problem). Cruise Control can have an unintended side effect (which has happened to one of the very young children) where the child falls off the car while Cruise Control is enabled, which results in the car continuing on its merry way without a driver.

<br>

## TECHNICAL NOTES

The software has been written with the intention of it being readable more than technically code efficient.
