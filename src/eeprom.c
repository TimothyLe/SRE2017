#include "eeprom.h"
#include "resource.h"
/***************************************************************
 
 The static keyword LOCAL allows for a value to be preserved in between 
 invocations
 
 "LOCAL is just the variable name"
 -Rusty

 ***************************************************************/

/********************************************************
 * *** NOTE ***
 * MULTI-BYTE VALUES FOR EEPROM ARE LITTLE ENDIAN (>>)
 * FOR THE PURPOSE OF ASSIGNING DIFFERENT ADDRESSES
 * TO EEPROM VALUES (MSB)
 ********************************************************/

#define LOCAL static 

//! Prototypes used in constructor
/*!< Functions partially defined in helper functions */
LOCAL ubyte2 getAddress(EEPROMManager* me, eepromValue value);
LOCAL void readInitialValues(EEPROMManager* me); 
// LOCAL void eepromLength_Classifier(EEPROMManager* me, eepromValue parameter);

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
    IO_EEPROM_Init();  // Might need to initialize the EEPROM in constructor first

    //Allocate memory for EEPROMManager object
    EEPROMManager* me = (EEPROMManager*)malloc(sizeof(struct _EEPROMManager));

    //Allocate memory for EEPROM data (2x)
    me->data_hardware = (ubyte1*)malloc(sizeof(ubyte1) * me->size);
    me->data_software = (ubyte1*)malloc(sizeof(ubyte1) * me->size);

    me->status = EEPROM_op_initialize;
    me->length = isByte8;

    //Start initial EEPROM read
    readInitialValues(me);
    
    return me;
}

//---------------------------------------------------------------
// Mutators
//---------------------------------------------------------------
/**  @ingroup Mutators
* @brief Sets the desired (software) EEPROM values
*/
bool EEPROMManager_set_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1 value){
    //! Accessing the indexes of the dynamic array and deep copying values
    bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
    ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
    flag = eepromLength_shift1(me, parameter, value);
            /*!< Shifts performs to ensure the value is the correct size for parameter */
    me->data_software[offset] = value;
        return flag; /*!< Access was successful */
}
    return flag; /*!< Access failed */
}
bool EEPROMManager_set_ubyte2(EEPROMManager* me, eepromValue parameter, ubyte2 value){
    //! Accessing the indexes of the dynamic array and deep copying values
    bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
    ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
    flag = eepromLength_shift2(me, parameter, value);
            /*!< Shifts performs to ensure the value is the correct size for parameter */
    me->data_software[offset] = (ubyte1)value;
        return flag; /*!< Access was successful */
}
    return flag; /*!< Access failed */
}
bool EEPROMManager_set_ubyte4(EEPROMManager* me, eepromValue parameter, ubyte4 value){
    //! Accessing the indexes of the dynamic array and deep copying values
    bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
    ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
    flag = eepromLength_shift4(me, parameter, value);
            /*!< Shifts performs to ensure the value is the correct size for parameter */
    me->data_software[offset] = (ubyte1)value;
        return flag; /*!< Access was successful */
}
    return flag; /*!< Access failed */
}
bool EEPROMManager_set_sbyte1(EEPROMManager* me, eepromValue parameter, sbyte1 value){
   //! Accessing the indexes of the dynamic array and deep copying values
    bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
    ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
    flag = eepromLength_shift1(me, parameter, value);
            /*!< Shifts performs to ensure the value is the correct size for parameter */
    me->data_software[offset] = (ubyte1)value;
        return flag; /*!< Access was successful */
}
    return flag; /*!< Access failed */
}
bool EEPROMManager_set_sbyte2(EEPROMManager* me, eepromValue parameter, sbyte2 value){
    //! Accessing the indexes of the dynamic array and deep copying values
    bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
    ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
    flag = eepromLength_shift2(me, parameter, value);
           /*!< Shifts performs to ensure the value is the correct size for parameter */
    me->data_software[offset] = (ubyte1)value;
        return flag; /*!< Access was successful */
}
    return flag; /*!< Access failed */
}
bool EEPROMManager_set_sbyte4(EEPROMManager* me, eepromValue parameter, sbyte4 value){
    //! Accessing the indexes of the dynamic array and deep copying values
    bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
    ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
    flag = eepromLength_shift4(me, parameter, value);
            /*!< Shifts performs to ensure the value is the correct size for parameter */
    me->data_software[offset] = (ubyte1)value;
        return flag; /*!< Access was successful */
}
    return flag; /*!< Access failed */
}
bool EEPROMManager_set_float4(EEPROMManager* me, eepromValue parameter, float4 value){
    //! Accessing the indexes of the dynamic array and deep copying values
    bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
    ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
    flag = eepromLength_shift4(me, parameter, value);
            /*!< Shifts performs to ensure the value is the correct size for parameter */
    me->data_software[offset] = (ubyte1)value;
        return flag; /*!< Access was successful */
}
    return flag; /*!< Access failed */
}
bool EEPROMManager_set_bool(EEPROMManager* me, eepromValue parameter, bool value){
    //! Accessing the indexes of the dynamic array and deep copying values
    bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
    ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
    flag = eepromLength_shift4(me, parameter, value);
            /*!< Shifts performs to ensure the value is the correct size for parameter */
    me->data_software[offset] = (ubyte1)value;
        return flag; /*!< Access was successful */
}
    return flag; /*!< Access failed */
}

