#The following tasks are there to be resolved:

Led-control:
- Individually callable led segements each numbered with their number of the address system
- allow control to set the lights color intensity and warmth


Lightsensor:
- Fully integratable measuerment unit
- Code should be a single class with the following methods:
    - measure()-> returns the raw value of the lightsensor
    - setCutoff(double cutoff)-> sets the cutoff of what constututes bright and dark
    - isBright()-> retunrs wheather or not the surrounding is bright

Motionsensor:
- develop a system to track the different sensors
- allow to register the sensitivity better
- how do i read it out via i2c or spi?

