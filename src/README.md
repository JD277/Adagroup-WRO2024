Serial Communication.
the process of sending data one bit at a time, sequentially, over a communication channel or computer bus.

The serial communication with the Raspberry Pi and the Arduino works this way:

first the Arduino sensors detects the vehicle's rotation and the color in front of the sensor, after these values are captured, they are divided into arrays of bytes, then are sent to the raspberry pi through serial communication, then in the Raspberry, these arrays are again converted to normal values, float or integer.

After this values are obtained they can be used to trigger functions in the vehicle, like changing direction, speeding up or slowing down. this serial communication between the arduino and the raspberry is key to the correct operation of the vehicle.