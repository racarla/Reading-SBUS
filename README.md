# Reading SBUS
In this tutorial we'll read an SBUS packet with our microcontroller. If you haven't already, you may consider going through the [SBUS Introduction tutorial](https://github.com/bolderflight/SBUS-Intro), which explains what SBUS is and provides practical experience through commanding a servo using the SBUS protocol.

For the initial part of this tutorial, we will loop our SBUS transmitter pin back to our SBUS receiver pin. This will allow us to: build an SBUS packet, send it, and then receive it to make sure that it's the same packet that was sent. The second part of this tutorial will involve reading SBUS packets from an SBUS receiver paired with an SBUS capable transmitter.

# Necessary Hardware
   * [Teensy 3.2](https://www.pjrc.com/store/teensy32.html)
   * [Teensy SBUS Backpack](http://bolderflight.com/products/teensy/sbus/)
   * An SBUS capable transmitter and receiver will be used during part of this tutorial, but you'll still be able to complete most of it without these items.

# SBUS Loopback
## Wiring
First, make sure the SBUS RX solderpad on the [Teensy SBUS Backpack](http://bolderflight.com/products/teensy/sbus/) is set to RX instead of TX. Then we'll connect the signal pin from the SBUS RX pin to the pin next to it. 

<img src="/images/loopback.JPG" alt="loopback" width="500">

Finally, plug your [Teensy 3.2](https://www.pjrc.com/store/teensy32.html) into the [Teensy SBUS Backpack](http://bolderflight.com/products/teensy/sbus/) with the USB connector on the end of the backpack that has a white dot; this dot marks the location of the Teensy ground pin. 

<img src="/images/loopback_setup.JPG" alt="loopback setup" width="500">

## Software
We'll use the [SBUS library](https://github.com/bolderflight/SBUS) from Bolder Flight Systems for this tutorial. This library contains methods for reading and writing SBUS packets using either raw counts (i.e. 172 - 1811) or normalized values (i.e. +/- 1).

### Installation
Simply clone or download the [SBUS library](https://github.com/bolderflight/SBUS) to your Arduino libraries folder. For this tutorial, you can clone or download the code in this [repository](https://github.com/bolderflight/Reading-SBUS) and follow along or code it yourself. We'll use the _SBUSLoopback.ino_ for this first test.

### Tutorial
#### Goals
The goal of this tutorial is to send an SBUS packet and then read it back, ensuring that it was the same packet that was sent. We'll use the [SBUS Introduction tutorial](https://github.com/bolderflight/SBUS-Intro) code as a starting point to generate SBUS commands, and add SBUS reading code to it.

### Code Walkthrough
We need to add a few global variables, the first will store an array of SBUS commands that we've received and the next two will store the received lost frames and failsafe values.

```C++
/* 16 SBUS channels, reading */
float SbusRead[16];
/* Failsafe and lost frame status */
bool Failsafe, LostFrame;
```

There is no change to setup, but in loop we'll add a statement to check for received SBUS packets. What we're doing here is checking for a good packet and, if a good packet is received, we'll print the transmitted SBUS command for channel 0 and the received SBUS command for channel 0. We would expect them to match if everything is working properly.

```C++
if (sbus.readCal(&SbusRead[0], &Failsafe, &LostFrame)) {
  Serial.print(SbusRead[0]);
  Serial.print("\t");
  Serial.println(SbusCmds[0]);
}
```

That's it! For a full code listing, please see _SBUSLoopback.ino_.

## Experiment
Upload your code to the Teensy and open the Serial Monitor. You should a stream of two columns of values and notice that these values match. This means we're successfully reading and parsing the SBUS packets that we've sent!

<img src="/images/loopback_exp.png" alt="loopback exp" width="500">

## Wrap Up
In this initial experiment, we looped one of our SBUS transmit pins to our SBUS receive pin. We created and transmitted SBUS packets and then received and parsed those packets. We validated our algorithms by checking that our received SBUS data matches our transmitted data.

# SBUS Transmitter and Receiver
## Wiring
For this part of the tutorial, we will be connecting an SBUS capable receiver to the [Teensy SBUS Backpack](http://bolderflight.com/products/teensy/sbus/). First, make sure the SBUS RX solderpad on the [Teensy SBUS Backpack](http://bolderflight.com/products/teensy/sbus/) is set to RX instead of TX. We would like to power the SBUS receiver from the Teensy as well, so make sure the voltage solderpad is set to 5V instead of VDD (this sends 5V to the SBUS RX power pin instead of the bussed SBUS servo power).

<img src="/images/rx.JPG" alt="receiver" width="500">

Bind an SBUS capable receiver to an SBUS capable transmitter and connect the SBUS receiver to the [Teensy SBUS Backpack](http://bolderflight.com/products/teensy/sbus/). Finally, plug your [Teensy 3.2](https://www.pjrc.com/store/teensy32.html) into the [Teensy SBUS Backpack](http://bolderflight.com/products/teensy/sbus/) with the USB connector on the end of the backpack that has a white dot; this dot marks the location of the Teensy ground pin.

<img src="/images/rx_setup.JPG" alt="receiver setup" width="500">

## Software
We'll use the [SBUS library](https://github.com/bolderflight/SBUS) from Bolder Flight Systems for this tutorial. This library contains methods for reading and writing SBUS packets using either raw counts (i.e. 172 - 1811) or normalized values (i.e. +/- 1).

### Installation
Simply clone or download the [SBUS library](https://github.com/bolderflight/SBUS) to your Arduino libraries folder. For this tutorial, you can clone or download the code in this [repository](https://github.com/bolderflight/Reading-SBUS) and follow along or code it yourself. We'll use the _SBUSReceiver.ino_ for this test.

### Tutorial
#### Goals
The goal of this tutorial is to receive packets from an SBUS receiver connected to an SBUS capable transmitter. We'll display the packet data to view the stick positions, lost frame, and failsafe data.

### Code Walkthrough
#### Globals
We'll declare an SBUS object which uses Serial2. Using the [Backpack pinout](http://bolderflight.com/products/teensy/pinout/), we can see that Serial2 has been reserved for the [Teensy SBUS Backpack](http://bolderflight.com/products/teensy/sbus/).

```C++
SBUS sbus(Serial2);
```

We need a few global variables, the first will store an array of SBUS commands that we've received and the next two will store the received lost frames and failsafe values.

```C++
/* 16 SBUS channels, reading */
float SbusRead[16];
/* Failsafe and lost frame status */
bool Failsafe, LostFrame;
```

#### Setup
We'll print the SBUS data to the Serial Monitor so that we can see how it's changing, so we better start the USB Serial communications.

```C++
Serial.begin(115200);
while (!Serial) {}
```

Now, we'll start the SBUS communication.

```C++
sbus.begin();
```

#### Loop
Within _loop_ we'll cehck for valid SBUS packets. If one is received, we'll print the SBUS commands, lost frame, and fail safe data to the Serial Monitor.

```C++
if (sbus.readCal(&SbusRead[0], &Failsafe, &LostFrame)) {
  for (unsigned int i = 0; i < sizeof(SbusRead) / sizeof(float); i++) {
    Serial.print(SbusRead[i]);
    Serial.print("\t");
  }
  Serial.print(LostFrame);
  Serial.print("\t");
  Serial.println(Failsafe);
}
```

That's it! Your code should compile and be ready to upload to the [Teensy 3.2](https://www.pjrc.com/store/teensy32.html).

## Experiment
Upload your code to the Teensy and open the Serial Monitor. Turn on your SBUS transmitter. You should see a stream of 18 columns of data. The first 16 columns are the SBUS commands for the servos, these are mapped to the sticks and switches on your transmitter. Try moving the sticks and switches while observing how the SBUS values on the Serial Monitor change. You should notice the value move over the +/- 1 range as you move the sticks across their travel.

<img src="/images/rx_exp.png" alt="receiver exp" width="500">

Now, try turning off the transmitter. You should notice the lost frame flag switch from 0 to 1 indicating that the receiver missed a frame of data from the transmitter. A short time later, the failsafe should switch from 0 to 1 indicating that the receiver switched into failsafe mode. Turn back on the transmitter and notice that these flags clear and that you're able to see the SBUS commands change as you move the sticks again.

[![Experiment Video](https://img.youtube.com/vi/UiUhGVO6D8g/0.jpg)](https://www.youtube.com/watch?v=UiUhGVO6D8g)

## Wrap Up
In this experiment, we used [Teensy 3.2](https://www.pjrc.com/store/teensy32.html) to recieve and iterpret SBUS packets from an SBUS capable receiver connected to an SBUS transmitter. We observed how we can measure pilot commands from the SBUS channel data and link status via the lost frame and fail safe flags.

# Further Reading
1. [Bolder Flight Systems SBUS library](https://github.com/bolderflight/SBUS): study the additional methods available and try them out for yourself!

# Next Steps
Armed with the knowledge and experience that you gained with this tutorial, see if you can do the following:
1. Add some SBUS servos and try using the [Teensy 3.2](https://www.pjrc.com/store/teensy32.html) and [Teensy SBUS Backpack](http://bolderflight.com/products/teensy/sbus/) as a mixer. Can you make a single stick control two servos in the same direction? In opposite directions?
2. Try applying some stick shaping. Can you make the servos more sensitive to stick movements? Less sensitive? Try writing software to apply exponential, where the stick is less sensitive near the center and progressively more sensitive toward the extremes. Can you write a function so the amount of exponential can be easily changed?

# Tutorials in the Series
1. [PWM Introduction](https://github.com/bolderflight/PWM-Intro): Learn about the de facto standard for controlling servos, PWM!
2. [Reading PWM](https://github.com/bolderflight/Reading-PWM): Gain experience measuring pulse widths!
3. [SBUS Introduction](https://github.com/bolderflight/SBUS-Intro): Learn about one of the newest, coolest methods of commanding servos, SBUS!
4. [Reading SBUS](https://github.com/bolderflight/Reading-SBUS): Gain experience reading SBUS packets!
5. [SBUS to PWM Converter](https://github.com/bolderflight/SBUS-to-PWM): Use your knowledge of PWM and SBUS to create your own SBUS to PWM converter!
