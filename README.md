# SRE-2017

[![Build Status](https://travis-ci.org/TimothyLe/SRE2017.svg?branch=master)](https://travis-ci.org/TimothyLe/SRE2017)

## Things of Interest
**Ignore the master and documents directories, they were added by Git Bash**  
1. EEPROM, can manager, and the car's main revisions located in *tim's stuff*  
2. Errors and warnings screenshots found in *Errors_Warnings*   
3. All the readings from EEPROM can be found in *eeprom_reads*   

## My custom rendition of SRE code  
**Goals this week**  
* Get that code to compile and link

**Completed**
* Basic EEPROM outline (data structures, declarations, definitions, etc) :white_check_mark:  
* DIO test phase 1 (*outcome not as expected*) :white_check_mark:  
* Unit testing for EEPROM (*successfully wrote to EEPROM*) :white_check_mark:  
* EEPROM Research :white_check_mark:    
* EEPROM parse assigns eepromValues to hex values  
* EEPROM set/get written out  
* EEPROM_initialized checks if constructor is successful and returns boolean  
* read_initialized returns the values of EEPROM  
* EEPROM getStatus using IO_EEPROM API to return condition  
* getAddress returns a paticular ubyte2 offset from an eepromValue as a paramter using parse function  
* Reading the EEPROM stores values into data_hardware dynamic array  
* Writing the EEPROM stores values into data_software dynamic array  
* New length member in EEPROMManager object  
* Bottom of header file stores set/get prototype in comments of individual eepromValues  

## Casual.pm Tasks
* Read/Write EEPROM testing (__me__)  
* Wheel Speed Sensor Read/Calculate Frequency :white_check_mark:  
* Sending text to dash for easier testing (__maybe__)  
* Water Pump Testing (__further revision__)  
* Rotary encoder input   
* BMS Linduinos (2026C, etc.)   
* Regen :white_check_mark:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; * Multiplier (Red Light) :white_check_mark:  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; * Torque shuts down after 1 error (e.g. low voltage)  

### SRE Wiki
For more information on certain components of the code or more clarity on topics, consult my wiki for more insight on certain topics and potential frequently asked questions after finishing this readme:   
https://github.com/TimothyLe/SRE2017/wiki

## Links
[EEPROM Addresses](https://app.box.com/s/nbz92djxusbby6u214ghj4r6kfeen3ia)

[Programming for the HY-TTC50](http://1drv.ms/1NQUppu)

[VCU Pinout](https://1drv.ms/x/s!Avhc248Pj7v5gt4A6qt_feUsrY_taA) *Incorrect*

[Updated Pinout w/ Ranges](https://app.box.com/s/b72z2jhthd8sobolhyxe6k7jv8ms0adg)

[Team Website](http://spartanracingelectric.com)
