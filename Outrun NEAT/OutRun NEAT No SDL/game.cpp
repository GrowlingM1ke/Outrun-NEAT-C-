/*
OneLoneCoder.com - Code-It-Yourself! Racing Game at the command prompt (quick and simple c++)
"Let's go, go, go!!!" - @Javidx9
License
~~~~~~~
Copyright (C) 2018  Javidx9
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions; See license for details.
Original works located at:
https://www.github.com/onelonecoder
https://www.onelonecoder.com
https://www.youtube.com/javidx9
GNU GPLv3
https://github.com/OneLoneCoder/videos/blob/master/LICENSE
From Javidx9 :)
~~~~~~~~~~~~~~~
Hello! Ultimately I don't care what you use this for. It's intended to be
educational, and perhaps to the oddly minded - a little bit of fun.
Please hack this, change it and use it in any way you see fit. You acknowledge
that I am not responsible for anything bad that happens as a result of
your actions. However this code is protected by GNU GPLv3, see the license in the
github repo. This means you must attribute me if you use it. You can view this
license here: https://github.com/OneLoneCoder/videos/blob/master/LICENSE
Cheers!
Background
~~~~~~~~~~
I'm a sim-racer when I'm not coding. Racing games are far more sophisticated than
they used to be. Frankly, retro racing games are a bit naff. But when done in the
command prompt they have a new level of craziness.
Controls
~~~~~~~~
Left Arrow/Right Arrow Steer, Up Arrow accelerates. There are no brakes!
Set the fastest lap times you can!
Author
~~~~~~
Twitter: @javidx9
Blog: www.onelonecoder.com
Video:
~~~~~~
https://youtu.be/KkMZI5Jbf18
Last Updated: 10/07/2017
*/
#include <iostream>
#include <string>
#include "Evaluator.h"
#include "neuralNetwork.h"
#include <sstream>
#include <iomanip>

// TODO have the game be controlled by the neural network and fitness function
using namespace std;

#include "olcConsoleGameEngine.h"

bool generateNetwork = true;

class OneLoneCoder_FormulaOLC : public olcConsoleGameEngine
{
public:
	OneLoneCoder_FormulaOLC()
	{
		m_sAppName = L"Formula OLC";
	}

private:

	// Instatiate the evaluator and Neural Network here
	evaluator eval = evaluator();
	neuralNetwork nn;
	vector<shared_ptr<genome>> currentPop;
	shared_ptr<genome> currentGenome;
	int genomePos = 0;
	int popSize;
	float shortTime;


	float fDistance = 0.0f;			// Distance car has travelled around track
	float fCurvature = 0.0f;		// Current track curvature, lerped between track sections
	float fTrackCurvature = 0.0f;	// Accumulation of track curvature
	float fTrackDistance = 0.0f;	// Total distance of track

	float fCarPos = 0.0f;			// Current car position
	float fOldCarPos = 0.0f;		// Previous position of the car
	float fPlayerCurvature = 0.0f;			// Accumulation of player curvature
	float fSpeed = 0.0f;			// Current player speed
	float fSavedDistance = 0.0f;			    // if the car doesnt move from position after 2 seconds
	float fCarSavedPos = 0;


	// Create roadblock properties
	//float fRoadBlockVerticalPosition = 0.0f;
	//float fRoadBlockHorizontalPosition = 0.0f;
	//bool exists = false;

	// Save the x positions to know when the car and roadblock collided
	int nCarPos = 0;
	// int nRoadBlockPos = 0;

	vector<pair<float, float>> vecTrack; // Track sections, sharpness of bend, length of section

	list<float> listLapTimes;		// List of previous lap times
	float fCurrentLapTime;			// Current lap time

