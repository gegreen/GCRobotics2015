/*
 * Stepper.h
 *
 * Created: 3/3/2015 3:21:22 PM
 *  Author: Grace Green
 */ 


#ifndef STEPPER_H_
#define STEPPER_H_

#define pwm 9
#define brk1 7
#define dir1 12
#define brk2 8
#define dir2 13
#define CW 1
#define CCW 0
//int rbrk1, rbrk2, rdir1, rdir2;

//#define secDelay 9    //delay time = 10 ms
#define overDelay 1    //delay time for 100% duty cycle for 1 ms
//#define pduty 255 // 100% for lift motor
//#define pduty 150 // % for arm motor

class Stepper
{
	public://variables
	int pduty;
	int secDelay;
	
	public://functions
	Stepper(int,int);
	void Open();
	void Close();
	private:
	void StepCycle(int steps, int direction);
};




#endif /* STEPPER_H_ */