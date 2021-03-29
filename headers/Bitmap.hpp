#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

/**
 * @file Module Texturing
 */

/** @brief class Bitmap
 * La classe Bitmap permet de contenir les données d'une bitmap graphique et de simplifier sa création.
 */
class Bitmap {
public:
	/**
	 * Une variable publique.
	 * Contient les informations d'une surface graphique (bitmap, dimensions, formats).
	 */
	SDL_Surface* surface = nullptr;

	// suppression du constructeur de base
	Bitmap() = delete;

	/**
	 * Un constructeur.
	 * @param path le chemin d'accès vers l'image source
	 * Vérifier si l'argument path est valide.
	 * Créer une nouvelle surface avec la fonction IMG_Load().
	 * Définir la variable temporaire pixelFormat avec la fonction SDL_AllocFormat().
	 * Convertir la surface avec le nouveau format défini.
	 * Bloquer les pixels de la surface avec SDL_LockSurface().
	 * Vérifier les erreurs avec SDL_GetError() et supprimer les variables temporaires.
	 */
	Bitmap(const char* path);

	/**
	 * Un destructeur.
	 * Débloquer et libérer les pixels de la bitmap.
	 */
	~Bitmap();
};