//---------------------------------------------------------------
// Accessors
//---------------------------------------------------------------
/**  @ingroup Accessors
* @brief Gets the actual hardware EEPROM values
*/
bool EEPROMManager_get_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1* value){
    //! Accessing the indexes of the dynamic array and deep copying values
    bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
    ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
    do {
                flag = eepromLength_shift1(me, parameter, *value); //!< Clears value
                *value++ = *me->data_hardware++; /*!< Performs a deep copy */
                offset++;
            } while(offset < me->size);
        return flag; /*!< Access was successful */
        }
    return flag; /*!< Access failed */
    }
    bool EEPROMManager_get_ubyte2(EEPROMManager* me, eepromValue parameter, ubyte2* value){
    //! Accessing the indexes of the dynamic array and deep copying values
        bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
        ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
        do {
                flag = eepromLength_shift2(me, parameter, *value); //!< Clears value
                *value++ = *me->data_hardware++; /*!< Performs a deep copy */
                offset++;
            } while(offset < me->size);
        return flag; /*!< Access was successful */
        }
    return flag; /*!< Access failed */
    }
    bool EEPROMManager_get_ubyte4(EEPROMManager* me, eepromValue parameter, ubyte4* value){
    //! Accessing the indexes of the dynamic array and deep copying values
        bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
        ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
        do {
                flag = eepromLength_shift4(me, parameter, *value); //!< Clears value
                *value++ = *me->data_hardware++; /*!< Performs a deep copy */
                offset++;
            } while(offset < me->size);
        return flag; /*!< Access was successful */
        }
    return flag; /*!< Access failed */
    }
    bool EEPROMManager_get_sbyte1(EEPROMManager* me, eepromValue parameter, sbyte1* value){
    //! Accessing the indexes of the dynamic array and deep copying values
        bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
        ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
        do {
                flag = eepromLength_shift1(me, parameter, *value); //!< Clears value
                *value++ = *me->data_hardware++; /*!< Performs a deep copy */
                offset++;
            } while(offset < me->size);
        return flag; /*!< Access was successful */
        }
    return flag; /*!< Access failed */
    }
    bool EEPROMManager_get_sbyte2(EEPROMManager* me, eepromValue parameter, sbyte2* value){
    //! Accessing the indexes of the dynamic array and deep copying values
        bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
        ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
        do {
                flag = eepromLength_shift2(me, parameter, *value); //!< Clears value
                *value++ = *me->data_hardware++; /*!< Performs a deep copy */
                offset++;
            } while(offset < me->size);
        return flag; /*!< Access was successful */
        }
    return flag; /*!< Access failed */
    }
    bool EEPROMManager_get_sbyte4(EEPROMManager* me, eepromValue parameter, sbyte4* value){
    //! Accessing the indexes of the dynamic array and deep copying values
        bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
        ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
        do {
                flag = eepromLength_shift4(me, parameter, *value); //!< Clears value
                *value++ = *me->data_hardware++; /*!< Performs a deep copy */
                offset++;
            } while(offset < me->size);
        return flag; /*!< Access was successful */
        }
    return flag; /*!< Access failed */
    }
    bool EEPROMManager_get_float4(EEPROMManager* me, eepromValue parameter, float4* value){
    //! Accessing the indexes of the dynamic array and deep copying values
        bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
        ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
        do {
                flag = eepromLength_shift4(me, parameter, *value); //!< Clears value
                *value++ = *me->data_hardware++; /*!< Performs a deep copy */
                offset++;
            } while(offset < me->size);
        return flag; /*!< Access was successful */
        }
    return flag; /*!< Access failed */
    }
    bool EEPROMManager_get_bool(EEPROMManager* me, eepromValue parameter, bool* value){
    //! Accessing the indexes of the dynamic array and deep copying values
        bool flag = FALSE;
    EEPROM_parseMessage(me,parameter);  /*!< Finds the EEPROMManager size */
        ubyte2 offset = getAddress(me, parameter); /*!< Created for getAddress param */
        if(EEPROMManager_initialized(me)){ /*!< Checks if the EEPROM has been read */
            //! Gets the EEPROM hardware value by storing it in our given parameter
        do {
                flag = eepromLength_shift4(me, parameter, *value); //!< Clears value
                *value++ = *me->data_hardware++; /*!< Performs a deep copy */
                offset++;
            } while(offset < me->size);
        return flag; /*!< Access was successful */
        }
    return flag; /*!< Access failed */
    }