	void randomlyGenerateTrack() {
		// randomly generate track		
		int number = rand() % 3 + 4;

		vecTrack.clear();
		vecTrack.push_back(make_pair(0.0f, 10.0f));		// Short section for start/finish line
		for (int i = 0; i < number; i++) {
			// making it biased for the turns so the player 50% of the time receives more extreme turns
			vecTrack.push_back(make_pair(generateBiasedRand(-1.0f, 1.0f, true), generateBiasedRand(100.0f, 400.0f, false)));
			vecTrack.push_back(make_pair(0.0f, generateBiasedRand(100.0f, 200.0f, false)));
		}

		// Calculate total track distance, so we can set lap times
		for (auto t : vecTrack)
			fTrackDistance += t.second;
	}



	float generateBiasedRand(float min, float max, bool biased) {

		if (biased && 0 + (rand() % (1 - 0 + 1)) == 1) {
			float extremeTrack = 0.75 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.25)));
			if (0 + (rand() % (1 - 0 + 1)) == 1)
				return extremeTrack;
			else return -extremeTrack;
		}
		else {
			return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		}
	}

	// Initiate phase 1 of starting a neural network
	void newPopRun() {
		genomePos = 0;
		eval.evaluate1();
		currentPop.clear();
		currentPop = eval.getGenomes();
		currentGenome = currentPop[genomePos];
		nn = neuralNetwork(*currentGenome);
		popSize = currentPop.size();
	}

	void newRun() {
		fDistance = 0.0f;
		fCurvature = 0.0f;
		fTrackCurvature = 0.0f;

		fCarPos = 0.0f;
		fOldCarPos = 0.0f;
		fPlayerCurvature = 0.0f;
		fSpeed = 0.0f;

		// Create roadblock properties
		//float fRoadBlockVerticalPosition = 0.0f;
		//float fRoadBlockHorizontalPosition = 0.0f;
		//bool exists = false;
		genomePos++;
		currentGenome = currentPop[genomePos];
		nn = neuralNetwork(*currentGenome);

	}

