/*
 * StepperTest.ino
 *
 * Created: 3/12/2015 2:58:51 PM
 * Author: Victory
 */ 

#include <avr/wdt.h>

#define pwm 10
#define brk1 6
#define dir1 12
#define brk2 7
#define dir2 13
#define CW 1
#define CCW 0
#define startsteps 4
#define Startspeed 100
//int rbrk1, rbrk2, rdir1, rdir2;

#define Delay 20    //delay time = ms
#define pduty 255 // % for arm motor

int steps = 32;
int direction;
int Count;
int secDelay;
int Decrement;

// the setup routine runs once when you press reset:
void setup() {

	WDTCSR = 0x00;  //watchdog timer is disabled
	
	TCCR1B = TCCR1B & B11111000 | B00000001;
	
	pinMode(pwm, OUTPUT);
	pinMode(brk1, OUTPUT);
	pinMode(dir1, OUTPUT);
	pinMode(brk2, OUTPUT);
	pinMode(dir2, OUTPUT);
	digitalWrite(brk1, LOW);
	digitalWrite(brk2, LOW);
	analogWrite(pwm, pduty);  //makes pwm pin high with a 20% duty cycle
	Decrement = (Startspeed - Delay)/startsteps;
	secDelay = Startspeed;
	direction = CCW;
}

// the loop routine runs over and over again forever:
void loop() {
	secDelay = 50;//Startspeed;
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
		
		/*if (i < startsteps)
		secDelay = secDelay - Decrement;
		else
		secDelay = Delay;*/
	}
	
	/*delay(2000);
	if (direction == CW)
	direction = CCW;
	else 
	direction = CW;*/
}