//---------------------------------------------------------------
// Endian Shifters
//---------------------------------------------------------------
/**  @ingroup Endian Shifters
 * @brief Performs little endian shift to preserve MSBs
 * @param[in]   me          The pointer to our EEPROM object
 * @param[in]   parameter   The EEPROM value that gives us an address
 * @param[out]  value       The little endian shifted result
 * @return      Whether or not the endian shift was successful
 */
    bool eepromLength_shift1(EEPROMManager* me, eepromValue parameter, ubyte1 value){
        //!< flag ---> shift was successful
        //!< shift --> the value was assigned a shifted one
        bool flag = FALSE, shift = FALSE;
        ubyte2 offset = getAddress(me, parameter);
        if(EEPROMManager_initialized(me)){
        /*!< If the value needs to be 1 byte, shift once, else then shift more */
            while(!shift){
                if (me->length == isByte8)
                    flag = eepromLength_shift4(me, parameter, (ubyte4)value);
                else if (me->length == isByte4)
                    flag = eepromLength_shift2(me, parameter, (ubyte2)value);
                else if (me->length == isByte2){
                    value = me->data_software[offset] << 4; 
                    shift = TRUE;
                }
                else{
                    value = me->data_software[offset];
                    shift = TRUE;
                }
            }
    // (me->length == isByte2) ? value = me->data_software[offset] >> 4 : value = (ubyte1)me->data_software[offset]; 
    /*!< Checks to see if little endian shift succeeded */
            (sizeof(value) == sizeof(ubyte1)) ? flag = TRUE: flag; 
        }
        return flag;
    }
    bool eepromLength_shift2(EEPROMManager* me, eepromValue parameter, ubyte2 value){
        //!< flag ---> shift was successful
        //!< shift --> the value was assigned a shifted one
        bool flag = FALSE, shift = FALSE;   
        ubyte2 offset = getAddress(me, parameter);
        if(EEPROMManager_initialized(me)){
        /*!< If the value needs to be 2 bytes, shift once, else then shift more */
            while(!shift){
                if (me->length == isByte8)
                    flag = eepromLength_shift4(me, parameter, (ubyte4)value);
                else if (me->length == isByte4){
                    value += me->data_software[offset] << 8; 
                    shift = TRUE;
                }
                else if (me->length == isByte2)
                    flag = eepromLength_shift1(me, parameter, (ubyte1)value);
                else{
                    value += (ubyte2)me->data_software[offset];
                    shift = TRUE;
                }
            }
    // (me->length == isByte4) ? value = value >> 8 : eepromLength_shift1(me,parameter,(ubyte1)value); 
    /*!< Checks to see if little endian shift succeeded */
            (sizeof(value) == sizeof(ubyte2)) ? flag = TRUE: flag; 
        }
        return flag;
    }
    bool eepromLength_shift4(EEPROMManager* me, eepromValue parameter, ubyte4 value){
        //!< flag ---> shift was successful
        //!< shift --> the value was assigned a shifted one
        bool flag = FALSE, shift = FALSE;
        ubyte2 offset = getAddress(me, parameter);
        if(EEPROMManager_initialized(me)){
        /*!< If the value needs to be 4 bytes, shift once, else then shift more */
            while(!shift){
                if (me->length == isByte8){
                    value += me->data_software[offset] << 15; // Why cannot use 16 bits?
                    shift = TRUE;
                }
                else if (me->length == isByte4)
                    flag = eepromLength_shift2(me, parameter, (ubyte2)value);
                else if (me->length == isByte2)
                    flag = eepromLength_shift1(me, parameter, (ubyte1)value);
                else{
                    value += (ubyte4)me->data_software[offset];
                    shift = TRUE;
                }
            }
    // (me->length == isByte8) ? value = value >> 16 : eepromLength_shift2(me,parameter,(ubyte2)value); 
    /*!< Checks to see if little endian shift succeeded */
            (sizeof(value) == sizeof(ubyte4)) ? flag = TRUE: flag;  
        }
        return flag;
    }

