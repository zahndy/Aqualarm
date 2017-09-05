# Aqualarm
Aquarium Alarm that we use to keep check of aquarium temperature using a inline thermistor.
2 stage Alarm goes off when temp is out of bounds.
you can also use this as leak detection as the alarm went off when our pump leaked on the connectors of the thermistor wires.
required:thermistor, Arduino(nano), a 4 pin I2C oled display (3v, grnd, scl, sda) and a few resistors.

Arduino setup:
0 ohm bridge between 3V3 & REF
10K or resistor with resistance equal to the thermistor, between 3V3 & A0  
thermistor between GND and A0 
Display on Gnd, 3v, SCL and SDA
Speaker between Gnd and D4

(10K resistor bands: brown(1) black(0) orange(*1K) + tolerance level)