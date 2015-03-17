/* 
* Motors.cpp
*
* Created: 3/16/2014 12:16:09 AM
* Author: Quang
*/


#include "Motors.h"
#include <Wire.h>
#include <Arduino.h>

// default constructor
Motors::Motors()
{
	FirstTime = 0;
	PreviousTime = 0;
} //Motors

// default destructor
Motors::~Motors()
{
} //~Motors

void Motors::forward(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Forward  );
	i2cWrite(Front_Right,  RobotSpeed,   Forward  );
	i2cWrite(Back_Left,    RobotSpeed,   Forward  );
	i2cWrite(Back_Right,   RobotSpeed,   Forward  );
}

void Motors::backward(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Backward  );
	i2cWrite(Front_Right,  RobotSpeed,   Backward  );
	i2cWrite(Back_Left,    RobotSpeed,   Backward  );
	i2cWrite(Back_Right,   RobotSpeed,   Backward  );
}

void Motors::left(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Backward  );
	i2cWrite(Front_Right,  RobotSpeed,   Forward   );
	i2cWrite(Back_Left,    RobotSpeed,   Forward   );
	i2cWrite(Back_Right,   RobotSpeed,   Backward  );
}

void Motors::right(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Forward   );
	i2cWrite(Front_Right,  RobotSpeed,   Backward  );
	i2cWrite(Back_Left,    RobotSpeed,   Backward  );
	i2cWrite(Back_Right,   RobotSpeed,   Forward   );
}

void Motors::cw(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Forward   );
	i2cWrite(Front_Right,  RobotSpeed,   Backward  );
	i2cWrite(Back_Left,    RobotSpeed,   Forward   );
	i2cWrite(Back_Right,   RobotSpeed,   Backward  );
}

void Motors::ccw(int RobotSpeed)
{
	i2cWrite(Front_Left,   RobotSpeed,   Backward   );
	i2cWrite(Front_Right,  RobotSpeed,   Forward    );
	i2cWrite(Back_Left,    RobotSpeed,   Backward   );
	i2cWrite(Back_Right,   RobotSpeed,   Forward    );
}

void Motors::stop()
{
	i2cWrite(Front_Left,   Stop,   Forward   );
	i2cWrite(Front_Right,  Stop,   Forward  );
	i2cWrite(Back_Left,    Stop,   Forward  );
	i2cWrite(Back_Right,   Stop,   Forward   );
}

void Motors::i2cWrite ( char Address, char Speed , char Direction)
{
	Wire.beginTransmission(Address >> 1 );
	Wire.write((byte)0);
	Wire.write((byte)Speed);
	Wire.write((byte)Direction);
	Wire.endTransmission();
}

/***************************************************
 *
 * Name:            moveForward
 * Parameters:      int *Target
 *					Pointer to the variable that contains the amount of counts 
 *					you wish to travel
 * Return:  
 *          int -- will return a zero if target has not yet been reached
 *					will return 1 if the target has been reached
 * Function: 
 *					This function will cause the robot to move forward by 
 *					the amount of counts contain in the Target variable.
 *
 *					Counts = Distance to travel ( in mm) / 0.315367
 *
 *					0.315367 = K * wheel_circumference / PulsesPerRev = 1.015 * 2 * PI * wheel_radius / 546
 *							 = 1.015 * 2 * PI * 27 / 546 = 1.015 * 0.310707
 *
 *					Ex. I wish to travel 304.8 mm (1 foot)
 *						Target = 304.8 / 0.315367 = 966
 *
 * Comments:		The reason why we don't handle the math in-code is because this is a 8 bits machine
 *					and it takes way too long to do float multiplication and addition.			
 *          
 ***************************************************/
int Motors::moveForward( int *Target)
{
	if (FirstTime != 1)
	{
		forward(MediumSpeed);
		FirstTime = 1;
	}
	if ( (millis() - PreviousTime) >= SAMPLING_PERIOD)
	{		
		PreviousTime = millis();
		(*Target) -= average();
		
		// If target is reached
		if ((*Target) <= 0)
		{
			FirstTime = 0;
			stop();
			return 1;
		}
		else return 0;
	}
}

int Motors::moveBackward( int *Target)
{
	if (FirstTime != 1)
	{
		backward(MediumSpeed);
		FirstTime = 1;
	}
	if ( (millis() - PreviousTime) >= SAMPLING_PERIOD)
	{
		PreviousTime = millis();
		(*Target) -= average();
		
		// If target is reached
		if ((*Target) <= 0)
		{
			FirstTime = 0;
			stop();
			return 1;
		}
		else return 0;
	}
}

