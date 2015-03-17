/*
 * Stepper.cpp
 *
 * Created: 3/3/2015 3:21:48 PM
 *  Author: Grace Green
 */ 
#include <Arduino.h>
#include "Stepper.h"


Stepper::Stepper(int pwm_percent, int Delay)
{
	analogWrite(pwm, 0);
	pduty = pwm_percent;
	secDelay = Delay;
	WDTCSR = 0x00;  //watchdog timer is disabled
	
	TCCR1B = TCCR1B & B11111000 | B00000001;
	
	pinMode(pwm, OUTPUT);
	pinMode(brk1, OUTPUT);
	pinMode(dir1, OUTPUT);
	pinMode(brk2, OUTPUT);
	pinMode(dir2, OUTPUT);
}


void Stepper::Open()
{
	StepCycle(1000, CCW);
}

void Stepper::Close()
{
	StepCycle(1000, CW);
}
void Stepper::StepCycle(int steps, int direction)
{
	int Count;

	for(int i = 0; i<steps; i++)
	{
		if (direction == CW)
			Count = 7 - i%7;
		else if (direction == CCW)
			Count = i%7;
		switch (Count)
		{
			case 0: 
			analogWrite(pwm, 254);             //makes pwm pin high with a 100% duty cycle
			delay(overDelay);               // wait for 1 ms
	
			analogWrite(pwm, pduty);  //makes pwm pin high with a 20% duty cycle
	
			digitalWrite(brk1, LOW);
			digitalWrite(dir1, HIGH);
			digitalWrite(brk2, HIGH);
			digitalWrite(dir2, HIGH);
		
			delay(secDelay);               // wait for 10 ms
			break;
			
			case 1:
			analogWrite(pwm, 254);             //makes pwm pin high with a 100% duty cycle
			delay(overDelay);               // wait for 1 ms
	
			analogWrite(pwm, pduty);  //makes pwm pin high with a 20% duty cycle
		
			digitalWrite(brk1, LOW);
			digitalWrite(dir1, HIGH);
			digitalWrite(brk2, LOW);
			digitalWrite(dir2, HIGH);
			delay(secDelay);               // wait for 10 ms
			break;
			
			case 2:
			analogWrite(pwm, 254);             //makes pwm pin high with a 100% duty cycle
			delay(overDelay);               // wait for 1 ms
		
			analogWrite(pwm, pduty);  //makes pwm pin high with a 20% duty cycle
		
			digitalWrite(brk1, HIGH);
			digitalWrite(dir1, HIGH);
			digitalWrite(brk2, LOW);
			digitalWrite(dir2, HIGH);
			delay(secDelay);               // wait for 10 ms
			break;
			
			case 3:		
			analogWrite(pwm, 254);             //makes pwm pin high with a 100% duty cycle
			delay(overDelay);               // wait for 1 ms
		
			analogWrite(pwm, pduty);  //makes pwm pin high with a 20% duty cycle
			
			digitalWrite(brk1, LOW);
			digitalWrite(dir1, LOW);
			digitalWrite(brk2, LOW);
			digitalWrite(dir2, HIGH);
			delay(secDelay);               // wait for 10 ms
			break;
			
			case4:
			analogWrite(pwm, 254);             //makes pwm pin high with a 100% duty cycle
			delay(overDelay);               // wait for 1 ms
		
			analogWrite(pwm, pduty);  //makes pwm pin high with a 20% duty cycle
		
			digitalWrite(brk1, LOW);
			digitalWrite(dir1, LOW);
			digitalWrite(brk2, HIGH);
			digitalWrite(dir2, HIGH);
			delay(secDelay);               // wait for 10 ms
			break;		
		
			case 5:
			analogWrite(pwm, 254);             //makes pwm pin high with a 100% duty cycle
			delay(overDelay);               // wait for 1 ms
		
			analogWrite(pwm, pduty);  //makes pwm pin high with a 20% duty cycle
		
			digitalWrite(brk1, LOW);
			digitalWrite(dir1, LOW);
			digitalWrite(brk2, LOW);
			digitalWrite(dir2, LOW);
			delay(secDelay);               // wait for 10 ms
			break;
			
			case 6:
			analogWrite(pwm, 254);             //makes pwm pin high with a 100% duty cycle
			delay(overDelay);               // wait for 1 ms
	
			analogWrite(pwm, pduty);  //makes pwm pin high with a 20% duty cycle
		
			digitalWrite(brk1, HIGH);
			digitalWrite(dir1, HIGH);
			digitalWrite(brk2, LOW);
			digitalWrite(dir2, LOW);
			delay(secDelay);               // wait for 10 ms
			break;
			
			case 7:
			analogWrite(pwm, 254);             //makes pwm pin high with a 100% duty cycle
			delay(overDelay);               // wait for 1 ms
		
			analogWrite(pwm, pduty);  //makes pwm pin high with a 20% duty cycle
		
			digitalWrite(brk1, LOW);
			digitalWrite(dir1, HIGH);
			digitalWrite(brk2, LOW);
			digitalWrite(dir2, LOW);
			delay(secDelay);
			break;
		}
		analogWrite(pwm, 0);
	}
}
