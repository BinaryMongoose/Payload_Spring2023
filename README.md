  # Payload_Spring2023

Code for CAC's Payload in Spring of 2023


⚠️ THE CODE USED IN THE PAYLOAD IS ON THE DEBUGGING BRANCH ⚠ 

Learn more down [here.](https://github.com/BinaryMongoose/Payload_Spring2023#software)

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
+ Takes up a lot of space. 
That's it. (I'm biased)

There are some problems with using CircuitPython. At first we wanted to use an [Adalogger 32u4.](https://www.adafruit.com/product/2795)
We ran into storage problems very early on. A basic blink sketch in C++ code took up 13% of the avaible FRAM! 
I didn't even think of programming CircuitPython on this microcontroller.

So, we turned to the [Adalogger M0.](https://www.adafruit.com/product/2796)
This guy was able to handle its code pretty well. The available space after loading the GPS_Log_Test was 23%. Nice!
But, when programmed in CircuitPython, I could only get around 250 lines of code without importing anything. Not going to work.

Finally, for CircuitPython programming, we settled on the GOAT (For me) of any microcontroller. The [Adafruit Feather RP2040.](https://www.adafruit.com/product/4884) \
This bad boy has ***8 MB SPI FLASH***! \
Then a 32-Bit dual Core?!  \
That's perfection. 

I think you can tell what Feather we used. 

Due to my lack of foresight, we used a adalogger 32uf. I didn't put in the order on time, so we weren't able to get the cool RP2040. 
So if you are reading this in the future, PURCHASE THOUSANDS OF RP2040 BOARDS. They are really cool. 

### Sensors

Nothing too extreme here. Some basic things like temperature, humidity, and altitude. (All done by the [BMP388](https://www.adafruit.com/product/4816) BTW) \
Ww included some "specialty" sensors, including a [Accelerometer + Gyro](https://www.adafruit.com/product/4502), and a [UV light sensor](https://www.adafruit.com/product/4831). 

The biggest "sensor" was the [GPS](https://www.adafruit.com/product/3133). \
This is a "wing", so it stacks on top of the Feather. Like an Arduino shield, but somehow better. (I'm clearly biased.)\
We are using an active GPS [antenna](), which will be situated outside of the payload box. \

All in all, we will be collecting Temperature inside the box, Altitude, G-Forces acting on the box, location, and time.

### Software
Now here's my specialty. 

Due to my stupidity, we had to end up using C# for the payload project. Not a terrible outcome, but not the best either. 

Now, for those familiar with VCS, or GitHub, you will notice that I have 2 branches. 
The names should be self-explanatory, yet the software that was sent up 110,000 feet into the air was the code on the _DEBUGGING_ branch.

Now, there is many ways I could explain away such an atrocity but take a look for yourself at the Debugging and main branches. 
I guess you can say I got bored. I finished the original (debugging) code, and then I decided to excercise some mental powers to create a 
true class like program. This is the result of the main branch program. I couldn’t finish before the project ended, but I still learned a lot.

There were three main ideas I was trying to accomplish with the new code. 

1. Make the code more readable.
2. Easier to debug.
3. More resistant to failure.

I'll go through these ideas more thoroughly below.

#### Increasing Readability
Just look at the code for the Debugging branch. It looks, tastes, smells, feels, like a train wreck.


#### Debug
This is closely tied to the idea above. Debugging something like the software in the debugging branch is a nightmare. One semicolon, one + 1, one
tiny logical error and you're screwed. We needed a better system. 


#### Failure Resistant
Oh the bliss that software companies have. They (usually) only have to deal with problems such as corrupted memory, low latency, and the occasional cable error. 

Not so much when you're setting a 3-pound box up 110 thousand feet into the air. Wires are unplugged! Friction locks break (as I learned all to painfully in the Fall 2022 semester...), JB-Weld tears foam, PCBs shatter, and worst of all, wires become unplugged. Must I explain more? 

The origional software is a total freight train. It'll get the job done and it'll get it done fast. However, place on obstacle in its path, and it will 
totally de-rail. The software is an idiot, blindly following orders with no thought into what is happening. There is a terrifying lack of redundancy checks and error avoidince systems.

The goal of the new system was to be more intelligent. 

- It would continually scan which sensors are attached. If a sensor becomes detached, then it writes an error code and continues.
- The software would actually know what the sensors are measuring. This is a profound thought; this means that the sensor can redelegate tasks.
  If you have two sensors capable of measuring temperature and one breaks, then the software gets the temp data from the other sensor.
  The software can make decisions based on what's available and redelegate tasks if need be.
- The output would be cleaner. I could "ask" for data that I wanted, and the box could tell me. Instead of continually trucking through the sensor readings, it'll
  only give me what I need/want to hear.

This was the goal of the main branch. But I ran out of time, and I am too dumb to do it in C#. I will try to create this system in Python next time.
