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
#define Button 52
#define INpin 51
#define OUTpin 53
	
	
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
			
			if (!digitalRead(Button))
			StateMachine++;
			break;
		//Start at [(52,65)]
		case 1:

			Ultra.checkPoint(BACK,52,65);

			if (Ultra.CheckPointFlag == 1)
			{
				StateMachine++;
				Ultra.StateMachine = 0;
				YTarget = 2826;
			}

			break;
		// Go to [52,180] by moving 90 cm forward
		case 2:
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
		// Check point at [52,150] by (52,65)  //I changed this so you need to reevaluate the points from now on
		case 3:
			//Ultra.checkPoint(FRONT, 52,65);
			// Compensating for the irregular back of the fridge
			Ultra.checkPoint(FRONT,52,80);
			if (Ultra.CheckPointFlag == 1){
				Ultra.StateMachine = 0;
				StateMachine++;
				YTarget = 314;
				digitalWrite(OUTpin, HIGH);
			}
			break;
		case 4:
			if (digitalRead(INpin) == HIGH)
			{
				digitalWrite(OUTpin, LOW);
				StateMachine++;
				YTarget = 2512;
			}
			break;
		// Move to [52,190] by moving forward by 10 cm 
		/*case 4:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				YTarget = 2512;
				
				Master.MasterRequestStateChange();
			}
			break;
			*/
		case 5:
			if (digitalRead(INpin) == LOW)
			StateMachine++;
			break;
		// this is to ensure that the robot waits till the lift is done
		case 6:
			//Ultra.checkPoint(FRONT, 52,65);
			// Compensating for the irregular back of the fridge
			Ultra.checkPoint(FRONT,52,50);
			if (Ultra.CheckPointFlag == 1){
				Ultra.StateMachine = 0;
				StateMachine++;
				digitalWrite(OUTpin, HIGH);
			}
			break;
		case 7:
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
		// Move to [52,100] by moving backward 80 cm
		case 9:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveBackward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				WTarget = 1181;
			}
			break;
		
		// Rotate 90* CCW
		case 10:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveCCW(&WTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				YTarget = 4019;
			}
			break;
		//Move to [180,100] by moving forward by 128 cm 
		case 11:
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				WTarget = 1181;
			}
			break;
		// Rotate 90* CCW
		case 12:
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
		case 13:
			Ultra.checkPoint(FRONT,65,65);
			if (Ultra.CheckPointFlag == 1){
				Ultra.StateMachine = 0;
				StateMachine++;
				YTarget = 314;
				digitalWrite(OUTpin, HIGH);
			}
			break;
		//Move to [180,65] by moving forward by 10 cm
		case 14:/*
			if (TargetFlag == 0){
				TargetFlag = RobotMove.moveForward(&YTarget);
			}
			if (TargetFlag == 1){
				TargetFlag = 0;
				StateMachine++;
				RobotMove.stop();
				delay(200);
				digitalWrite(13, LOW);
				digitalWrite(OUTpin, HIGH);
			}*/
		StateMachine++;
			break;
			
		case 15:
			if (digitalRead(INpin) == HIGH)
			{
				digitalWrite(OUTpin, LOW);
				StateMachine++;
				YTarget = 314;
			}
			break;
		case 16:
			if (digitalRead(INpin) == LOW)
			StateMachine++;
			break;
			
		case 17:
		
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
			WTarget = 1181;
		}
		break;
		
		case 18:
		
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
			YTarget = 2900;
		}
		break;
		case 19:
		
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
		case 20:
		
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
		}
		break;
		case 21:
		Ultra.checkPoint(BACK,52,65);

		if (Ultra.CheckPointFlag == 1)
		{
			StateMachine++;
			Ultra.StateMachine = 0;
			StateMachine = 0;
		}
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

