/*
 * MegaProject.ino
 *
 * Created: 3/5/2015 12:27:01 PM
 * Author: Victory
 */ 

#include "Ultrasonic.h"
#include <Wire.h>
#include "Motors.h"
#include "Lift.h"
#define Button 49
#define INpin 51
#define OUTpin 53
#define Xplate 61
#define Closed 40
//#define Serial_black 20
//#define Serial_white 21
	
	
/********************************************************
*          Constants for Ultrasonic Sensors
*********************************************************/


// Target Variables
int SensorTargetRight = 30, SensorTargetBack = 40;
int StateMachine = 0;

int YTarget = 0;
int XTarget = 0;
int WTarget = 0;
int TargetFlag = 0;
int LiftAck = 0;
int X, Y;
int direction = 0;


// Create instance of the class Ultrasonic
Ultrasonic Ultra;
// Create instance of the class Motors
Motors RobotMove;
Lift Master;


/*********************************************************
*
*				Setup Loop
*
**********************************************************/
void setup()
{
	Wire.begin();
	// Attach int.0 interrupt at pin 2
	attachInterrupt(4,ultraInterrupt,CHANGE);
	attachInterrupt(1,ultraInterrupt,CHANGE);
	attachInterrupt(0,ultraInterrupt,CHANGE);
	attachInterrupt(5,ultraInterrupt,CHANGE);
	Ultra.initialize();
	// initialize for master
	Master.MasterInitialize();
	Master.MasterSetLevel(1);
	Master.MasterRequestStateChange();
	pinMode(Button,INPUT);
	pinMode(13,OUTPUT);
	digitalWrite(13, LOW);
	pinMode(INpin, INPUT);
	pinMode(OUTpin, OUTPUT);
	digitalWrite(OUTpin, LOW);
	pinMode(Closed, INPUT);
	//ForwardTarget = 305;
	//XTarget = 1124;
	//YTarget = 1914;
	//WTarget = 2362;
	//Ultra.Select = 0;

	delay(6000);
	Master.MasterRequestStateChange();
}

