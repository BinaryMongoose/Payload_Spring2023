# Payload_Spring2023

Code for CAC's Payload in Spring of 2023


## What we Used:

This year we really changed things up. \
Instead of using the Arduino Uno or Mega, like teams usually do, we used an Adafruit Feather with i2c sensors.

I think it's a great setup. \
Lower energy consumption, better processer, dramatically reduced size, and easier to use.

Adafruit needs to sponsor or me or something, I swear by their products, **AND** force everyone to use them!

### Microcontroller

Our goal was to use python as our programming language. 
The best way to do this while keeping the code simple and easy to use was to use [CircuitPython.](https://circuitpython.org/)

Pros:
+ Easy to learn and use.
+ It's Python!
+ Insanely elegant file managment, reading/writing files, etc.

Cons:
+ Takes a lot of space. 
That's it. (I'm biased)

There we're some problems with using CircuitPython. At first we wanted to use an [Adalogger 32u4.](https://www.adafruit.com/product/2795)
We ran into storage problems very early on. A basic blink sketch in C++ code tooke up 13% of the avaible FRAM! 
I didn't even think of programming CircuitPython on this micrcontroller.

So, we turned to the [Adalogger M0.](https://www.adafruit.com/product/2796)
This guy was able to handle it's code pretty well. THe avaible space after loading the GPS_Log_Test was 23%. Nice!
But, when programmed in CircuitPython, I could only get around 250 lines of code without importing anything. Not going to work.

Finally, for CircuitPyhton programming, we setteled on the GOAT (For me) of any microcontroller. The [Adafruit Feather RP2040.](https://www.adafruit.com/product/4884) \
This bad boy has ***8 MB SPI FLASH***! \
Then a 32-Bit dual Core?!  \
That's perfection. 

I think you can tell what Feather we used. 

Due to my lack of foresight we used a adalogger 32uf. I didn't put in the order on time so we weren't able to get the cool RP2040. 
So if you are reading this in the future, PURCHASE THOUSANDS OF RP2040 BOARDS. They are really cool. 

### Sensors

Nothing too extreme here. Some basic things like tempature, humidity, and altitude. (All done by the [BMP388](https://www.adafruit.com/product/4816) BTW) \
Ww included some "specialty" sensors, including a [Accelerometer + Gyro](https://www.adafruit.com/product/4502), and a [UV light sensor](https://www.adafruit.com/product/4831). 

The biggest "sensor" was the [GPS](https://www.adafruit.com/product/3133). \
This was actually, a "wing", so it stacks on top of the Feather. Like an Arduino shield, but somehow better. (I'm clearly biased.)\
We are using an active GPS [antenna](), whcih will be situated outside of the payload box. \

All in all, we will be collecting Temperature inside the box, Altitude, G-Forces acting on the box, location, and time.

### Software
Now here's my specialty. 

Due to my stupidity, we had to end up using C# for the payload project. Not a terrible outcome, but not the best either. 

Now, for those familiar with VCS, or GItHub, you will notice that I have three branches. 



