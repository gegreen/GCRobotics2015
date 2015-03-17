/*
 * SteppertestBasicCW.ino
 *
 * Created: 3/13/2015 3:24:59 PM
 * Author: Victory
 */ 

#include <avr/wdt.h>

#define pwm 9
#define brk1 6
#define dir1 4
#define brk2 5
#define dir2 7

//int rbrk1, rbrk2, rdir1, rdir2;

#define secDelay 50    //delay time = ms
#define pduty 255 // % for arm motor


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
	
}

// the loop routine runs over and over again forever:
void loop() {
	
	digitalWrite(dir1, HIGH);
	digitalWrite(dir2, LOW);
	delay(secDelay);               // wait for 10 ms
	
	digitalWrite(dir1, LOW);
	digitalWrite(dir2, LOW);
	
	delay(secDelay);               // wait for 10 ms	
	
	digitalWrite(dir1, LOW);
	digitalWrite(dir2, HIGH);
	
	delay(secDelay);               // wait for 10 ms
	
	digitalWrite(dir1, HIGH);
	digitalWrite(dir2, HIGH);
	
	delay(secDelay);               // wait for 10 ms

}