// //---------------------------------------------------------------
// // EEPROM Value Accessors
// //---------------------------------------------------------------
// /**  @ingroup EEPROM Value Accessors
// * @brief Gets the specific hardware EEPROM values
// */
// ubyte4 get_EEPROM_val_doNotUse(EEPROMManager* me, eepromValue parameter, ubyte4 value){
//     char buff[32];
//     sprintf(buff,"EEPROM_val_doNotUse: %i\n",value);
//     return value;
// }         //!< ubyte4 0x0000
// ubyte2 get_EEPROM_val_TPS0_calibMin(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x0004
// ubyte2 get_EEPROM_val_TPS0_calibMax(EEPROMManager* me, eepromValue parameter, ubyte2 value); //!< ubyte2 0x0006
// ubyte2 get_EEPROM_val_TPS1_calibMin(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x0008
// ubyte2 get_EEPROM_val_TPS1_calibMax(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x000A
// sbyte2 get_EEPROM_val_MCM_torqueMaximumDNm(EEPROMManager* me, eepromValue parameter, sbyte2 value);  //!< sbyte2 0x000C
// ubyte2 get_EEPROM_val_regen_torqueLimitDNm(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x000E
// ubyte2 get_EEPROM_val_regen_torqueAtZeroPedalDNm(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x0010
// float4 get_EEPROM_val_regen_percentAPPSForCoasting(EEPROMManager* me, eepromValue parameter, float4 value); //!< float4 0x0014
// float4 get_EEPROM_val_regen_percentBPSForMaxRegen(EEPROMManager* me, eepromValue parameter, float4 value); //!< float4 0x0018
// ubyte2 get_EEPROM_val_regen_minimumSpeedKPH(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x001C
// ubyte2 get_EEPROM_val_regen_SpeedRampStart(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x001E
// ubyte2 get_EEPROM_val_regen_throttlePedal(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x0020

