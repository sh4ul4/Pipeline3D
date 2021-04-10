#pragma once

/**
 * @file Issu du module de Rendering, sous-module (?) 
 * Rendering et Affichage des triangles
 */

/*===========================================================================================
	Render
===========================================================================================*/

class Render
{
	// Framerate de rendering depuis la classe StableFramerate
	StableFramerate framerate;

	// Prochain triangle à render
	std::vector<Triangle*> toRender;

	// nombre de shapes incluses après la dernière mise-à-jour de toRender
	size_t shapesNumber = 0;

public:
	// Texture globale
	GlobalTexture globalTexture; 

	// Interdiction d'utilisation d'un constructeur vide
	Render() = delete; 

	// Constructeur de la classe Render, appelant le constructeur de globalTexture
	Render(const Window& window) :globalTexture(window) {
		SDL_GL_SetSwapInterval(0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		//window.ToggleWindow(window->getWidth(), window->getHeight());
		//InitMixer(8);
		TTF_Init();
		SDL_ShowCursor(1);
	}

	// Change la taille d'un triangle pour que ça rentre 
	void updateTriangles(const ShapeManager& manager) {
		const size_t size = toRender.size();
		if (shapesNumber == manager.shapes.size()) return;
		toRender.clear();
		toRender.shrink_to_fit();
		toRender.reserve(size);

		const size_t max1 = manager.shapes.size();
		shapesNumber = max1;
		for (int j = 0; j < max1; j++) {
			const size_t max2 = manager.shapes[j]->triangles.size();
			for (int i = 0; i < max2; i++) {
				toRender.push_back(&manager.shapes[j]->triangles[i]);
			}
		}
		toRender.shrink_to_fit();
	}

private:
	// Comparaison de triangles
	struct PointerCompare {
		// Opérateur : fonction de comparaison entre triangles
		bool operator()(const Triangle* l, const Triangle* r) {
			return (l->distanceToCamera > r->distanceToCamera);
		}
	};

	// Fonction de threading pour le rendering
	static void threadRendering(const std::vector<Triangle*> toRender, GlobalTexture& globalTexture, const Window& w, int min, int max) {
		for (int i = min; i < max; i++) {
			toRender[i]->setScreenCoord(w, true);
			// render triangle
			toRender[i]->render(w, globalTexture);
		}
	}

	// Rendering des triangles
	void renderTriangles(const Window& window) {
		if (Camera::currentExists() == false) { std::cout << "Error : current camera does not exist.\n"; exit(1); }
		//std::sort(toRender.begin(), toRender.end(), PointerCompare()); // no need to sort triangles when zbuffer is enabled
		const size_t max3 = toRender.size();
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
			toRender[i]->setScreenCoord(window, true);
			// render triangle
			toRender[i]->render(window, globalTexture);
		}
		//globalTexture.applySobel();
		//globalTexture.applyBlackNWhite();
		globalTexture.updateTexture(window);
		globalTexture.renderTexture(window.getRenderer(), { 0,0 }, window.getRenderWidth(), window.getRenderHeight(), 0, 0);
		//toRenderOneFrame.clear();
	}

public:
	// Rendering écran
	void render(InputEvent& inputEvent, const Window& window, ShapeManager& manager, Bitmap* background = nullptr) {
		inputEvent.update();
		if (Camera::currentExists()) {
			Physics::move(inputEvent, manager);
			Camera::getCurrent().update(inputEvent, window);
			renderTriangles(window);
		}

		//framerate.stabilizeCalculationAndRendering(60);
		//Wait(10);

		framerate.renderFrameRate(10, 10, window.getRenderer());
		Draw::DrawLine({ 0,window.getRenderHeight() / 2 }, { window.getRenderWidth(),window.getRenderHeight() / 2 }, gray, window.getRenderer());
		Draw::DrawLine({ window.getRenderWidth() / 2,0 }, { window.getRenderWidth() / 2,window.getRenderHeight() }, gray, window.getRenderer());
		Draw::DrawCircle(window.getRenderWidth() / 2, window.getRenderHeight() / 2, 5, dark_gray, window.getRenderer());
		//window.RenderScreen();
		//window.FillScreen(teal);
	}
};