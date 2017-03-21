# SRE-2017

[![Build Status] (https://travis-ci.org/TimothyLe/SRE2017.svg?branch=master)] (https://travis-ci.org/TimothyLe/SRE2017)

## Things of Interest
**Ignore the master and documents directories, they were added by Git Bash**  
1. EEPROM, can manager, and the car's main revisions located in *tim's stuff*  
2. Errors and warnings screenshots found in *Errors_Warnings*   
3. All the readings from EEPROM can be found in *eeprom_reads*   

## My custom rendition of SRE code  
**Goals this week**  
* __Rusty's EEPROM Architecture__   
P:
Included EEPROM functions previously written
Helper functions
Accessors
Mutators
Shifting EEPROM messages (float, ubyte, signed byte)
UML Diagrams ☺
Displays interaction between the EEPROM, manager, AVL tree (if implemented) 
I:  
Pushed to personal Github branch SRE2017 (eManager)
Push to SRE3 EEPROM branch 
G:
Eeprom.c Special function sync
Doxygen eeprom.h
Assign data types to EEPROM typedefs
Manage the static variables and functions
AVL Tree for binary search (maybe)  
* AVL Tree (*binary search for copies*)  
* Write EEPROM header and implementation files  
* Edit canManager to send EEPROM messages     

**Completed**
* Basic EEPROM outline (data structures, declarations, definitions, etc) :white_check_mark:  
* DIO test phase 1 (*outcome not as expected*) :white_check_mark:  
* Unit testing for EEPROM (*successfully wrote to EEPROM*) :white_check_mark:  
* EEPROM Research :white_check_mark:    

### Casual.pm Tasks
* Read/Write EEPROM testing (__me__)  
* Wheel Speed Sensor Read/Calculate Frequency :white_check_mark:  
* Sending text to dash for easier testing (__maybe__)  
* Water Pump Testing (__further revision__)  
* Rotary encoder input   
* BMS Linduinos (2026C, etc.)   
* Regen :white_check_mark:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; * Multiplier (Red Light) :white_check_mark:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; * Torque shuts down after 1 error (e.g. low voltage)  

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

### Making a push request
Open up a Git Bash, cmder, or etc and type the following commands.  
```
git clone https://github.com/TimothyLe/SRE2017.git [Your local directory]  
git add . [or any single files you like]  
git commit -m "Your message"  
git remote add origin https://github.com/TimothyLe/SRE2017.git  
git remote -v  
git pull origin master  
git push origin master  
log [optional]  
```
Shortly, the request should arrive here.   

## Links
[Programming for the HY-TTC50](http://1drv.ms/1NQUppu)

[VCU Pinout](https://1drv.ms/x/s!Avhc248Pj7v5gt4A6qt_feUsrY_taA) *Incorrect*

[Updated Pinout w/ Ranges](https://app.box.com/s/b72z2jhthd8sobolhyxe6k7jv8ms0adg)

### For information, consult the wiki
