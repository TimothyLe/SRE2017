#include <stdio.h>
#include <string.h>
#include "IO_Driver.h"
#include "IO_UART.h"
#include "IO_DIO.h"
#include "IO_RTC.h"
#include "APDB.h"
#include "IO_EEPROM.h"

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
  
    IO_DI_Init( IO_DI_01
              , IO_DI_PD_10K );
    IO_DO_Init( IO_ADC_CUR_01); //turns ground on/off
    //TCS switch
    IO_DI_Init( IO_DI_02, IO_DI_PD_10K );
    IO_DO_Init(IO_ADC_CUR_00);
    IO_DO_Init(IO_DO_03);

        bool ecoSwitch_prev;
        bool ecoSwitch_now;
        bool tcs;
        ubyte1 eeprom_store = FALSE; //might be garbage
        //values from previous run possibly?                                    

    while (1)
    {
        IO_RTC_StartTime(&timestamp);
        IO_Driver_TaskBegin();

        ecoSwitch_prev = ecoSwitch_now;

        IO_DI_Get( IO_DI_01, &ecoSwitch_now);
        IO_DI_Get( IO_DI_02, &tcs);
        IO_DO_Set( IO_ADC_CUR_01, ecoSwitch_now);
        IO_DO_Set( IO_ADC_CUR_00, tcs);
        
        if(ecoSwitch_prev == FALSE && ecoSwitch_now == TRUE){
            //on-click
            IO_EEPROM_Write(1,1, &ecoSwitch_now);
        }

        IO_EEPROM_Read(3,1, &eeprom_store); //might be garbage
        IO_DO_Set( IO_DO_03, eeprom_store);

        IO_Driver_TaskEnd();

        while (IO_RTC_GetTimeUS(timestamp) < 50000);
        {
        }
    }
}

void EEPROM_Read(ubyte2 offset, ubyte2 length, ubyte1 * const data)
{
    //check if the EEPROM is busy
    if(IO_EEPROM_GetStatus() == IO_E_OK)
    {
        //not busy starts reading
        IO_EEPROM_Read(offset, length, data);
    }
    //data is busy not available
    //needs IO_EEPROM_GetStatus 
    //to return IO_E_OK 
}

void EEPROM_Write(ubyte2 offset, ubyte2 length, ubyte1 * const data)
{
     //check if the EEPROM is busy
    if(IO_EEPROM_GetStatus() == IO_E_OK)
    {
        //not busy starts reading
        IO_EEPROM_Write(offset, length, data);
    }
    //data is busy not available
    //needs IO_EEPROM_GetStatus 
    //to return IO_E_OK 
}


