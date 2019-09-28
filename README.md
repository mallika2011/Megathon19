# Drainage
Innovative solution that can exploit the potential of Artificial Intelligence, the Internet of Things, and Cloud to solve the problem of water drainage in modern-day metropolitan cities.

## Flowmeter.ino 

The Arduino Sketch uses the external interrupt (int 0) on Arduino’s digital pin 2 (D2). 

This is used to read the output pulses coming from the water flow sensor. 

When Arduino detects the pulse, it immediately triggers the pulseCounter() function. This function then counts the total number of pulses detected.

Flow sensors typically output a series of pulses proportional to the instantaneous flow rate which means that to interpret them it is necessary to implement a simple frequency counter.

# 