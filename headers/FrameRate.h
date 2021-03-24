#pragma once

//#include "library.h"
#include "Draw.h"
#include "Window.h"

struct Chrono {
private:
	Uint32 start_c = NULL;
	Uint32 stop_c = NULL;
public:
	bool set(const Uint32& end)
	{
		start_c = clock(); // gets current time
		if (stop_c == NULL)
		{
			stop_c = start_c + end; // inits chrono
		}
		if (start_c > stop_c)
		{
			start_c = NULL;
			stop_c = NULL;
			return true; // returns 1 if time is elapsed
		}
		return false; // returns 0 in case chrono is still runnning
	}
	bool isElapsed(void) {
		start_c = clock(); // gets current time
		if (start_c > stop_c)
		{
			start_c = NULL;
			stop_c = NULL;
			return true; // returns 1 if time is elapsed
		}
		return false; // returns 0 in case chrono is still runnning
	}
};

class StableFramerate {
private:
	Uint32 frameCounter = 0; // increments by 1 with each frame
	Uint32 nextTime = NULL; // next time a frame is rendered
	Chrono chrono; // wait for 1000
	short int frames = NULL; // adds 1 per frame
	short int framerate = NULL;
	Uint32 now = NULL; // SDL_GetTicks()
public:
	void stabilizeCalculationAndRendering(const double& TickInterval = 0) // limit calculation and rendering
	{
		if (TickInterval == 60) nextTime += 17;
		else if (TickInterval == 25)nextTime += 43.65954;
		else { nextTime += 1.621059 + 181237898.4 / (1 + pow(TickInterval / 0.0000460458, 1.156912)); }
		now = clock(); // current time in SDL_Ticks
		if (nextTime > now) {
			SDL_Delay(Uint32(nextTime - now)); // time to wait before rendering next frame
		}
	}
	void stabilizeRendering(Window* window, double TickInterval = 0) // limit rendering
	{
		// optimized formula with approximative constants
		if (TickInterval == 60) { TickInterval = 17; }
		else { TickInterval = 1.621059 + (181237898 / (1 + pow(TickInterval / 0.0000460458, 1.156912))); }
		now = SDL_GetTicks(); // current time in SDL_Ticks
		if (nextTime == 0) { nextTime = now + TickInterval; }
		if (nextTime < now) {
			window->RenderScreen();
			nextTime = 0;
		}
	}
	void render(const int& X, const int& Y, SDL_Renderer* renderer)
	{
		frames++;
		if (chrono.set(1000)) {
			framerate = frames; // return the framerate after 1 sec
			frames = 0;
		}
		Draw::RenderDigits(X, Y, 8, framerate, red, renderer);
	}
	void newFrame(void) {
		frameCounter++;
	}
};
