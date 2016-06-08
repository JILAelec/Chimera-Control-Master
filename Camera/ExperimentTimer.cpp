#include "stdafx.h"
#include "ExperimentTimer.h"
#include "externals.h"
#include "constants.h"
#include "Commctrl.h"
#include "Uxtheme.h"
#include <vector>
#include "reorganizeControl.h"
#include "fonts.h"
ExperimentTimer::ExperimentTimer()
{
	timeColorID = ID_BLUE;
	// nothing
}
ExperimentTimer::~ExperimentTimer()
{
	// nothing
}

int ExperimentTimer::initializeControls(POINT& topLeftPositionKinetic, POINT& topLeftPositionAccumulate, POINT& topLeftPositionContinuous, HWND parentWindow, bool isTriggerModeSensitive)
{
	timeDisplay.kineticSeriesModePos = { topLeftPositionKinetic.x, topLeftPositionKinetic.y, topLeftPositionKinetic.x + 168, topLeftPositionKinetic.y + 40 };
	timeDisplay.accumulateModePos = { topLeftPositionAccumulate.x, topLeftPositionAccumulate.y, topLeftPositionAccumulate.x + 168,
		topLeftPositionAccumulate.y + 40 };
	timeDisplay.continuousSingleScansModePos = { topLeftPositionContinuous.x, topLeftPositionContinuous.y, topLeftPositionContinuous.x + 168,
		topLeftPositionContinuous.y + 40 };
	RECT initPos = timeDisplay.kineticSeriesModePos;
	timeDisplay.hwnd = CreateWindowEx(0, "EDIT", "", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
		initPos.left, initPos.top, initPos.right - initPos.left, initPos.bottom - initPos.top,
		parentWindow, (HMENU)IDC_TIME_DISPLAY, eHInst, NULL);
	timeDisplay.fontType = "Normal";

	/// PROGRESS BARS
	// subseries progress bar
	variationProgress.kineticSeriesModePos = { topLeftPositionKinetic.x + 168, topLeftPositionKinetic.y, topLeftPositionKinetic.x + 1168, topLeftPositionKinetic.y + 15 };
	variationProgress.accumulateModePos = { topLeftPositionAccumulate.x + 168, topLeftPositionAccumulate.y, topLeftPositionAccumulate.x + 1168,
		topLeftPositionAccumulate.y + 15 };
	variationProgress.continuousSingleScansModePos = { topLeftPositionContinuous.x + 168, topLeftPositionContinuous.y, topLeftPositionContinuous.x + 1168,
		topLeftPositionContinuous.y + 15 };
	initPos = variationProgress.kineticSeriesModePos;

	variationProgress.hwnd = CreateWindowEx(0, PROGRESS_CLASS, 0, WS_CHILD | WS_VISIBLE,
		initPos.left, initPos.top, initPos.right - initPos.left, initPos.bottom - initPos.top, parentWindow, (HMENU)IDC_SUBSERIES_PROGRESS_BAR, eHInst, NULL);
	SetWindowTheme(variationProgress.hwnd, 0, 0);
	SendMessage(variationProgress.hwnd, (UINT)PBM_SETBKCOLOR, 0, RGB(100, 110, 100));
	SendMessage(variationProgress.hwnd, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(0, 200, 0));
	// series progress bar display
	overallProgress.kineticSeriesModePos = { topLeftPositionKinetic.x + 168, topLeftPositionKinetic.y + 15, topLeftPositionKinetic.x + 1168, topLeftPositionKinetic.y + 40 };
	overallProgress.accumulateModePos = { topLeftPositionAccumulate.x + 168, topLeftPositionAccumulate.y + 15, topLeftPositionAccumulate.x + 1168,
		topLeftPositionAccumulate.y + 40 };
	overallProgress.continuousSingleScansModePos = { topLeftPositionContinuous.x + 168, topLeftPositionContinuous.y + 15, topLeftPositionContinuous.x + 1168,
		topLeftPositionContinuous.y + 40 };
	initPos = overallProgress.kineticSeriesModePos;
	overallProgress.hwnd = CreateWindowEx(0, PROGRESS_CLASS, 0, WS_CHILD | WS_VISIBLE,
		initPos.left, initPos.top, initPos.right - initPos.left, initPos.bottom - initPos.top, parentWindow, (HMENU)IDC_SERIES_PROGRESS_BAR, eHInst, NULL);
	SetWindowTheme(overallProgress.hwnd, 0, 0);
	SendMessage(overallProgress.hwnd, (UINT)PBM_SETBKCOLOR, 0, RGB(100, 110, 100));
	SendMessage(overallProgress.hwnd, (UINT)PBM_SETBARCOLOR, 0, (LPARAM)RGB(255, 255, 255));
	
	topLeftPositionKinetic.y += 40;
	topLeftPositionAccumulate.y += 40;
	topLeftPositionContinuous.y += 40;

	return 0;
}

