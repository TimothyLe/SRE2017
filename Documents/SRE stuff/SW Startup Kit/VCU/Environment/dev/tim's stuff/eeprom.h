/**********************************************************************//**
 * @file eeprom.h
 *
 * @brief VCU functions for reading/writing EEPROM
 *
 *      The EEPROM manager allows VCU code to function at any main loop speed
 *      without needing to be concerned for EEPROM read/write delays.
 *
 *      This works by keeping two copies of EEPROM in memory, "desired" and "actual"
 *      in the form of ubyte1 arrays.  VCU software interactions will directly
 *      access the "desired" copy, called the cache.  EEPROMManager keeps track
 *      of what values are actually in hardware (known after the initial read),
 *      and will sync() the changes when the hardware is available.
 *
 *      This means that hardware will lag behind the values known to / used by
 *      software, but this is preferable to waiting for hardware.
 *
 *      Hardware adresses do not need to be known to call functions.  Each value
 *      in EEPROM has a representation in the eepromValue enum.  This is passed
 *      to the EEPROMManager any time a specific value needs to be read/written.
 *
 **************************************************************************/
#ifndef _EEPROM_H
#define _EEPROM_H

#include "IO_Driver.h"
#include "IO_EEPROM.h"

/*
 *  This enum provides a list of all the variables stored by EEPROM. This
 *  gives developers using an IDE an easy way to call up a specific value.
 */
typedef enum
{
    EEPROM_val_doNotUse    //!< ubyte4
    , EEPROM_val_TPS0_calibMin    //!< ubyte2
    , EEPROM_val_TPS0_calibMax  //!< ubyte2
    , EEPROM_val_TPS1_calibMin  //!< ubyte2
    , EEPROM_val_TPS1_calibMax  //!< ubyte2

    , EEPROM_val_MCM_torqueMaximumDNm //!< sbyte2

    , EEPROM_val_regen_torqueLimitDNm
    , EEPROM_val_regen_torqueAtZeroPedalDNm
    , EEPROM_val_regen_percentAPPSForCoasting
    , EEPROM_val_regen_percentBPSForMaxRegen
    , EEPROM_val_regen_minimumSpeedKPH
    , EEPROM_val_regen_SpeedRampStart

    //Faults, warnings
} eepromValue;


/*
*  Keep track of the operation being performed (one of these 3 states)
*/
typedef enum
{
    EEPROM_op_initialize    //!< EEPROMManager values are being initialized
    , EEPROM_op_read        //!< A value is being read
    , EEPROM_op_write       //!< A value is being written
    , EEPROM_op_validate    //!< A written value is being validated
    , EEPROM_op_idle        //!< All operations have been completed
    , EEPROM_op_fault       //!< An error occured.  EEPROM will not be used until car is restarted
} eepromOperation;

typedef struct _EEPROMManager
{
    ubyte2 size;            //!< Size of EEPROM actually used by our software
    ubyte1* data_desired;   //!< "Desired" EEPROM values.  Pointer to array of bytes.
    ubyte1* data_actual;    //!< "Actual" (confirmed) EEPROM values.  Pointer to array of bytes.

    eepromOperation status; //!< The current operation being performed by EEPROM
}
EEPROMManager;

//
EEPROMManager* EEPROMManager_new();  //Constructor


/**********************************************************************//**
 *
 * @brief Writes cached changes to EEPROM and validates written data.
 *
 *      This function compares the locally cached EEPROM values ("desired") 
 *      to the values known to be in EEPROM hardware ("actual").  Any
 *      differences will be sent to EEPROM hardware when it is available.
 *
 *      This function only needs to be called once per main() loop.
 *
 * @param   me          Pointer to the EEPROMManager
 *
 * \return IO_ErrorType
 * \retval IO_E_OK                  everything fine / no changes needed
 * \retval IO_E_BUSY                EEPROM module is still busy
 * \retval IO_E_EEPROM_RANGE        invalid address offset or range
 * \retval IO_E_NULL_POINTER        a null pointer has been passed
 * \retval IO_E_CHANNEL_NOT_CONFIGURED the module is not initialized
 *
 ***************************************************************************/
