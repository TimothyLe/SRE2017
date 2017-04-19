#include "eeprom.h"
#include <stdlib.h> //malloc
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
    ubyte2* address = 0; /*!< Created for getAddress param */
    EEPROM_endianShift* shift;  //! Declaring a pointer to EEPROM_endianShift
    if(getAddress(parameter, address, value)){ /*!< Checks if the address can be found */
        if(!writeEP(address,me->size,value)){ /*!< Checks if the EEPROM has been written to */
    while(address < me.size){
            //! Sets the hardware value to our given value
            *data_hardware++ = *value++; /*!< Performs a deep copy */
        *address++;
            EEPROM_shifter(me,shift,value); //! Performs the shift if necessary
        }
        return true; /*!< Mutation was successful */
    }
    return false; /*!< Mutation failed */
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
    ubyte2* address = 0; /*!< Created for getAddress param */
    EEPROM_endianShift* shift;  //! Declaring a pointer to EEPROM_endianShift
    if(getAddress(parameter, address, value)){ /*!< Checks if the address can be found */
        if(!readEP(address,me->size,value)){ /*!< Checks if the EEPROM has been read */
    while(address < me->size){
            //! Gets the hardware value by storing it in our given parameter
            *value++ = *data_hardware++; /*!< Performs a deep copy */
        *address++;
            EEPROM_shifter(me,shift,value); //! Performs the shift if necessary
        }
        return true; /*!< Access was successful */
    }
    return false; /*!< Access failed */
}
}

//---------------------------------------------------------------
// Special Functions
//---------------------------------------------------------------
IO_ErrorType EEPROMManager_sync(EEPROMManager* me, ubyte2 offset)
{
    /*
    ubyte2 size;            //!< Size of EEPROM actually used by our software
    ubyte1* data_software;   //!< "Desired" EEPROM values.  Pointer to array of bytes.
    ubyte1* data_hardware;    //!< "Actual" (confirmed) EEPROM values.  Pointer to array of bytes.

    eepromOperation status; //!< The current operation being performed by EEPROM

    */
    do{
        if(me->data_hardware!=me->data_software){ /*!< Checks if it is already equivalent */
        writeEP(offset, me->size, me->data_software);  /*!< Physically write to hardware */
        //! Check if hardware is operating, EEPROM will lag car 
        //  so push EEPROMoperations after
    }
    *data_hardware++ = *data_software++;    /*!< Performs a deep copy of software into hardware */
}while(EEPROMManager_getStatus!=IO_E_OK);   /*!< Iterates through address while EEPROM is busy */
}

eepromOperation EEPROMManager_getStatus(EEPROMManager* me){
    //! Uses the EEPROM API to return value to eepromOperation status
    me->status = IO_EEPROM_GetStatus();
}

bool EEPROMManager_initialized(EEPROMManager* me){
    // Returns boolean value whether EEPROMManager has been initialized
    ubyte2 offset = 0;
    if(readEP(offset,me->size,me->data_hardware)){ //status
        // use status to get states if eepromOperation
        return true;
    }
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
LOCAL void readInitialValues(EEPROMManager* me)  //->might be void since we aren't modifying anything
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

//! Helper Big Endian shifting function
void EEPROM_shifter(EEPROMManager* me, EEPROM_endianShift* shift, ubyte1* value){
    /*!< Use switch cases to sequentially shift the data size */
    ubyte2 sizeEEPROM = 0; /*!< Keeps track of the data size */
    switch(shift->isByte8){
        case isByte8:                           //! Current state
        if(sizeof(value)>sizeof(ubyte8)){       //! Checks if data is larger than size 8
            sizeOf(value) = value >> 4;         //! Assigns shift as new size
            sizeEEPROM = sizeOf(value);         //! Variable to store the size of the data
            shift = shift->isByte4;             //! Switches to the next case
        } else exit(1);                       //! Exits the cases if the size is fine
        break;
        case isByte4:
        if(sizeof(value)>sizeof(ubyte4)){
            sizeOf(value) = value >> 2;
            sizeEEPROM = sizeOf(value);
            shift = shift->isByte2;
        } else exit(1); 
        break;
        case isByte2:
        if(sizeof(value)>sizeof(ubyte2)){
            sizeOf(value) = value >> 1;
            sizeEEPROM = sizeOf(value);
            shift = shift->isByte1;
        } else exit(1); 
        break;
        case isByte1:
        sizeEEPROM = sizeOf(value);
        break;
        default:
        EEPROMManager_getStatus(me->status);
        break;
    }
}
