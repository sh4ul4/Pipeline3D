//-------------------------------------------------------------------------------------------
// Free-to-use, cross-platform library
// for advanced IO-streaming in C++
// using Simple Direct-Media Layer 2
// , mostly compatible with C
//-------------------------------------------------------------------------------------------
// Last Updates :	- multithreading for chunk-loading
//					- gaussian blur on textures
//					- fast saturation handling
//					- better audio chunk IO
// Currently Updating : - advanced BMP retouching
//						- fast conversion from HSV to RGB
//						- general code optimization
//-------------------------------------------------------------------------------------------
// Version 2.3
//-------------------------------------------------------------------------------------------
// Made by Louis Schürmann  <louis-blau@orange.fr>
//-------------------------------------------------------------------------------------------
//  HEADERS
#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>
#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <SDL_thread.h>
#include <climits>
#include "Vertex.h"
#include "Point.h"

//  SOUND
SDL_AudioSpec wavSpec_GLOBAL, secWavSpec_GLOBAL;
Uint32 wavLength_GLOBAL, secWavLength_GLOBAL;
Uint8* wavBuffer_GLOBAL, secondWavBuffer_GLOBAL;
unsigned int NumberOfChannels_GLOBAL = 0; // total number of channels
bool blockedChannel_GLOBAL[8] = { 0 };
// needs to be initialized at the beginning of main
void InitMixer(unsigned int numberOfChannels = 8) {
	Mix_AllocateChannels(numberOfChannels); // Define the number of channels
	NumberOfChannels_GLOBAL = numberOfChannels;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		std::cout << "ERROR : " << Mix_GetError() << std::endl;
	}
}

