#include "time.h"

void InitTime(TimeParameters Time);
void InitTimeSet(TimeSetParameters TimeSet);
void InitStopper();
void UpdateSeconds(long CurrentTime);
void UpdateMinutes(long CurrentTime);
void UpdateHours(long CurrentTime);
void DisplayTime(long CurrentTime);
void HandleTimeSet(long CurrentTime, TimeParameters Time, TimeSetParameters TimeSet, bool IsAlarm);
void UpdateBTNCNumOfPushsInSetTime(TimeSetParameters TimeSet);
void CheckIfNeedToChangeDisplay(long CurrentTime, TimeSetParameters TimeSet);
void DisplayTheRightSetClock(StateEnum HH, StateEnum MM, StateEnum SS, bool IsAlarm);
void DisplaySetTime(StateEnum HH, StateEnum MM, StateEnum SS);
void DisplayAlarmTime(StateEnum HH, StateEnum MM, StateEnum SS);
void HandleLongPush(TimeParameters Time, TimeSetParameters TimeSet, bool Increase);
void UpdateClockByOne(TimeParameters Time, TimeSetParameters TimeSet, bool Increase);
void HandleStopper(long CurrentTime);
void ResetStopper();
void UpdateStopper();
void DisplayStopper();

void InitTime(TimeParameters Time) {
	ClockTime.Seconds = 0;
	ClockTime.Minutes = 0;
	ClockTime.Hours = 0;
}

void InitTimeSet(TimeSetParameters TimeSet) {
	TimeSet.Active = false;
	TimeSet.TimeSetTimeStamp = 0;
	TimeSet.BTNCNumOfPushs = None;
	TimeSet.CurrentState = None;
	TimeSet.DisplayFirstTimeOnNone = true;
	TimeSet.FirstUpdate = true;
	TimeSet.PushTimeStamp = 0;
	TimeSet.NeedToChangeDisplay = true;
}

void InitStopper() {
	Stopper.Seconds = 0;
	Stopper.Hundredths = 0;
	Stopper.Active = false;
}

void UpdateTime(long CurrentTime) {
	UpdateSeconds(CurrentTime);
	UpdateMinutes(CurrentTime);
	UpdateHours(CurrentTime);
}

void UpdateSeconds(long CurrentTime) {
	if (CurrentTime % 1000 == 0) {
		ClockTime.Seconds++;
	}
}
void UpdateMinutes(long CurrentTime) {
	if (ClockTime.Seconds % 60 == 0) {
		ClockTime.Minutes++;
		ClockTime.Seconds = 0;
	}
}

void UpdateHours(long CurrentTime) {
	if (ClockTime.Minutes % 60 == 0) {
		ClockTime.Hours++;
		ClockTime.Minutes = 0;
		if (ClockTime.Hours == 24) {
			ClockTime.Hours == 0;
		}
	}
}

void DisplayTime(long CurrentTime) {
	// todo
	if (CurrentTime % 1000 == 0) { // need to update seconds
		Asarot = Time.Seconds / 10;
		Ahadot = Time.Seconds % 10;
		// write to seconds...
	}
	if (Time.Seconds == 0) { // need to update minutes
		// write to minutes...
	}
	if (Time.Minutes == 0) { // need to update hours
		// write to hours...
	}
}

void HandleTimeSet(long CurrentTime, TimeParameters Time, TimeSetParameters TimeSet, bool IsAlarm) {

	UpdateBTNCNumOfPushsInSetTime(TimeSet);
	CheckIfNeedToChangeDisplay(CurrentTime);

	if (TimeSet.DisplayFirstTimeOnNone) {
		DisplayTheRightSetClock(On, On, On, IsAlarm);
		TimeSet.DisplayFirstTimeOnNone = false;
	}
	if (NeedToChangeDisplay) {
		switch (TimeSet.BTNCNumOfPushsInSetTime) {
		case HH:
			DisplayTheRightSetClock(CurrentState, On, On, IsAlarm);
			break;
		case MM:
			DisplayTheRightSetClock(On, CurrentState, On, IsAlarm);
			break;
		case SS:
			DisplayTheRightSetClock(On, On, CurrentState, IsAlarm);
			break;
		}
	}

	if (TimeSet.BTNCNumOfPushsInSetTime != None) {
		if (BTN_GetValue(3)) { // when button BTNR is pushed
			HandleLongPush(Time, TimeSet, true);
		}
		else if (BTN_GetValue(1)) { // when button BTNL is pushed
			HandleLongPush(Time, TimeSet, false);
		}
		else {
			TimeSet.FirstUpdate = true;
		}
	}
}

