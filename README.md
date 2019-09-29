# Drainage

> Innovative solution that can exploit the potential of Artificial Intelligence, the Internet of Things, and Cloud to solve the problem of water drainage in modern-day metropolitan cities.

## Flowmeter.ino 

The Arduino Sketch uses the external interrupt (int 0) on Arduino’s digital pin 2 (D2). This is used to read the output pulses coming from the water flow sensor. When Arduino detects the pulse, it immediately triggers the pulseCounter() function. This function then counts the total number of pulses detected.

> Flow sensors typically output a series of pulses proportional to the instantaneous flow rate which means that to interpret them it is necessary to implement a simple frequency counter.

## PressureWaterLevel.ino

The Arduino Sketch uses the reading from the A0 pin and outputs the level of water detected using the diffrential pressure, since the pressure at a point is directly proportianal to the height of the water column above it.

## MultichannelGasSensor.ino

The Arduino Sketch uses the readings from the grove multichannel gas sensor to obtain readings for the levels of NH<sub>3</sub>, CO, NO<sub>2</sub>, C<sub>3</sub>H<sub>8</sub>, C<sub>4</sub>H<sub>10</sub>, CH<sub>4</sub>, and H<sub>2</sub>.

## AllSensors.ino

The Arduino Sketch integrates four sensors and shows the sending of data to Thingspeak.