static int unldChnk(void* ptr);
// Chunk-struct
struct Chunk {
	bool loaded_c = false;
	bool reloading = false;
private:
	static void deBlockChannel(int channel_f) {
		if (channel_f > 7 || channel_f < 0) {
			//std::cout << "Channel " << channel_f << " is not initialized in the Mixer.\n";
			return;
		}
		if (blockedChannel_GLOBAL[channel_f]) { // channel blocked
			blockedChannel_GLOBAL[channel_f] = 0; // deblock channel
			//std::cout << "Channel " << channel_f << " finished playing and got deblocked and depaused.\n";
		}
		else {
			//std::cout << "Channel " << channel_f << " is already deblocked.\n";
		}
	}
public:
	Chunk(const char* link = NULL, bool play = NULL, unsigned int level = NULL, unsigned int frequency = NULL, int loop = NULL)
		: link_c(link), play_c(play), level_c(level), frequency_c(frequency), loop_c(loop) {
		chunk_c = Mix_LoadWAV(link);
		loaded_c = true;
		if (!chunk_c) {
			printf("Mix_LoadWAV %s: %d %s\n", link_c, channelNumber_c, Mix_GetError());
			// handle error
			loaded_c = false;
		}
	}
	~Chunk() {}
	void unloadChunk(void) {
		//std::cout << loaded_c << std::endl;
		if (loaded_c == false) return;
		Mix_FreeChunk(chunk_c);
		//std::cout << "unloaded worked" << std::endl;
		loaded_c = false;
		play_c = false;
	}
	void reloadChunk(void) {
		if (loaded_c == true) return;
		if (reloading == true) return;
		reloading = true;
		//int threadReturnValue;
		SDL_Thread* chunkThread = SDL_CreateThread(unldChnk, "unldChnk", this);
		if (NULL == chunkThread) {
			printf("SDL_CreateThread failed: %s\n", SDL_GetError());
		}
		else SDL_DetachThread(chunkThread);
		/*else {
			SDL_WaitThread(chunkThread, &threadReturnValue);
			printf("Thread returned value: %d\n", threadReturnValue);
		}*/
		//chunk_c = Mix_LoadWAV(link_c);
	}
	bool isLoaded(void) const { return loaded_c; }
	void playChunk(int loop, bool block) // loop = 0 for single play // if channel is paused you need to use resume
	{
		if (loaded_c == false) return;
		if (channelNumber_c >= 0 && Mix_Paused(channelNumber_c)) {
			//std::cout << "Channel " << channelNumber_c << " is paused. ResumeChunk() is expected.\n";
			return;
		}
		for (int i = 0; i < 8; i++) {
			if (!Mix_Playing(i) && !Mix_Paused(i)) { // deblock the non playing and non paused channels
				deBlockChannel(i);
			}
		}
		channelNumber_c = 0; // set back channelNumber to go through all the channels
		while (blockedChannel_GLOBAL[channelNumber_c]) { // channel is blocked
			//std::cout << channelNumber_c << " is blocked.\n";
			channelNumber_c++; // go to next channel
			if (channelNumber_c > 7) { // there are only 8 different channels
				channelNumber_c = -1;
				//std::cout << "All channels are blocked.\n";
				return;
			}
		}
		if (!blockedChannel_GLOBAL[channelNumber_c]) { // channel is not blocked
			if (block) { blockedChannel_GLOBAL[channelNumber_c] = 1; } // block channel
			loop_c = loop;
			Mix_PlayChannel(channelNumber_c, chunk_c, loop_c); // play chunk on channel
			play_c = true;
			//std::cout << "Channel " << channelNumber_c << " started playing.\n";
			return;
		}
	}
	void fadeIn(Chunk chunk, int fadingTime, int block) // fades a specific chunk in and starts it
	{
		if (channelNumber_c >= 0 && Mix_Paused(channelNumber_c)) {
			//std::cout << "Channel " << channelNumber_c << " is paused. ResumeChunk() is expected.\n";
			return;
		}
		for (int i = 0; i < 8; i++) {
			if (!Mix_Playing(i) && !Mix_Paused(i)) { // deblock the non playing and non paused channels
				deBlockChannel(i);
			}
		}
		channelNumber_c = 0; // set back channelNumber to go through all the channels
		while (blockedChannel_GLOBAL[channelNumber_c]) { // channel is blocked
			//std::cout << channelNumber_c << " is blocked.\n";
			channelNumber_c++; // go to next channel
			if (channelNumber_c > 7) { // there are only 8 different channels
				channelNumber_c = -1;
				//std::cout << "All channels are blocked.\n";
				return;
			}
		}
		if (!blockedChannel_GLOBAL[channelNumber_c]) { // channel is not blocked
			if (block) { blockedChannel_GLOBAL[channelNumber_c] = 1; } // block channel
			Mix_FadeInChannel(chunk.channelNumber_c, chunk.chunk_c, chunk.loop_c, fadingTime); // play chunk on channel
			//std::cout << "Channel " << channelNumber_c << " started playing.\n";
			return;
		}
	}
	void fadeOut(unsigned int fadingTime) // makes all the chunks of this channel fade out and stops them
	{
		if (channelNumber_c == -1) { std::cout << "Empty channel :  " << channelNumber_c << std::endl; return; }
		Mix_FadeOutChannel(channelNumber_c, fadingTime);
		//std::cout << channelNumber_c << " fading out\n";
		blockedChannel_GLOBAL[channelNumber_c] = 0;
		channelNumber_c = -1; // set to "non playing"
		play_c = false;
	}
	void pauseChunk(void) // Pause a specific Channel
	{
		if (channelNumber_c == -1) { /*std::cout << "Empty channel :  " << channelNumber_c << std::endl;*/ return; }
		else if (Mix_Paused(channelNumber_c)) { std::cout << "Channel  " << channelNumber_c << " is already paused." << std::endl; return; }
		Mix_Pause(channelNumber_c);
		//std::cout << channelNumber_c << " paused\n";
		play_c = false;
	}
	void haltChunk(void) {
		if (channelNumber_c == -1) { /*std::cout << "Empty channel :  " << channelNumber_c << std::endl;*/ return; }
		Mix_HaltChannel(channelNumber_c);
		//std::cout << channelNumber_c << " halted\n";
		blockedChannel_GLOBAL[channelNumber_c] = 0;
		channelNumber_c = -1; // set to "non playing"
		play_c = false;
	}
	void resumeChunk(void) // Resume a paused Channel
	{
		if (channelNumber_c == -1) { /*std::cout << "Empty channel :  " << channelNumber_c << std::endl;*/ return; }
		Mix_Resume(channelNumber_c);
		play_c = true;
	}
	void changeVolume(unsigned int volume) // Change Volume of a Chunk
	{
		if (channelNumber_c == -1) { /*std::cout << "Empty channel :  " << channelNumber_c << std::endl;*/ return; }
		Mix_VolumeChunk(chunk_c, volume);
	}
	Mix_Chunk* chunk_c;
	const char* link_c;
	unsigned int frequency_c;
	bool play_c;
	int loop_c; // 0 for no loop, -1 for infinite loop, else number of loops
	short int channelNumber_c = -1;
	unsigned int level_c; // SoundLevel out of 100
};
static int unldChnk(void* ptr) {
	static_cast<Chunk*>(ptr)->chunk_c = Mix_LoadWAV((const char*)static_cast<Chunk*>(ptr)->link_c);
	static_cast<Chunk*>(ptr)->loaded_c = true;
	if (!static_cast<Chunk*>(ptr)->chunk_c) {
		printf("Mix_LoadWAV %s: %d %s\n", static_cast<Chunk*>(ptr)->link_c, static_cast<Chunk*>(ptr)->channelNumber_c, Mix_GetError());
		// handle error
		static_cast<Chunk*>(ptr)->loaded_c = false;
	}
	static_cast<Chunk*>(ptr)->reloading = false;
	return 0;
}
std::vector<Chunk> chunk_GLOBAL; // dynamic array of textures
bool AddChunk(const char* path) // add a new texture at the end of the array
{
	if (path == nullptr) { std::cout << "Wrong path name.\n"; return 1; } // error
	chunk_GLOBAL.push_back({ path,0,0,0,0 }); // enhance the array size by one
	return 0; // no error
}
// Music-struct
struct Music {
	Music(const char* link, int play, unsigned int level, unsigned int frequency, int loop)
		: link_c(link), play_c(play), frequency_c(frequency), loop_c(loop) {
		music_c = Mix_LoadMUS(link_c);
	}
	~Music() {}
	Mix_Music* music_c;
	const char* link_c;
	unsigned int frequency_c;
	bool play_c;
	int loop_c;
	void playMusic(int loop)
	{
		loop_c = loop;
		Mix_PlayMusic(music_c, loop_c);
	}
	void pauseMusic(void)
	{
		Mix_PauseMusic();
	}
	void resumeMusic(void)
	{
		Mix_ResumeMusic();
	}
	void fadeIn(unsigned int fadingTime)
	{
		Mix_FadeInMusic(music_c, loop_c, fadingTime);
	}
	void fade_out(unsigned int fadingTime)
	{
		Mix_FadeOutMusic(fadingTime);
	}
	void changeVolume(unsigned int volume)
	{
		Mix_VolumeMusic(volume);
	}
};
//-------------------------------------------------------------------------------------------
//  OUTPUT
struct Color {
	Uint8 r; // Red
	Uint8 g; // Green
	Uint8 b; // Blue
	Uint8 a; // Transparency
	Color(const Uint8 R = NULL, const Uint8 G = NULL, const Uint8 B = NULL, const Uint8 A = 255) : r(R), g(G), b(B), a(A) {}
	~Color() {}
	SDL_Color toSDL_Color() const {
		SDL_Color color;
		color.a = a;
		color.r = r;
		color.g = g;
		color.b = b;
		return color;
	}
	bool operator == (const Color& col) {
		return (col.r == r && col.g == g && col.b == b && col.a == a);
	}
	bool operator != (const Color& col) {
		return (col.r != r || col.g != g || col.b != b || col.a != a);
	}
};
// Basic Colors
const Color white(255, 255, 255);
const Color light_gray(200, 200, 200);
const Color gray(127, 127, 127);
const Color dark_gray(80, 80, 80);
const Color black(0, 0, 0);
const Color red(255, 0, 0);
const Color lime(0, 255, 0);
const Color blue(0, 0, 255);
const Color maroon(128, 0, 0);
const Color green(0, 128, 0);
const Color navy(0, 0, 128);
const Color olive(128, 128, 0);
const Color purple(128, 0, 128);
const Color teal(0, 128, 128);
const Color yellow(255, 255, 0);
const Color magenta(255, 0, 255);
const Color cyan(0, 255, 255);
//-------------------------------------------------------------------------------------------
//  FUNCTIONS
template <class T>
inline const T& Max(const T& first_value, const T& second_value) noexcept // returns maximum
{
	return (first_value > second_value) ? first_value : second_value;
}
template <class T>
inline const T& Min(const T& first_value, const T& second_value) noexcept // returns minimum
{
	return (first_value < second_value) ? first_value : second_value;
}
bool isOnLineSegments(const Point2& P, const Point2& A, const Point2& B, const Point2& C, const Point2& D) {
	const int approx = 0;
	return P.x - approx <= Max(A.x, B.x) && P.x + approx >= Min(A.x, B.x) &&
		P.x - approx <= Max(C.x, D.x) && P.x + approx >= Min(C.x, D.x) &&
		P.y - approx <= Max(A.y, B.y) && P.y + approx >= Min(A.y, B.y) &&
		P.y - approx <= Max(C.y, D.y) && P.y + approx >= Min(C.y, D.y);
}
Point2 lineLineIntersection(const Point2& A, const Point2& B, const Point2& C, const Point2& D)
{
	// Line AB represented as a1x + b1y = c1
	const double a1 = (double)(B.y) - (double)(A.y);
	const double b1 = (double)(A.x) - (double)(B.x);
	const double c1 = a1 * (A.x) + b1 * (A.y);

	// Line CD represented as a2x + b2y = c2
	const double a2 = (double)(D.y) - (double)(C.y);
	const double b2 = (double)(C.x) - (double)(D.x);
	const double c2 = a2 * (C.x) + b2 * (C.y);

	const double determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		// The lines are parallel. This is simplified
		// by returning a pair of FLT_MAX
		return { INT_MAX, INT_MAX };
	}
	else
	{
		int x = (int)((b2 * c1 - b1 * c2) / determinant);
		int y = (int)((a1 * c2 - a2 * c1) / determinant);
		return { x, y };
	}
}
template <class T>
void Swap(T& first_value, T& second_value) noexcept // swap the variable values
{
	const T transition_value = first_value;
	first_value = second_value;
	second_value = transition_value;
}

int RandomInt(int min, int max) // returns a rondam value between given min and max
{
	if (max < min) { const int trans = max; max = min; min = trans; }
	return (rand() % max) + min;
}

#include "Texture.h"