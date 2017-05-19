#include <stdlib.h> //malloc
#include <stdio.h>
#include "IO_Driver.h"
#include "IO_EEPROM.h"
#include "eeprom.h"

void writeEP(ubyte2 offset, ubyte2 length, EEPROMManager* me){
    //! Checks if the EEPROM is busy
    //! Set can use eepromValue type
    if(IO_EEPROM_GetStatus() == IO_E_OK){           /*!< EEPROM is not busy */
        //me->status = EEPROM_op_write;               /*!< EEPROM is in write state */
        IO_EEPROM_Write(offset, length, me->data_software);      /*!< Not busy starts writing. */
        //me->status = EEPROM_op_idle;                /*!< EEPROM operations successful */
}
    //! Data is busy not available.
    /*!< It needs IO_EEPROM_GetStatus */
    /*!< to return IO_E_OK. */ 
    //me->status = EEPROM_op_fault; /*!< EEPROM operation failed, thus quit */
}

//! Reads to EEPROM and reports current state
void readEP(ubyte2 offset, ubyte2 length, EEPROMManager* me){
    //! Checks if the EEPROM is busy
    //! Get can use eepromValue type
    if(IO_EEPROM_GetStatus() == IO_E_OK){           /*!< EEPROM is not busy */
        //me->status = EEPROM_op_read;                /*!< EEPROM is in read state */
        IO_EEPROM_Read(offset, length, me->data_hardware);      /*!< Not busy starts reading. */
        //me->status = EEPROM_op_idle;                /*!< EEPROM operations successful */
}
    //! Data is busy not available.
    /*!< It needs IO_EEPROM_GetStatus */
    /*!< to return IO_E_OK. */ 
    //me->status = EEPROM_op_fault; /*!< EEPROM operation failed, thus quit */
}
