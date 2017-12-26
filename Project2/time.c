#include "time.h"

void InitTime(); // was void InitTime(TimeParameters Time);
void InitTimeSet(TimeSetParameters TimeSet);
void InitStopper();
void UpdateSeconds(long CurrentTime);
void UpdateMinutes();
void UpdateHours();
void HandleTimeSet(long CurrentTime, TimeParameters Time, TimeSetParameters TimeSet, bool IsAlarm);
void DisplayTime(long CurrentTime, bool *FirstTime);
void UpdateBTNCNumOfPushsInSetTime(TimeSetParameters TimeSet);
void CheckIfNeedToChangeDisplay(long CurrentTime, TimeSetParameters TimeSet);
//void DisplayTheRightSetClock(StateEnum HH, StateEnum MM, StateEnum SS, bool IsAlarm, long CurrentTime);
void DisplayTuningTime(StateEnum HH, StateEnum MM, StateEnum SS, bool IsAlarm);
//void DisplayAlarmTime(StateEnum HH, StateEnum MM, StateEnum SS, long CurrentTime);
void HandleLongPush(TimeParameters Time, TimeSetParameters TimeSet, bool Increase, long CurrentTime);
void UpdateClockByOne(TimeParameters Time, TimeSetParameters TimeSet, bool Increase);
void HandleStopper(long CurrentTime);
void ResetStopper();
void UpdateStopper();
void DisplayStopper();

void InitTime() { //was void InitTime(TimeParameters Time)
	ClockTime.Seconds = 0;
	ClockTime.Minutes = 0;
	ClockTime.Hours = 0;
	AlarmTime.Seconds = 0;
	AlarmTime.Minutes = 0;
	AlarmTime.Hours = 0;
}

