//***************************************************************************************
// GameTimer.h by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#pragma once
#include "framework.h"

class GameTimer
{
public:
	GameTimer();

	float TotalTime() const; // in seconds
	float DeltaTime() const; // in seconds

	void Reset(); // Call before message loop.
	void Start(); // Call when unpaused.
	void Stop();  // Call when paused.
	void Tick();  // Call every frame.

private:
	double mSecondsPerCount;
	double mDeltaTime;

	INT64 mBaseTime;
	INT64 mPausedTime;
	INT64 mStopTime;
	INT64 mPrevTime;
	INT64 mCurrTime;

	bool mStopped;
};
