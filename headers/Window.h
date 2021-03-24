#pragma once

#include "library.h"

class Window {
	int width = NULL; unsigned int height = NULL;
	int renderWidth = NULL; unsigned int renderHeight = NULL;
	bool fullScreen = false;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_AudioDeviceID deviceId = NULL;
	bool shuttingDown = false;
public:
	int getWidth()const { return width; }
	int getHeight()const { return height; }
	int getRenderWidth()const { return renderWidth; }
	int getRenderHeight()const { return renderHeight; }
	int getWidthCenter() const { return width / 2; }
	int getHeightCenter() const { return height / 2; }
	SDL_Renderer* getRenderer()const { return renderer; }
	SDL_Window* getWindow() const { return window; }
	Window() = delete;
	Window(const int& resolutionWidth, const int& resolutionHeight, const int& width, const int& height):width(width),height(height),renderWidth(resolutionWidth),renderHeight(resolutionHeight) {
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
		SDL_Init(SDL_INIT_VIDEO);
		SDL_Init(SDL_INIT_TIMER);
		window = SDL_CreateWindow("Pipeline3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, resolutionWidth, resolutionHeight, SDL_WINDOW_OPENGL);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}
	Window(const int& width, const int& height) :width(width), height(height), renderWidth(width), renderHeight(height) {
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
		SDL_Init(SDL_INIT_VIDEO);
		SDL_Init(SDL_INIT_TIMER);
		window = SDL_CreateWindow("Pipeline3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	}
	~Window() {
		Shutdown();
	}
private:
	void Shutdown(void) // Shuts down all the different components
	{
		std::cout << "Prepare to shut down.\n";
		shuttingDown = true;
		//---------------------------------------
		SDLK_CLEAR;
		std::cout << "Keys cleared.\n";
		//---------------------------------------
		Mix_FreeMusic(0);
		Mix_FreeChunk(0);
		std::cout << "Channels cleared.\n";
		//---------------------------------------
		Mix_Quit();
		std::cout << "Mixer cleared.\n";
		//---------------------------------------
		TTF_Quit();
		//---------------------------------------
		if (renderer) SDL_DestroyRenderer(renderer);
		std::cout << "Renderer destroyed.\n";
		//---------------------------------------
		if (window) SDL_DestroyWindow(window);
		std::cout << "Window destroyed.\n";
		//---------------------------------------
		int numclosed = fcloseall();
		printf("Number of files closed by fcloseall() : %u\n", numclosed);
		//---------------------------------------
		std::cout << "Shutdown complete." << std::endl;
	}
public:
	void Wait(const Uint32& time)const // Pause everything for a certain time
	{
		SDL_Delay(time); // takes time in milliseconds
	}
	void FillScreen(const Color& color)const
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); // set the requested color
		SDL_RenderClear(renderer);
	}
	void RenderScreen()const
	{
		SDL_RenderPresent(renderer);
	}
	void ToggleWindow(const int& width, const int& height) // Toggle the window between Fullscreen-mode and Windowed-mode
	{
		fullScreen = !fullScreen;
		SDL_SetWindowResizable(window, SDL_TRUE);
		if (!fullScreen) {
			SDL_DisplayMode DM;
			SDL_GetCurrentDisplayMode(0, &DM);
			this->renderWidth = DM.w;
			this->renderHeight = DM.h;
			SDL_SetWindowSize(window, this->renderWidth, this->renderHeight);
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else if (fullScreen) {
			this->renderWidth = width;
			this->renderHeight = height;
			SDL_SetWindowSize(window, this->renderWidth, this->renderHeight);
			SDL_SetWindowFullscreen(window, 0);
		}
		SDL_SetWindowResizable(window, SDL_FALSE);
	}
};
