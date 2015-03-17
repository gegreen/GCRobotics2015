/*
 * Stepper.h
 *
 * Created: 3/3/2015 3:21:22 PM
 *  Author: Victory
 */ 


#ifndef STEPPER_H_
#define STEPPER_H_

#define pwm 9
#define brk1 6
#define dir1 4
#define brk2 5
#define dir2 7
#define CW 1
#define CCW 0
#define Startspeed 100

class Stepper
{
	public://variables
	int pduty;
	int secDelay;
	int Decrement;
	int Delay;
	
	public://functions
	Stepper(int,int);
	void Open();
	void Close();
	void Hold();
	void Start();
	private:
	void StepCycle(int steps, int direction, int percent, int startsteps);
};




#endif /* STEPPER_H_ */