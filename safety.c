#include <stdlib.h> //malloc
//#include <math.h>
#include "IO_Driver.h"
#include "IO_RTC.h"
#include "IO_DIO.h"

#include "safety.h"
#include "mathFunctions.h"

#include "sensors.h"

#include "torqueEncoder.h"
#include "brakePressureSensor.h"

#include "motorController.h"
#include "bms.h"
#include "serial.h"

//last flag is 0x 8000 0000 (32 flags, 8 hex characters)
//Faults -------------------------------------------
//nibble 1
static const ubyte4 F_tpsOutOfRange = 1;
static const ubyte4 F_bpsOutOfRange = 2;
static const ubyte4 F_tpsPowerFailure = 4;
static const ubyte4 F_bpsPowerFailure = 8;
//nibble 2
static const ubyte4 F_tpsSignalFailure = 0x10;
static const ubyte4 F_bpsSignalFailure = 0x20;
static const ubyte4 F_tpsNotCalibrated = 0x40;
static const ubyte4 F_bpsNotCalibrated = 0x80;

//nibble 3
static const ubyte4 F_tpsOutOfSync = 0x100;
static const ubyte4 F_bpsOutOfSync = 0x200; //NOT USED
static const ubyte4 F_tpsbpsImplausible = 0x400;
//static const ubyte4 UNUSED = 0x800;

//nibble 4
//static const ubyte4 F_ = 0x1000;
//static const ubyte4 F_ = 0x2000;
//static const ubyte4 F_ = 0x4000;
//static const ubyte4 F_ = 0x8000;

//nibble 5
static const ubyte4 F_lvsBatteryVeryLow = 0x10000;
//static const ubyte4 F_ = 0x20000;
//static const ubyte4 F_ = 0x40000;
//static const ubyte4 F_ = 0x80000;

//nibble 6
//nibble 7
//nibble 8
//                             nibble: 87654321
static const ubyte4 F_unusedFaults = 0xFFFEF800;


//Warnings -------------------------------------------
static const ubyte2 W_lvsBatteryLow = 1;

//Notices
static const ubyte2 N_HVILTermSenseLost = 1;

static const ubyte2 N_Over75kW_BMS = 0x10;
static const ubyte2 N_Over75kW_MCM = 0x20;


/*****************************************************************************
* SafetyChecker object
******************************************************************************
* ToDo: change to ubyte1[8] (64 flags possible)
* 1 = fault
* 0 = no fault
****************************************************************************/
struct _SafetyChecker {
	//Problems that require motor torque to be disabled
    SerialManager* serialMan;
    ubyte4 faults;
    ubyte2 warnings;
    ubyte2 notices;
    ubyte1 maxAmpsCharge;
    ubyte1 maxAmpsDischarge;

    bool tpsbpsImplausible;
};

/*****************************************************************************
* Torque Encoder (TPS) functions
* RULE EV2.3.5:
* If an implausibility occurs between the values of these two sensors the power to the motor(s) must be immediately shut down completely.
* It is not necessary to completely deactivate the tractive system, the motor controller(s) shutting down the power to the motor(s) is sufficient.
****************************************************************************/
SafetyChecker* SafetyChecker_new(SerialManager* sm, ubyte2 maxChargeAmps, ubyte2 maxDischargeAmps)
{
    SafetyChecker* me = (SafetyChecker*)malloc(sizeof(struct _SafetyChecker));

    me->serialMan = sm;
	//Initialize all safety checks to FAIL? Not anymore
    me->faults = 1;
    me->warnings = 0;

    me->tpsbpsImplausible = TRUE;

    me->maxAmpsCharge = maxChargeAmps;
    me->maxAmpsDischarge = maxDischargeAmps;
    return me;
}

