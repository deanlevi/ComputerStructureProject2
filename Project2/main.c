#include "config.h"
#include "lcd.h"
#include "led.h"
#include "time.h" 
#include "alarm.h" 
#include "utils.h"
#include "rgbled.h"
#include "btn.h"
#include <xc.h>  
#include <sys/attribs.h>
#include <string.h>

//==================================================
//              Global Configuration
//==================================================
// Device Config Bits in  DEVCFG1:	
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =	OFF
#pragma config POSCMOD =	EC
#pragma config OSCIOFNC =	ON
#pragma config FPBDIV =     DIV_1

// Device Config Bits in  DEVCFG2:	
#pragma config FPLLIDIV =	DIV_2
#pragma config FPLLMUL =	MUL_20
#pragma config FPLLODIV =	DIV_1

#define _XTAL_FREQ 8000000

void Timer4Setup()
{
	static int fTimerInitialised = 0;
	if (!fTimerInitialised)
	{
		//  setup peripheral
		PR4 = 10000;                        //             set period register, generates one interrupt every 1 ms
		TMR4 = 0;                           //             initialize count to 0
		T4CONbits.TCKPS = 3;                //            1:256 prescale value
		T4CONbits.TGATE = 0;                //             not gated input (the default)
		T4CONbits.TCS = 0;                  //             PCBLK input (the default)
		T4CONbits.ON = 1;                   //             turn on Timer1
		IPC4bits.T4IP = 2;                  //             priority
		IPC4bits.T4IS = 0;                  //             subpriority
		IFS0bits.T4IF = 0;                  //             clear interrupt flag
		IEC0bits.T4IE = 1;                  //             enable interrupt
		fTimerInitialised = 1;
	}
}

//==============================================================================
//Interrupt Handler- handled every 1msec
//==============================================================================
long CurrentTime = 0;

void __ISR(_TIMER_4_VECTOR, ipl2auto) Timer4SR(void)
{
	CurrentTime++;
	if (!ClockTimeSet.Active) {
		UpdateTime(CurrentTime);
	};
	IFS0bits.T4IF = 0;                  // clear interrupt flag
}

//==============================================================================
//this function initializes the needed components
//==============================================================================
void init() {
	LCD_Init();
	LED_Init();
	BTN_Init();
	RGBLED_Timer5Setup();
	Timer4Setup();
	SWT_Init();
	RGBLED_Init();
	SSD_Init();
}


void main() {

	init(); //initialize all needed components

			// initialize time
	InitTime();

	// initialize tuning
	InitTimeSet(&ClockTimeSet);
	InitTimeSet(&AlarmTimeSet);

	// initialize stopper
	InitStopper();
	DisplayStopper();

	// initialize alarm
	InitAlarm();
	// todo check if need to initialize all LEDs

	// initialize audio
	AUDIO_Init(3);

	bool FirstTimeClockIsBackOn = true;
	//DisplayTime(CurrentTime, &FirstTimeClockIsBackOn);



	/*char C[100];
	int i=0;
	while (1){
	if ((CurrentTime%2000 == 0) && (CurrentTime>10)){
	itoa(C,CurrentTime,10);
	LCD_WriteStringAtPos(C, 0, 0);
	i++;
	if (i == 100){
	break;
	}
	}

	}*/


	//return;
	ClockTime.Minutes = 59; //to do remove
	ClockTime.Seconds = 58; //to do remove
	CurrentTime = 59 * MINUTE_IN_MS + 58 * SECOND_IN_MS; //to do remove

	while (1)
	{
		if ((SWT_GetValue(0) == 0 && SWT_GetValue(1) == 0) || (SWT_GetValue(0) == 1 && SWT_GetValue(1) == 1)) {
			DisplayTime(CurrentTime, &FirstTimeClockIsBackOn);
			HandleStopper(CurrentTime);
		}
		else {
			SSD_Close();
		}

		// check if need to start alarm
		if (!Alarm.InMiddleOfAlarm) {
			Alarm.NeedToStartAlarm = CheckIfNeedToStartAlarm();
			if (Alarm.NeedToStartAlarm) {
				Alarm.StartAlarmTimeStamp = CurrentTime;
				Alarm.InMiddleOfAlarm = true;
			}
		}
		if (Alarm.InMiddleOfAlarm) {
			HandleAlarm(CurrentTime);
		}


		//Clock tuning
		while (SWT_GetValue(0) == 1 && SWT_GetValue(1) == 0) {
			FirstTimeClockIsBackOn = true;
			if (!ClockTimeSet.Active) {
				ClockTimeSet.TimeSetTimeStamp = CurrentTime;
				ClockTimeSet.CurrentState = On;
				ClockTimeSet.Active = true;
			}
			HandleTimeSet(CurrentTime, &ClockTime, &ClockTimeSet, false);
		}
		ClockTimeSet.Active = false;
		ClockTimeSet.CurrentState = None;
		ClockTimeSet.DisplayFirstTimeOnNone = true;

		// Alarm tuning
		if (SWT_GetValue(0) == 0 && SWT_GetValue(1) == 1) {
			FirstTimeClockIsBackOn = true;
			if (!AlarmTimeSet.Active) {
				AlarmTimeSet.TimeSetTimeStamp = CurrentTime;
				AlarmTimeSet.CurrentState = On;
				AlarmTimeSet.Active = true;
			}
			HandleTimeSet(CurrentTime, &AlarmTime, &AlarmTimeSet, true);
		}
		else {
			AlarmTimeSet.Active = false;
			AlarmTimeSet.CurrentState = None;
			AlarmTimeSet.DisplayFirstTimeOnNone = true;
		}
	}
}




