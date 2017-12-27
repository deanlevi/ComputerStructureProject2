#include "alarm.h"
#include "time.h"

void InitAlarm();
bool CheckIfNeedToStartAlarm();
void HandleAlarm(long CurrentTime);
void HandleRingtone();
void HandleLeds(long CurrentTime);
void DisplayLED();

void InitAlarm() {
	Alarm.CurrentLEDToDisplay = 0; // 0 - 7
	Alarm.NeedToStartAlarm = false;
	Alarm.InMiddleOfAlarm = false;
	Alarm.StartAlarmTimeStamp = 0;
	Alarm.TurningLEDsFromLowToHigh = true;
}

bool CheckIfNeedToStartAlarm() {
	return ClockTime.Seconds == AlarmTime.Seconds &&
		ClockTime.Minutes == AlarmTime.Minutes &&
		ClockTime.Hours == AlarmTime.Hours;
}

void HandleAlarm(long CurrentTime) {
	HandleRingtone();
	HandleLeds(CurrentTime);
	if (CurrentTime - Alarm.StartAlarmTimeStamp == MINUTE_IN_MS) {
		InitAlarm();
		LED_SetGroupValue(0);
		// stop ringtone
	}
}

void HandleRingtone() {
	// todo
	if (SWT_GetValue(7) == 1) {
		// play
	}
	else {
		// todo ask metargel, can init or do nothing and keep from last point
		// stop
	}
}

void HandleLeds(long CurrentTime) {
	// todo
	if (SWT_GetValue(6) == 1) {
		if ((CurrentTime - Alarm.StartAlarmTimeStamp) % SECOND_IN_MS == 0) {
			DisplayLED();
		}
	}
	else {
		// todo ask metargel, can initialize or do nothing and keep from last point
		LED_SetGroupValue(0);
	}
}

void DisplayLED() {
	if (Alarm.CurrentLEDToDisplay == 8) {
		Alarm.TurningLEDsFromLowToHigh = false;
		Alarm.CurrentLEDToDisplay == 6;
	}
	else if (Alarm.CurrentLEDToDisplay == -1) {
		Alarm.TurningLEDsFromLowToHigh = true;
		Alarm.CurrentLEDToDisplay = 1;
	}
	int LEDToTurnOff = Alarm.TurningLEDsFromLowToHigh ? Alarm.CurrentLEDToDisplay - 1 : Alarm.CurrentLEDToDisplay + 1;

	// todo turn on Alarm.CurrentLEDToDisplay

	Alarm.CurrentLEDToDisplay = Alarm.TurningLEDsFromLowToHigh ? Alarm.CurrentLEDToDisplay + 1 : Alarm.CurrentLEDToDisplay - 1;
	if (LEDToTurnOff == -1) {
		return;
	}

	// todo turn off LEDToTurnOff

}