//Updates all values based on sensor readings, safety checks, etc
void SafetyChecker_update(SafetyChecker* me, MotorController* mcm, BatteryManagementSystem* bms, TorqueEncoder* tps, BrakePressureSensor* bps, Sensor* HVILTermSense, Sensor* LVBattery)
{
    ubyte1* message[50];  //For sprintf'ing variables to print in serial
    //SerialManager_send(me->serialMan, "Entered SafetyChecker_update().\n");
    /*****************************************************************************
    * Faults
    ****************************************************************************/
	//===================================================================
	// Get calibration status
	//===================================================================
    //me->faults = 0xFFFF; //Set ALL faults by default.  only clear if truly safe
    me->faults &= ~F_unusedFaults;

    //me->faults = 1;
    if (tps->calibrated == FALSE) { me->faults |= F_tpsNotCalibrated; }
    else { me->faults &= ~F_tpsNotCalibrated; }

    if (bps->calibrated == FALSE) { me->faults |= F_bpsNotCalibrated; }
    else { me->faults &= ~F_bpsNotCalibrated; }

	//===================================================================
	// Check if VCU was able to get a TPS/BPS reading
	//===================================================================
	if (tps->tps0->ioErr_powerInit != IO_E_OK
		|| tps->tps1->ioErr_powerInit != IO_E_OK
		|| tps->tps0->ioErr_powerSet != IO_E_OK
		|| tps->tps1->ioErr_powerSet != IO_E_OK)
    {
        me->faults |= F_tpsPowerFailure;
    }
    else
    {
        me->faults &= ~F_tpsPowerFailure;
    }

	if (   tps->tps0->ioErr_signalInit != IO_E_OK
		|| tps->tps1->ioErr_signalInit != IO_E_OK
		|| tps->tps0->ioErr_signalGet != IO_E_OK
		|| tps->tps1->ioErr_signalGet != IO_E_OK)
	{
		//me->faults |= F_tpsSignalFailure;
        SerialManager_send(me->serialMan, "TPS signal error\n");
	}
    else
    {
        me->faults &= ~F_tpsSignalFailure;
    }

	if (bps->bps0->ioErr_powerInit != IO_E_OK
		|| bps->bps0->ioErr_powerSet != IO_E_OK)
	{
		me->faults |= F_bpsPowerFailure;
	}
    else
    {
        me->faults &= ~F_bpsPowerFailure;
    }

	if (bps->bps0->ioErr_signalInit != IO_E_OK
		|| bps->bps0->ioErr_signalGet != IO_E_OK)
	{
		me->faults |= F_bpsSignalFailure;
	}
    else
    {
        me->faults &= ~F_bpsSignalFailure;
    }

	//===================================================================
	// Make sure raw sensor readings are within operating range
	//===================================================================
	//RULE: EV2.3.10 - signal outside of operating range is considered a failure
	//  This refers to SPEC SHEET values, not calibration values
	//Note: IC cars may continue to drive for up to 100ms until valid readings are restored, but EVs must immediately cut power
	//Note: We need to decide how to report errors and how to perform actions when those errors occur.  For now, I'm calling an imaginary Err.Report function
	
	//-------------------------------------------------------------------
	//Torque Encoder
	//-------------------------------------------------------------------
	if (tps->tps0->sensorValue < tps->tps0->specMin || tps->tps0->sensorValue > tps->tps0->specMax
	||  tps->tps1->sensorValue < tps->tps1->specMin || tps->tps1->sensorValue > tps->tps1->specMax)
	{
		me->faults |= F_tpsOutOfRange;
	}
    else
    {
        me->faults &= ~F_tpsOutOfRange;
    }

	//-------------------------------------------------------------------
	//Brake Pressure Sensor
	//-------------------------------------------------------------------
	if (bps->bps0->sensorValue < bps->bps0->specMin || bps->bps0->sensorValue > bps->bps0->specMax)
	{
		me->faults |= F_bpsOutOfRange;
	}
    else
    {
        me->faults &= ~F_bpsOutOfRange;
    }

	//===================================================================
	// Make sure calibrated TPS readings are in sync with each other
	//===================================================================
	// EV2.3.5 If an implausibility occurs between the values of these two sensors
	//  the power to the motor(s) must be immediately shut down completely. It is not necessary 
	//  to completely deactivate the tractive system, the motor controller(s) shutting down the 
	//  power to the motor(s) is sufficient.
	// EV2.3.6 Implausibility is defined as a deviation of more than 10 % pedal travel between the sensors.
	//-------------------------------------------------------------------
	
	//Check for implausibility (discrepancy > 10%)
	//RULE: EV2.3.6 Implausibility is defined as a deviation of more than 10% pedal travel between the sensors.
	float4 tps0Percent;   //Pedal percent float (a decimal between 0 and 1
	float4 tps1Percent;

	TorqueEncoder_getIndividualSensorPercent(tps, 0, &tps0Percent); //borrow the pedal percent variable
	TorqueEncoder_getIndividualSensorPercent(tps, 1, &tps1Percent);

    //sprintf(message, "TPS0: %f\n", tps0Percent);
    //SerialManager_send(me->serialMan, message);
    //sprintf(message, "TPS1: %f\n", tps1Percent);
    //SerialManager_send(me->serialMan, message);

	if ((tps1Percent - tps0Percent) > .1 || (tps1Percent - tps0Percent) < -.1)  //Note: Individual TPS readings don't go negative, otherwise this wouldn't work
	{

		//Err.Report(Err.Codes.TPSDiscrepancy, "TPS discrepancy of over 10%", Motor.Stop);
        SerialManager_send(me->serialMan, "TPS discrepancy of over 10%\n");

        me->faults |= F_tpsOutOfSync;
	}
    else
    {
        me->faults &= ~F_tpsOutOfSync;
    }

    //Only one BPS right now - this fault doesn't happen
    me->faults &= ~F_bpsOutOfSync;


	//===================================================================
	//Torque Encoder <-> Brake Pedal Plausibility Check
	//===================================================================
	// EV2.5 Torque Encoder / Brake Pedal Plausibility Check
	//  The power to the motors must be immediately shut down completely, if the mechanical brakes 
	//  are actuated and the torque encoder signals more than 25 % pedal travel at the same time.
	//  This must be demonstrated when the motor controllers are under load.
	// EV2.5.1 The motor power shut down must remain active until the torque encoder signals less than 5 % pedal travel,
	//  no matter whether the brakes are still actuated or not.
	//-------------------------------------------------------------------
	//Implausibility if..
    //float4 twelve = 12.0;
    //SerialManager_sprintf(me->serialMan, "The number twelve: %d\n", &twelve);
    bool tpsHigh = FALSE;
    bool bpsHigh = FALSE;
    if (bps->percent > .25) 
    {
        bpsHigh = TRUE;
        SerialManager_send(me->serialMan, "BPS > .02\n"); 
    }
    else
    {
        bpsHigh = FALSE;
    }

    if (tps->percent > .25) 
    {
        tpsHigh = TRUE;
        SerialManager_send(me->serialMan, "TPS > .25\n"); 
    }
    else
    {
        tpsHigh = FALSE;
    }
    
    
    
    //if (bps->percent > .05 && tps->percent > .25)
    if (tpsHigh == TRUE && bpsHigh == TRUE)
    {
        //If mechanical brakes actuated && tps > 25%
       
            me->faults |= F_tpsbpsImplausible;
            me->tpsbpsImplausible = TRUE;
            SerialManager_send(me->serialMan, "TPS BPS implausiblity detected.\n");
            //From here, assume that motor controller will check for implausibility before accepting commands
       
    }
	//Clear implausibility if...
	//if ((me->faults & F_tpsbpsImplausible) > 0)
	//{
		if (tps->percent < .10) //TPS is reduced to < 5%
		{
            //me->tpsbpsImplausible = FALSE;
            SerialManager_send(me->serialMan, "TPS below .05.  No implausibility.\n");
			me->faults &= ~(F_tpsbpsImplausible);  //turn off the implausibility flag
		}
	//}

        SerialManager_send(me->serialMan, "\n");




    /*****************************************************************************
    * Warnings
    ****************************************************************************/
    //===================================================================
    // LVS Battery Check - FAULTS LATCH UNTIL RETURN TO PREVIOUS STAGE
    //===================================================================
    //  IO_ADC_UBAT: 0..40106  (0V..40.106V)
    //-------------------------------------------------------------------
    if (LVBattery->sensorValue <= 9200)  //12730 = 10% SOC but hard to tell under load. 9200 = empty
    {
        me->faults |= F_lvsBatteryVeryLow;
        me->warnings |= W_lvsBatteryLow;
        sprintf(message, "LVS battery %.03fV EXTREMELY LOW!\n", (float4)LVBattery->sensorValue / 1000);
        SerialManager_send(me->serialMan, message);
    }
    else if (LVBattery->sensorValue <= 12730)  //13100 = Recharge percentage, per Shorai
    {
        me->faults &= ~F_lvsBatteryVeryLow;
        me->warnings |= W_lvsBatteryLow;
        sprintf(message, "LVS battery %.03fV LOW.\n", (float4)LVBattery->sensorValue / 1000);
        SerialManager_send(me->serialMan, message);
    }
    else
    {
        me->warnings &= ~F_lvsBatteryVeryLow;
        me->warnings &= ~W_lvsBatteryLow;
        //sprintf(message, "LVS battery %.03fV good.\n", (float4)LVBattery->sensorValue / 1000);
    }



    //===================================================================
    // HVIL Term Sense Check
    //===================================================================
    // If HVIL term sense goes low (because HV went down), motor torque
    // command should be set to zero before turning off the controller
    //-------------------------------------------------------------------
    if (HVILTermSense->sensorValue == FALSE)
    {
        me->notices |= N_HVILTermSenseLost;
    }
    else
    {
        me->notices &= ~N_HVILTermSenseLost;
    }


    if (BMS_getPower(bms) > 75000)
    {
        me->notices |= N_Over75kW_BMS;
    }
    else
    {
        me->notices &= ~N_Over75kW_BMS;
    }

        
    if (MCM_getPower(mcm) > 75000)
    {
        me->notices |= N_Over75kW_MCM;
    }
    else
    {
        me->notices &= ~N_Over75kW_MCM;
    }

}


