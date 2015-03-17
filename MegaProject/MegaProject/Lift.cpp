/* 
* Lift.cpp
*
* Created: 3/31/2014 12:27:31 AM
* Author: Quang
*/


#include "Lift.h"
#include <Arduino.h>


// default constructor
Lift::Lift()
{
} //Lift

/*******************************************************************
*
*				Functions for the master
*
*******************************************************************/
void Lift::MasterInitialize()
{
	// initialize all the variables
	Change = 0;
	SetFlag = 0;

	// Set all the pins to the proper mode
	pinMode(LiftBusy, INPUT);           // Busy pin = input
	pinMode(LiftData, OUTPUT);          // Data pin = output
	pinMode(LiftLevel, OUTPUT);         // Level pin = output

	// Make sure that Data and Level line are LOW to begin with
	digitalWrite(LiftData, 0);
	digitalWrite(LiftLevel, 0);
}

void Lift::MasterSetLevel(int Shelf)
{
	// Write to the Level line the value of shelf
	// 0 = lower shelf, 1 = higher shelf
    digitalWrite(LiftLevel, Shelf);
}

// This function doesn't look very intuitive to begin with but the function of it is to 
// let the master decide whether it needs to request a statechange for the slave or not.
// Look at the conditional statement of liftMasterSpinOnce() to better understand the function
// of the variable Change.
void Lift::MasterRequestStateChange()
{
    Change = 1;
}

// Return 0 - Request for state change wasn't complete
// Return 1 - Request for state change was complete
// Return 2 - The slave may or may not be free but we don't really care.
int Lift::MasterSpinOnce()
{
    // If the slave is not busy and it need to progress to the next state of the lift
    if ((digitalRead(LiftBusy) == 0) && (Change == 1) && (SetFlag == 0))
    {
	    digitalWrite(LiftData, 1);
	    // Set flag to indicate that we just set the data line
	    SetFlag = 1;
		return 0;
    }
	else if ((digitalRead(LiftBusy) == 1) && (Change == 1) && (SetFlag == 0)){
		return 0;
	}
    
    // If the slave became busy because it register our request for a change in state
    else if ((digitalRead(LiftBusy) == 1) && (Change == 1) && (SetFlag == 1)){
	    digitalWrite(LiftData, 0);
	    // Clear all the flags since we've just successfully requested a statechange for the slave
	    SetFlag = 0;
	    Change = 0;
		return 1;
    }
	else return 0;
}

/***********************************************
 *
 *      Functions for Slave
 *
 ***********************************************/

void Lift::SlaveInitialize()
{
    // Set all the pins to the proper mode
    pinMode(LiftBusy, OUTPUT);
    pinMode(LiftData, INPUT);
    pinMode(LiftLevel, INPUT);

	//Make sure the Busy line is initially set to ready
    digitalWrite(LiftBusy, 0);
}


void Lift::SlaveSetBusy()
{
	// Slave set the Busy line to busy
	digitalWrite(LiftBusy, HIGH);
}

void Lift::SlaveSetReady()
{
	// Slave set the busy line to Ready
	digitalWrite(LiftBusy, LOW);
}

int  Lift::SlaveGetLevel()
{
	// Return the value of the Level line
	return digitalRead(LiftLevel);
}

int  Lift::SlaveStateChangeCheck()
{
	// Return the value of the Data line
	return digitalRead(LiftData);
}