/*********************************************************
*
*				Infinite Loop
*
**********************************************************/
void loop()
{	
 //Testing RobotMove routine
 //move forward 1 foot and then turn 180*
 
 /*
	if (StateMachine == 0)
	{
		if (TargetFlag == 0){
			TargetFlag = RobotMove.moveCCW(&WTarget);
		}
		if (TargetFlag == 1){
			TargetFlag = 0;
			StateMachine++;
			delay(20);
		}
	}
	else
	{
		RobotMove.stop();
		delay(300);
	}
 */
 
 
// Working Statemachine
	//int temp = StateMachine;
	// Code for the break message:
	// {Ultra.EchoDistance[0]},{Ultra.EchoDistance[1]}, {Ultra.EchoDistance[2]},{Ultra.EchoDistance[3]},{Ultra.EchoDistance[4]},{Ultra.EchoDistance[5]}
	//Ultra.spinOnce(BACK);
	
	// First state = get off the starting block by moving to (52, 65)
		// X = 52 pretty much line the robot up with the plate
	__asm__("nop\n\t");
	//switch (StateMachine) 
	//{
		//case 0:
			//if (Master.MasterSpinOnce() == 1)
			//{
				//Master.MasterRequestStateChange();
				//StateMachine++;
			//}
				//break;
		//case 1:
			//if (Master.MasterSpinOnce() == 1)
				//StateMachine++;
				//break;
		//default:
			//delay(100);
			//break;
	//}
	
	switch (StateMachine)
	{
		case 0:
			//wait for the button
			if (!digitalRead(Button))
			StateMachine++;
			break;
		//Start at [(52,65)]
		case 1:
			//get ready
			Ultra.checkPoint(BACK,Xplate,65);

			if (Ultra.CheckPointFlag == 1)
			{
				StateMachine++;
				Ultra.StateMachine = 0;
				YTarget = 2500;//YTarget = 2826;
			}

			break;
		// Go to [52,180] by moving 90 cm forward
		case 2://cross the room
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				StateMachine++;
				TargetFlag = 0;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
			}
			break;
		
		case 3:
			//line up
			// Compensating for the irregular back of the fridge
			Ultra.checkPoint(FRONT,Xplate,72);
			if (Ultra.CheckPointFlag == 1){
				Ultra.StateMachine = 0;
				StateMachine++;
				YTarget = 314;
				digitalWrite(OUTpin, HIGH);
			}
			break;
		case 4: //raise the lift
			if (digitalRead(INpin) == HIGH)
			{
				digitalWrite(OUTpin, LOW);
				StateMachine++;
				YTarget = 2512;
			}
			break;
		case 5:
			if (digitalRead(INpin) == LOW)
			StateMachine++;
			break;
		// this is to ensure that the robot waits till the lift is done
		case 6: //line up with plate
			//Ultra.checkPoint(FRONT, 52,65);
			// Compensating for the irregular back of the fridge
			Ultra.checkPoint(FRONT,Xplate,50);
			if (Ultra.CheckPointFlag == 1){
				Ultra.StateMachine = 0;
				X = Xplate;
				//Y = 180;
				StateMachine++;
				digitalWrite(OUTpin, HIGH);
			}
			break;
		case 7:// lower to shelf and grab plate
			if (digitalRead(INpin) == HIGH)
			{
				digitalWrite(OUTpin, LOW);
				StateMachine++;
			}
			break;
		case 8:
			if (digitalRead(INpin) == LOW)
			StateMachine++;
			break;
		case 9:
			if (!digitalRead(Closed))
			{
				/*
				if (TargetFlag == 0){
					if (XPosition <= (Xplate + plateLimit))
					TargetFlag = RobotMove.moveRight(&XTarget);
					else if (XPosition >= (Xplate - plateLimit))
					TargetFlag = RobotMove.moveRight(&XTarget);
					
				}
				if (TargetFlag == 1){
					XTarget = 1;
					TargetFlag = 0;
					RobotMove.stop();
					delay(200);
				}
				*/
				
				Ultra.PlateFinder(BACK,X,51);
				if (Ultra.CheckPointFlag == 1)
				{
					Ultra.StateMachine = 0;
					if (!direction)
					{
						if (X <= (Xplate + plateLimit))
						X++;
						else 
						direction = 1;
					}
					else if (direction)
					{
						if (X >= (Xplate - plateLimit))
						X--;
						else
						direction = 0;
					}
					//X = (Ultra.PlateFinder(X, Y, Xplate));
				}
				
			}
			else if (digitalRead(Closed))
			{
				digitalWrite(OUTpin, HIGH);
				StateMachine++;
				
			}
			break;
			case 10:
			if (digitalRead(INpin) == HIGH)
			{
				digitalWrite(OUTpin, LOW);
				StateMachine++;
			}
			break;
			case 11:
			if (digitalRead(INpin) == LOW)
			StateMachine++;
			break;
		case 12:
		Ultra.checkPoint(FRONT, Xplate, 51);
		if (Ultra.CheckPointFlag == 1)
		{
			Ultra.StateMachine = 0;
			StateMachine++;
			
		}
		break;
		// Move to [52,100] by moving backward 80 cm
		case 13: // move to table
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveBackward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				WTarget = 1100;//1181;
			}
			break;
		
		// Rotate 90* CCW
		case 14:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveCCW(&WTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				YTarget = 3500//4019;
			}
			break;
		//Move to [180,100] by moving forward by 128 cm 
		case 15:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				WTarget = 1100;//1181;
			}
			break;
		// Rotate 90* CCW
		case 16:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveCCW(&WTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				YTarget = 785;
			}
			break;
		
		// Check point at [180,75] by (65,70)
		case 17://line up with table
			Ultra.checkPoint(FRONT,65,65);
			if (Ultra.CheckPointFlag == 1){
				Ultra.StateMachine = 0;
				StateMachine++;
				YTarget = 314;
				digitalWrite(OUTpin, HIGH);
			}
			break;
			
		case 18:// drop the plate
			if (digitalRead(INpin) == HIGH)
			{
				digitalWrite(OUTpin, LOW);
				StateMachine++;
				YTarget = 314;
			}
			break;
		case 19:
			if (digitalRead(INpin) == LOW)
			StateMachine++;
			break;
			
		case 20://move to start position
		
		if (TargetFlag == 0){
			TargetFlag = RobotMove.moveBackward(&YTarget);
		}
		if (TargetFlag == 1)
		{
			TargetFlag = 0;
			StateMachine++;
			RobotMove.stop();
			delay(200);
			digitalWrite(13, LOW);
			WTarget = 1100;//1181;
		}
		break;
		
		case 21:
		
		if (TargetFlag == 0)
		{
			TargetFlag = RobotMove.moveCCW(&WTarget);
		}
		if (TargetFlag == 1)
		{
			TargetFlag = 0;
			StateMachine++;
			RobotMove.stop();
			delay(200);
			digitalWrite(13, LOW);
			YTarget = 2700;//2900;
		}
		break;
		case 22:
		
		if (TargetFlag == 0)
		{
			TargetFlag = RobotMove.moveForward(&YTarget);
		}
		if (TargetFlag == 1)
		{
			StateMachine++;
			TargetFlag = 0;
			RobotMove.stop();
			delay(200);
			digitalWrite(13, LOW);
			WTarget = 1000;
		}
		break;
		case 23:
		
		if (TargetFlag == 0){
			TargetFlag = RobotMove.moveCCW(&WTarget);
		}
		if (TargetFlag == 1)
		{
			TargetFlag = 0;
			StateMachine++;
			RobotMove.stop();
			delay(200);
			digitalWrite(13, LOW);
			YTarget = 314;
		}
		break;
		case 24:
		
		if (TargetFlag == 0){
			TargetFlag = RobotMove.moveBackward(&YTarget);
		}
		if (TargetFlag == 1)
		{
			TargetFlag = 0;
			StateMachine++;
			RobotMove.stop();
			delay(200);
			digitalWrite(13, LOW);
		}
		break;
		case 25: //lower to start position
		Ultra.checkPoint(BACK,Xplate,65);

		if (Ultra.CheckPointFlag == 1)
		{
			StateMachine++;
			Ultra.StateMachine = 0;
			digitalWrite(OUTpin, HIGH);
		}
		break;
		case 26:
		if (digitalRead(INpin) == HIGH)
		{
			digitalWrite(OUTpin, LOW);
			StateMachine++;
		}
		break;
		case 27:
		if (digitalRead(INpin) == LOW)
		StateMachine = 0;
		break;
		
		default:
			RobotMove.stop();
			delay(40);
			break;
	}
	
}

/*********************************************************
*
*				Ultrasonic Interrupt Routine
*
**********************************************************/
void ultraInterrupt()
{
	Ultra.interrupt();
}

