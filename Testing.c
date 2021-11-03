/*
 \file		Testing.c
 \author	${user}
 \date		${date}
 \brief		Simple Hello World! for the Ev3
*/

#include <ev3.h>

char orientation = 'n';


Tone a[2] = {
		{TONE_C4, NOTE_HALF},
		{TONE_F6, NOTE_EIGHT}
};

void moveforwardone()
{
	RotateMotor(OUT_AD,10,502);
	return;
}

//Clockwise turn
void turnright()
{
	SetDirection(OUT_A, OUT_FWD);
	RotateMotor(OUT_A, 10, 170);
	SetDirection(OUT_D, OUT_REV);
	RotateMotor(OUT_D, 10, 170);

	switch(orientation)
	{
		case 'n':{
			orientation = 'e';
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 'e':{
			orientation = 's';
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 's':{
			orientation = 'w';
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 'w':{
			orientation = 'n';
			PlayTones(a);
			break;
		}
	}
}

void turnleft()
{
	SetDirection(OUT_A, OUT_REV);
	RotateMotor(OUT_A, 10, 170);
	SetDirection(OUT_D, OUT_FWD);
	RotateMotor(OUT_D, 10, 170);

	switch(orientation)
	{
		case 'n':{
			orientation = 'w';
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 'e':{
			orientation = 'n';
			PlayTones(a);
			break;
		}
		case 's':{
			orientation = 'e';
			PlayTones(a);
			PlayTones(a);
			break;
		}
		case 'w':{
			orientation = 's';
			PlayTones(a);
			PlayTones(a);
			PlayTones(a);
			break;
		}
	}
}

int main(void)
{
	/*
	Orientation will always start as North ('n'). To let user know what the orientation is
	 once it switches it will beep a specific number of times based on orientation.
	 North = 1 beep
	 East = 2 beeps
	 South = 3 beeps
	 West = 4 beeps

	SetDirection(uint8_t Outputs, uint8_t Dir)
	#define OUT_REV    0x00
	#define OUT_TOGGLE 0x40
	#define OUT_FWD    0x80

	void RotateMotorEx(uint8_t Outputs, int8_t Speed, int Angle, short Turn, bool Sync, bool Stop);

	#define RotateMotor(_outputs, _speed, _angle)
		RotateMotorEx((_outputs), (_speed), (_angle), 0, true, true)

	Speed 0-100
	Set Motor Direction before Rotating
	Angle Anything

	pi*d = circumference

	A full rotation (360 degrees) on our wheel is about .2199 meters
	.305/.2199 = 1.387*(360) = 500 degrees (This is the degrees required for a distance of .305 meters,
	which is also the length of a full tile.)

	Clockwise
	SetDirection(OUT_A, OUT_FWD);
	RotateMotor(OUT_A, 10, 170);
	SetDirection(OUT_D, OUT_REV);
	RotateMotor(OUT_D, 10, 170);


	Counterclockwise
	SetDirection(OUT_A, OUT_FWD);
	RotateMotor(OUT_A, 10, 170);
	SetDirection(OUT_D, OUT_REV);
	RotateMotor(OUT_D, 10, 170);


	*/
	// INFO This code template works only with recent versions of the API. If TermPrintln is not found,
		//      please update the API or use the "Hello World EV3 Project Old API" as template.


	//TODO Place here your variables


	//TODO Place here your code
	int InitEV3(void);
	TermPrintln("Hello World!");

	turnleft();
	turnleft();
	turnleft();
	turnleft();
	turnright();
	turnright();
	turnright();
	turnright();
	TermPrintf("%c", orientation);


	TermPrintf("Press ENTER to exit");

	ButtonWaitForPress(BUTTON_ID_ENTER);
	FreeEV3();
	return 0;
}