// //---------------------------------------------------------------
// // EEPROM Value Mutators
// //---------------------------------------------------------------
// /**  @ingroup EEPROM Value Accessors
// * @brief Sets the specific hardware EEPROM values
// */
// ubyte4 set_EEPROM_val_doNotUse(EEPROMManager* me, eepromValue parameter, ubyte4 value);         //!< ubyte4 0x0000
// ubyte2 set_EEPROM_val_TPS0_calibMin(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x0004
// ubyte2 set_EEPROM_val_TPS0_calibMax(EEPROMManager* me, eepromValue parameter, ubyte2 value); //!< ubyte2 0x0006
// ubyte2 set_EEPROM_val_TPS1_calibMin(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x0008
// ubyte2 set_EEPROM_val_TPS1_calibMax(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x000A
// sbyte2 set_EEPROM_val_MCM_torqueMaximumDNm(EEPROMManager* me, eepromValue parameter, sbyte2 value);  //!< sbyte2 0x000C
// ubyte2 set_EEPROM_val_regen_torqueLimitDNm(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x000E
// ubyte2 set_EEPROM_val_regen_torqueAtZeroPedalDNm(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x0010
// float4 set_EEPROM_val_regen_percentAPPSForCoasting(EEPROMManager* me, eepromValue parameter, float4 value); //!< float4 0x0014
// float4 set_EEPROM_val_regen_percentBPSForMaxRegen(EEPROMManager* me, eepromValue parameter, float4 value); //!< float4 0x0018
// ubyte2 set_EEPROM_val_regen_minimumSpeedKPH(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x001C
// ubyte2 set_EEPROM_val_regen_SpeedRampStart(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x001E
// ubyte2 set_EEPROM_val_regen_throttlePedal(EEPROMManager* me, eepromValue parameter, ubyte2 value);  //!< ubyte2 0x0020

//---------------------------------------------------------------
// Special Functions
//---------------------------------------------------------------
    //! Syncs cache to the physical EEPROM through API
eepromOperation EEPROMManager_sync(EEPROMManager* me, ubyte2 offset) //! Changed from IO_Error_Type to eepromOperation
{
    /* @brief EEPROMManager
     * ubyte2 size;            Size of EEPROM actually used by our software
     * ubyte1* data_software;  "Desired" EEPROM values.  Pointer to array of bytes.
     * ubyte1* data_hardware;  "Actual" (confirmed) EEPROM values.  Pointer to array of bytes.
     * eepromValue type;       Gives developers easy way to request a specific value
     * eepromOperation status; The current operation being performed by EEPROM
    */
    eepromOperation temp = EEPROMManager_getStatus(me);
    ubyte2 length = me->size - offset;  /*!< Keeps track of remainder of EEPROM length */
    // TODO
    if(EEPROMManager_getStatus(me) == EEPROM_op_idle){   /*!< EEPROM object should be initialized already */
        readEP(offset, length, me); /*!< Reads previous EEPROM values before changes */
        writeEP(offset, length, me); /*!< Writes to the remainder of the EEPROM */
        temp = EEPROMManager_getStatus(me); /*!< Returns state based on successful write to EEPROM */
    return temp;
}
return temp = EEPROM_op_fault;
}

//! Returns the state of the EEPROM
eepromOperation EEPROMManager_getStatus(EEPROMManager* me){
    //! Uses the EEPROM API to return value to eepromOperation status
    /* @brief IO_EEPROM_GetStatus()    Description
     * retval IO_E_OK                  everything fine / no changes needed
     * retval IO_E_BUSY                EEPROM module is still busy
     * retval IO_E_EEPROM_RANGE        invalid address offset or range
     * retval IO_E_NULL_POINTER        a null pointer has been passed
     * retval IO_E_CHANNEL_NOT_CONFIGURED the module is not initialized
     */
    eepromOperation temp = me->status;
    if(IO_EEPROM_GetStatus()==IO_E_OK)
        return temp = EEPROM_op_idle;                      /*!< EEPROM status is fine */
        else if(IO_EEPROM_GetStatus()==IO_E_BUSY)
        return temp = EEPROM_op_validate;                /*!< EEPROM is current performing an operation */
            else
        return temp = EEPROM_op_fault;                     /*!< Invalid range, null pointer passed, or not initialized */

        }

    bool EEPROMManager_initialized(EEPROMManager* me){
    // Returns boolean value whether EEPROMManager has been initialized
        bool flag = FALSE;
        if(me->data_software == (ubyte1*)malloc(sizeof(ubyte1) * me->size)
            && me->data_hardware == (ubyte1*)malloc(sizeof(ubyte1) * me->size)
            && me->status == EEPROM_op_initialize) flag = TRUE;
            return flag;
    }

