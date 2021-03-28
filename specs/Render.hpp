#pragma once

#include "FrameRate.h"
#include "Physics.h"

class Render
{
	StableFramerate framerate; // Framerate de rendering depuis la classe StableFramerate
	std::vector<Triangle*> toRender; // Prochain triangle à render
public:
	GlobalTexture globalTexture; // Texture globale
	Render() = delete; // constructeur vide
	Render(const Window& window):globalTexture(window) {
		SDL_GL_SetSwapInterval(0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		InitMixer(8);
		TTF_Init();
		SDL_ShowCursor(0);
	}
	void updateTriangleSize(const ShapeManager& manager) {
		const size_t size = toRender.size();
		toRender.clear();
		toRender.shrink_to_fit();
		toRender.reserve(size);

		const size_t max1 = manager.shapes.size();
		for (int j = 0; j < max1; j++) {
			const size_t max2 = manager.shapes[j]->triangles.size();
			for (int i = 0; i < max2; i++) {
				toRender.push_back(&manager.shapes[j]->triangles[i]);
			}
		}
		toRender.shrink_to_fit();
	}
private:
	// compare function
	struct PointerCompare {
		bool operator()(const Triangle* l, const Triangle* r) {
			return (l->distanceToCamera > r->distanceToCamera);
		}
	};
	static void threadRendering(const std::vector<Triangle*> toRender, GlobalTexture& globalTexture, const Window& w, int min, int max) {
		for (int i = min; i < max; i++) {
			toRender[i]->set2d(w, true);
			// render triangle
			toRender[i]->render(w, globalTexture);
		}
	}
	// sort the triangles in "toRender" and render them
	void renderTriangles(const Window& window) { // Triage et Rendering des triangles
		if (Camera::currentExists() == false) { std::cout << "Error : current camera does not exist.\n"; exit(1); }
		//std::sort(toRender.begin(), toRender.end(), PointerCompare()); // no need to sort triangles when zbuffer is enabled
		const size_t max3 = toRender.size(); taille du prochain à render
		globalTexture.refreshZbuffer();
		// set all pixels of surface to NULL
		globalTexture.clearPixels();
		// render textures on surface
		// multithreaded version
		/*std::thread thread1(threadfunc1, std::cref(toRender), std::ref(globalTexture), std::cref(window), 0, (int)(max3 / 3));
		std::thread thread2(threadfunc1, std::cref(toRender), std::ref(globalTexture), std::cref(window), (int)(max3 / 3), (int)(max3 / 3 * 2));
		std::thread thread3(threadfunc1, std::cref(toRender), std::ref(globalTexture), std::cref(window), (int)(max3 / 3 * 2), max3);
		thread1.join();
		thread2.join();
		thread3.join();*/
		// signlethreaded version
		for (int i = 0; i < max3; i++) {
			toRender[i]->set2d(window, true);
			// render triangle
			toRender[i]->render(window, globalTexture);
		}
		globalTexture.updateTexture(window);
		//globalTexture.applySobel();
		//globalTexture.applyBlackNWhite();
		globalTexture.renderTexture(window.getRenderer(), { 0,0 }, window.getRenderWidth(), window.getRenderHeight(), 0, 0);
		//toRenderOneFrame.clear();
	}
public:
	void render(InputEvent& inputEvent, const Window& window, const ShapeManager& manager, Bitmap* background = nullptr) {
		inputEvent.update();
		if (Camera::currentExists()) {
			Physics::move(inputEvent, manager);
			Camera::getCurrent().update(inputEvent, window);
			renderTriangles(window);
		}

		//framerate.stabilizeCalculationAndRendering(60);
		//Wait(10);

		framerate.render(10, 10, window.getRenderer());
		Draw::DrawLine({ 0,window.getRenderHeight() / 2 }, { window.getRenderWidth(),window.getRenderHeight() / 2 }, gray,window.getRenderer());
		Draw::DrawLine({ window.getRenderWidth() / 2,0 }, { window.getRenderWidth() / 2,window.getRenderHeight() }, gray, window.getRenderer());
		Draw::DrawCircle(window.getRenderWidth() / 2, window.getRenderHeight() / 2, 5, dark_gray, window.getRenderer());
		window.RenderScreen();
		window.FillScreen(teal);
	}
};