int ExperimentTimer::update(int currentAccumulationNumber, int accumulationsPerVariation, int numberOfVariations, HWND parentWindow)
{
	int totalRepetitions = accumulationsPerVariation * numberOfVariations;
	int minAverageNumber, maxAverageNumber = 1000;
	if (numberOfVariations < 3)
	{
		minAverageNumber = totalRepetitions / 20 + 1;
	}
	else
	{
		minAverageNumber = accumulationsPerVariation / 2 + 1;
	}
	int variationPosition = (currentAccumulationNumber % accumulationsPerVariation) * 100.0 / accumulationsPerVariation;
	int overalPosition = currentAccumulationNumber / (double)totalRepetitions * 100;
	SendMessage(variationProgress.hwnd, PBM_SETPOS, (WPARAM)variationPosition, 0);
	SendMessage(overallProgress.hwnd, PBM_SETPOS, (WPARAM)overalPosition, 0);
	if (currentAccumulationNumber == 1)
	{
		timeColorID = ID_GREEN;
		recentDataPoints.resize(maxAverageNumber);
		lastTime = GetTickCount64();
		RECT parentRectangle;
		GetWindowRect(parentWindow, &parentRectangle);
		timeDisplay.fontType = "Normal";
		reorganizeControl(timeDisplay, "Kinetic Series Mode", parentRectangle);
		SendMessage(timeDisplay.hwnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"Estimating Time...");
	}
	else if (currentAccumulationNumber <= minAverageNumber)
	{
		long long thisTime = GetTickCount64();
		recentDataPoints[currentAccumulationNumber % minAverageNumber] = thisTime - lastTime;
		lastTime = thisTime;
	}
	else if (currentAccumulationNumber == minAverageNumber + 1)
	{

		RECT parentRectangle;
		GetWindowRect(parentWindow, &parentRectangle);
		timeDisplay.fontType = "Large";
		reorganizeControl(timeDisplay, "Kinetic Series Mode", parentRectangle);

		long long thisTime = GetTickCount64();
		recentDataPoints[currentAccumulationNumber] = thisTime - lastTime;
		lastTime = thisTime;
		// only update at the beginning of experiments
		if (currentAccumulationNumber % ePicturesPerRepetition == 0)
		{
			double total = 0;
			for (int timesInc = 0; timesInc < currentAccumulationNumber; timesInc++)
			{
				total += recentDataPoints[timesInc];
			}
			double averageTime = total / currentAccumulationNumber;
			// in seconds...
			int timeLeft = (accumulationsPerVariation * numberOfVariations - currentAccumulationNumber) * averageTime / 1000;
			int hours = timeLeft / 3600;
			int minutes = (timeLeft % 3600) / 60;
			int seconds = (timeLeft % 3600) % 60;
			std::string timeString;
			timeString += std::to_string(hours) + ":";
			if (minutes < 10)
			{
				timeString += "0" + std::to_string(minutes);
			}
			else
			{
				timeString += std::to_string(minutes);
			}
			if (hours == 0 && minutes < 5)
			{
				if (seconds < 10)
				{
					timeString += ":0" + std::to_string(seconds);
				}
				else
				{
					timeString += ":" + std::to_string(seconds);
				}
			}
			SendMessage(timeDisplay.hwnd, WM_SETTEXT, (WPARAM)0, (LPARAM)timeString.c_str());
		}
	}
	else if (currentAccumulationNumber < maxAverageNumber)
	{
		long long thisTime = GetTickCount64();
		recentDataPoints[currentAccumulationNumber] = thisTime - lastTime;
		lastTime = thisTime;
		if (currentAccumulationNumber % ePicturesPerRepetition == 0)
		{
			double total = 0;
			for (int timesInc = 0; timesInc < currentAccumulationNumber; timesInc++)
			{
				total += recentDataPoints[timesInc];
			}
			double averageTime = total / currentAccumulationNumber;
			// in seconds...
			int timeLeft = (accumulationsPerVariation * numberOfVariations - currentAccumulationNumber) * averageTime / 1000;
			int hours = timeLeft / 3600;
			int minutes = (timeLeft % 3600) / 60;
			int seconds = (timeLeft % 3600) % 60;
			std::string timeString;
			timeString += std::to_string(hours) + ":";
			if (minutes < 10)
			{
				timeString += "0" + std::to_string(minutes);
			}
			else
			{
				timeString += std::to_string(minutes);
			}
			if (hours == 0 && minutes < 5)
			{
				if (seconds < 10)
				{
					timeString += ":0" + std::to_string(seconds);
				}
				else
				{
					timeString += ":" + std::to_string(seconds);
				}
			}
			SendMessage(timeDisplay.hwnd, WM_SETTEXT, (WPARAM)0, (LPARAM)timeString.c_str());
		}
	}
	else if (currentAccumulationNumber < totalRepetitions - 1)
	{
		long long thisTime = GetTickCount64();
		recentDataPoints[currentAccumulationNumber % maxAverageNumber] = thisTime - lastTime;
		lastTime = thisTime;
		if (currentAccumulationNumber % ePicturesPerRepetition == 0)
		{
			double total = 0;
			for (int timesInc = 0; timesInc < recentDataPoints.size(); timesInc++)
			{
				total += recentDataPoints[timesInc];
			}
			double averageTime = total / recentDataPoints.size();
			// in seconds...
			int timeLeft = (accumulationsPerVariation * numberOfVariations - currentAccumulationNumber) * averageTime / 1000;
			int hours = timeLeft / 3600;
			int minutes = (timeLeft % 3600) / 60;
			int seconds = (timeLeft % 3600) % 60;
			std::string timeString;
			timeString += std::to_string(hours) + ":";
			if (minutes < 10)
			{
				timeString += "0" + std::to_string(minutes);
			}
			else
			{
				timeString += std::to_string(minutes);
			}
			if (hours == 0 && minutes < 5)
			{
				if (seconds < 10)
				{
					timeString += ":0" + std::to_string(seconds);
				}
				else
				{
					timeString += ":" + std::to_string(seconds);
				}
			}
			SendMessage(timeDisplay.hwnd, WM_SETTEXT, (WPARAM)0, (LPARAM)timeString.c_str());
		}

	}
	else if (currentAccumulationNumber < totalRepetitions)
	{
		long long thisTime = GetTickCount64();
		recentDataPoints[currentAccumulationNumber % minAverageNumber] = thisTime - lastTime;
		lastTime = thisTime;
		double total = 0;
		for (int timesInc = 0; timesInc < recentDataPoints.size(); timesInc++)
		{
			total += recentDataPoints[timesInc];
		}
		double averageTime = total / recentDataPoints.size();
		// in seconds...
		int timeLeft = (totalRepetitions - currentAccumulationNumber) * averageTime / 1000;
		int hours = timeLeft / 3600;
		int minutes = (timeLeft % 3600) / 60;
		int seconds = (timeLeft % 3600) % 60;
		std::string timeString;
		timeString += std::to_string(hours) + ":";
		if (minutes < 10)
		{
			timeString += "0" + std::to_string(minutes);
		}
		else
		{
			timeString += std::to_string(minutes);
		}
		if (hours == 0 && minutes < 5)
		{
			if (seconds < 10)
			{
				timeString += ":0" + std::to_string(seconds);
			}
			else
			{
				timeString += ":" + std::to_string(seconds);
			}
		}
		SendMessage(timeDisplay.hwnd, WM_SETTEXT, (WPARAM)0, (LPARAM)timeString.c_str());
	}
	else
	{
		SendMessage(timeDisplay.hwnd, WM_SETTEXT, (WPARAM)0, (LPARAM)"Fin.");
		timeColorID = ID_BLUE;
	}
	return 0;
}

int ExperimentTimer::reorganizeControls(RECT parentRectangle, std::string mode)
{
	reorganizeControl(timeDisplay, mode, parentRectangle);
	reorganizeControl(variationProgress, mode, parentRectangle);
	reorganizeControl(overallProgress, mode, parentRectangle);
	return 0;
}

int ExperimentTimer::getColorID(void)
{
	return timeColorID;
}

void ExperimentTimer::setColorID(int newColorID)
{
	timeColorID = newColorID;
	UpdateWindow(timeDisplay.hwnd);
	//RedrawWindow(timeDisplay.hwnd);
}
int ExperimentTimer::setTimerDisplay(std::string newText)
{
	SendMessage(timeDisplay.hwnd, WM_SETTEXT, 0, (LPARAM)newText.c_str());
	return 0;
}