void InitTimeSet(TimeSetParameters TimeSet) {
	TimeSet.Active = false;
	TimeSet.TimeSetTimeStamp = 0;
	TimeSet.BTNCNumOfPushs = None;
	TimeSet.CurrentState = NoState;
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
void UpdateMinutes() {//void UpdateMinutes(long CurrentTime)
	if (ClockTime.Seconds % 60 == 0) {
		ClockTime.Minutes++;
		ClockTime.Seconds = 0;
	}
}

void UpdateHours() { //void UpdateHours(long CurrentTime)
	if (ClockTime.Minutes % 60 == 0) {
		ClockTime.Hours++;
		ClockTime.Minutes = 0;
		if (ClockTime.Hours == 24) {
			ClockTime.Hours == 0;
		}
	}
}

void HandleTimeSet(long CurrentTime, TimeParameters Time, TimeSetParameters TimeSet, bool IsAlarm) {

	UpdateBTNCNumOfPushsInSetTime(TimeSet);
	CheckIfNeedToChangeDisplay(CurrentTime, TimeSet);

	if (TimeSet.DisplayFirstTimeOnNone) {
		DisplayTuningTime(On, On, On, IsAlarm);
		TimeSet.DisplayFirstTimeOnNone = false;
	}
	if (TimeSet.NeedToChangeDisplay) {
		switch (TimeSet.BTNCNumOfPushs) {
		case HH:
			DisplayTuningTime(TimeSet.CurrentState, On, On, IsAlarm);
			break;
		case MM:
			DisplayTuningTime(On, TimeSet.CurrentState, On, IsAlarm);
			break;
		case SS:
			DisplayTuningTime(On, On, TimeSet.CurrentState, IsAlarm);
			break;
		}
	}

	if (TimeSet.BTNCNumOfPushs != None) {
		if (BTN_GetValue(3)) { // when button BTNR is pushed
			HandleLongPush(Time, TimeSet, true, CurrentTime);
		}
		else if (BTN_GetValue(1)) { // when button BTNL is pushed
			HandleLongPush(Time, TimeSet, false, CurrentTime);
		}
		else {
			TimeSet.FirstUpdate = true;
		}
	}
}

void DisplayTime(long CurrentTime, bool *FirstTime) {
	int H1, H2, M1, M2, S1, S2;
	if (*FirstTime) {
		S1 = ClockTime.Seconds / 10;
		S2 = ClockTime.Seconds % 10;
		LCD_WriteStringAtPos(S2, 0, 16);
		LCD_WriteStringAtPos(S1, 0, 15);
		M1 = ClockTime.Minutes / 10;
		M2 = ClockTime.Minutes % 10;
		LCD_WriteStringAtPos(M2, 0, 19);
		LCD_WriteStringAtPos(M1, 0, 18);
		H1 = ClockTime.Hours / 10;
		H2 = ClockTime.Hours % 10;
		LCD_WriteStringAtPos(H2, 0, 22);
		LCD_WriteStringAtPos(H1, 0, 21);
		LCD_WriteStringAtPos(":", 0, 20);
		LCD_WriteStringAtPos(":", 0, 17);
		*FirstTime = false;
	}
	else {
		if (CurrentTime % 1000 == 0) {   // need to update seconds
			S1 = ClockTime.Seconds / 10;
			S2 = ClockTime.Seconds % 10;
			LCD_WriteStringAtPos(S2, 0, 16);
			LCD_WriteStringAtPos(S1, 0, 15);
		}
		if (ClockTime.Seconds == 0) {    // need to update minutes
			M1 = ClockTime.Minutes / 10;
			M2 = ClockTime.Minutes % 10;
			LCD_WriteStringAtPos(M2, 0, 19);
			LCD_WriteStringAtPos(M1, 0, 18);
		}
		if (ClockTime.Minutes == 0) {    // need to update hours
			H1 = ClockTime.Hours / 10;
			H2 = ClockTime.Hours % 10;
			LCD_WriteStringAtPos(H2, 0, 22);
			LCD_WriteStringAtPos(H1, 0, 21);
		}
	}
}

void UpdateBTNCNumOfPushsInSetTime(TimeSetParameters TimeSet) {
	if (BTN_GetValue(2)) { // when button BTNC is pushed
		while (!BTN_GetValue(2)) {} // wait for user to release button
		TimeSet.BTNCNumOfPushs = (TimeSet.BTNCNumOfPushs + 1) % 4;
		if (TimeSet.BTNCNumOfPushs == None) {
			TimeSet.DisplayFirstTimeOnNone = false; // was true
		}
	}
}

void CheckIfNeedToChangeDisplay(long CurrentTime, TimeSetParameters TimeSet) {
	if (CurrentTime - TimeSet.TimeSetTimeStamp % (SECOND_IN_MS / 2) == 0 && TimeSet.BTNCNumOfPushs != None) { // todo verify that half second
		TimeSet.NeedToChangeDisplay = true;
		if (TimeSet.CurrentState == On) {
			TimeSet.CurrentState = Off;
		}
		else {
			TimeSet.CurrentState = On;
		}
	}
}

void DisplayTuningTime(StateEnum HH, StateEnum MM, StateEnum SS, bool IsAlarm) {
	// todo display SET HH:MM:SS
	int H1, H2, M1, M2, S1, S2;
	TimeParameters Time = IsAlarm ? AlarmTime : ClockTime;
	if (IsAlarm) {
		LCD_WriteStringAtPos("SET", 0, 9);
	}
	else {
		LCD_WriteStringAtPos("ALARM", 0, 7);
	}
	LCD_WriteStringAtPos(":", 0, 20);
	LCD_WriteStringAtPos(":", 0, 17);
	if (HH == On) {
		H1 = Time.Hours / 10;
		H2 = Time.Hours % 10;
		LCD_WriteStringAtPos(H2, 0, 22);
		LCD_WriteStringAtPos(H1, 0, 21);
	}
	else {
		LCD_WriteStringAtPos("", 0, 22);
		LCD_WriteStringAtPos("", 0, 21);
	}
	if (MM == On) {
		M1 = Time.Minutes / 10;
		M2 = Time.Minutes % 10;
		LCD_WriteStringAtPos(M2, 0, 19);
		LCD_WriteStringAtPos(M1, 0, 18);
	}
	else {
		LCD_WriteStringAtPos("", 0, 19);
		LCD_WriteStringAtPos("", 0, 18);
	}
	if (HH == On) {
		S1 = Time.Seconds / 10;
		S2 = Time.Seconds % 10;
		LCD_WriteStringAtPos(S2, 0, 16);
		LCD_WriteStringAtPos(S1, 0, 15);
	}
	else {
		LCD_WriteStringAtPos("", 0, 16);
		LCD_WriteStringAtPos("", 0, 15);
	}
}

void HandleLongPush(TimeParameters Time, TimeSetParameters TimeSet, bool Increase, long CurrentTime) {
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
	int Q1, Q2, S1, S2;
	S1 = Stopper.Seconds / 10;
	S2 = Stopper.Seconds % 10;
	Q1 = Stopper.Hundredths / 10;
	Q2 = Stopper.Hundredths % 10;
	SSD_WriteDigits(Q2, Q1, S2, S1, 1, 1, 1, 1);
}