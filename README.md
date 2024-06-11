Engineering materials
====

This repository contains engineering materials of a self-driven vehicle's model participating in the WRO Future Engineers competition in the season 2024.

## Content

* `t-photos` contains 2 photos of the team (an official one and one funny photo with all team members)
* `v-photos` contains 6 photos of the vehicle (from every side, from top and bottom)
* `video` contains the video.md file with the link to a video where driving demonstration exists
* `schemes` contains one or several schematic diagrams in form of JPEG, PNG or PDF of the electromechanical components illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `src` contains code of control software for all components which were programmed to participate in the competition
* `models` is for the files for models used by 3D printers, laser cutting machines and CNC machines to produce the vehicle elements. If there is nothing to add to this location, the directory can be removed.
* `other` is for other files which can be used to understand how to prepare the vehicle for the competition. It may include documentation how to connect to a SBC/SBM and upload files there, datasets, hardware specifications, communication protocols descriptions etc. If there is nothing to add to this location, the directory can be removed.

## Introduction

`DistanceSensor` This class have 2 private atributes:
  * `Trigger` Pin connected to the trigger pin of the ultrasonic sensor.
  * `Echo` Pin connected to the echo pin of the ultrasonic sensor.
 
It also have 3 public methods:
  * `distanceSensor()` The class constructor. It takes 2 arguments: `trigger_pin` and `echo_pin`, that are used to set up the trigger and echo pins of the sensor.
  * `init()` Set up the trigger and echo pins as output and input pins respectively. It also initialized the trigger pin in a low state.
  * `getDistance()` Measure the distance to an object using the ultrasonic sensor. It sends an ultrasonic pulse, measures the time it takes for the pulse to reflect off an object and return, and then calculate the distance based on time and speed of sound.

### GetDisance() Operating

The getDistance() method perform the following steps to measure the distance to an object:

* **1. Sends an ultrasonic pulse:**
  * Set up the trigger pin in a high state for a short period of time to send an ultrasonic pulse.
* **2. Measure the time of the pulse:**
  * Set up the echo pin as an input.
  * Wait for the echo pin to go high state, which indicates that the reflected pulse has been received.
  * Measures the time it takes for the echo pin to go from low to high. This time represents the half total time that it takes to the ultrasonic pulse to travel to the object and return.
* **3. Calculate the distance:**
  * Multiply the pulse time by the speed of sound to calculate the total distance traveled by the ultrasonic pulse.
  * Divide the total distance by 2 to get the distance to the object, because the pulse travels the distance 2 times.
 
`ImuAndAdps` This class is utilized to read data from the inertial sensor (IMU) and the color sensor (APDS) from the `Arduino Nano BLE 33 Sense Rev2`.

* **1. Class Properties:**
  * `Threshold` Private variable that defines the sensibility of the z axis.
  * `x`,`y`,`z` Public variables that save the inclination data from the x, y, z axis readed by IMU.
  * `r`,`g`,`b` Public variables that save the values of red, green, blue colors (in a 8-bits format) readed by APDS.

* **2. Constructor `imuAndAdps()`:**
  * Takes an argument `_threshold` that it asigned to the `threshold` variable.

* **3. `init()` Method:**
  * Set up the 3 LEDs pins (red, green and blue) as output pins.
  * Try to initialized the APDS sensor (`APDS.begin()`) and print a message in the serial monitor if the inicialization fail.
  * Try to initialized the IMU sensor (`IMU.begin()`) and print a message in the serial monitor if the inicialization fail.
 
* **4. `ReadColor()` Method:**
  * Wait until color data are available from the color sensor APDS (`APDS.colorAvailable()`).
  * Turn on the 3 LEDs (red, green, blue).
  * Read the colors values of the APDS sensor and save them in the `r`,`g`,`b` variables.
  * Applied corrections to the color values (multiply by `RedCorrection`, `GreenCorrection` and `BlueCorrection`) and limit them to a range from 0 to 255.
 
### Motors

The `DC Motor` together the `L298N` functions with 4 methods: 
  * `InitMotor()` Set up the L298N 'IN' pins as output pins.
  * `GoForward()` Power up the DC motor to go forward.
  * `GoBackward()` Power up the DC motor to go backward.
  * `Stop()` Power off the DC Motor to stop.

The `Servo Motor` functions with 3 methods:
  * `GoRight()` Set up the Servo angle to 45°.
  * `GoLeft()` Set up the Servo angle to 135°.
  * `Gofront()` Set up the Servo angle to 0°.
