/////////////////////////////////////////////////////////////
//	AUTONOMOUS PROGRAM STRATEGY CHOOSER
//	ORIGINAL CODE BY FTC TEAM# 3785, THE BESTIE BOTS
//	https://github.com/hprobotics/ftcresources/blob/master/AutonomousChooser/menu_helper.h
//	MODIFIED BY FTC TEAM# 5029, THE POWERSTACKERS
////////////////////////////////////////////////////////////
#include "JoystickDriver.c"

// Constants to make it easier to use the buttons
#define NEXT_BUTTON kRightButton
#define PREV_BUTTON kLeftButton
#define DOWN_BUTTON kEnterButton

///////////////////////////////////////////////////////////
//
//	Switch a boolean to the opposite value
//
///////////////////////////////////////////////////////////
void switchBool(bool* in, TButtons activeButton){
	if(activeButton == NEXT_BUTTON || activeButton == PREV_BUTTON)
		*in = !*in;
}

//////////////////////////////////////////////////////////
//
//	Increment or decrement an integer by 1
//
/////////////////////////////////////////////////////////
void switchInt(int* in, TButtons activeButton){
	if(activeButton == NEXT_BUTTON)
		*in = *in + 1;
	if(activeButton == PREV_BUTTON)
		*in = *in - 1;
}

/////////////////////////////////////////////////////////
//
//	Increment or decrement an integer by 5
//
////////////////////////////////////////////////////////
void switchIntByFive(int* in, TButtons activeButton){
	if(activeButton == NEXT_BUTTON)
		*in = *in + 5;
	if(activeButton == PREV_BUTTON)
		*in = *in - 5;
}

////////////////////////////////////////////////////////////
//
//	Increment or decrement a floating point number by 0.1
//
////////////////////////////////////////////////////////////
void switchFloat(float* in, TButtons activeButton){
	if(activeButton == NEXT_BUTTON)
		*in = *in + 0.1;
	if(activeButton == PREV_BUTTON)
		*in = *in - 0.1;
}

// Game option variables (offensive play style):
// startNear: Starting with the ramp to the robot's right or its left
// doIr: Place the IR block or not
// goAround: After placing the IR block, go around the far side of the ramp, or the near side
// rampOtherSide: Go up our alliance's half of the ramp, or the other alliance's half
bool startNear = true;
bool doIr = true;
bool goAround = false;
bool rampOtherSide = false;

// Game option variables (general):
// delay: Amount of time to wait before starting the run (in seconds)
// maxDelay: The maximum time you can wait
// forwardMotorRatio: The ratio of the right wheel to the left wheel speed, going forward
// backwardMotorRatio: The ratio of the right wheel to the left wheel speed, going backward
int delay = 0;
const int maxDelay = 15;
int forwardMotorRatio = 90;
int backwardMotorRatio = 90;

/////////////////////////////////////////////////////////////
//
//	Run the menu
//
/////////////////////////////////////////////////////////////
task runMenuOffensive()
{
	bDisplayDiagnostics = false;
	eraseDisplay();

	void* currVar = &startNear;
	char currType = 'b';

	while (true){
		// Can't wait for negative seconds, can we?
		if(delay < 0)
			delay = 0;
		else if(delay > maxDelay)
			delay = maxDelay;

		// Print all the variable names and their current values to the screen
		nxtDisplayString(0, "Near:     %s", startNear ? "yes":"no ");
		nxtDisplayString(1, "Do Ir:    %s", doIr ? "yes":"no ");
		nxtDisplayString(2, "Go Around:%s", goAround ? "yes":"no ");
		nxtDisplayString(3, "RmpOthrSd:%s", rampOtherSide ? "yes":"no ");
		nxtDisplayString(4, "Delay:    %2d", delay);
		nxtDisplayString(5, "FrwrdRatio:%d ", forwardMotorRatio);
		nxtDisplayString(6, "BkwrdRatio:%d ", backwardMotorRatio);

		// Print a selection icon next to the active variable name
		switch(currVar){
			case &startNear:
				nxtDisplayStringAt(94, 63, "]");
				nxtDisplayStringAt(94, 17, " ");
				break;
			case &doIr:
				nxtDisplayStringAt(94, 63, " ");
				nxtDisplayStringAt(94, 55, "]");
				break;
			case &goAround:
				nxtDisplayStringAt(94, 55, " ");
				nxtDisplayStringAt(94, 47, "]");
				break;
			case &rampOtherSide:
				nxtDisplayStringAt(94, 47, " ");
				nxtDisplayStringAt(94, 39, "]");
				break;
			case &delay:
				nxtDisplayStringAt(94, 39, " ");
				nxtDisplayStringAt(94, 31, "]");
				break;
			case &forwardMotorRatio:
				nxtDisplayStringAt(94, 31, " ");
				nxtDisplayStringAt(94, 24, "]");
				break;
			case &backwardMotorRatio:
				nxtDisplayStringAt(94, 24, " ");
				nxtDisplayStringAt(94, 17, "]");
				break;
			default:
				break;
		}

		// Shift value
		if(nNxtButtonPressed == NEXT_BUTTON || nNxtButtonPressed == PREV_BUTTON){
			switch(currType){
				case 'b':
					switchBool(currVar, nNxtButtonPressed);
					break;
				case 'i':
					switchInt(currVar, nNxtButtonPressed);
					break;
				case 'l':
					switchIntByFive(currVar, nNxtButtonPressed);
					break;
				default:
					break;
			}

			PlaySound(soundBlip);

			// User must hold for 4 seconds to move another increment
			ClearTimer(T1);
			while(nNxtButtonPressed != kNoButton && time1[T1] <= 400){}
		}

		// Move down the list of options
		if(nNxtButtonPressed == DOWN_BUTTON){
			switch(currVar){
				case &startNear:
					currVar = &doIr;
					currType = 'b';
					break;
				case &doIr:
					currVar = &goAround;
					currType = 'b';
					break;
				case &goAround:
					currVar = &rampOtherSide;
					currType = 'b';
					break;
				case &rampOtherSide:
					currVar = &delay;
					currType = 'i';
					break;
				case &delay:
					currVar = &forwardMotorRatio;
					currType = 'l';
					break;
				case &forwardMotorRatio:
					currVar = &backwardMotorRatio;
					currType = 'l';
					break;
				case &backwardMotorRatio:
					currVar = &startNear;
					currType = 'b';
					break;
				default:
					break;
			}

			PlaySound(soundBlip);

			// User must hold for 4 seconds before moving down another row
			ClearTimer(T1);
			while(nNxtButtonPressed != kNoButton && time1[T1] <= 400){}
		}
	}
}

///////////////////////////////////////////////////////////////////////
//
//	Print all the chosen options to the debug stream
//
//////////////////////////////////////////////////////////////////////
void printMenuChoices(){
	writeDebugStreamLine("Start on near side: %s\nFind IR basket: %s\nGo around far end of ramp: %s\nGo to the other half of the ramp: %s\nDelay: %d seconds",
		(startNear)? "Yes":"No", (doIr)? "Yes":"No", (goAround)? "Yes":"No", (rampOtherSide)? "Yes":"No", delay);
}
