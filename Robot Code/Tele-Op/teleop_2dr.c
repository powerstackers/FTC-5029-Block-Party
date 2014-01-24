#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     IRS_L,          sensorI2CCustom)
#pragma config(Sensor, S4,     IRS_R,          sensorI2CCustom)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,          mBlockStop,    tmotorNXT, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_1,     mDriveLeft,    tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     mDriveRight,   tmotorTetrix, openLoop, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     mBsConveyor,   tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     mBsAngle,      tmotorTetrix, openLoop, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"  //Include file to "handle" the Bluetooth messages.

#define STICK_TO_MOTOR(x)	((float)x * 0.78125)	// Convert the joystick's -128 - 127 value to a -99 - 100 value
#define MODE_STRAIGHT true					// Drive modes, straight or tank
#define MODE_NORMAL false

/*
 * Global variables
 */

// Analog joysticks
short 	stickDriveLeft;
short 	stickDriveRight;
short		stickAngle;
short		stickBlockStop;

// Single-bit buttons
bool 		btnStraightDr;
bool		btnConveyor;
bool		btnReverse;
bool		btnBlockStop;

// Flags
bool		driveMode;
bool		brickBlocked = true;
bool		blockerMoving = false;

// Constants
const short		stickThreshold = 10;
const short 	encoderStartValue = nMotorEncoder[mBlockStop];
const int			blockClosedPos = 90;
const int			blockOpenPos = -5;
const int 		blockOpenThresh = -360;
const int 		blockClosedThresh = 360;
const int			blockSpeed = 25;

/*
 * Set all motors to the input value
 */
void allMotorsTo(int i){
		motor[mDriveLeft] 	= i;
		motor[mDriveRight] 	= i;
		motor[mBsAngle] 		= i;
		motor[mBsConveyor] 	= i;
}

/*
 * Set all drive motors to the input value
 */
void driveMotorsTo(int i){
		motor[mDriveLeft] 	= i;
		motor[mDriveRight] 	= i;
}

/*
 * Set all encoders to the input value (good for zeroing)
 */
void allEncodersTo(int i){
	nMotorEncoder[mBlockStop] = i;
	nMotorEncoder[mBsAngle] = i;
	nMotorEncoder[mBsConveyor] = i;
	nMotorEncoder[mDriveLeft] = i;
	nMotorEncoder[mDriveRight] = i;
	writeDebugStreamLine("Encoders initialized");
}

/*
 * Initialize robot positions
 */
void initializeRobot()
{
	allMotorsTo(0);
	allEncodersTo(0);
  return;
}


/*
 * Function to transfer normal joystick values to the custom variables
 */
void getCustomJoystickSettings(){
	stickDriveLeft 	= joystick.joy1_y1;	// dr1 left joystick
	stickDriveRight = joystick.joy1_y2;	//
	btnConveyor		=	(joy2Btn(6) == 1);
	stickAngle			= joystick.joy2_y1;
	btnStraightDr		= (joy1Btn(3) == 1);
	stickBlockStop	=	joystick.joy2_y2;
	btnReverse			= (joy2Btn(8) == 1);
	btnBlockStop		= (joy2Btn(3) == 1);
}

/*
 * Print information to the screen
 */
void displayButtonValues(){
	  nxtDisplayTextLine(0, "btnConv:%d",		btnConveyor);	// Left drive joystick
  	nxtDisplayTextLine(1, "btnRev:%d",	btnReverse);	// Right drive joystick
		nxtDisplayTextLine(3, "stickBlSt:%d", stickBlockStop);	// Brick stopper button
		nxtDisplayTextLine(4, "mtrBlock:%d", motor[mBlockStop]);
  	nxtDisplayTextLine(5, "blEnc:%d", nMotorEncoder[mBlockStop]);	// Encoder on NXT motor
		nxtDisplayTextLine(6, "stickAngle:%d", stickAngle);	// Brick sucker joystick
		nxtDisplayTextLine(7, "btnBlockStop:%d", btnBlockStop);	// Conveyor joystick
}
/*
 * Main task
 */
