#ifndef ALARM_H
#define ALARM_H

#include <stdbool.h>

typedef struct _AlarmParameters {
	int CurrentLEDToDisplay;
	bool NeedToStartAlarm;
	bool InMiddleOfAlarm;
	long StartAlarmTimeStamp;
	bool TurningLEDsFromLowToHigh;
} AlarmParameters;

AlarmParameters Alarm;

void InitAlarm();
bool CheckIfNeedToStartAlarm();
void HandleAlarm(long CurrentTime);

#endif