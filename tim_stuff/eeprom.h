
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
    //, EEPROM_val_regen_throttlePedal
    
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
    isByte1
    , isByte2
    , isByte4
    , isByte8
} eeprom_endianShift;

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
    eepromValue type;       //!< Gives developers easy way to request a specific value
    eepromOperation status; //!< The current operation being performed by EEPROM
    eeprom_endianShift memory; //!< The current size of the memory address
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

void eeprom_endianShift_1(EEPROMManager* me, eeprom_endianShift* shift, ubyte1 value);
void eeprom_endianShift_2(EEPROMManager* me, eeprom_endianShift* shift, ubyte1 value);
void eeprom_endianShift_4(EEPROMManager* me, eeprom_endianShift* shift, ubyte1 value);
void eeprom_endianShift_8(EEPROMManager* me, eeprom_endianShift* shift, ubyte1 value);

// bool getAddress(eepromValue value, ubyte2* address, ubyte1* bytes);
// void readInitialValues(ubyte1* data); 

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


#endif
