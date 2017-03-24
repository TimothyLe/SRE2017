#include "IO_Driver.h"
#include "IO_EEPROM.h"
#include "eeprom.h"

/***************************************************************
 
 The static keyword allows for a value to be preserved in between 
 invocations
 
 "LOCAL is just the variable name"
 -Rusty

 ***************************************************************/

#define LOCAL static 

LOCAL bool getAddress(eepromValue value, ubyte2* address, ubyte1* bytes);
LOCAL ? readInitialValues(?);

//---------------------------------------------------------------
// Constructor
//---------------------------------------------------------------
/**
* @brief 
*
* @param[in]   value       Which EEPROM value we want the address/length of
* @param[out]  address     The EEPROM address of the desired value
* @param[out]  bytes       The length (number of bytes) of the value
* @return      Whether or not the address/length were successfully found.
*/
EEPROMManager* EEPROMManager_new()
{
    ubyte2 size = 34;  //bytes

    //Allocate memory for EEPROMManager object
    EEPROMManager* me = (EEPROMManager*)malloc(sizeof(struct _EEPROMManager));

    //Allocate memory for EEPROM data (2x)
    me->data_actual = (ubyte1*)malloc(sizeof(ubyte1) * me->size);
    me->data_desired = (ubyte1*)malloc(sizeof(ubyte1) * me->size);

    me->status = EEPROM_op_initialize;

    //Start initial EEPROM read
    readInitialValues();
    
    return me;
}

//---------------------------------------------------------------
// Mutators
//---------------------------------------------------------------
/**  @ingroup Mutators
* @brief Sets the desired EEPROM values
*/
bool EEPROMManager_set_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1* value)
{
    if(writeEP){
        me -> value;
    data_desired = *value;
        return true;
    }
    return false;
}

//---------------------------------------------------------------
// Accessors
//---------------------------------------------------------------
/**  @ingroup Accessors
* @brief Gets the actual EEPROM values
*/
bool EEPROMManager_get_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1* value)
{
    if(readEP){
        me -> value;
        *value = *data_actual;
        return true;
    }
    return false;
}

//---------------------------------------------------------------
// Special Functions
//---------------------------------------------------------------
IO_ErrorType EEPROMManager_sync(EEPROMManager* me)
{
    // if(data_actual!=data_desired){
    //     data_actual = data_desired;
    // }
}


//---------------------------------------------------------------
// Helper functions (private)
//---------------------------------------------------------------
/**
 * @brief Returns the address and length of any variable in the eepromValue enum
 *
 * @param[in]   value       Which EEPROM value we want the address/length of
 * @param[out]  address     The EEPROM address of the desired value
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

//Reads EEPROM and stores data in data_actual.  Waits until read is complete.
LOCAL void readInitialValues(ubyte* data)  //might be void since we aren't modifying anything
{
    //Read eeprom
    //Loop until status == ok
    do{
        me->status = EEPROM_op_idle;
    } while(EEPROMManager_getStatus!=IO_E_OK);
    *data_actual = *data;
}

// Writes to EEPROM
LOCAL void writeEP(ubyte2 offset, ubyte2 length, ubyte1 * data){
    //checks if the EEPROM is busy

    if(IO_EEPROM_GetStatus() == IO_E_OK){

        //not busy starts reading

        IO_EEPROM_Write(offset, length, data);

    }

    //data is busy not available

    //needs IO_EEPROM_GetStatus 

    //to return IO_E_OK 
}

// Reads to EEPROM
LOCAL void readEP(ubyte2 offset, ubyte2 length, ubyte1 data){
    //checks if the EEPROM is busy

    if(IO_EEPROM_GetStatus() == IO_E_OK){

        //not busy starts reading

        IO_EEPROM_Read(offset, length, &data);

    }

    //data is busy not available

    //needs IO_EEPROM_GetStatus 

    //to return IO_E_OK 
}
