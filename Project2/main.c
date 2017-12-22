#include "config.h"
#include "lcd.h"
#include "led.h"
#include "time.h" // todo
#include "alarm.h" // todo
//#include "timed.h"
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
    if(!fTimerInitialised)
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
    IFS0bits.T4IF = 0;                  // clear interrupt flag
}

//==============================================================================
//this function initializes the needed components
//==============================================================================
void init(){
    LCD_Init(); 
    LED_Init();
    BTN_Init();
    RGBLED_Timer5Setup();
    Timer4Setup();
}


void main(){
	// alarm
	bool NeedToStartAlarm = false;
	bool InMiddleOfAlarm = false;
	long StartAlarmTimeStamp = 0;

	init(); //initialize all needed components
	//SWT_Init(); // todo check why not in init
	//RGBLED_Init(); // todo check why not in init

	InitTime(AlarmTime);
	InitTime(ClockTime);
	InitTime(AlarmTime);
	InitTimeSet(ClockTimeSet);
	InitTimeSet(AlarmTimeSet);
	InitStopper();
	DisplayStopper();
	
	while (1)
	{
		UpdateTime(CurrentTime);
		if (SW0 == OFF && SW1 == OFF) {
			DisplayTime(CurrentTime);
			HandleStopper(CurrentTime);
			//check stopper
		}
		// clock view - need to be updated all time (ALARM tuning, 
		
		if (InMiddleOfAlarm) {
			HandleAlarm(CurrentTime, StartAlarmTimeStamp, &InMiddleOfAlarm);
		}
		// check SW7 to see if need to start alarm
		if (SW7 == ON) {
			NeedToStartAlarm = CheckIfNeedToStartAlarm();
			if (NeedToStartAlarm && !InMiddleOfAlarm) {
				StartAlarmTimeStamp = CurrentTime;
				InMiddleOfAlarm == true;
			}
		}
		
		//Clock tuning
		while (SW0 == ON && SW1 == OFF) {
			if (!ClockTimeSet.Active) {
				ClockTimeSet.TimeStamp = CurrentTime;
				ClockTimeSet.CurrentState = On;
				ClockTimeSet.Active = true;
			}
			HandleTimeSet(CurrentTime, ClockTime, ClockTimeSet, false);
		}
		ClockTimeSet.Active = false;
		ClockTimeSet.CurrentState = None;
		ClockTimeSet.DisplayFirstTimeOnNone = true;
		
		// Alarm tuning
		if (SW0 == OFF && SW1 == ON) {
			if (!AlarmTimeSet.Active) {
				AlarmTimeSet.TimeStamp = CurrentTime;
				AlarmTimeSet.CurrentState = On;
				AlarmTimeSet.Active = true;
			}
			HandleTimeSet(CurrentTime, AlarmTime, AlarmTimeSet, true);
		}
		else {
			AlarmTimeSet.Active = false;
			AlarmTimeSet.CurrentState = None;
			AlarmTimeSet.DisplayFirstTimeOnNone = true;
		}


		/*if (!BTN_GetValue(2)){}
		if (BTN_GetValue(2)){ // when button BTNC is pusshed and left
		// hours tuning 
		
		if (!BTN_GetValue(2)){}
		if (BTN_GetValue(2)){ // when button BTNC is pusshed and left
		// minutes tuning and hours stills
		
		if (!BTN_GetValue(2)){}
		if (BTN_GetValue(2)){ // when button BTNC is pusshed and left
		// seconds tuning and minutes stills
		
		// seconds stills
		
		}
			}
		} TODO handle
		
		
		// Ring sound
		//if (SW7 == ON) //for sound enabling
		if ( SW6 == ON ) // for leds enabling 
		
		// stopper support
		while (SW6 == 0FF && SW7 == OFF)
		(7 segment display)
	}*/
}





