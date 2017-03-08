#include <stdio.h>
#include "IO_EEPROM.h"
#include "IO_Driver.h"
#include "IO_CAN.h"
#include "eeprom.h"
using namespace std;

/*Definitions*/
void EEPROM::readEP(ubyte2 offset, ubyte2 length, ubyte1 data){
    //check if the EEPROM is busy
    if(IO_EEPROM_GetStatus() == IO_E_OK){
        //not busy starts reading
        IO_EEPROM_Read(offset, length, &data);
    }
    //data is busy not available
    //needs IO_EEPROM_GetStatus 
    //to return IO_E_OK 
}

void EEPROM::writeEP(ubyte2 offset, ubyte2 length, ubyte1 * data){
     //check if the EEPROM is busy
    if(IO_EEPROM_GetStatus() == IO_E_OK){
        //not busy starts reading
        IO_EEPROM_Write(offset, length, data);
    }
    //data is busy not available
    //needs IO_EEPROM_GetStatus 
    //to return IO_E_OK 
}
