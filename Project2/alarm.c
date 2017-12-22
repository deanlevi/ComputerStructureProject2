#include "alarm.h"
#include "time.h"

bool CheckIfNeedToStartAlarm() {
	return ClockTime.Seconds == AlarmTime.Seconds &&
		   ClockTime.Minutes == AlarmTime.Minutes &&
		   ClockTime.Hours == AlarmTime.Hours;
}

void HandleAlarm(long CurrentTime, long StartAlarmTimeStamp, bool *InMiddleOfAlarm) {

	if (CurrentTime - StartAlarmTimeStamp == MINUTE_IN_MS) {
		*InMiddleOfAlarm = false;
	}
}