//---------------------------------------------------------------
// Static Helper functions (private)
//---------------------------------------------------------------
/**
 * @brief Returns the address and length of any variable in the 
 *        eepromValue enum within this context
 *
 * @param[in]   value       Which EEPROM value we want the address/length of
 * @param[out]  address     The EEPROM address of the desired (software) value
 * @param[out]  bytes       The length (number of bytes) of the value
 * @return      Whether or not the address/length were successfully found.
 */
    LOCAL ubyte2 getAddress(EEPROMManager* me, eepromValue value)
    {
        ubyte2 address = 0;
    //!< Comments on the side are incorrect, fix later?
    // (value == EEPROM_val_doNotUse                               //!< ubyte4 0x0000
    //    || value == EEPROM_val_TPS0_calibMin                     //!< ubyte2 0x0004
    //    || value == EEPROM_val_TPS0_calibMax                     //!< ubyte2 0x0006
    //    || value == EEPROM_val_TPS1_calibMin                     //!< ubyte2 0x0008
    //    || value == EEPROM_val_TPS1_calibMax                     //!< ubyte2 0x000A
    //    || value == EEPROM_val_MCM_torqueMaximumDNm              //!< sbyte2 0x000C
    //    || value == EEPROM_val_regen_throttlePedal               //!< ubyte2 0x000E
    //    || value == EEPROM_val_regen_torqueLimitDNm              //!< ubyte2 0x0010
    //    || value == EEPROM_val_regen_percentAPPSForCoasting      //!< float4 0x0014
    //    || value == EEPROM_val_regen_torqueLimitDNm              //!< float4 0x0018
    //    || value == EEPROM_val_regen_SpeedRampStart              //!< ubyte2 0x001C
    //    || value == EEPROM_val_regen_minimumSpeedKPH             //!< ubyte2 0x001E
    //    || value == EEPROM_val_regen_torqueAtZeroPedalDNm        //!< ubyte2 0x0020
    //    || value == EEPROM_val_regen_percentBPSForMaxRegen) ? success = TRUE : success;
        if (EEPROMManager_initialized(me)){
            if(value == EEPROM_val_doNotUse)
                return 0x0000;
            else if(value == EEPROM_val_TPS0_calibMin)
                return 0x0004; 
            else if(value == EEPROM_val_TPS0_calibMax)
                return 0x0006; 
            else if(value == EEPROM_val_TPS1_calibMin)
                return 0x0008; 
            else if(value == EEPROM_val_TPS1_calibMax)
                return 0x000A; 
            else if(value == EEPROM_val_MCM_torqueMaximumDNm)
                return 0x000C; 
            else if(value == EEPROM_val_regen_torqueLimitDNm)
                return 0x000E; 
            else if(value == EEPROM_val_regen_torqueAtZeroPedalDNm)
                return 0x0010; 
            else if(value == EEPROM_val_regen_percentAPPSForCoasting)
                return 0x0014; 
            else if(value == EEPROM_val_regen_percentBPSForMaxRegen)
                return 0x0018; 
            else if(value == EEPROM_val_regen_minimumSpeedKPH)
                return 0x001C; 
            else if(value == EEPROM_val_regen_SpeedRampStart)
                return 0x001E; 
            else if(value == EEPROM_val_regen_throttlePedal)
                return 0x0020; 
            return 0;
        }
        return address;
    }

