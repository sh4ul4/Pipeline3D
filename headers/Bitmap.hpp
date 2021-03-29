#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

/**
 * @file Module Texturing
 */

/** @brief class Bitmap
 * La classe Bitmap permet de contenir les donn�es d'une bitmap graphique et de simplifier sa cr�ation.
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
	 * @param path le chemin d'acc�s vers l'image source
	 * V�rifier si l'argument path est valide.
	 * Cr�er une nouvelle surface avec la fonction IMG_Load().
	 * D�finir la variable temporaire pixelFormat avec la fonction SDL_AllocFormat().
	 * Convertir la surface avec le nouveau format d�fini.
	 * Bloquer les pixels de la surface avec SDL_LockSurface().
	 * V�rifier les erreurs avec SDL_GetError() et supprimer les variables temporaires.
	 */
	Bitmap(const char* path);

	/**
	 * Un destructeur.
	 * D�bloquer et lib�rer les pixels de la bitmap.
	 */
	~Bitmap();
};