IO_ErrorType EEPROMManager_sync(EEPROMManager* me);

eepromOperation EEPROMManager_getStatus(EEPROMManager* me);

bool EEPROMManager_initialized(EEPROMManager* me);

/**************************************************************
 *
 * @brief Reads or writes data to EEPROM in single byte
 *
 * @param[in]	offset	The index(location) of the EEPROM hex address
 * @param[in]	length	The amount of indexes to read from or write over
 * @param[out]	data 	The data from the EEPROM stored bytes
 *
 * \return IO_ErrorType
 * \retval IO_E_OK                  everything fine / no changes needed
 * \retval IO_E_BUSY                EEPROM module is still busy
 * \retval IO_E_EEPROM_RANGE        invalid address offset or range
 * \retval IO_E_NULL_POINTER        a null pointer has been passed
 * \retval IO_E_CHANNEL_NOT_CONFIGURED the module is not initialized
 *
 **************************************************************/
void readEP(ubyte2 offset, ubyte2 length, ubyte1 data);

void writeEP(ubyte2 offset, ubyte2 length, ubyte1 * data);


/** \defgroup Accessors Different function for each datatype to get a locally stored EEPROM value.
* \brief Gets value from the locally cached copy of EEPROM.
* @param[in]    me          Pointer to the EEPROMManager
* @param[in]    parameter   Which value to be read from EEPROM cache
* @param[out]   value       The value from EEPROM will be returned here
* \retval Whether the value was successfully read or not (in the cache).
* \{
*/
bool EEPROMManager_get_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1* value);
bool EEPROMManager_get_ubyte2(EEPROMManager* me, eepromValue parameter, ubyte2* value);
bool EEPROMManager_get_ubyte4(EEPROMManager* me, eepromValue parameter, ubyte4* value);
bool EEPROMManager_get_sbyte1(EEPROMManager* me, eepromValue parameter, sbyte1* value);
bool EEPROMManager_get_sbyte2(EEPROMManager* me, eepromValue parameter, sbyte2* value);
bool EEPROMManager_get_sbyte4(EEPROMManager* me, eepromValue parameter, sbyte4* value);
bool EEPROMManager_get_float4(EEPROMManager* me, eepromValue parameter, float4* value);
bool EEPROMManager_get_bool(EEPROMManager* me, eepromValue parameter, bool* value);
/** \} */


/** \defgroup Mutators Different function for each datatype to set a locally stored EEPROM value.
* \brief Edits a value in the locally cached copy of EEPROM.
* @param[in]    me          Pointer to the EEPROMManager
* @param[in]    parameter   Which variable to be edited in EEPROM cache
* @param[in]    value       What to change the variable to
* \retval Whether the value was successfully changed or not (in the cache).
* @{
*/
bool EEPROMManager_set_ubyte1(EEPROMManager* me, eepromValue parameter, ubyte1 value);
bool EEPROMManager_set_ubyte2(EEPROMManager* me, eepromValue parameter, ubyte2 value);
bool EEPROMManager_set_ubyte4(EEPROMManager* me, eepromValue parameter, ubyte4 value);
bool EEPROMManager_set_sbyte1(EEPROMManager* me, eepromValue parameter, sbyte1 value);
bool EEPROMManager_set_sbyte2(EEPROMManager* me, eepromValue parameter, sbyte2 value);
bool EEPROMManager_set_sbyte4(EEPROMManager* me, eepromValue parameter, sbyte4 value);
bool EEPROMManager_set_float4(EEPROMManager* me, eepromValue parameter, float4 value);
bool EEPROMManager_set_bool(EEPROMManager* me, eepromValue parameter, bool value);
/** @} */


#endif