//Reads EEPROM and stores data in data_hardware.  Waits until read is complete.
LOCAL void readInitialValues(EEPROMManager* me)  //->might be void since we aren't modifying anything
{
    //Read eeprom
    //Loop until status == ok
    me->status = EEPROM_op_initialize;
    while(EEPROMManager_initialized(me)){ //!< EEPROM object must be initialized already
        readEP(0x0, me->size, me);  /*!< Initiates first read to data_hardware */
    }
    
}

// //! Reads type of memory and classifies them in bytes (assignments)
// LOCAL void eepromLength_Classifier(EEPROMManager* me, eepromValue parameter){ 
//         EEPROM_parseMessage(me, parameter); //! Initializes a parse
//         if(sizeof(me->data_software)==sizeof(ubyte1)){  //! ubyte1
//             me->length = isByte1; 
//         } else if(sizeof(me->data_software)==sizeof(ubyte2)){   //! ubyte2
//             me->length = isByte2; 
//         } else if(sizeof(me->data_software)==sizeof(ubyte4)){   //! ubyte4
//             me->length = isByte4; 
//         } else{ //! Larger than ubyte4 or some unknown size ~ set to ubyte8 state
//             me->length = isByte8;   //! ubyte8 or unknown size
//         }
// }

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
//! Writes to EEPROM and reports current state
void writeEP(ubyte2 offset, ubyte2 length, EEPROMManager* me){
    //! Checks if the EEPROM is busy
    //! Set can use eepromValue type
    if(IO_EEPROM_GetStatus() == IO_E_OK){           /*!< EEPROM is not busy */
        me->status = EEPROM_op_write;               /*!< EEPROM is in write state */
        IO_EEPROM_Write(offset, length, me->data_software);      /*!< Not busy starts writing. */
        me->status = EEPROM_op_idle;                /*!< EEPROM operations successful */
}
    //! Data is busy not available.
    /*!< It needs IO_EEPROM_GetStatus */
    /*!< to return IO_E_OK. */ 
    me->status = EEPROM_op_fault; /*!< EEPROM operation failed, thus quit */
}

//! Reads to EEPROM and reports current state
void readEP(ubyte2 offset, ubyte2 length, EEPROMManager* me){
    //! Checks if the EEPROM is busy
    //! Get can use eepromValue type
    if(IO_EEPROM_GetStatus() == IO_E_OK){           /*!< EEPROM is not busy */
        me->status = EEPROM_op_read;                /*!< EEPROM is in read state */
        IO_EEPROM_Read(offset, length, me->data_hardware);      /*!< Not busy starts reading. */
        me->status = EEPROM_op_idle;                /*!< EEPROM operations successful */
}
    //! Data is busy not available.
    /*!< It needs IO_EEPROM_GetStatus */
    /*!< to return IO_E_OK. */ 
    me->status = EEPROM_op_fault; /*!< EEPROM operation failed, thus quit */
}

