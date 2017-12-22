#ifndef ALARM_H
#define ALARM_H

#include <stdbool.h>

bool CheckIfNeedToStartAlarm();
void HandleAlarm(long CurrentTime, long StartAlarmTimeStamp, bool *InMiddleOfAlarm);

#endif