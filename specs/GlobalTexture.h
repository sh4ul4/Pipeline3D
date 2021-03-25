class GlobalTexture {

private:

	// variable SDL qui permet de contenir une texture accelérée pour l'affichage
	SDL_Texture* texture;

	// largeur de la bitmap
	size_t width;

	// hauteur de la bitmap
	size_t height;

	// vecteur modifiable des pixels de la bitmap
	std::vector<Uint32> pixels;

	// pitch de la bitmap
	int pitch = 0;

public:

	// buffer qui contient les distances caméra-pixel de chaque pixel dessiné
	float* zbuffer;

	// pixel format from SDL
	SDL_PixelFormat* pixelFormat;

public:

	// suppression du constructeur de base
	GlobalTexture() = delete;

	/**	@brief GlobalTexture
      *
      *	- créer une texture à l'aide de SDL_CreateTexture()
      *	- définir les variables width et height
      *	- allouer l'espace nécessaire au vecteur de pixels et les mettre à zéro
      *	- définir la variable pitch avec width * 4
	  * - allouer l'espace nécessaire au zbuffer avec width * height
	  * - définir la variable pixelFormat avec la fonction SDL_AllocFormat()
	  * - vérifier les erreurs SDL avec SDL_GetError()
      */
	GlobalTexture(const Window& window) {}

	/**	@brief ~GlobalTexture
      *
      *	- détruire la texture avec SDL_DestroyTexture()
      *	- libérer la mémoire du zbuffer
	  * - libérer le format pixel avec SDL_FreeFormat
	  * - vérifier les erreurs SDL avec SDL_GetError()
      */
	~GlobalTexture() {}

	// renvoyer la valeur du zbuffer correspondant à la position demandée
	inline float getZvalue(const int& at) {
		return zbuffer[at];
	}

	// écrire une nouvelle valeur dans le zbuffer correspondant à la position demandée
	inline void putZvalue(const int& at, const float& value) {
		zbuffer[at] = value;
	}

	// mettre à jour le zbuffer en mettant toutes les valeur à float-max
	inline void refreshZbuffer() {
		for (int i = 0; i < width * height; i++) zbuffer[i] = /*float-max*/;
	}

	// mettre à jour la texture avec le vecteur de pixels
	void updateTexture(const Window& w) {}

	// appliquer un filtre 
	void applyFilter(/*tableau de filtrage*/) {}

	// accéder en lecture-écriture le pixel à la position demandée
	inline Uint32& operator[] (const unsigned& at) {
		return pixels[at];
	}

	// renvoyer la largeur de la bitmap
	inline const int& getWidth()const { return width; }
	// renvoyer la hauteur de la bitmap
	inline const int& getHeight()const { return height; }

	// transformer des valeurs 8-bit rouge, vert, bleu, transparence en un pixel 32-bit
	static inline Uint32 rgbaToUint32(const Uint8& r, const Uint8& g, const Uint8& b, const Uint8& a) {
		return (b << 24) + (g << 16) + (r << 8) + (a);
	}

	// mettre tous les pixels du vecteur à une valeur arbitraire
	void clearPixels() {
		const Uint32 pixel = rgbaToUint32(170, 170, 255, 255);
		for (auto& p : pixels) p = pixel;
	}

	// afficher la texture à la position demandée dans la fenêtre graphique à l'aide de SDL_RenderCopyEx()
	void renderTexture(SDL_Renderer* renderer, const Point2D& topLeft, const unsigned int& width, const unsigned int& height, const int& flip, const double& angle) const {}

private:

	// algorithme de Bresenham pour tracer une ligne
	inline static void ScanLine(const int& x1, const int& y1, const int& x2, const int& y2, std::vector<Point2D>& line, const int& maxHeight, const int& maxWidth) {}

public:

	// dessiner une ligne à lintérieur de la bitmap de pixels
	void drawLine(const GlobalTexture& globalTexture, const Point2D& a, const float& adepth, const Point2D& b, const float& bdepth, const Color& color) {}
};