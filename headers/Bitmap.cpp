#include "Bitmap.hpp"

Bitmap::Bitmap(const char* path) {
	if (path == nullptr) { std::cout << "Wrong path name for object of class [Bitmap].\n"; exit(1); }
	SDL_Surface* newsurface = nullptr;
	newsurface = IMG_Load(path);
	if (newsurface == nullptr) { std::cout << "ERROR : [" << path << "] could not be loaded.\n"; exit(1); }
	SDL_PixelFormat* pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB32); // convert the pixel-format of the surface to ARGB32/ARGB8888
	surface = SDL_ConvertSurface(newsurface, pixelFormat, 0);
	SDL_LockSurface(surface);
	SDL_FreeSurface(newsurface);
	SDL_FreeFormat(pixelFormat);
	const char* error_f = SDL_GetError();
	if (*error_f) { std::cout << "Error occured in Bitmap(): " << error_f << std::endl; }
	if (surface == nullptr) { std::cout << "ERROR : surface conversion.\n"; exit(1); }
}

Bitmap::~Bitmap() {
	SDL_UnlockSurface(surface);
	SDL_FreeSurface(surface);
}