/***************************************************
 *
 * Name:            moveRight
 * Parameters:      int *Target
 *					Pointer to the variable that contains the amount of counts 
 *					you wish to travel
 * Return:  
 *          int -- will return a zero if target has not yet been reached
 *					will return 1 if the target has been reached
 * Function: 
 *					This function will cause the robot to move Right by 
 *					the amount of counts contain in the Target variable.
 *
 *					Counts = Distance to travel ( in mm) / 0.24235146
 *
 *					0.24235146 = K * wheel_circumference / PulsesPerRev = 1.015 * 2 * PI * wheel_radius / 546
 *							 = 0.78 * 2 * PI * 27 / 546 = 0.78 * 0.310707
 *
 *					Ex. I wish to travel 304.8 mm (1 foot)
 *						Target = 304.8 / 0.24235146 = 1257
 *
 * Comments:		The reason why we don't handle the math in-code is because this is a 8 bits machine
 *					and it takes way too long to do float multiplication and addition.			
 *          
 ***************************************************/
int Motors::moveRight( int *Target)
{
	if (FirstTime != 1)
	{
		right(MediumSpeed);
		FirstTime = 1;
	}
	if ( (millis() - PreviousTime) >= SAMPLING_PERIOD)
	{
		PreviousTime = millis();
		(*Target) -= average();
		
		// If target is reached
		if ((*Target) <= 0)
		{
			FirstTime = 0;
			stop();
			return 1;
		}
		else return 0;
	}
}

int Motors::moveLeft( int *Target)
{
	if (FirstTime != 1)
	{
		left(MediumSpeed);
		FirstTime = 1;
	}
	if ( (millis() - PreviousTime) >= SAMPLING_PERIOD)
	{
		PreviousTime = millis();
		(*Target) -= average();
		
		// If target is reached
		if ((*Target) <= 0)
		{
			FirstTime = 0;
			stop();
			return 1;
		}
		else return 0;
	}
}

/***************************************************
 *
 * Name:            moveCW
 * Parameters:      int *Target
 *					Pointer to the variable that contains the amount of counts 
 *					you wish to travel
 * Return:  
 *          int -- will return a zero if target has not yet been reached
 *					will return 1 if the target has been reached
 * Function: 
 *					This function will cause the robot to move Right by 
 *					the amount of counts contain in the Target variable.
 *
 *					Counts = Distance to travel ( in mm) / 0.09898628997
 *
 *						w_conversion = 0.93
 *
 *					degrees_per_count = w_conversion * (degrees_per_circle * wheel_circumference) / (cpr * circle_circumference);
 *								      = w_conversion * (360 * 169.646003294 ) / (546 * 1.13)
 *									  = w_conversion * 98.9862899702 = 0.93 * 98.9862899702
 *									  = 92.0572496723
 *
 *					Ex. I wish to travel 304.8 mm (1 foot)
 *						Target = 304.8 / 0.24235146 = 1257
 *
 * Comments:		The reason why we don't handle the math in-code is because this is a 8 bits machine
 *					and it takes way too long to do float multiplication and addition.			
 *          
 ***************************************************/
int Motors::moveCW( int *Target)
{
	if (FirstTime != 1)
	{
		cw(MediumSpeed);
		FirstTime = 1;
	}
	if ( (millis() - PreviousTime) >= SAMPLING_PERIOD)
	{
		PreviousTime = millis();
		(*Target) -= average();
		
		// If target is reached
		if ((*Target) <= 0)
		{
			FirstTime = 0;
			stop();
			return 1;
		}
		else return 0;
	}
}

int Motors::moveCCW( int *Target)
{
	if (FirstTime != 1)
	{
		ccw(MediumSpeed);
		FirstTime = 1;
	}
	if ( (millis() - PreviousTime) >= SAMPLING_PERIOD)
	{
		PreviousTime = millis();
		(*Target) -= average();
		
		// If target is reached
		if ((*Target) <= 0)
		{
			FirstTime = 0;
			stop();
			return 1;
		}
		else return 0;
	}
}

int Motors::average()
{
	int OdometryCounts [4] = {0,0,0,0};
	int OdometryDirection [4] = {0,0,0,0};
	
	ReadOne(0x02, &OdometryCounts [0], &OdometryDirection [0]);
	ReadOne(0x04, &OdometryCounts [1], &OdometryDirection [1]);
	ReadOne(0x06, &OdometryCounts [2], &OdometryDirection [2]);
	ReadOne(0x08, &OdometryCounts [3], &OdometryDirection [3]);
	
	int AverageResult = 0;
	for (int i=0 ; i < 4; i++)
	{
		AverageResult += OdometryCounts [i];
	}
	AverageResult = AverageResult >> 2;
	return AverageResult;
}

void Motors::ReadOne(char address, int *Odometry, int *Direction) 
{ 
	unsigned int encoder[4] = {0,0,0,0};
	Wire.requestFrom(address >>1 , 4);    // request 3 bytes from address
	int i = 0;
	while(Wire.available())   // slave may send less than requested
	{
		encoder[i] = Wire.read();   // receive a byte as character
		i++;
	}
	
	encoder[1] = encoder[1] << 8; // Combine the two bytes into one value, lower byte is sent first, upper second.
	*Odometry = encoder[1] + encoder[0];
	
	encoder[3] = encoder[3] << 8; // Combine the two bytes into one value, lower byte is sent first, upper second.
	*Direction = encoder[3] + encoder[2];
}