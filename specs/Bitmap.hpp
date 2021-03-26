/**
 * @file Module Texturing ?
 */ 

/** @brief class Bitmap
 * La classe Bitmap permet de contenir les données d'une bitmap graphique et de simplifier sa création.
 */
class Bitmap{
public:
	// Classe SDL qui contient les informations d'une surface graphique, à savoir la bitmap, les dimensions et les formats.
	SDL_Surface* surface = nullptr;

	// suppression du constructeur de base
	Bitmap() = delete;

  	/**	@brief Constructeur 
      *
      *	- vérifier si l'argument path est valide
      *	- créer une nouvelle surface avec la fonction IMG_Load()
      *	- définir la variable temporaire pixelFormat avec la fonction SDL_AllocFormat()
      *	- convertir la surface avec le nouveau format défini
      *	- bloquer les pixels de la surface avec SDL_LockSurface()
      * - vérifier les erreurs avec SDL_GetError() et supprimer les variables temporaires
      */
	Bitmap(const char* path) {}

  	/** @brief Destructeur
	  *
	  * débloquer et libérer les pixel de la bitmap
	  */
	~Bitmap() {}
};