void UpdateBTNCNumOfPushsInSetTime(TimeSetParameters TimeSet) {
	if (BTN_GetValue(2)) { // when button BTNC is pushed
		while (!BTN_GetValue(2)) {} // wait for user to release button
		TimeSet.BTNCNumOfPushsInSetTime = (TimeSet.BTNCNumOfPushsInSetTime + 1) % 4;
		if (TimeSet.BTNCNumOfPushsInSetTime == None) {
			TimeSet.DisplayFirstTimeOnNone = true;
		}
	}
}

void CheckIfNeedToChangeDisplay(long CurrentTime, TimeSetParameters TimeSet) {
	if (CurrentTime - TimeSet.TimeSetTimeStamp % (SECOND_IN_MS / 2) == 0 && TimeSet.BTNCNumOfPushsInSetTime != None) { // todo verify that half second
		TimeSet.NeedToChangeDisplay = true;
		if (TimeSet.CurrentState == On) {
			TimeSet.CurrentState = Off;
		}
		else {
			TimeSet.CurrentState = On;
		}
	}
}

void DisplayTheRightSetClock(StateEnum HH, StateEnum MM, StateEnum SS, bool IsAlarm) {
	if (!IsAlarm) {
		DisplaySetTime(HH, MM, SS);
	}
	else {
		DisplayAlarmTime(HH, MM, SS);
	}
}

void DisplaySetTime(StateEnum HH, StateEnum MM, StateEnum SS) {
	// todo display SET HH:MM:SS
	// display SET
	if (HH == On) {
		// display HH of clock (ClockTime.Hours...)
	}
	else {
		// put
	}
}

void DisplayAlarmTime(StateEnum HH, StateEnum MM, StateEnum SS) {
	// todo display ALARM HH:MM:SS
	// display ALARM
	if (HH == On) {
		// display HH of alarm (AlarmTime.Hours...)
	}
	else {
		// put
	}
}

void HandleLongPush(TimeParameters Time, TimeSetParameters TimeSet, bool Increase) {
	if (TimeSet.FirstUpdate) {
		UpdateClockByOne(Time, TimeSet, Increase);
		TimeSet.FirstUpdate = false;
		TimeSet.PushTimeStamp = CurrentTime;
	}
	if (CurrentTime - TimeSet.PushTimeStamp == TIME_SET_INITIAL_GAP) {
		UpdateClockByOne(Time, TimeSet, Increase);
	}
	if (CurrentTime - TimeSet.PushTimeStamp > TIME_SET_INITIAL_GAP &&
		(CurrentTime - TimeSet.PushTimeStamp - TIME_SET_INITIAL_GAP) % TIME_SET_SPEED_GAP == 0) {
		UpdateClockByOne(Time, TimeSet, Increase);
	}
}

void UpdateClockByOne(TimeParameters Time, TimeSetParameters TimeSet, bool Increase) {
	int AmountToAdd = Increase ? 1 : -1;
	switch (TimeSet.CurrentState) {
	case HH:
		Time.Hours += AmountToAdd;
		Time.Hours = Time.Hours == 24 ? 0 :
					 Time.Hours == -1 ? 23 : Time.Hours;
		break;
	case MM:
		Time.Minutes += AmountToAdd;
		Time.Minutes = Time.Minutes == 60 ? 0 :
					   Time.Minutes == -1 ? 59 : Time.Minutes;
		break;
	case SS:
		Time.Seconds += AmountToAdd;
		Time.Seconds = Time.Seconds == 60 ? 0 :
					   Time.Seconds == -1 ? 59 : Time.Seconds;
		break;
	}
}

void HandleStopper(long CurrentTime) {
	if (BTN_GetValue(1)) { // when button BTNL is pushed
		while (!BTN_GetValue(1)) {} // wait for user to release button
		ResetStopper();
		Stopper.Active = false;
	}
	if (BTN_GetValue(3)) { // when button BTNR is pushed
		while (!BTN_GetValue(3)) {} // wait for user to release button
		Stopper.TimeStamp = CurrentTime;
		Stopper.Active = !Stopper.Active;
		DisplayStopper();
	}
	if (Stopper.Active && CurrentTime - Stopper.TimeStamp % HUNDREDTH_IN_MS == 0) {
		UpdateStopper();
		DisplayStopper();
	}
}

void ResetStopper() {
	Stopper.Seconds = 0;
	Stopper.Hundredths = 0;
}

void UpdateStopper() {
	Stopper.Hundredths++;
	Stopper.Hundredths = Stopper.Hundredths == 100 ? 0 : Stopper.Hundredths;

	Stopper.Seconds = Stopper.Hundredths == 0 ? Stopper.Seconds + 1 : Stopper.Seconds;
	Stopper.Seconds = Stopper.Seconds == 100 ? 0 : Stopper.Seconds;
}

void DisplayStopper() {
	// todo display
}