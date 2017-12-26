#ifndef TIME_H
#define TIME_H

#include <stdbool.h>

#define MINUTE_IN_MS 60000
#define SECOND_IN_MS 1000
#define TIME_SET_INITIAL_GAP 500
#define TIME_SET_SPEED_GAP 200
#define HUNDREDTH_IN_MS 10

typedef struct _TimeParameters {
	int Seconds;
	int Minutes;
	int Hours;

} TimeParameters;

typedef struct _StopperParameters {
	int Seconds;
	int Hundredths;
	long TimeStamp;
	bool Active;
} StopperParameters;

typedef enum _SetTimeEnum {
	None,
	HH,
	MM,
	SS
} SetTimeEnum;

typedef enum _StateEnum {
	NoState,
	Off,
	On
} StateEnum;

typedef struct _TimeSetParameters {
	bool Active;
	long TimeSetTimeStamp;
	SetTimeEnum BTNCNumOfPushs;
	StateEnum CurrentState;
	bool DisplayFirstTimeOnNone;
	bool NeedToChangeDisplay;
	long PushTimeStamp;
	bool FirstUpdate;
} TimeSetParameters;

TimeParameters ClockTime;
TimeParameters AlarmTime;
TimeSetParameters ClockTimeSet;
TimeSetParameters AlarmTimeSet;
StopperParameters Stopper;

void InitTime();// was void InitTime(TimeParameters Time);
void InitTimeSet(TimeSetParameters TimeSet);
void InitStopper();
void UpdateTime(long CurrentTime);
void DisplayTime(long CurrentTime, bool *FirstTime);
void HandleTimeSet(long CurrentTime, TimeParameters Time, TimeSetParameters TimeSet, bool IsAlarm);
void HandleStopper(long CurrentTime);
void DisplayStopper();

#endif