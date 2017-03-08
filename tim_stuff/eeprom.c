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
//#include "canmanager.h"

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

    IO_Driver_Init( NULL );
    IO_EEPROM_Init(); //added
  
    /**************************************
    Digital Input/Output Initializations
    ***************************************/
    /*
    EEPROM* epMan = EEPROMmanager_new(500, 40, 40, 500, 20, 20, 200000);
    void canOutput_sendDebugMessage(EEPROMManager* me
                                    , TorqueEncoder* tps
                                    , BrakePressureSensor* bps
                                    , MotorController* mcm
                                    , WheelSpeeds* wss
                                    , SafetyChecker* sc);
    */

    IO_DI_Init( IO_DI_00 
              , IO_DI_PD_10K); //RTD switch
    IO_DI_Init( IO_DI_01
              , IO_DI_PD_10K ); //Eco switch
    IO_ADC_ChannelInit( IO_ADC_5V_04 
                      , IO_ADC_RESISTIVE
                      , 0
                      , 0
                      , 0
                      , NULL ); //TCS Adjustment POT
    IO_DO_Init( IO_ADC_CUR_01); //grounds eco
    IO_DO_Init( IO_ADC_CUR_03); //grounds rtd

        bool ecoSwitch = TRUE; //initialize these?
        bool rtdSwitch = FALSE;
        /****eeprom test****/
        ubyte2 eOffset = 0x00;
        ubyte2 eLength = 20;
        ubyte1 eTest[20] = "Char"; //array is fine
          // ubyte1 * eTest = "Char";
        /********************/
        bool tcs;
        //ubyte1 eeprom_store; 
        ubyte2 pos2 = 0x230;
        ubyte2 pot_res; //took out equals 0
        // Non-volatile memory may produce previous  
        // Runs EEPROM values or garbage values                                

        // Unit test (maybe write a char 8-bits)
        writeEP(eOffset,eLength, &eTest);
        //readEP(eOffset,eLength, eTest);
          
          // Dynamic memory
          // eTest = malloc(strlen(eTest)+1);
          // free(eTest);
    while (1)
    {
        IO_RTC_StartTime(&timestamp);
        IO_Driver_TaskBegin();

        //ecoSwitch = rtdSwitch;

        IO_DI_Get( IO_DI_01, &ecoSwitch); //Eco switch
        IO_DI_Get( IO_DI_00, &rtdSwitch); //RTD switch
        IO_ADC_Get( IO_ADC_5V_04
                  , &pot_res //points to different type
                  , &tcs ); //TCS Adjustment Pot

        /***************************************
        Checks if TCS POT is adjusted before or
        after the 2nd position and either the Eco switch
        or the RTD switch light will read/write to EEPROM
        and a light up will indicate so
        ***************************************/

        /*!!!
        Change if statement, add messages directly to EEPROM hex 
        spaces etc
        */

        /***************************************
          Conditional Statements
          **************************************
        if(*pot_res > *pos2){
            if(ecoSwitch == TRUE && rtdSwitch == FALSE){
                readEP(3,1, &ecoSwitch);
                IO_DO_Set( IO_ADC_CUR_01
                 , ecoSwitch); //grounds eco
            } else if (ecoSwitch == FALSE && rtdSwitch == TRUE){
                readEP(3,1, &rtdSwitch);
                 IO_DO_Set( IO_ADC_CUR_03, rtdSwitch); //grounds rtd
            }
        } else {
            if(ecoSwitch == TRUE && rtdSwitch == FALSE){
                writeEP(3,1, &ecoSwitch);
                IO_DO_Set( IO_ADC_CUR_01
                 , ecoSwitch); //grounds eco
            } else if (ecoSwitch == FALSE && rtdSwitch == TRUE){
                writeEP(3,1, &rtdSwitch);
                 IO_DO_Set( IO_ADC_CUR_03, rtdSwitch); //grounds rtd
            }
        }
        ***************************************/

        /* Old Code */
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
    //IO_EEPROM_DeInit(); //Needs to be called to reinitialize EEPROM
}



/*Definitions*/
void readEP(ubyte2 offset, ubyte2 length, ubyte1 data){
    //check if the EEPROM is busy
    if(IO_EEPROM_GetStatus() == IO_E_OK){
        //not busy starts reading
        IO_EEPROM_Read(offset, length, &data);
    }
    //data is busy not available
    //needs IO_EEPROM_GetStatus 
    //to return IO_E_OK 
}

void writeEP(ubyte2 offset, ubyte2 length, ubyte1 * data){
     //check if the EEPROM is busy
    if(IO_EEPROM_GetStatus() == IO_E_OK){
        //not busy starts reading
        IO_EEPROM_Write(offset, length, data);
    }
    //data is busy not available
    //needs IO_EEPROM_GetStatus 
    //to return IO_E_OK 
}

