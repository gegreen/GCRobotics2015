/* 
* Lift.h
*
* Created: 3/31/2014 12:27:31 AM
* Author: Quang
*/


#ifndef __LIFT_H__
#define __LIFT_H__

#define LiftBusy    12    // Busy flag.      0 = ready , 1 = busy
#define LiftData    11    // Data flag.      0 = do nothing, 1 = move to next state
#define LiftLevel   10    // Tells which level the plate is located on
// 0 = bottom shelf
// 1 = top shelf


class Lift
{
//variables
public:
	int Change;            // 0 = don't need to change state, 1 = need to change state
	int SetFlag;

//functions
public:
	Lift();
	
	// Initialize functions for the master
	void MasterInitialize();

	void MasterSetLevel(int Shelf);

	// This function is called to indicate that we want to request the slave to change state
	void MasterRequestStateChange();

	// This function is the heart of the library. It handles the communication between the master and the slave
	
	int MasterSpinOnce();

	// Initialize functions for the master
	void SlaveInitialize();

	// The slave will pull the Busy line HIGH to indicate that it is busy
	void SlaveSetBusy();

	// The slave will pull the Busy line LOW to indicate that it is not busy
	void SlaveSetReady();

	// The slave will read the Level line and return the result
	int  SlaveGetLevel();

	// The Slave will read the Data line and return whether or not the master is telling it to change state
	int  SlaveStateChangeCheck();


}; //Lift

#endif //__LIFT_H__
