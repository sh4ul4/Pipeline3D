#pragma once

#include "FrameRate.h"
#include "Physics.h"

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
public:
	// Texture globale
	GlobalTexture globalTexture; 
	// Interdiction d'utilisation d'un constructeur vide
	Render() = delete; 
	// Constructeur de la classe Render, appelant le constructeur de globalTexture
	Render(const Window& window):globalTexture(window) { }
	// Change la taille d'un triangle pour que ça rentre 
	void updateTriangleSize(const ShapeManager& manager) {}
private:
	// Comparaison de triangles
	struct PointerCompare {
		// Opérateur : fonction de comparaison entre triangles
		bool operator()(const Triangle* l, const Triangle* r) {
			return (l->distanceToCamera > r->distanceToCamera);
		}
	};
	// Fonction de threading pour le rendering
	static void threadRendering(const std::vector<Triangle*> toRender, GlobalTexture& globalTexture, const Window& w, int min, int max) { }

	// Rendering des triangles
	void renderTriangles(const Window& window) { }
public:
	// Rendering écran
	void render(InputEvent& inputEvent, const Window& window, const ShapeManager& manager, Bitmap* background = nullptr) { }
};