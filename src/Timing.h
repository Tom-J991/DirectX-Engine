#pragma once

#include "Common.h"
#include <Windows.h>

#define GameTime (Timing::Instance())

class Timing
{
private:
	double currentTimeInSeconds;
	float deltaTime;

	LONGLONG startPerfCount;
	LONGLONG perfCounterFreq;

public:
	void Create();
	void Destroy();

	void CalculateTimings();

	double GetTime() { return currentTimeInSeconds; }
	float GetDelta() { return deltaTime; }

private:
	Timing() { }

public:
	// Singleton Design Pattern
	static Timing& Instance()
	{
		static Timing instance;
		return instance;
	}

	Timing(Timing const&) = delete;
	void operator=(Timing const&) = delete;
};
