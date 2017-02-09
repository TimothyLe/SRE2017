#include <stdio.h>
#include <string.h>
#include "IO_Driver.h"
#include "IO_UART.h"
#include "IO_DIO.h"
#include "IO_RTC.h"
#include "APDB.h"
#include "IO_EEPROM.h"
#include "IO_ADC.h"
#include "IO_PWD.h"

APDB appl_db =
          { 0                      /* ubyte4 versionAPDB        */
          , {0}                    /* BL_T_DATE flashDate       */
                                   /* BL_T_DATE buildDate                   */
          , { (ubyte4) (((((ubyte4) RTS_TTC_FLASH_DATE_YEAR) & 0x0FFF) << 0) |
                        ((((ubyte4) RTS_TTC_FLASH_DATE_MONTH) & 0x0F) << 12) |
                        ((((ubyte4) RTS_TTC_FLASH_DATE_DAY) & 0x1F) << 16) |
                        ((((ubyte4) RTS_TTC_FLASH_DATE_HOUR) & 0x1F) << 21) |
                        ((((ubyte4) RTS_TTC_FLASH_DATE_MINUTE) & 0x3F) << 26)) }
          , 0                      /* ubyte4 nodeType           */
          , 0                      /* ubyte4 startAddress       */
          , 0                      /* ubyte4 codeSize           */
          , 0                      /* ubyte4 legacyAppCRC       */
          , 0                      /* ubyte4 appCRC             */
          , 1                      /* ubyte1 nodeNr             */
          , 0                      /* ubyte4 CRCInit            */
          , 0                      /* ubyte4 flags              */
          , 0                      /* ubyte4 hook1              */
          , 0                      /* ubyte4 hook2              */
          , 0                      /* ubyte4 hook3              */
          , APPL_START             /* ubyte4 mainAddress        */
          , {0, 1}                 /* BL_T_CAN_ID canDownloadID */
          , {0, 2}                 /* BL_T_CAN_ID canUploadID   */
          , 0                      /* ubyte4 legacyHeaderCRC    */
          , 0                      /* ubyte4 version            */
          , 500                    /* ubyte2 canBaudrate        */
          , 0                      /* ubyte1 canChannel         */
          , {0}                    /* ubyte1 reserved[8*4]      */
          , 0                      /* ubyte4 headerCRC          */
          };


volatile ubyte4 count = 0;
ubyte4 timestamp = 0;

void main(void)
{
    ubyte1 data[128] = {0};
    ubyte1 size, len_tx;

    IO_Driver_Init( NULL );
    IO_EEPROM_Init( NULL ); //added
  
    //Digital Input Initializations
    IO_DI_Init( IO_DI_00 ); //RTD switch
    IO_DI_Init( IO_DI_01
              , IO_DI_PD_10K ); //Eco switch
    IO_ADC_ChannelInit( IO_ADC_5V_04 
                      , IO_ADC_RESISTIVE
                      , 0
                      , 0
                      , 0
                      , NULL ); //TCS Adjustment POT
    IO_DO_Init( IO_ADC_CUR_01); //turns ground on/off
    IO_DO_Init( IO_ADC_CUR_00);  //TCS?

        bool ecoSwitch; //initialize these?
        bool rtdSwitch;
        bool tcs;
        ubyte1 * const eeprom_store = FALSE; 
        ubyte2 pos2 = 0x230; 
        ubyte2 * const pot_res = 0;
        //Non-volatile memory may produce previous  
        //run's EEPROM values or garbage values                                

    while (1)
    {
        IO_RTC_StartTime(&timestamp);
        IO_Driver_TaskBegin();

        ecoSwitch = rtdSwitch;

        IO_DI_Get( IO_DI_01, &ecoSwitch); //Eco switch
        IO_DI_Get( IO_DI_00, &rtdSwitch); //RTD switch
        IO_ADC_Get( IO_ADC_5V_04
                  , &pot_res
                  , &tcs ); //TCS Adjustment Pot

        IO_DO_Set( IO_ADC_CUR_01
                 , ecoSwitch); //ground
        IO_DO_Set( IO_ADC_CUR_00, tcs); //ground
        
        if(pot_res > pos2){
            if(ecoSwitch == TRUE && rtdSwitch == FALSE){
                readEEPROM(3,1, &ecoSwitch);
                
            } else if (ecoSwitch == FALSE && rtdSwitch == TRUE){
                readEEPROM(3,1, &rtdSwitch);
                
            }
        } else {
            if(ecoSwitch == TRUE && rtdSwitch == FALSE){
                writeEEPROM(3,1, &ecoSwitch);
                
            } else if (ecoSwitch == FALSE && rtdSwitch == TRUE){
                writeEEPROM(3,1, &rtdSwitch);
                
            }
        }

        // if(ecoSwitch_prev == FALSE && ecoSwitch_now == TRUE){
        //     //on-click
        //     writeEEPROM(1,1, &ecoSwitch_now); 
        // }
        // /*
        // if(ecoSwitch_prev == FALSE && ecoSwitch_now == FALSE){
        //     //on-click
        //     IO_EEPROM_Read(3,1, &eeprom_store); //might be garbage
        //     IO_DO_Set( IO_DO_03, eeprom_store);
        // }
        // */
        // readEEPROM(3,1, &eeprom_store); //might be garbage
        

        IO_Driver_TaskEnd();

        while (IO_RTC_GetTimeUS(timestamp) < 50000);
        {
        }
    }
}

void readEEPROM(ubyte2 offset, ubyte2 length, ubyte1 * const data)
{
    //check if the EEPROM is busy
    if(IO_EEPROM_GetStatus() == IO_E_OK)
    {
        //not busy starts reading
        IO_EEPROM_Read(offset, length, &data);
    }
    //data is busy not available
    //needs IO_EEPROM_GetStatus 
    //to return IO_E_OK 
}

void writeEEPROM(ubyte2 offset, ubyte2 length, ubyte1 * const data)
{
     //check if the EEPROM is busy
    if(IO_EEPROM_GetStatus() == IO_E_OK)
    {
        //not busy starts reading
        IO_EEPROM_Write(offset, length, &data);
    }
    //data is busy not available
    //needs IO_EEPROM_GetStatus 
    //to return IO_E_OK 
}


