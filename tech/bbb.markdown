---
title: Low power board requirement
layout: default
---

Arduino compatible low power board requirement

# Why Arduino Uno?

First we need to understand why we went with Arduino Uno boards for prototyping. Say, why not MSP430 or 8051 or 
PIC or ARM board etc. ? Arduino allows for quick prototyping because of a vast ecosystem. Whether you want to do 
SPI or I2C or plug in a SD card or wire a sensor, chances are that someone has done it before. Availability of 
ready made libraries is a big win. 

[Arduino Uno](http://arduino.cc/en/Main/arduinoBoardUno)

Arduino environment also has wrappers to make programming easier. So instead of turning individual bits you can 
simply write  pinout(13,HIGH). So you can accomplish things using simple software that otherwise require knowledge
of hardware.

That was the reason we did prototyping of AWS circuit on Arduino Uno boards.

# Problem with Arduino Uno 

Arduino Uno is a hobbyist board and is as such not optimized for low power usage. This board is supposed to be 
run from DC adapter or USB power so it contains linear voltage regulators like 7805 that waste lot of power.

After AWS prototyping, now we want the same circuit to run on batteries. An aws need not work all the time. It 
needs to wake up every 20 minute or so, collect data from sensors and go back to sleep. The target is to run this 
circuit on 6 AA 1.5V 1500 mAH batteries for six months. Most of the peripherals in use need 3.3V-5V.

We thought that if we power down the chip and peripherals then we can save power. However, the issue is that an 
Arduino Uno board sucks 35mA kind of current even in power down mode. At that rate we need 840mAH per day even for
a board sleeping all the time. This board will not survive even two days on a 1500mAh battery.


# Options

## Ditch Arduino, switch to another board

Clearly, we cannot use Arduino Uno board but at the same time just switching the board is no silver bullet. Any kind of 
development board will have components not optimized for low power usage. Like, there are lot of claims that MSP430 is a 
low power board, but you will only get the data for the chip and not the full board. It is not like I can just switch to 
off-the-shelf MSP430 board and all problem would be solved.  There are two efforts involved here, fix the hardware so it 
consumes less power and fix the software to utilize that hardware.

If we had a readymade low power board then we are not spending effort on hardware and porting software to a new board makes
sense. So far I have not seen any ready made board that justifies this trade off.

## Retain ATMEGA and Arduino environment

Next option is to fix the board. Make a board that is Arduino compatible and at the same time can be put down to uA kind of 
current in sleep mode. That way we are spending effort on hardware and software effort is easier beacuse we are working in 
a familiar environment.

Are there any precedents where people have managed to retain the Arduino Envrionment and yet work on batteries? A very good
reference for ATMEGA 328, the chip on Arduino Uno is 

[Nick Gammon's page on ATMEGA328 power saving techniques](http://www.gammon.com.au/forum/?id=11497)

Then there are people like Rocketstream who have created Arduino compatible low power boards ,e.g. the Mini Ultra 8MHZ and 
software libraries. The JeeNode from JeeLabs is another low power optimized board. So, yes, it can be done if someone has 
experience working at the board level.

### Rocketscream 

* [Rocketscream page](http://www.rocketscream.com/blog/2011/07/04/lightweight-low-power-arduino-library/)
* [Mini Ultra 8MHZ board](http://www.rocketscream.com/blog/category/projects/mini-ultra-8-mhz/)
* [Rocketscream low power library](https://github.com/rocketscream/Low-Power)

### Jeenode

* [3-years on one set of batteries](http://jeelabs.org/tag/lowpower/)
* [Jeenode AA power board](http://jeelabs.net/projects/hardware/wiki/AA_Power_Board)
* [Jeenode board](http://jeelabs.net/projects/hardware/wiki/JeeNode)


# Theory of ATMEGA 328

The chip conatined in Arduino Uno board is ATMEGA 328 which has six sleep modes. If the ATMEGA 328 is put in 
*SLEEP_MODE_PWR_DOWN* and ADC is disabled then we can get down to micro ampere (uA) current draw.

* [ATMEGA 328 datasheet](http://www.atmel.com/images/atmel-8271-8-bit-avr-microcontroller-atmega48a-48pa-88a-88pa-168a-168pa-328-328p_datasheet.pdf)
* [ATMEGA 328 on Atmel site](http://www.atmel.com/devices/atmega328.aspx)


# Our requirement

To power our circuit on batteries and retain our development environment

- We need a board that will have very low sleep current ~ uA
- Other components like voltage regulators should only draw uA in sleep mode
- 5V input and peak 1A current requirement for GSM modem (we cannot go down to 3.3V because of peripherals)
- Use ATMEGA 328 so our software can be reused
- Software changes e.g. powering down the rig and peripherals 
- Circuit components - e.g. RTC chip to wakeup the board periodically
	

So we would like to retain as much of Arduino Uno as possible, throw out things that increase sleep current, like linear voltage regulator and add components that will allow us to run it with a uA sleep current. There are variations of Arduino Uno available that are geared towards low power consumption and we can learn from them.

We want a board that is 

- Arduino compatible (same SPI/I2C/pinout etc.) 
- can work with 4AA (6V) or 6AA (9V) batteries for six months
- Allows us to wire all AWS peripherals 
- Allows us to reuse our current software 
- Add watchdog to reboot in case of hung board
- Add an RTC with  alarm to send level interrupts


# Our Plan

## Get an Arduino Compatible low power board

First make a board that can replace Arduino Uno. It should be able to run our current software unmodified and we should be
able to wire all our sensors without changes. Then we should measure the sleep current of board + peripherlas after putting 
them in sleep mode. The current draw should be in uA. 

## Add RTC with alarm and watchdog to circuit

After we have the board, we need software changes to put the board and peripherals in sleep mode. To wake up the board 
periodically we need an external mechanism. An RTC chip with alarm capability can send a periodic level interrupt to 
wake up our board. The board on wake up can power up the peripherals, do the chores and put everything back to sleep.

We also need a watchdog to ensure that we can reboot the hung board.


## Software

The board alone is not sufficient. We also need to make changes in software to make use of the low power board. 

### Questions

- How to put the board in sleep mode?
- How to wake up the board?
- How to shut down the peripherals?
- How to wake up the peripherals?

# Useful links

* [Arduino forums] (http://forum.arduino.cc/)
* [Arduino sleep code] (http://playground.arduino.cc/Learning/arduinoSleepCode)
* [Rocketscream low power library](https://github.com/rocketscream/Low-Power)
* [AWS code] (https://github.com/rjha/aws/blob/master/code/arduino/main/aws_gprs/aws_gprs.ino)
* [AVR and Arduino sleep mode basics](http://www.engblaze.com/hush-little-microprocessor-avr-and-arduino-sleep-mode-basics/)
* [Sparkfun low power page](https://www.sparkfun.com/tutorials/309)



