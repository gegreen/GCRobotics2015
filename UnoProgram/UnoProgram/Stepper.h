/*
 * Stepper.h
 *
 * Created: 3/3/2015 3:21:22 PM
 *  Author: Victory
 */ 


#ifndef STEPPER_H_
#define STEPPER_H_

/*
#define pwm 9
#define brk1 6
#define dir1 4
#define brk2 5
#define dir2 7
#define CW 0
#define CCW 1
#define Startspeed 100
#define wide 10
#define open 30
*/

#define pwm 10
#define brk1 6
#define dir1 12
#define brk2 7
#define dir2 13
#define CW 0
#define CCW 1
#define Startspeed 100
#define wide 10
#define open 30


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
	void Wider();
	private:
	void StepCycle(int steps, int direction, int percent, int startsteps);
};




#endif /* STEPPER_H_ */