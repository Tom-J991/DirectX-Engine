#include "Timing.h"

void Timing::Create()
{
	startPerfCount = 0;
	perfCounterFreq = 0;
	{
		LARGE_INTEGER perfCount;
		QueryPerformanceCounter(&perfCount);
		startPerfCount = perfCount.QuadPart;
		LARGE_INTEGER perfFreq;
		QueryPerformanceFrequency(&perfFreq);
		perfCounterFreq = perfFreq.QuadPart;
	}
	currentTimeInSeconds = 0.0f;
}
void Timing::Destroy()
{ }

void Timing::CalculateTimings()
{
	double previousTimeInSeconds = currentTimeInSeconds;
	LARGE_INTEGER perfCount;
	QueryPerformanceCounter(&perfCount);
	currentTimeInSeconds = (double)(perfCount.QuadPart - startPerfCount) / (double)perfCounterFreq;
	deltaTime = (float)(currentTimeInSeconds - previousTimeInSeconds);

	//if (deltaTime > (1.0f / WINDOW_FPS))
		//deltaTime = (1.0f / WINDOW_FPS);
}
