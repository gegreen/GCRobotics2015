/*
 * UnoProgram.ino
 *
 * Created: 3/5/2015 12:20:59 PM
 * Author: Grace Green
 */ 

#include "Stepper.h"


#define In1        4//Pin#
#define In2        5//Pin#
#define ChannelA   2 //Pin#
#define Interupt_num 0
#define EN1        10 //Pin#
#define MaxPWM 255 // Max PWM
#define UP 1
#define DOWN 0
#define Encoder_counts 1000
#define Percent_spin 50
#define Powerpin A5
#define Switch A4

#define inPin A0
#define outPin A1

int State;
volatile int numEncoder;
Stepper ArmMotor(150, 9); 

void setup()
{
	numEncoder = 0;
	State = 0;
	pinMode(outPin, OUTPUT);
	pinMode(inPin, INPUT);

	pinMode(Switch, INPUT);
	pinMode(In1,OUTPUT);
	pinMode(In2,OUTPUT);
	pinMode(EN1,OUTPUT);
	pinMode(ChannelA,INPUT);
	pinMode(Powerpin, OUTPUT);
	digitalWrite(Powerpin, HIGH);

	attachInterrupt(Interupt_num, incEncoder, RISING);

		// Prototypes
	void MotorSpin(long,int,int);
	void IncEncoder();
}

void loop()
{
		__asm__("nop\n\t");
	// Polls till an input is recieved from the Mega
	
	switch (State)
	{
		
		//waiting for input to come high //This is the Refridgerator pickup case
		case 0:
			if (digitalRead(inPin) == HIGH)
			{
				State++;
			}
			
			break;
		case 1:
			MotorSpin(Encoder_counts,Percent_spin, UP);//lift plate above shelf height
			digitalWrite(outPin,HIGH);
			State++;
			break;
		case 2:
			if (digitalRead(inPin) == LOW)
			{
				digitalWrite(outPin,LOW);
				State++;
			}
			break;
		case 3:
		if (digitalRead(inPin) == HIGH)
		{
			State++;
		}
		
		break;
		case 4:
		MotorSpin(0,Percent_spin, DOWN);//bring arms down to shelf 
		ArmMotor.Close();//grab the plate
		digitalWrite(outPin,HIGH);
		State++;
		break;
		case 5:
		if (digitalRead(inPin) == LOW)
		{
			digitalWrite(outPin,LOW);
			State++;
		}
		break;
		case 6:
		if (digitalRead(inPin) == HIGH)
		{
			State++;
		}
		
		break;
		case 7:
		MotorSpin(0,Percent_spin, DOWN);//lower plate to table
		ArmMotor.Open();//release plate
		digitalWrite(outPin,HIGH);
		State++;
		break;
		case 8:
		if (digitalRead(inPin) == LOW)
		{
			digitalWrite(outPin,LOW);
			State++;
		}
		break;
		case 9:
		if (digitalRead(inPin) == HIGH)
		{
			State++;
		}
		
		break;
		case 10:
		MotorSpin(Encoder_counts,Percent_spin, DOWN);//lower lift to start position
		digitalWrite(outPin,HIGH);
		State++;
		break;
		case 11:
		if (digitalRead(inPin) == LOW)
		{
			digitalWrite(outPin,LOW);
			State=0;
		}
		break;
	}
}
			


/* Syntax of the Function:
MotorSpin( long HowLongToSpin, int PercentSpin, int Direction);

HowLongToSpin is how many decoder bits you want to pass before stopping the motor.  This should be a large number (Long).

PercentSpin is how fast you want it to spin.  It should be passed in as a percentage, (0 - 100%).

Direction is which direction to spin the motor.  I'm not sure which direction is going to be up/down, so the passed in variable should be a 0 or a 1 depending on the direction

Make sure that the EN1 pin is PWM.  Pins 3,5,6, 9, 10, 11 can be configured for PWM output.

Default Frequencies:
Arduino Pins 5 and 6: 1kHz
Arduino Pins 9, 10, 11, and 3: 500Hz

If you want to change the Frequency (It should be fine as is):

//---------------------------------------------- Set PWM frequency for D5 & D6 -------------------------------
 
//TCCR0B = TCCR0B & B11111000 | B00000001;    // set timer 0 divisor to     1 for PWM frequency of 62500.00 Hz
//TCCR0B = TCCR0B & B11111000 | B00000010;    // set timer 0 divisor to     8 for PWM frequency of  7812.50 Hz
//TCCR0B = TCCR0B & B11111000 | B00000011;    // set timer 0 divisor to    64 for PWM frequency of   976.56 Hz (The DEFAULT)
//TCCR0B = TCCR0B & B11111000 | B00000100;    // set timer 0 divisor to   256 for PWM frequency of   244.14 Hz
//TCCR0B = TCCR0B & B11111000 | B00000101;    // set timer 0 divisor to  1024 for PWM frequency of    61.04 Hz
 
 
//---------------------------------------------- Set PWM frequency for D9 & D10 ------------------------------
 
//TCCR1B = TCCR1B & B11111000 | B00000001;    // set timer 1 divisor to     1 for PWM frequency of 31372.55 Hz
//TCCR1B = TCCR1B & B11111000 | B00000010;    // set timer 1 divisor to     8 for PWM frequency of  3921.16 Hz
//TCCR1B = TCCR1B & B11111000 | B00000011;    // set timer 1 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
//TCCR1B = TCCR1B & B11111000 | B00000100;    // set timer 1 divisor to   256 for PWM frequency of   122.55 Hz
//TCCR1B = TCCR1B & B11111000 | B00000101;    // set timer 1 divisor to  1024 for PWM frequency of    30.64 Hz
 
//---------------------------------------------- Set PWM frequency for D3 & D11 ------------------------------
 
//TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
//TCCR2B = TCCR2B & B11111000 | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz
//TCCR2B = TCCR2B & B11111000 | B00000011;    // set timer 2 divisor to    32 for PWM frequency of   980.39 Hz
//TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
//TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
//TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
//TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz
 

*/





// Interrupt
void incEncoder()
{
  numEncoder++;
}




// Function
void MotorSpin(long spinTime, int percentSpin, int dirSpin)
{
	int pwmSet;
  
  // This chooses the direction the motor will spin based on the passed in variable
   if (dirSpin == 1)
 {
   digitalWrite(In2,LOW);
   digitalWrite(In1,HIGH);
 }
   else
 {
   digitalWrite(In1,LOW);
   digitalWrite(In2,HIGH);   
 }
 
 // Calculates the percent of the time to spin the motor
  pwmSet = MaxPWM * percentSpin;
  pwmSet = pwmSet/100;
 
 // Activates the PWM on the EN1
  analogWrite(EN1,pwmSet);
  // Keeps motor spinning for a set amount of time
  if (spinTime == 0)
  {
	  while(!digitalRead(Switch))
	  {}
  }
  else 
  {
	  while ( numEncoder < spinTime){}
  }
  numEncoder = 0;
  
  // Turn off motor
  analogWrite(EN1, 0);
  delay(2000);
  return; 
}
