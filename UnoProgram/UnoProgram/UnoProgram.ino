/*
 * UnoProgram.ino
 *
 * Created: 3/5/2015 12:20:59 PM
 * Author: Grace Green
 */ 

#include "Stepper.h"
/*
#define In1        A5//Pin#
#define In2        A4//Pin#
#define ChannelA   2 //Pin#
#define Interupt_num 0
#define EN1 10 //Pin#
#define MaxPWM 255 //Max PWM
#define UP 1
#define DOWN 0
#define first_lift 5000
#define clearance 900
#define PlateHeight 250
#define Percent_spin 100

#define Switch A3
#define inPin 12
#define outPin 13
*/

#define In1        A0//Pin#
#define In2        A1//Pin#
#define ChannelA   3 //Pin#
#define Interupt_num 1
#define EN1 2 //Pin#
#define MaxPWM 255 //Max PWM
#define UP 1
#define DOWN 0
#define first_lift 5000
#define clearance 900
#define PlateHeight 50
#define Percent_spin 100

#define Switch A2
#define inPin A5
#define outPin A4

int height;
int State;
int numEncoder;
int hold;
Stepper ArmMotor(255, 20); 

void setup()
{
	height = 0;
	numEncoder = 0;
	State = 0;
	hold = 0;
	pinMode(outPin, OUTPUT);
	pinMode(inPin, INPUT);
	pinMode(Switch, INPUT);
	
	
	pinMode(In1,OUTPUT);
	pinMode(In2,OUTPUT);
	pinMode(EN1,OUTPUT);
	pinMode(ChannelA,INPUT);

	attachInterrupt(Interupt_num, incEncoder, RISING);

		// Prototypes
	void MotorSpin(int,int,int);
	void IncEncoder();
}

void loop()
{
		__asm__("nop\n\t");
	
	if (hold == 1)
	ArmMotor.Hold();
	
	switch (State)
	{
		case 0:
			if (digitalRead(inPin) == HIGH)
			{
				State++;
			}
			
			break;
		case 1:
			MotorSpin(first_lift,Percent_spin, UP);//lift above shelf height
			height = height + numEncoder;
			numEncoder = 0;
			ArmMotor.Wider();
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
		if (digitalRead(inPin) == HIGH)// wait to move to shelf
		{
			State++;
		}
		
		break;
		case 4:
		MotorSpin(0,Percent_spin, DOWN);//bring arms down to shelf 
		height = height - numEncoder;
		numEncoder = 0;
		MotorSpin(PlateHeight,Percent_spin, UP);
		height = height + numEncoder;
		numEncoder = 0;
		ArmMotor.Start();
		hold = 1;
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
		if (digitalRead(inPin) == HIGH)//wait to align with plate and grab it
		{
			State++;
		}
		break;
		case 7:
		MotorSpin(clearance, Percent_spin, UP);//get clearance for table
		height = height + numEncoder;
		numEncoder = 0;
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
		MotorSpin(0,Percent_spin, DOWN);//lower plate to table
		height = height - numEncoder;
		numEncoder = 0;
		hold = 0;
		State++;
		break;
		case 11:
		MotorSpin(PlateHeight,Percent_spin, UP);
		height = height + numEncoder;
		numEncoder = 0;
		ArmMotor.Open();//release plate
		MotorSpin(clearance, Percent_spin, UP);//get clearance from table
		height = height + numEncoder;
		numEncoder = 0;
		digitalWrite(outPin,HIGH);
		State++;
		break;
		case 12:
		if (digitalRead(inPin) == LOW)
		{
			digitalWrite(outPin,LOW);
			State++;
		}
		break;
		case 13:
		if (digitalRead(inPin) == HIGH)
		{
			State++;
		}
		
		break;
		
		case 14:
		MotorSpin(0,Percent_spin, DOWN);//bring arms down to table and grab plate
		height = height - numEncoder;
		numEncoder = 0;
		MotorSpin(PlateHeight,Percent_spin, UP);
		height = height + numEncoder;
		numEncoder = 0;
		ArmMotor.Start();
		hold = 1;
		digitalWrite(outPin,HIGH);
		State++;
		break;
		
		case 15:
		if (digitalRead(inPin) == LOW)
		{
			digitalWrite(outPin,LOW);
			State++;
		}
		break;
		
		case 16:
		if (digitalRead(inPin) == HIGH)//wait to align with plate and grab it
		{
			State++;
		}
		break;
		
		case 17:
		MotorSpin(1500, Percent_spin, UP);//get clearance for sink
		height = height + numEncoder;
		numEncoder = 0;
		digitalWrite(outPin,HIGH);
		State++;
		
		break;
		case 18:
		if (digitalRead(inPin) == LOW)
		{
			digitalWrite(outPin,LOW);
			State++;
		}
		break;
		
		case 19:
		if (digitalRead(inPin) == HIGH)
		{
			State++;
		}
		break;
		
		case 20:
		ArmMotor.Open();//release plate
		digitalWrite(outPin,HIGH);
		State++;
		break;
		
		case 21:
		if (digitalRead(inPin) == LOW)
		{
			digitalWrite(outPin,LOW);
			State++;
		}
		break;
		
		case 22:
		if (digitalRead(inPin) == HIGH)
		{
			State++;
		}
		break;
		
		case 23:
		MotorSpin(height,Percent_spin, DOWN);//lower lift to start position
		digitalWrite(outPin,HIGH);
		numEncoder = 0;
		height = 0;
		State++;
		break;
		
		case 24:
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
void MotorSpin(int spinTime, int percentSpin, int dirSpin)
{
	int pwmSet;
  
  // This chooses the direction the motor will spin based on the passed in variable
   if (dirSpin == UP)
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
  //analogWrite(EN1,pwmSet);
  digitalWrite(EN1, HIGH);
// Keeps motor spinning for a set amount of time
  if (spinTime == 0)
  {
	  while(!digitalRead(Switch))
	  {
		  spinTime = 0;
		  }
		 
  }
  else 
  {
		  while ( numEncoder < spinTime)
		{
		    delay(1);
		}
	  
  }
   // Turn off motor
  //analogWrite(EN1, 0);
  digitalWrite(EN1, LOW);
  digitalWrite(In1,LOW);
  digitalWrite(In2,LOW);
  delay(500);
  return; 
}