task main(){
	/*
	 * Do some housekeeping
	 */
	bNxtLCDStatusDisplay = false;	// Turn off battery level display on the NXT screen (no one uses that anyway)
  clearDebugStream();						// Clear the debug screen from last time
	eraseDisplay();								// Cear the screen
  initializeRobot();						// Initialize all the motors and servos
  ClearTimer(T1);								// Reset the timer


  //waitForStart();   						// wait for start of tele-op phase

  while (true){											// Loop forever
  	getJoystickSettings(joystick);	// Get the regular joystick information
  	getCustomJoystickSettings();		// Get the custom joystick information
		displayButtonValues();					// Print it all on the NXT screen


		/*
		* Changing drive mode
		*/
		if(btnStraightDr){	// If the straight drive button is pressed, switch to straight drive mode
			driveMode = MODE_STRAIGHT;
			//writeDebugStreamLine("Changed mode to straight");
		}
		else{
			driveMode = MODE_NORMAL;
			//writeDebugStreamLine("Changed mode to normal");
		}


		/*
		* Straight drive mode
		*/
		if (driveMode == MODE_STRAIGHT){
			if(abs(stickDriveLeft) > stickThreshold){
					driveMotorsTo(STICK_TO_MOTOR(stickDriveLeft));	// Move both motors at the same speed
			}
			else{
				driveMotorsTo(0);
			}

		}

		/*
		* Normal drive mode
		*/
		else if(driveMode == MODE_NORMAL){

			/*
			* Left drive motor
			*/
			if(abs(stickDriveLeft) < stickThreshold){
				motor[mDriveLeft] = 0;
			}
			else{
				motor[mDriveLeft] = STICK_TO_MOTOR(stickDriveLeft);	// Left drive motor to a function of the stick value
			}

			/*
			* Right drive motor
			*/
			if(abs(stickDriveRight) < stickThreshold){
				motor[mDriveRight] = 0;
			}
			else{
				motor[mDriveRight] = STICK_TO_MOTOR(stickDriveRight);	// Right drive motor to a function of the stick value
			}
		}


		/*
		* Conveyor motor
		*/
		if(btnConveyor){
			motor[mBsConveyor] = 75;
		}
		else if(btnReverse){
			motor[mBsConveyor] = -75;
		}else{
			motor[mBsConveyor] = 0;
		}


		/*
		* Brick sucker lift motor
		*/
		if(abs(stickAngle) < stickThreshold){	// If the stick is at the center, don't move
			motor[mBsAngle] = 0;
		}
		else{
			motor[mBsAngle] = ((stickAngle > 0)? 100 : -100);	// If the stick value is positive,
																					//move in a positive direction. Else don't move.
		}

		/*
		* Brick gate motor
		*/
		if(abs(stickBlockStop) < stickThreshold){	// If the stick is in the center, don't move
			motor[mBlockStop] = 0;
		}else if(nMotorEncoder[mBlockStop] >= blockOpenThresh && stickBlockStop > 0){	// If the stick is pushed up, and the
				if(nMotorEncoder[mBlockStop] < blockClosedPos){										// motor is higher than the open threshold...
					motor[mBlockStop] = blockSpeed;	// If the motor is higher than the closed position, move the motor
				}else{
					motor[mBlockStop] = 0;					// Else, stop the motor
				}
		}else if(nMotorEncoder[mBlockStop] <= blockClosedThresh &&  stickBlockStop < 0){	// If the stick is pushed down, and
			if(nMotorEncoder[mBlockStop] > blockOpenPos){											// the motor is lower than the closed threshold...
				motor[mBlockStop] = blockSpeed * -1;	// If the motor hasn't reached the open position, move the motor
			}else{
				motor[mBlockStop] = 0;								// Else, stop the motor
			}
		}else{
			motor[mBlockStop] = 0;							// Else, stop the motor
		}
  }
}
