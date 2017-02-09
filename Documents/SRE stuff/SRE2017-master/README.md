# SRE-2017

[![Build Status] (https://travis-ci.org/TimothyLe/SRE2017.svg?branch=master)] (https://travis-ci.org/TimothyLe/SRE2017)

### Build Error Message
* Git remote add attempt
**Resolve whenever**
```git
$ git push origin master
To https://github.com/TimothyLe/SRE2017.git
 ! [rejected]        master -> master (fetch first)
error: failed to push some refs to 'https://github.com/TimothyLe/SRE2017.git'
hint: Updates were rejected because the remote contains work that you do
hint: not have locally. This is usually caused by another repository pushing
hint: to the same ref. You may want to first integrate the remote changes
hint: (e.g., 'git pull ...') before pushing again.
hint: See the 'Note about fast-forwards' in 'git push --help' for details.

```


## My custom rendition of SRE code
Focuses will be on
* Read/Write EEPROM testing (__me__)
* Wheel Speed Sensor Read/Calculate Frequency :white_check_mark:
* Sending text to dash for easier testing (__maybe__)
* Water Pump Testing (__further revision__)
* Rotary encoder input
* BMS Linduinos (2026C, etc.)

### TCS POT Ranges
![POT ranges](https://github.com/TimothyLe/SRE2017/blob/master/tim_stuff/potranges.JPG)

## Libraries
| Name | Includes | 
| ---- | -------- |
|`IO_Driver.h`|  Basic SPI driver functions from the VCU                                                      |
|`IO_EEPROM.h`|  Read/Write EEPROM along with check EE status                                                 |
|`APDB.h`     |  Defines the application database which is used by the boot loader to determine if app starts |
|`IO_UART.h`  |  Reads in serial numbers and other serial things                                              |
|`IO_RTC.h`   |  Real time clock                                                                              |
|`IO_ADC`     |  Analog to Digital Converter channel inits and gets for analog pins                           |
|`IO_DIO.h`   |  Digital input and output for controlling LEDs and switching them on/off                      |
|`IO_PWD.h`   |  Pulse Width Demodulation with timer inputs to measure frequency and/or pulse-width           |

## Links
[Programming for the HY-TTC50](http://1drv.ms/1NQUppu)

[VCU Pinout](https://1drv.ms/x/s!Avhc248Pj7v5gt4A6qt_feUsrY_taA) *Incorrect*

[Updated Pinout w/ Ranges](https://app.box.com/s/b72z2jhthd8sobolhyxe6k7jv8ms0adg)