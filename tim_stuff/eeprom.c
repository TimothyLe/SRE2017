#include "IO_Driver.h"
#include "IO_EEPROM.h"
#include "eeprom.h"

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
* @brief Specific description
*/
bool EEPROMManager_set_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1* value)
{

}
// Writes to EEPROM
void writeEP(ubyte2 offset, ubyte2 length, ubyte1 * data){
    //checks if the EEPROM is busy

    if(IO_EEPROM_GetStatus() == IO_E_OK){

        //not busy starts reading

        IO_EEPROM_Write(offset, length, data);

    }

    //data is busy not available

    //needs IO_EEPROM_GetStatus 

    //to return IO_E_OK 
}

//---------------------------------------------------------------
// Accessors
//---------------------------------------------------------------
/**  @ingroup Accessors
* @brief Specific description
*/
bool EEPROMManager_get_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1* value)
{

}
// Reads to EEPROM
void readEP(ubyte2 offset, ubyte2 length, ubyte1 data){
    //checks if the EEPROM is busy

    if(IO_EEPROM_GetStatus() == IO_E_OK){

        //not busy starts reading

        IO_EEPROM_Read(offset, length, &data);

    }

    //data is busy not available

    //needs IO_EEPROM_GetStatus 

    //to return IO_E_OK 
}

//---------------------------------------------------------------
// Special Functions
//---------------------------------------------------------------
IO_ErrorType EEPROMManager_sync(EEPROMManager* me)
{
    //
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
LOCAL ? readInitialValues(???)
{
    //Read eeprom
    //Loop until status == ok
    me->status = EEPROM_op_idle;

}

LOCAL read(? ? ? )
{
    IO_EEPROM_Read()
}

LOCAL write(? ? ? )
{

}
