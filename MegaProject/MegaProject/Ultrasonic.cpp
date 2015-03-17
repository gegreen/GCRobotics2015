/*
* Ultrasonic.cpp
*
* Created: 3/15/2014 4:32:08 PM
* Author: Quang
*/


#include "Ultrasonic.h"
#include <Arduino.h>
#include "Motors.h"
//#include "UltrasonicSensors.ino" 

Motors Robot;


// default constructor
Ultrasonic::Ultrasonic()
{
	// Initializing everything with zero
	
	PreviousTime =0;
	InterruptEchoTime = 0;
	Select = 0;
	for (int i = 0; i < 6; i++)
	{
		EchoDistance[i] = 0;
	}
	FullSet = 0;
	StateMachine = 0;
	CheckPointFlag = 0;
	TargetFlag = 0;
	TRIGGER_PIN = 5;
	ECHO_PIN = 19;
	interrupt_flag = false;
} //Ultrasonic

// default destructor
Ultrasonic::~Ultrasonic()
{
} //~Ultrasonic


void Ultrasonic::initialize()
{
	// Set echo pin as input
	pinMode(ECHO_PIN, INPUT);
	// set trigger pin as output
	pinMode(TRIGGER_PIN, OUTPUT);
	
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	pinMode(18, INPUT);
	pinMode(19, INPUT);
	
	pinMode(5, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(22, OUTPUT);
	pinMode(17, OUTPUT);
	
	
	
	// Set the address pins as output
	pinMode(ADDR0_PIN, OUTPUT);
	pinMode(ADDR1_PIN, OUTPUT);
	pinMode(ADDR2_PIN, OUTPUT);
	
	// Set these pin to LOW to begin with
	digitalWrite(ADDR0_PIN, LOW);
	digitalWrite(ADDR1_PIN, LOW);
	digitalWrite(ADDR2_PIN, LOW);
	digitalWrite(TRIGGER_PIN, LOW);
	
	Select = 0;
	
}



void Ultrasonic::interrupt()
{
	// If it just went from lOW to HIGH, then this is the beginning of the echo cycle
	if (digitalRead(ECHO_PIN) == 1) 
	{
		PreviousTime = micros();
	}
	
	// If it just went from HIGH to LOW, then this is the end of the echo cycle
	else if (digitalRead(ECHO_PIN) == 0)
	{
		InterruptEchoTime = micros() - PreviousTime;
		//Increment the select variable to even
		interrupt_flag = false;
		//Select++;
	}
	
	
}

void Ultrasonic::trigger()
{
	// According to the data sheet, we need to set trigger to high for 10 us
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(10);
	digitalWrite(TRIGGER_PIN,LOW);
}

void Ultrasonic::addressSelect(int Address)
{
	switch (Address)
	{
		/*
		
		case 0:
		pinSelect(0);
		break;
		case 2:
		pinSelect(1);
		break;
		case 4:
		pinSelect(2);
		break;
		case 6:
		pinSelect(3);
		break;
		case 8:
		pinSelect(4);
		break;
		case 10:
		pinSelect(5);
		break;
		*/
		
		
		case 2: //right 1
		
		pinSelect(5, 19); //send pin number for the first righthand sensor to pinSelect
		break;
		case 4: //right 2
		pinSelect(17, 18);//send pin number for the second righthand sensor to pinSelect
		break;
		case 6: //back
		pinSelect(22, 2); //send pin number for the back sensor to pinSelect
		break;
		case 8: //front
		pinSelect(4, 3); //send pin number for the front sensor to pinSelect
		break;
		
		
		
	}
}

void Ultrasonic::pinSelect (int TRIGGER_Address, int ECHO_Address)
{
//void Ultrasonic::pinSelect (int Address)
//{
	pinMode(ECHO_PIN, OUTPUT);
	TRIGGER_PIN = TRIGGER_Address; //Set trigger variable to correct pin number
	ECHO_PIN = ECHO_Address; //Set echo variable to echo pin number
	pinMode(ECHO_PIN, INPUT);

	
/*
	// Set the least sig. digit
	digitalWrite(ADDR0_PIN, (Address & 0x01));			// & is to mask the bits
	// Set the middle digit
	digitalWrite(ADDR1_PIN, (Address & 0x02));
	// Set the most sig. digit
	digitalWrite(ADDR2_PIN, (Address & 0x04));
	
	*/
}

// So this is like an even/odd state machine. "Select" will start off at 0. It will send an ultrasonic pulse and then increment to odd.
// Once the signal is received in the interrupt routine then "Select" will be incremented to even. At which we are suppose to go an read
// the value that we just got. 
void Ultrasonic::spinOnce(char Face)
{
	static unsigned long PreviousSpinTime = 0;
	
	// This allows for a little bit of a delay between triggers so that there's no interference 
	if ((millis() - PreviousSpinTime) >= SPIN_PERIOD)
	{




		PreviousSpinTime = millis();
		//if ((Select == 0) || (Select == 2) || (Select == 4) || (Select == 6) || (Select == 8))
		if (interrupt_flag == false)
		{
			// Convert from us to cm
			int Temp = InterruptEchoTime / 58;
			// Storing the value that that we got
			switch (Select)
			{
				case 2:
					if ((Temp >= 0) && (Temp <= MAX_DISTANCE))
						EchoDistance[0] = Temp;
					break;
				case 4:
					if ((Temp >= 0) && (Temp <= MAX_DISTANCE))
						EchoDistance[1] = Temp;
					// Front sensor have a "Select" value of 6 so we need to skip the back sensor.
					if (Face == FRONT)
						Select = 6;//Select += 2;
					break;
				case 6:
					if ((Temp >= 0) && (Temp <= MAX_DISTANCE))
						EchoDistance[2] = Temp;
						Select = 1;
						FullSet++;
					break;
				case 8:
					if ((Temp >= 0) && (Temp <= MAX_DISTANCE))
						EchoDistance[3] = Temp;
					Select = 1;
					FullSet++;
					break;
			}
			
			Select++;
			// Set the address pins to the correct address
			addressSelect(Select);
			// Set the trigger pin to HIGH for 10 us
			interrupt_flag = true;
			trigger();
			// Increment to odd. The interrupt routine will increment it to even.
			
		}
	}
}




void Ultrasonic::checkPoint(char Face, int XTarget, int YTarget)
{
	static unsigned long PreviousCheckpointTime = 0;
	static unsigned long PreviousMotorTime = 0;
	static int XParallelFlag = 0;
	static int XAtTargetFlag = 0;
	static int YAtTargetFlag = 0;
	// Acquire new data
	spinOnce(Face);
	
	// This allows for a delay period before the telling the motor what to do again
	// Sending motor commands at too fast of a speed can effect the function of the motor
	if ((millis() - PreviousCheckpointTime) >= CHECKPOINT_PERIOD)
	{
		PreviousCheckpointTime = millis();
		CheckPointFlag = 0;
		// If we have finally collect the first set of the ultrasonic data
		int XCurrent1 = EchoDistance[0] + X_OFFSET;
		int XCurrent2 = EchoDistance[1] + X_OFFSET;
		int YCurrent  = EchoDistance[Face ? 3:2] + Y_OFFSET;
		//{XCurrent1},{XCurrent2},{YCurrent},{StateMachine},{XTarget}, {YTarget}
		if ((XCurrent1 != X_OFFSET) && (XCurrent2 != X_OFFSET) && (YCurrent != Y_OFFSET))
		{
			XParallelFlag = ((XCurrent1 - XCurrent2) <= X_TOLERANCE) && ((XCurrent1 - XCurrent2) >= -X_TOLERANCE);
			XAtTargetFlag = (XCurrent1 >= (XTarget - X_TOLERANCE)) && (XCurrent1 <= (XTarget + X_TOLERANCE));
			YAtTargetFlag = (YCurrent >= (YTarget - Y_TOLERANCE)) && (YCurrent <= (YTarget + Y_TOLERANCE));
			
			// Making sure both sides are parallel
			if (StateMachine == 0)
			{
				// If the difference of the 2 right sensors are greater than the tolerance
				//if ( ((XCurrent1 - XCurrent2) > X_TOLERANCE) || ((XCurrent1 - XCurrent2) < -X_TOLERANCE) )
				if (!XParallelFlag)
				{
					if ((millis() - PreviousMotorTime) >= MOTOR_PERIOD)
					{
						PreviousMotorTime = millis();
						// Determining if robot needs to rotate CW or CCW
						if (XCurrent1 > XCurrent2)
						{
							Robot.cw(SlowSpeed);
						}
						else
						{
							Robot.ccw(SlowSpeed);
						}
					}
				}
				// Stop the robot and move to the next state if the robot parallel
				else {
					if ((millis() - PreviousMotorTime) >= MOTOR_PERIOD)
					{
						PreviousMotorTime = millis();
						//Robot.stop();
						StateMachine++;
					}
				}
			}
		
			// Making sure the robot reaches the x-axis target
			else if (StateMachine == 1)
			{
				// If the Front right sensor is greater than the X-Target
				// We are only using one of the sensor to check for distance
				if ((XCurrent1 < (XTarget - X_TOLERANCE)) || (XCurrent1 > (XTarget + X_TOLERANCE)))
				{
					if ((millis() - PreviousMotorTime) >= MOTOR_PERIOD)
					{
						PreviousMotorTime = millis();
						// Determining if the robot needs to strafe left or right
						if (XCurrent1 < 10) 
						{
							Robot.right(SlowSpeed);
						}
						else 
						{
							if (XCurrent1 < (XTarget - X_TOLERANCE))
							{
								Robot.left(SlowSpeed);
							}
							else
							{
								Robot.right(SlowSpeed);
							}
						}
					}
				}
			
				// The robot is at the x-axis target
				else
				{
					// Determine if the robot is at the x-axis target && is parallel to the wall
					//if ((XCurrent1 >= (XTarget - X_TOLERANCE)) && (XCurrent1 <= (XTarget + X_TOLERANCE))
																	//&&
						//(XCurrent2 >= (XTarget - X_TOLERANCE)) && (XCurrent2 <= (XTarget + X_TOLERANCE)))
					if ( XParallelFlag && XAtTargetFlag)
					{
						if ((millis() - PreviousMotorTime) >= MOTOR_PERIOD)
						{
							PreviousMotorTime = millis();
							StateMachine++;
						}
					}
					// else the robot needs to go back to state 0 and try to get it self parallel to the wall
					else
					{
						if ((millis() - PreviousMotorTime) >= MOTOR_PERIOD)
						{
							PreviousMotorTime = millis();
							StateMachine--;
						}
					}
				}
			}
		
			// Making sure the robot reaches the y-axis
			else if (StateMachine == 2)
			{
				// Making sure the back sensor is outside the target value
				if ((YCurrent < (YTarget - Y_TOLERANCE)) || (YCurrent > (YTarget + Y_TOLERANCE)))
				{
					if ((millis() - PreviousMotorTime) >= MOTOR_PERIOD)
					{
						PreviousMotorTime = millis();
						if (Face == BACK)
						{
							// Deciding if the robot needs to move forward or backward
							if (YCurrent < (YTarget - Y_TOLERANCE))
							{
								Robot.forward(SlowSpeed);
							}
							else
							{
								Robot.backward(SlowSpeed);
							}
						}
						else {
							// Deciding if the robot needs to move forward or backward
							if (YCurrent < (YTarget - Y_TOLERANCE))
							{
								Robot.backward(SlowSpeed);
							}
							else
							{
								Robot.forward(SlowSpeed);
							}
						}	
					}
					
				}
				// The robot is at the y-axis target
				else
				{
					// Determining if the robot is (parallel to the wall) && (at the x-axis target) && (at the y-axis target)
					//if (
					//(XCurrent1 >= (XTarget - X_TOLERANCE)) &&
					//(XCurrent1 <= (XTarget + X_TOLERANCE)) &&
					//(XCurrent2 >= (XTarget - X_TOLERANCE)) &&
					//(XCurrent2 <= (XTarget + X_TOLERANCE)) &&
					//(YCurrent >= (XTarget - X_TOLERANCE)) &&
					//(YCurrent <= (XTarget + X_TOLERANCE))   )
					if (XAtTargetFlag && XParallelFlag & YAtTargetFlag)
					{
						//if ((millis() - PreviousMotorTime) >= MOTOR_PERIOD)
						//{
							PreviousMotorTime = millis();
							//stop the robot and go to the next state
							Robot.stop();
							digitalWrite(13,HIGH);
							delay(200);
							encoderClear();
							XCurrent1 = 0;
							XCurrent2 = 0;
							YCurrent = 0;
							StateMachine ++;
							CheckPointFlag = 1;
						//}
					}
					// else the robot needs to go back to state 0 and try to get itself parallel to the wall
					else
					{
						if ((millis() - PreviousMotorTime) >= MOTOR_PERIOD)
						{
							PreviousMotorTime = millis();
							StateMachine = 0;
						}
					}
				}
			}
		}
	}
}

void Ultrasonic::encoderClear()
{
	int Temp1, Temp2;
	Robot.ReadOne(0x02,&Temp1,&Temp2);
	Robot.ReadOne(0x04,&Temp1,&Temp2);
	Robot.ReadOne(0x06,&Temp1,&Temp2);
	Robot.ReadOne(0x08,&Temp1,&Temp2);
}


