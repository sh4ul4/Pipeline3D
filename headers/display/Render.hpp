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

	void savePNG(const std::string& file) {
		globalTexture.savePNG(file);
	}

	// Interdiction d'utilisation d'un constructeur vide
	Render() = delete;

	// Constructeur de la classe Render, appelant le constructeur de globalTexture
	Render(const Window& window, int w, int h) :globalTexture(window, w, h) {
		SDL_GL_SetSwapInterval(0);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		//window.ToggleWindow(window->getWidth(), window->getHeight());
		//InitMixer(8);
		TTF_Init();
	}

	// Change la taille d'un triangle pour que ça rentre 
	void updateTriangles(ShapeManager& manager) {
		const size_t size = toRender.size();
		
		// Quitter la fonction s'il n'y a pas eu de changement depuis la précédente update
		if (manager.rendered) 	return;
		
		toRender.clear();
		toRender.shrink_to_fit();
		toRender.reserve(size);

		for (size_t j = 0; j < manager.shapes.size(); j++) {
			// Ne pas render les shapes marquées non visibles
			if (!manager.shapes[j]->visible)  
				continue;

			const size_t max2 = manager.shapes[j]->triangles.size();
			for (size_t i = 0; i < max2; i++) {
				toRender.push_back(&manager.shapes[j]->triangles[i]);
			}
		}
		toRender.shrink_to_fit();
		manager.rendered = true;
	}

private:
	// Rendering des triangles
	void renderTriangles(const Window& window) {
		// render textures on surface
		// signlethreaded version
		const size_t size = toRender.size();
		const Point2D<int> center(globalTexture.getWidth() / 2, globalTexture.getHeight() / 2);
		for (size_t i = 0; i < size; i++) {
			toRender[i]->visible = false;
			if (toRender[i]->normalVec.dot(Camera::getCurrent().look) > 0.2)continue;
			toRender[i]->setScreenCoord(window, center, Camera::getCurrent());
			// render triangle
			toRender[i]->render(window, globalTexture, center);
		}
	}

public:
	void renderStatic(const Point2D<int>& topLeft, const int& width, const int& height, const Window& window) {
		SDL_ShowCursor(true);
		globalTexture.renderTexture(window.getRenderer(), topLeft, width, height, 0, 0);
	}
	// Rendering écran
	void render(const Point2D<int>& topLeft, const int& width, const int& height, InputEvent& inputEvent, const Window& window, ShapeManager& manager) {
		if (Camera::currentExists()) {
			Physics::move(inputEvent, manager);
			Camera::getCurrent().update(inputEvent, window, globalTexture, topLeft);
			if (Camera::currentExists() == false) { std::cout << "Error : current camera does not exist.\n"; exit(1); }
			globalTexture.refreshZbuffer();
			globalTexture.clearPixels();
			renderTriangles(window);
			//globalTexture.filterBnW();
			globalTexture.updateTexture();
			globalTexture.renderTexture(window.getRenderer(), topLeft, width, height, 0, 0);
		}

		//framerate.stabilizeCalculationAndRendering(60);
		//Wait(10);

		framerate.renderFrameRate(10, 10, window.getRenderer());
	}

	void renderOrientation(const Point2D<int>& pos, float size, const Window& window)const {
		const Vertex camera(0, 0, 0);
		Matrix<4, 4> viewMatrix = Camera::makeFPviewMatrix(camera, Camera::getCurrent().angleY, Camera::getCurrent().angleX);
		const Vertex x(size / 2 - 2, 0, 0);
		const Vertex y(0, size / 2 - 2, 0);
		const Vertex z(0, 0, size / 2 - 2);
		const Vertex origin(0, 0, 0);
		Matrix<4, 4> mo;
		mo.m[0] = { origin.x,origin.y,origin.z,1 };
		Matrix<4, 4> mx;
		mx.m[0] = { x.x,x.y,x.z,1 };
		Matrix<4, 4> my;
		my.m[0] = { y.x,y.y,y.z,1 };
		Matrix<4, 4> mz;
		mz.m[0] = { z.x,z.y,z.z,1 };
		bool clip{ false };
		const Point2D<int> center(0, 0);
		const Vertex oScreen = Camera::getCurrent().get2DWithoutPerspective(mo, clip, center, viewMatrix);
		const Vertex xScreen = Camera::getCurrent().get2DWithoutPerspective(mx, clip, center, viewMatrix);
		const Vertex yScreen = Camera::getCurrent().get2DWithoutPerspective(my, clip, center, viewMatrix);
		const Vertex zScreen = Camera::getCurrent().get2DWithoutPerspective(mz, clip, center, viewMatrix);
		Draw::DrawFillRoundedRectContoured(pos, size, size, 5, gray, black, window.getRenderer());
		Draw::DrawLine(Point2D<int>(oScreen.x + pos.x + size / 2, oScreen.y + pos.y + size / 2), Point2D<int>(xScreen.x + pos.x + size / 2, xScreen.y + pos.y + size / 2), red, window.getRenderer());
		Draw::DrawLine(Point2D<int>(oScreen.x + pos.x + size / 2, oScreen.y + pos.y + size / 2), Point2D<int>(yScreen.x + pos.x + size / 2, yScreen.y + pos.y + size / 2), green, window.getRenderer());
		Draw::DrawLine(Point2D<int>(oScreen.x + pos.x + size / 2, oScreen.y + pos.y + size / 2), Point2D<int>(zScreen.x + pos.x + size / 2, zScreen.y + pos.y + size / 2), blue, window.getRenderer());
	}
};
