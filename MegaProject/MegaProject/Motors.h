/* 
* Motors.h
*
* Created: 3/16/2014 12:16:09 AM
* Author: Quang
*/


#ifndef __MOTORS_H__
#define __MOTORS_H__
// Pulses per rev = 546
// Wheel radius = 27 mm
// wheel_circumference = 2 * PI * wheel_radius = 169.646
//wheel_circumference / cpr = 0.3107
// K = 1.015 ----> 0.315367
#define Y_MM_DISTANCE_PER_COUNT 0.3107			//Unit = mm/count	


#define Front_Left   0x08
#define Front_Right  0x02
#define Back_Right   0x04
#define Back_Left    0x06

#define Forward      0x00
#define Backward     0x01

#define SlowSpeed    20
#define MediumSpeed  50
#define FastSpeed    70
#define Stop         00

#define SAMPLING_PERIOD 5

class Motors
{
//functions
public:
	Motors();
	~Motors();
	
	unsigned long PreviousTime;
	
	void forward(int RobotSpeed);
	void backward(int RobotSpeed);
	void left(int RobotSpeed);
	void right(int RobotSpeed);
	void cw(int RobotSpeed);
	void ccw(int RobotSpeed);
	void stop();
	void i2cWrite ( char Address, char Speed , char Direction);
	
	int moveForward(int *Target);		//Target in mm
	int moveBackward(int *Target);		//Target in mm
	int moveRight(int *Target);		//Target in mm
	int moveLeft(int *Target);		//Target in mm
	int moveCW(int *Target);		//Target in mm
	int moveCCW(int *Target);		//Target in mm
	int average();
	void ReadOne(char address, int *Odometry, int *Direction);
	char FirstTime;
	

}; //Motors

#endif //__MOTORS_H__
