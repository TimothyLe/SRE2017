#include "eeprom.h"

/***************************************************************
 
 The static keyword allows for a value to be preserved in between 
 invocations
 
 "LOCAL is just the variable name"
 -Rusty

 ***************************************************************/

#define LOCAL static 

//! Prototypes used in constructor
/*!< Functions partially defined in helper functions */
LOCAL bool getAddress(eepromValue value, ubyte2* address, ubyte1* bytes);
LOCAL void readInitialValues(ubyte1* data); 

//---------------------------------------------------------------
// Constructor
//---------------------------------------------------------------
/**
* @brief 
*
* @param[in]   value       Which EEPROM value we want the address/length of
* @param[out]  address     The EEPROM address of the desired (software) value
* @param[out]  bytes       The length (number of bytes) of the value
* @return      Whether or not the address/length were successfully found.
*/
EEPROMManager* EEPROMManager_new()
{
    ubyte2 size = 34;  //bytes

    //Allocate memory for EEPROMManager object
    EEPROMManager* me = (EEPROMManager*)malloc(sizeof(struct _EEPROMManager));

    //Allocate memory for EEPROM data (2x)
    me->data_hardware = (ubyte1*)malloc(sizeof(ubyte1) * me->size);
    me->data_software = (ubyte1*)malloc(sizeof(ubyte1) * me->size);

    me->status = EEPROM_op_initialize;

    //Start initial EEPROM read
    readInitialValues();
    
    return me;
}

//---------------------------------------------------------------
// Mutators
//---------------------------------------------------------------
/**  @ingroup Mutators
* @brief Sets the desired (software) EEPROM values
*/
bool EEPROMManager_set_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1* value)
{
    //! Accessing the indexes of the dynamic array and deep copying values
    ubyte2* address; /*!< Created for getAddress param */
    if(getAddress(parameter, address, value)){ /*!< Checks if the address can be found */
        if(writeEP){ /*!< Checks if the EEPROM has been written to */
        for(int i = 0; i< me->size; i++){
            value[i] = data_hardware[i]; /*!< Performs a deep copy */
        }
        return true; /*!< Mutation was successful */
    }
    return false; /*!< Mutation failed */
    }
    //! Switch cases for checking the size of hardware data
    switch (sizeof(ubyte1)){
        /*!< Keeps decreasing the passing data until it reaches optimal size */
        case size8: //! Biggest size

        break;
        case size4:
        break;
        case size2:
        break;
        case size1: //! Smallest size
        break;
        default: //! Not a good sign if it gets here
        data_hardware >> 8;
        break;
    }
}

//---------------------------------------------------------------
// Accessors
//---------------------------------------------------------------
/**  @ingroup Accessors
* @brief Gets the actual hardware EEPROM values
*/
bool EEPROMManager_get_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1* value)
{
    //! Accessing the indexes of the dynamic array and deep copying values
    ubyte2* address; /*!< Created for getAddress param */
    if(getAddress(parameter, address, value)){ /*!< Checks if the address can be found */
        if(readEP){ /*!< Checks if the EEPROM has been read */
        for(int i = 0; i< me->size; i++){
            value[i] = data_hardware[i]; /*!< Performs a deep copy */
        }
        return true; /*!< Access was successful */
    }
    return false; /*!< Access failed */
    }
}

//---------------------------------------------------------------
// Special Functions
//---------------------------------------------------------------
IO_ErrorType EEPROMManager_sync(EEPROMManager* me)
{
    do{
        if(data_hardware!=data_software){ /*!< Checks if it is already equivalent */
        writeEP();  /*!< Physically write to hardware */
        //! Check if hardware is operating, EEPROM will lag car so push EEPROMoperations after
        *data_hardware = *data_software; /*!< Sets the hardware to the cache data */
    }
    }while(EEPROMManager_getStatus!=IO_E_OK);
}

eepromOperation EEPROMManager_getStatus(EEPROMManager* me){
    //! Uses the EEPROM API to return value to eepromOperation status
    me->status = IO_EEPROM_GetStatus();
}

bool EEPROMManager_initialized(EEPROMManager* me){
    //left empty
    return false;

}

//---------------------------------------------------------------
// Helper functions (private)
//---------------------------------------------------------------
/**
 * @brief Returns the address and length of any variable in the eepromValue enum
 *
 * @param[in]   value       Which EEPROM value we want the address/length of
 * @param[out]  address     The EEPROM address of the desired (software) value
 * @param[out]  bytes       The length (number of bytes) of the value
 * @return      Whether or not the address/length were successfully found.
 */
LOCAL bool getAddress(eepromValue value, ubyte2* address, ubyte1* bytes)
{
    bool success = FALSE;


    *address = 0;
    *bytes = 0;
    return success;
}

//Reads EEPROM and stores data in data_hardware.  Waits until read is complete.
LOCAL void readInitialValues(ubyte1* data)  //->might be void since we aren't modifying anything
{
    //Read eeprom
    //Loop until status == ok
    do{
        me->status = EEPROM_op_idle;
    } while(EEPROMManager_getStatus!=IO_E_OK);
    *data_hardware = *data; //->shallow copy
}

//! Writes to EEPROM
bool writeEP(ubyte2 offset, ubyte2 length, ubyte1 * data){
    //! Checks if the EEPROM is busy

    if(IO_EEPROM_GetStatus() == IO_E_OK){

        IO_EEPROM_Write(offset, length, data); /*!< Not busy starts writing. */
        return true;
    }
    return false;
    //! Data is busy not available.
    /*!< It needs IO_EEPROM_GetStatus */
    /*!< to return IO_E_OK. */ 
}

//! Reads to EEPROM
bool readEP(ubyte2 offset, ubyte2 length, ubyte1 data){
    //! Checks if the EEPROM is busy

    if(IO_EEPROM_GetStatus() == IO_E_OK){

        IO_EEPROM_Read(offset, length, &data); /*!< Not busy starts reading. */
        return true;
    }
    return false;
    //! Data is busy not available.
    /*!< It needs IO_EEPROM_GetStatus */
    /*!< to return IO_E_OK. */ 
}