//! Assigns eepromValues to the object and checks if the data is the proper size
//! Sets the length of the EEPROM value and classifies length
void EEPROM_parseMessage(EEPROMManager* me, eepromValue parameter){
    switch(parameter){
        case EEPROM_val_doNotUse:
            me->size = 0x0020; //!< ubyte4
            me->length = isByte4; 
            break;
            case EEPROM_val_TPS0_calibMin:
            me->size = 0x0010; //!< ubyte2
            me->length = isByte2; 
            break;
            case EEPROM_val_TPS0_calibMax:
            me->size = 0x0010; //!< ubyte2
            me->length = isByte2; 
            break;
            case EEPROM_val_TPS1_calibMin:
            me->size = 0x0010; //!< ubyte2
            me->length = isByte2;
            break;
            case EEPROM_val_TPS1_calibMax:
            me->size = 0x0010; //!< ubyte2
            me->length = isByte2;
            break;
            case EEPROM_val_MCM_torqueMaximumDNm:
            me->size = 0x0010; //!< sbyte2
            me->length = isByte2;
            break;
            case EEPROM_val_regen_torqueLimitDNm:
            me->size = 0x0010; //!< ubyte2
            me->length = isByte2;
            break;
            case EEPROM_val_regen_torqueAtZeroPedalDNm:
            me->size = 0x0010; //!< ubyte2
            me->length = isByte2;
            break;
            case EEPROM_val_regen_percentAPPSForCoasting:
            me->size = 0x0020; //!< float4
            me->length = isByte4;
            break;
            case EEPROM_val_regen_percentBPSForMaxRegen:
            me->size = 0x0020; //!< float4
            me->length = isByte4;
            break;
            case EEPROM_val_regen_minimumSpeedKPH:
            me->size = 0x0010; //!< ubyte2
            me->length = isByte2;
            break;
            case EEPROM_val_regen_SpeedRampStart:
            me->size = 0x0010; //!< ubyte2
            me->length = isByte2;
            break;
            case EEPROM_val_regen_throttlePedal:
            me->size = 0x0010; //!< ubyte2
            me->length = isByte2;
            break;
        }

    //!< Previous rendition 
    // switch(offset){
    //     case 0x0000:
    //         me->type = EEPROM_val_doNotUse; //!< ubyte4
    //         me->length <= isByte4 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x0004:
    //         me->type = EEPROM_val_TPS0_calibMin; //!< ubyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x0006:
    //         me->type = EEPROM_val_TPS0_calibMax; //!< ubyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x0008:
    //         me->type = EEPROM_val_TPS1_calibMin; //!< ubyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x000A:
    //         me->type = EEPROM_val_TPS1_calibMax; //!< ubyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x000C:
    //         me->type = EEPROM_val_MCM_torqueMaximumDNm; //!< sbyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x000E:
    //         me->type = EEPROM_val_regen_torqueLimitDNm; //!< ubyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x0010:
    //         me->type = EEPROM_val_regen_torqueAtZeroPedalDNm; //!< ubyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x0014:
    //         me->type = EEPROM_val_regen_percentAPPSForCoasting; //!< float4
    //         me->length <= isByte4 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x0018:
    //         me->type = EEPROM_val_regen_percentBPSForMaxRegen; //!< float4
    //         me->length <= isByte4 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x001C:
    //         me->type = EEPROM_val_regen_minimumSpeedKPH; //!< ubyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x001E:
    //         me->type = EEPROM_val_regen_SpeedRampStart; //!< ubyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    //     case 0x0020:
    //         me->type = EEPROM_val_regen_throttlePedal; //!< ubyte2
    //         me->length <= isByte2 ? me->status=EEPROM_op_idle : me->status=EEPROM_op_fault;
    //         break;
    // }
    }

// //! Helper Big Endian shifting function
// void EEPROM_shifter(EEPROMManager* me, eepromLength* shift, ubyte1 value){
//     /*!< Use switch cases to sequentially shift the data size */
//     ubyte2 sizeEEPROM = 0; /*!< Keeps track of the data size */
//     switch(me->length){
//         case isByte8:                           //! Current state
//         if(sizeof(value)>sizeof(ubyte8)){       //! Checks if data is larger than size 8
//             sizeOf(value) = value >> 4;         //! Assigns shift as new size
//             sizeEEPROM = sizeOf(value);         //! Variable to store the size of the data
//             shift = shift->isByte4;             //! Switches to the next case
//         } else exit(1);                       //! Exits the cases if the size is fine
//         break;
//         case isByte4:
//         if(sizeof(value)>sizeof(ubyte4)){
//             sizeOf(value) = value >> 2;
//             sizeEEPROM = sizeOf(value);
//             shift = shift->isByte2;
//         } else exit(1); 
//         break;
//         case isByte2:
//         if(sizeof(value)>sizeof(ubyte2)){
//             sizeOf(value) = value >> 1;
//             sizeEEPROM = sizeOf(value);
//             shift = shift->isByte1;
//         } else exit(1); 
//         break;
//         case isByte1:
//         sizeEEPROM = sizeOf(value);
//         break;
//         default:
//         EEPROMManager_getStatus(me->status);
//         break;
//     }
// }