//Updates all values based on sensor readings, safety checks, etc
bool SafetyChecker_allSafe(SafetyChecker* me)
{
    return (me->faults == 0);
}

//Updates all values based on sensor readings, safety checks, etc
ubyte4 SafetyChecker_getFaults(SafetyChecker* me)
{
    return (me->faults);
}

//Updates all values based on sensor readings, safety checks, etc
ubyte4 SafetyChecker_getWarnings(SafetyChecker* me)
{
    return (me->warnings);
}

//Updates all values based on sensor readings, safety checks, etc
ubyte4 SafetyChecker_getNotices(SafetyChecker* me)
{
    return (me->notices);
}

void SafetyChecker_reduceTorque(SafetyChecker* me, MotorController* mcm, BatteryManagementSystem* bms)
{
    float4 multiplier = 1;
    //float4 tempMultiplier = 1;
    sbyte1 groundSpeedKPH = MCM_getGroundSpeedKPH(mcm);

    //-------------------------------------------------------------------
    // Critical conditions - set 0 torque
    //-------------------------------------------------------------------
    if (me->faults > 0) //Any VCU fault exists
    {
        multiplier = 0;
    }
    //////////if ((me->notices & N_HVILTermSenseLost) > 0) // HVIL is low (must command 0 torque before opening MCM relay
    //////////{
    //////////    multiplier = 0;
    //////////    SerialManager_send(me->serialMan, "SC.0: HVIL term sense low\n");
    //////////}
    ////////if (MCM_commands_getTorque(mcm) < 0 && groundSpeedKPH < 5)  //No regen below 5kph
    ////////{
    ////////    SerialManager_send(me->serialMan, "SC.0: Regen < 5kph\n");
    ////////    multiplier = 0;
    ////////}
    //-------------------------------------------------------------------
    // Other limits (% reduction) - set torque to the lowest of all these
    // IMPORTANT: Be aware of direction-sensitive situations (accel/regen)
    //-------------------------------------------------------------------
    //80kW limit ---------------------------------
    // if either the bms or mcm goes over 75kw, limit torque 
    //////////if ((BMS_getPower(bms) > 75000) || (MCM_getPower(mcm) > 75000))
    //////////{
    //////////    // using bmsPower since closer to e-meter
    //////////    tempMultiplier = 1 - getPercent(max(BMS_getPower(bms), MCM_getPower(mcm)), 75000, 80000, TRUE);
    //////////    SerialManager_send(me->serialMan, "SC.Mult: 80kW\n");
    //////////}
    //////////if (tempMultiplier < multiplier) { multiplier = tempMultiplier; }

    //CCL/DCL from BMS --------------------------------
    //why the DCL/CCL could be limited:
    //0: No limit
    //1 : Pack voltage too low
    //2 : Pack voltage high
    //3 : Cell voltage low
    //4 : Cell voltage high
    //5 : Temperature high for charging
    //6 : Temperature too low for charging
    //7 : Temperature high for discharging
    //8 : Temperature too low for discharging
    //9 : Charging current peak lasted too long
    //10 = A : Discharging current peak lasted too long
    //11 = B : Power up delay(Charge testing)
    //12 = C : Fault
    //13 = D : Contactors are off
    ////////if (MCM_commands/*_getTorque(mcm) >= 0)
    ////////{*/
        ///////////*tempMultiplier = getPercent(BMS_getDCL(bms), 0, me->maxAmpsDischarge, TRUE);
        //////////if (tempMultiplier < 1)
        //////////{
        //////////    SerialManager_send(me->serialMan, "SC.Mult: DCL\n");
        //////////}*/
    //////////}
    //////////else //regen - Pick the lowest of CCL and speed reductions
    //////////{
    //////////    tempMultiplier = getPercent(BMS_getCCL(bms), 0, me->maxAmpsCharge, TRUE);
    //////////    if (tempMultiplier < 1)
    //////////    {
    //////////        SerialManager_send(me->serialMan, "SC.Mult: CCL\n");
    //////////    }
    //////////    //Also, regen should be ramped down as speed approaches minimum
    //////////    if ( groundSpeedKPH < 15)
    //////////    {
    //////////        float4 regenMultiplier = 1 - getPercent(groundSpeedKPH, MCM_getRegenMinSpeed(mcm), MCM_getRegenRampdownStartSpeed(mcm), TRUE);
    //////////        if (tempMultiplier < 1) { SerialManager_send(me->serialMan, "SC.Mult: Regen < 15kph\n"); }
    //////////        if (regenMultiplier < tempMultiplier) { tempMultiplier = regenMultiplier; } // Pick the lesser of CCL (tempMultiplier) or speed reduction (regenMultiplier)
    //////////    }
    //////////}
    ////////if (tempMultiplier < multiplier) { multiplier = tempMultiplier; }

    //Reduce the torque command.  Multiplier should be a percent value (between 0 and 1)
    MCM_commands_setTorque(mcm, MCM_commands_getTorque(mcm) * multiplier);
}

//-------------------------------------------------------------------
// 80kW Limit Check
//-------------------------------------------------------------------
//Change this to return a multiplier instead of torque value
//ubyte2 checkPowerDraw(BatteryManagementSystem* bms, MotorController* mcm)
//{
//    ubyte2 torqueThrottle = 0;
//
//    // if either the bms or mcm goes over 75kw, limit torque 
//    if ((BMS_getPower(bms) > 75000) || (MCM_getPower(mcm) > 75000))
//    {
//        // using bmsPower since closer to e-meter
//        torqueThrottle = MCM_getCommandedTorque(mcm) - (((BMS_getPower(bms) - 80000) / 80000) * MCM_getCommandedTorque(mcm));
//    }
//
//    return torqueThrottle;
//}