protected:
	// Called by olcConsoleGameEngine
	virtual bool OnUserCreate()
	{
		// Define track
		vecTrack.push_back(make_pair(0.0f, 10.0f));		// Short section for start/finish line
		vecTrack.push_back(make_pair(-1.0f, 200.0f));
		vecTrack.push_back(make_pair(1.0f, 200.0f));
		vecTrack.push_back(make_pair(0.0f, 400.0f));
		vecTrack.push_back(make_pair(-1.0f, 100.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));
		vecTrack.push_back(make_pair(-1.0f, 200.0f));
		vecTrack.push_back(make_pair(1.0f, 200.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));
		vecTrack.push_back(make_pair(0.2f, 500.0f));
		vecTrack.push_back(make_pair(0.0f, 200.0f));

		// Calculate total track distance, so we can set lap times
		for (auto t : vecTrack)
			fTrackDistance += t.second;

		// Instatiate the evaluator with the correct settings
		// Input: fPlayerPos, fCurvature
		// Output: Up down left right
		srand((unsigned)time(NULL));
		eval.initPopulation(2, 4, false);
		newPopRun();

		listLapTimes = { 0,0,0,0,0 };
		fCurrentLapTime = 0.0f;

		return true;
	}




	// Called by olcConsoleGameEngine
	virtual bool OnUserUpdate(float fElapsedTime)
	{
		// Check if the roadblock exists
		/*if (!exists) {
			fRoadBlockVerticalPosition = generateBiasedRand(0.0, 0.7, false);
			fRoadBlockHorizontalPosition = fDistance;
			exists = true;
		}
		else {
			if (fDistance - fRoadBlockHorizontalPosition >= 150.0f)
				exists = false;
		}*/


		// Handle control input
		int nCarDirection = 0;

		// Put the values into the neural network and receive outputs
		vector<float> inputs;
		inputs.push_back(fCarPos);
		inputs.push_back(fCurvature);
		vector<float> outputs = nn.calculate(inputs);

		/*if ((fRoadBlockHorizontalPosition + 75 <= fDistance && fRoadBlockHorizontalPosition + 110 >= fDistance) && (nCarPos + 48 >= nRoadBlockPos && nCarPos +3 <= nRoadBlockPos))
			fSpeed -= 5.0f * fElapsedTime;*/

		if (outputs[0] > 0.5)
			fSpeed += 2.0f * fElapsedTime;
		else if (outputs[1] > 0.5)
			fSpeed -= 2.0f * fElapsedTime;
		else
			fSpeed -= 1.0f * fElapsedTime;

		// Car Curvature is accumulated left/right input, but inversely proportional to speed
		// i.e. it is harder to turn at high speed
		if (outputs[2] > 0.5)
		{
			fPlayerCurvature -= 0.7f * fElapsedTime * (1.0f - fSpeed / 2.0f);
			nCarDirection = -1;
		}

		if (outputs[3] > 0.5)
		{
			fPlayerCurvature += 0.7f * fElapsedTime * (1.0f - fSpeed / 2.0f);
			nCarDirection = +1;
		}

		// If car curvature is too different to track curvature, slow down
		// as car has gone off track
		if (fabs(fPlayerCurvature - fTrackCurvature) >= 0.8f)
			fSpeed -= 5.0f * fElapsedTime;

		// Clamp Speed
		if (fSpeed < 0.0f)	fSpeed = 0.0f;
		if (fSpeed > 1.0f)	fSpeed = 1.0f;

		// Move car along track according to car speed
		fDistance += (70.0f * fSpeed) * fElapsedTime;

		// Get Point on track
		float fOffset = 0;
		int nTrackSection = 0;

		// Lap Timing and counting
		fCurrentLapTime += fElapsedTime;

		// Check if the car moved after 2 seconds
		bool forceReset = false;
		shortTime += fElapsedTime;
		if (shortTime > 0.1f) {
			if (fDistance == fSavedDistance) {
				forceReset = true;
				fCurrentLapTime = 60.0f;
			}
			fSavedDistance = fDistance;
			shortTime = 0.0f;
		}
		// Finished the run reset
		if (fDistance >= fTrackDistance || fCurrentLapTime > 60.0f || forceReset)
		{
			// Save the fitness to the genome
			// currentGenome->setFitness(fDistance + (10000.0f / fCurrentLapTime));
			currentGenome->setFitness(fDistance + 1.0f);
			// fDistance -= fTrackDistance;
			listLapTimes.push_front(fCurrentLapTime);
			listLapTimes.pop_back();
			fCurrentLapTime = 0.0f;
			if (genomePos + 1 == popSize) {
				eval.evaluate2();
				newPopRun();
				return true;
			}
			newRun();
			// exists = false;
			return true;
		}

		// Find position on track (could optimise)
		while (nTrackSection < vecTrack.size() && fOffset <= fDistance)
		{
			fOffset += vecTrack[nTrackSection].second;
			nTrackSection++;
		}

		// Interpolate towards target track curvature
		float fTargetCurvature = vecTrack[nTrackSection - 1].first;
		float fTrackCurveDiff = (fTargetCurvature - fCurvature) * fElapsedTime * fSpeed;

		// Accumulate player curvature
		fCurvature += fTrackCurveDiff;

		// Accumulate track curvature
		fTrackCurvature += (fCurvature)* fElapsedTime * fSpeed;

		// Draw Sky - light blue and dark blue
		for (int y = 0; y < ScreenHeight() / 2; y++)
			for (int x = 0; x < ScreenWidth(); x++)
				Draw(x, y, y < ScreenHeight() / 4 ? PIXEL_HALF : PIXEL_SOLID, FG_DARK_BLUE);

		// Draw Scenery - our hills are a rectified sine wave, where the phase is adjusted by the
		// accumulated track curvature
		for (int x = 0; x < ScreenWidth(); x++)
		{
			int nHillHeight = (int)(fabs(sinf(x * 0.01f + fTrackCurvature) * 16.0f));
			for (int y = (ScreenHeight() / 2) - nHillHeight; y < ScreenHeight() / 2; y++)
				Draw(x, y, PIXEL_SOLID, FG_DARK_YELLOW);
		}


		// Draw Track - Each row is split into grass, clip-board and track
		for (int y = 0; y < ScreenHeight() / 2; y++)
			for (int x = 0; x < ScreenWidth(); x++)
			{
				// Perspective is used to modify the width of the track row segments
				float fPerspective = (float)y / (ScreenHeight() / 2.0f);
				float fRoadWidth = 0.1f + fPerspective * 0.8f; // Min 10% Max 90%
				float fClipWidth = fRoadWidth * 0.15f;
				fRoadWidth *= 0.5f;	// Halve it as track is symmetrical around center of track, but offset...

				// ...depending on where the middle point is, which is defined by the current
				// track curvature.
				float fMiddlePoint = 0.5f + fCurvature * powf((1.0f - fPerspective), 3);

				// Work out segment boundaries
				int nLeftGrass = (fMiddlePoint - fRoadWidth - fClipWidth) * ScreenWidth();
				int nLeftClip = (fMiddlePoint - fRoadWidth) * ScreenWidth();
				int nRightClip = (fMiddlePoint + fRoadWidth) * ScreenWidth();
				int nRightGrass = (fMiddlePoint + fRoadWidth + fClipWidth) * ScreenWidth();

				int nRow = ScreenHeight() / 2 + y;

				// Using periodic oscillatory functions to give lines, where the phase is controlled
				// by the distance around the track. These take some fine tuning to give the right "feel"
				int nGrassColour = sinf(20.0f * powf(1.0f - fPerspective, 3) + fDistance * 0.1f) > 0.0f ? FG_GREEN : FG_DARK_GREEN;
				int nClipColour = sinf(80.0f * powf(1.0f - fPerspective, 2) + fDistance) > 0.0f ? FG_RED : FG_WHITE;
				int nMiddlePointColour = sinf(80.0f * powf(1.0f - fPerspective, 2) + fDistance) > 0.0f ? FG_WHITE : FG_GREY;

				// Start finish straight changes the road colour to inform the player lap is reset
				int nRoadColour = (nTrackSection - 1) == 0 ? FG_WHITE : FG_GREY;

				// Draw the row segments
				if (x >= 0 && x < nLeftGrass)
					Draw(x, nRow, PIXEL_SOLID, nGrassColour);
				if (x >= nLeftGrass && x < nLeftClip)
					Draw(x, nRow, PIXEL_SOLID, nClipColour);
				if (x >= nLeftClip && x < nRightClip)
					Draw(x, nRow, PIXEL_SOLID, nRoadColour);
				if (x >= nRightClip && x < nRightGrass)
					Draw(x, nRow, PIXEL_SOLID, nClipColour);
				if (x >= nRightGrass && x < ScreenWidth())
					Draw(x, nRow, PIXEL_SOLID, nGrassColour);
				if (x >= (int)(fMiddlePoint * ScreenWidth()) && x <= (int)(fMiddlePoint * ScreenWidth()) + 1)
					Draw(x, nRow, PIXEL_SOLID, nMiddlePointColour);
				/*if(x >= (int)(nLeftClip + (fRoadWidth * fRoadBlockVerticalPosition * 380)) && x <= (int)(nLeftClip + (fRoadWidth * fRoadBlockVerticalPosition * 380)) + 1 * (48 * fPerspective) )
					if (y <= (fDistance - fRoadBlockHorizontalPosition) * 0.4 && y >= (fDistance - fRoadBlockHorizontalPosition) * 0.4 - (12 * fPerspective)) {
						Draw(x, nRow, PIXEL_SOLID, FG_BLACK);
						nRoadBlockPos = x;
					}*/
			}

		// Draw Car - car position on road is proportional to difference between
		// current accumulated track curvature, and current accumulated player curvature
		// i.e. if they are similar, the car will be in the middle of the track
		fOldCarPos = fCarPos;
		fCarPos = fPlayerCurvature - fTrackCurvature;
		if (fCarPos < -0.92 || fCarPos > 0.92)
			fCarPos = fOldCarPos;
		nCarPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * fCarPos) / 2.0) - 7; // Offset for sprite

		// Draw a car that represents what the player is doing. Apologies for the quality
		// of the sprite... :-(
		switch (nCarDirection)
		{
		case 0:
			DrawStringAlpha(nCarPos, 80, L"   ||####||   ");
			DrawStringAlpha(nCarPos, 81, L"      ##      ");
			DrawStringAlpha(nCarPos, 82, L"     ####     ");
			DrawStringAlpha(nCarPos, 83, L"     ####     ");
			DrawStringAlpha(nCarPos, 84, L"|||  ####  |||");
			DrawStringAlpha(nCarPos, 85, L"|||########|||");
			DrawStringAlpha(nCarPos, 86, L"|||  ####  |||");
			break;

		case +1:
			DrawStringAlpha(nCarPos, 80, L"      //####//");
			DrawStringAlpha(nCarPos, 81, L"         ##   ");
			DrawStringAlpha(nCarPos, 82, L"       ####   ");
			DrawStringAlpha(nCarPos, 83, L"      ####    ");
			DrawStringAlpha(nCarPos, 84, L"///  ####//// ");
			DrawStringAlpha(nCarPos, 85, L"//#######///O ");
			DrawStringAlpha(nCarPos, 86, L"/// #### //// ");
			break;

		case -1:
			DrawStringAlpha(nCarPos, 80, L"\\\\####\\\\      ");
			DrawStringAlpha(nCarPos, 81, L"   ##         ");
			DrawStringAlpha(nCarPos, 82, L"   ####       ");
			DrawStringAlpha(nCarPos, 83, L"    ####      ");
			DrawStringAlpha(nCarPos, 84, L" \\\\\\\\####  \\\\\\");
			DrawStringAlpha(nCarPos, 85, L" O\\\\\\#######\\\\");
			DrawStringAlpha(nCarPos, 86, L" \\\\\\\\ #### \\\\\\");
			break;
		}

		// Draw Stats
		DrawString(0, 1, L"Player Speed    : " + to_wstring(fSpeed));
		DrawString(0, 2, L"Distance Traveled : " + to_wstring(fDistance));
		DrawString(0, 3, L"fCarPos : " + to_wstring(fCarPos));
		DrawString(0, 4, L"fCurvature : " + to_wstring(fCurvature));
		DrawString(0, 5, L"Generation : " + to_wstring(eval.getCurrentGeneration()));
		DrawString(0, 6, L"Genome ID in generation: " + to_wstring(genomePos));
		DrawString(0, 7, L"Number of Species: " + to_wstring(eval.getNumberOfSpecies()));
		// DrawString(0, 7, L"nRoadBlockPos : " + to_wstring(nRoadBlockPos));

		auto disp_time = [](float t) // Little lambda to turn floating point seconds into minutes:seconds:millis string
		{
			int nMinutes = t / 60.0f;
			int nSeconds = t - (nMinutes * 60.0f);
			int nMilliSeconds = (t - (float)nSeconds) * 1000.0f;
			return to_wstring(nMinutes) + L"." + to_wstring(nSeconds) + L":" + to_wstring(nMilliSeconds);
		};

		// Display current laptime
		DrawString(10, 8, disp_time(fCurrentLapTime));

		// Display last 5 lap times
		int j = 10;
		for (auto l : listLapTimes)
		{
			DrawString(10, j, disp_time(l));
			j++;
		}

		return true;
	}
};


int main()
{
	// Use olcConsoleGameEngine derived app
	OneLoneCoder_FormulaOLC game;
	game.ConstructConsole(100, 100, 6, 6);
	game.Start();




	return 0;
}