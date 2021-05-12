#pragma once

/**
 * @file Module Texturing
 */

/** @brief class Bitmap
 * La classe Bitmap permet de contenir les donn�es d'une bitmap graphique et de simplifier sa cr�ation.
 */
class Bitmap {
public:

	/**
	 * @brief Contient les bitmaps créées avec la fonction newBitmap().
	 */
	static std::vector<Bitmap*> bitmaps;

	/**
	 * @brief Contient le nom unique de la bitmap.
	 */
	std::string name{};

	/**
	 * @brief Contient le chemin d'accès vers l'image source.
	 */
	std::string path{};

	/**
	 * @brief Contient les informations d'une surface graphique (bitmap, dimensions, formats).
	 */
	SDL_Surface* surface = nullptr;

public:

	/**
	 * @brief Renvoie true si une bitmap avec le nom donné existe dans le vecteur.
	 * @param name Le nom unique de la bitmap dans le vecteur.
	 */
	static bool bitmapExists(const std::string& name) {
		for (size_t i = 0; i < bitmaps.size(); i++)
			if (!name.compare(bitmaps[i]->name)) return true;
		return false;
	}
	
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
	Bitmap(const std::string& name, const std::string& path) :name(name), path(FIND_FILE(path)) {
		SDL_Surface* newsurface = nullptr;
		newsurface = IMG_Load(path.c_str());
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

	/**
	 * @brief Ajoute une nouvelle bitmap au vecteur.
	 * @param path Le chemin d'accès vers l'image source.
	 * @param name Le nom unique de la bitmap dans le vecteur.
	 */
	static void newBitmap(const std::string& name, const std::string& path) {
		if (!bitmapExists(name)) bitmaps.emplace_back(new Bitmap(name, FIND_FILE(path)));
	}

	/**
	 * @brief Supprime la bitmap avec le nom donné du vecteur.
	 * @param name Le nom unique de la bitmap dans le vecteur.
	 */
	static void deleteBitmap(const std::string& name) {
		for (size_t i = 0; i < bitmaps.size(); i++) {
			if (!name.compare(bitmaps[i]->name)) {
				delete bitmaps[i];
				bitmaps[i] = nullptr;
				bitmaps.erase(bitmaps.begin() + i);
				--i;
				bitmaps.shrink_to_fit();
			}
		}
	}

	/**
	 * @brief Renvoie un pointeur vers la bitmap au nom donné.
	 * @param name Le nom unique de la bitmap dans le vecteur.
	 */
	static Bitmap* getBitmap(const std::string& name) {
		for (size_t i = 0; i < bitmaps.size(); i++)
			if (!name.compare(bitmaps[i]->name)) return bitmaps[i];
		return nullptr;
	}

	/**
	 * @brief Débloque et libére les pixels de la bitmap.
	 */
	~Bitmap() {
		SDL_UnlockSurface(surface);
		SDL_FreeSurface(surface);
	}
};

std::vector<Bitmap*> Bitmap::bitmaps{};