/*
 * Stepper.cpp
 *
 * Created: 3/3/2015 3:21:48 PM
 *  Author: Victory
 */ 
#include <Arduino.h>
#include "Stepper.h"


Stepper::Stepper(int pwm_percent, int D)
{
	WDTCSR = 0x00;  //watchdog timer is disabled
	
	TCCR1B = TCCR1B & B11111000 | B00000001;
	
	Delay = D;
	pduty = pwm_percent;
	pinMode(pwm, OUTPUT);
	pinMode(brk1, OUTPUT);
	pinMode(dir1, OUTPUT);
	pinMode(brk2, OUTPUT);
	pinMode(dir2, OUTPUT);
	digitalWrite(brk1, LOW);
	digitalWrite(brk2, LOW);
	secDelay = Startspeed;
	analogWrite(pwm, 0);
	

	
	
}


void Stepper::Open()
{
	StepCycle(44, CW, pduty, 8);
	delay(500);
}

void Stepper::Close()
{
	StepCycle(44, CCW, pduty, 8);
	delay(500);
}

void Stepper::Hold()
{
	StepCycle(8, CCW, pduty, 0);
}

void Stepper::Start()
{
	StepCycle(16, CCW, pduty, 16);
}

void Stepper::StepCycle(int steps, int direction, int percent, int startsteps)
{
	int Count;
	analogWrite(pwm, percent);  
	Decrement = (Startspeed - Delay)/startsteps;
	secDelay = Startspeed;
	for(int i = 0; i<steps; i++)
	{
		
		if (direction == CW)
		Count = 3 - i%4;
		else if (direction == CCW)
		Count = i%4;
		switch (Count)
		{
			
			case 0:
			digitalWrite(dir1, HIGH);
			digitalWrite(dir2, HIGH);
			
			delay(secDelay);               // wait for 10 ms
			break;
			
			case 1:
			digitalWrite(dir1, LOW);
			digitalWrite(dir2, HIGH);
			
			delay(secDelay);               // wait for 10 ms
			break;
			
			case 2:
			digitalWrite(dir1, LOW);
			digitalWrite(dir2, LOW);
			
			delay(secDelay);               // wait for 10 ms
			break;
			
			case 3:
			digitalWrite(dir1, HIGH);
			digitalWrite(dir2, LOW);
			delay(secDelay);               // wait for 10 ms
			break;
		}
		
		if (i < startsteps)
		secDelay = secDelay - Decrement;
	
		}
		analogWrite(pwm, 0);
	
}
