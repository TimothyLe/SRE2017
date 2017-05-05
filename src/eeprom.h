
/**********************************************************************//**
 * @file eeprom.h
 *
 * @brief VCU functions for reading/writing EEPROM
 *
 *      The EEPROM manager allows VCU code to function at any main loop speed
 *      without needing to be concerned for EEPROM read/write delays.
 *
 *      This works by keeping two copies of EEPROM in memory, "software" and "hardware"
 *      in the form of ubyte1 arrays.  VCU software interactions will directly
 *      access the "software" copy, called the cache.  EEPROMManager keeps track
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

#include <stdlib.h> //malloc
#include <stdio.h>
#include "IO_Driver.h"
#include "IO_EEPROM.h"

/*
 *  This enum provides a list of all the variables stored by EEPROM. This
 *  gives developers using an IDE an easy way to call up a specific value.
 */
typedef enum
{
    EEPROM_val_doNotUse = 0x0000         //!< ubyte4 0x0000
    , EEPROM_val_TPS0_calibMin = 0x0004  //!< ubyte2 0x0004
    , EEPROM_val_TPS0_calibMax = 0x0006  //!< ubyte2 0x0006
    , EEPROM_val_TPS1_calibMin = 0x0008  //!< ubyte2 0x0008
    , EEPROM_val_TPS1_calibMax = 0x000A  //!< ubyte2 0x000A

    , EEPROM_val_MCM_torqueMaximumDNm = 0x000C //!< sbyte2 0x000C
    , EEPROM_val_regen_torqueLimitDNm = 0x000E  //!< ubyte2 0x000E
    , EEPROM_val_regen_torqueAtZeroPedalDNm = 0x0010 //!< ubyte2 0x0010
    , EEPROM_val_regen_percentAPPSForCoasting =0x0014 //!< float4 0x0014 
    , EEPROM_val_regen_percentBPSForMaxRegen = 0x0018 //!< float4 0x0018
    , EEPROM_val_regen_minimumSpeedKPH = 0x001C //!< ubyte2 0x001C
    , EEPROM_val_regen_SpeedRampStart = 0x001E //!< ubyte2 0x001E
    , EEPROM_val_regen_throttlePedal = 0x0020 //!< ubyte2 0x0020
    
    //! Faults and warnings can be found in the eepromOperation
    //, EEPROM_val_faults
    //, EEPROM_val_warnings
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

/*
*   @brief  the stages for performing a big endian shift on the 
*           set_ubyte and get_byte helper functions
*   
*   Endian shift referenced by
*   EEPROM_endianShift* shift
*   shift->isByte1
*   shift->isByte2
*   shift->isByte4
*   shift->isByte8
*/
typedef enum _EEPROM_endianShift{
    isByte1 //= sizeof(ubyte1)
    , isByte2 //= sizeof(ubyte2)
    , isByte4 //= sizeof(ubyte4)
    , isByte8 //= sizeof(bool)
} eepromLength;

/*
*   The main EEPROM Manager that is typically referenced by
*   EEPROMManager *me
*   me->size
*   me->data_software
*   me->data_hardware
*   me->status
*/
typedef struct _EEPROMManager /*!< struct identifier */
{
    ubyte2 size;            //!< Size of EEPROM actually used by our software
    ubyte1* data_software;   //!< "Desired" EEPROM values.  Pointer to array of bytes.
    ubyte1* data_hardware;    //!< "Actual" (confirmed) EEPROM values.  Pointer to array of bytes.
    // eepromValue type;       //!< Gives developers easy way to request a specific value
    eepromOperation status; //!< The current operation being performed by EEPROM
    eepromLength length;    //!< Describes the amount of bytes in a value
}EEPROMManager;

//! Constructor
EEPROMManager* EEPROMManager_new();  


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
eepromOperation EEPROMManager_sync(EEPROMManager* me, ubyte2 offset);

eepromOperation EEPROMManager_getStatus(EEPROMManager* me);

bool EEPROMManager_initialized(EEPROMManager* me);

/**************************************************************
 *
 * @brief Reads or writes data to EEPROM in single byte. 
 *
 *      These private functions are intended to help other special 
 *      functions with insertion or deletion into the addresses
 *      of the EEPROM. 
 *
 *      They can be called multiple times per iteration. 
 *
 * @param       me      Pointer to EEPROMManager
 * @param       shift   Pointer to EEPROM_endianShift
 * @param[in]   offset  The index(location) of the EEPROM hex address
 * @param[in]   length  The amount of indexes to read from or write over
 * @param[out]  data    The data from the EEPROM stored bytes
 * @param[out]  value   The data set that carries the size of the mutator/accessor parameters
 *
 * \return IO_ErrorType
 * \retval IO_E_OK                  everything fine / no changes needed
 * \retval IO_E_BUSY                EEPROM module is still busy
 * \retval IO_E_EEPROM_RANGE        invalid address offset or range
 * \retval IO_E_NULL_POINTER        a null pointer has been passed
 * \retval IO_E_CHANNEL_NOT_CONFIGURED the module is not initialized
 *
 **************************************************************/
void readEP(ubyte2 offset, ubyte2 length, EEPROMManager* me);

void writeEP(ubyte2 offset, ubyte2 length, EEPROMManager* me);

void EEPROM_parseMessage(EEPROMManager* me, eepromValue parameter);
// bool getAddress(eepromValue value, ubyte2* address, ubyte1* bytes);
// void readInitialValues(ubyte1* data); 

/** \defgroup Mutators Different function for each datatype to set a locally stored EEPROM value.
* \brief Edits a value in the locally cached copy of EEPROM.
* @param[in]    me          Pointer to the EEPROMManager
* @param[in]    parameter   Which variable to be edited in EEPROM cache
* @param[in]    value       What to change the variable to
* \retval Whether the value was successfully changed or not (in the cache).
* \retval The size of the data (1 byte, 2 byte, 4 byte, or 8 byte->should be no larger).
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

/** \defgroup Accessors Different function for each datatype to get a locally stored EEPROM value.
* \brief Gets value from the locally cached copy of EEPROM.
* @param[in]    me          Pointer to the EEPROMManager
* @param[in]    parameter   Which value to be read from EEPROM cache
* @param[out]   value       The value from EEPROM will be returned here
* \retval Whether the value was successfully read or not (in the cache).
* \retval The size of the data (1 byte, 2 byte, 4 byte, or 8 byte->should be no larger).
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

/** \defgroup Endian Shifters function for each datatype to get a locally stored EEPROM value.
* \brief Performs little endian shift to preserve most significant bits of value
* @param[in]    me          Pointer to the EEPROMManager
* @param[in]    parameter   Which value to be read from EEPROM cache
* @param[out]   value       The value from EEPROM will be returned here
* \retval Whether the value was successfully read or not (in the cache).
* \retval The size of the data (1 byte, 2 byte, 4 byte, or 8 byte->should be no larger).
* \{
*/
bool eepromLength_shift1(EEPROMManager* me, eepromValue parameter, ubyte1 value);
bool eepromLength_shift2(EEPROMManager* me, eepromValue parameter, ubyte2 value);
bool eepromLength_shift4(EEPROMManager* me, eepromValue parameter, ubyte4 value);

// /** \defgroup Accessors that return specific EEPROM values (calibrations, faults, etc)
// * \brief Edits a value in the locally cached copy of EEPROM.
// * @param[in]    me          Pointer to the EEPROMManager
// * @param[in]    parameter   Which variable to be edited in EEPROM cache
// * @param[in]    value       What to change the variable to
// * \retval Whether the value was successfully changed or not (in the cache).
// * \retval The size of the data (1 byte, 2 byte, 4 byte, or 8 byte->should be no larger).
// * @{
//  */
// ubyte4 get_EEPROM_val_doNotUse(EEPROMManager* me, eepromValue parameter, ubyte4 value);         //!< ubyte4 0x0000
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

// * \defgroup Mutators Different function that sets specific EEPROM values (calibrations, faults, etc)
// * \brief Edits a value in the locally cached copy of EEPROM.
// * @param[in]    me          Pointer to the EEPROMManager
// * @param[in]    parameter   Which variable to be edited in EEPROM cache
// * @param[in]    value       What to change the variable to
// * \retval Whether the value was successfully changed or not (in the cache).
// * \retval The size of the data (1 byte, 2 byte, 4 byte, or 8 byte->should be no larger).
// * @{
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

#endif
