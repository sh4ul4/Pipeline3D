#pragma once
/**
 * @class GlobalTexture pour contenir et mettre à jour la frame en bout de Pipeline
 */
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
	GlobalTexture(const Window& window) {
		zbuffer = nullptr;
		texture = SDL_CreateTexture(window.getRenderer(), SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STREAMING, window.getWidth(), window.getHeight());
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		width = window.getWidth();
		height = window.getHeight();
		// generate pixel-array
		pixels.reserve(width * height);
		for (int i = 0; i < width * height; i++) pixels.emplace_back(0);
		pixels.shrink_to_fit();
		pitch = width * 4;
		//pixelsV.reserve(width * height * 4);
		zbuffer = new float[width * height];
		pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_ARGB32);
		const char* error_f = SDL_GetError();
		if (*error_f) { std::cout << "Error occured in Texture(): " << error_f << std::endl; }
	}

	/**	@brief ~GlobalTexture
      *
      *	- détruire la texture avec SDL_DestroyTexture()
      *	- libérer la mémoire du zbuffer
	  * - libérer le format pixel avec SDL_FreeFormat
	  * - vérifier les erreurs SDL avec SDL_GetError()
      */
	~GlobalTexture() {
		SDL_DestroyTexture(texture);
		delete[] zbuffer;
		zbuffer = nullptr;
		SDL_FreeFormat(pixelFormat);
		const char* error_f = SDL_GetError();
		if (*error_f) { std::cout << "Error occured in ~Texture(): " << error_f << std::endl; }
	}

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
		for (int i = 0; i < width * height; i++) zbuffer[i] = 9999999;
	}

	// mettre à jour la texture avec le vecteur de pixels
	void updateTexture(const Window& w) {
		if (texture == nullptr || pixels.empty()) { std::cout << "Error occured in GlobalTexture::updateTexture()" << std::endl; exit(1); }
		SDL_UpdateTexture(texture, NULL, pixels.data(), pitch);
	}

	// appliquer un filtre 
	void applyFilter(/*tableau de filtrage*/) {}

	// accéder en lecture-écriture le pixel à la position demandée
	inline Uint32& operator[] (const unsigned& at) {
		return pixels[at];
	}

	// renvoyer la largeur de la bitmap
	inline int getWidth()const { return width; }
	// renvoyer la hauteur de la bitmap
	inline int getHeight()const { return height; }

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
	void renderTexture(SDL_Renderer* renderer, const Point2D<int>& topLeft, const unsigned int& width, const unsigned int& height, const int& flip, const double& angle) const {
		if (renderer == nullptr || texture == nullptr) { std::cout << "Error occured in renderTexture()" << std::endl; return; }
		SDL_Rect rect_f;
		rect_f.x = 0;
		rect_f.y = 0;
		if (this->width == 0) { rect_f.w = width; }
		else rect_f.w = this->width;
		if (this->height == 0) { rect_f.h = height; }
		else rect_f.h = this->height;

		SDL_Rect dstrect_f{ topLeft.x,topLeft.y,(int)width,(int)height };

		switch (flip) {
		case 0: SDL_RenderCopyEx(renderer, texture, &rect_f, &dstrect_f, angle, NULL, SDL_FLIP_NONE); break;
		case 1: SDL_RenderCopyEx(renderer, texture, &rect_f, &dstrect_f, angle, NULL, SDL_FLIP_HORIZONTAL); break;
		case 2: SDL_RenderCopyEx(renderer, texture, &rect_f, &dstrect_f, angle, NULL, SDL_FLIP_VERTICAL); break;
		default: SDL_RenderCopyEx(renderer, texture, &rect_f, &dstrect_f, angle, NULL, SDL_FLIP_NONE); break;
		}
	}

private:

	// algorithme de Bresenham pour tracer une ligne
	inline static void ScanLine(const int& x1, const int& y1, const int& x2, const int& y2, std::vector<Point2D<int>>& line, const int& maxHeight, const int& maxWidth) {
		int dx1, dy1, dx2, dy2, x, y, m, n, k, cnt;
		const int sx = x2 - x1;
		const int sy = y2 - y1;

		if (sx > 0) dx1 = 1;
		else if (sx < 0) dx1 = -1;
		else dx1 = 0;

		if (sy > 0) dy1 = 1;
		else if (sy < 0) dy1 = -1;
		else dy1 = 0;

		m = std::abs(sx);
		n = std::abs(sy);
		dx2 = dx1;
		dy2 = 0;

		if (m < n) {
			m = std::abs(sy);
			n = std::abs(sx);
			dx2 = 0;
			dy2 = dy1;
		}

		x = x1; y = y1;
		cnt = m + 1;
		k = n / 2;

		while (cnt--) {
			if (y >= 0 && y < maxHeight /*&& x >= 0 && x < maxWidth*/) {
				int xtmp;
				if (x < 0)xtmp = 0;
				else if (x >= maxWidth) xtmp = maxWidth - 1;
				else xtmp = x;
				line.emplace_back(xtmp, y);
			}
			k += n;
			if (k < m) {
				x += dx2;
				y += dy2;
			}
			else {
				k -= m;
				x += dx1;
				y += dy1;
			}
		}
	}

public:

	// dessiner une ligne à lintérieur de la bitmap de pixels
	void drawLine(const GlobalTexture& globalTexture, const Point2D<int>& a, const float& adepth, const Point2D<int>& b, const float& bdepth, const Color& color) {
		std::vector<Point2D<int>> line;
		ScanLine(a.x, a.y, b.x, b.y, line, height, width);
		//float rela = bdepth - adepth;
		for (auto& p : line) {
			/*int l1 = p.distance(a);
			int l2 = p.distance(b);
			float depth = adepth + (rela) / (l1 + l2) * l1;
			if (globalTexture.zbuffer[p.x + width * p.y] < depth) continue;
			globalTexture.zbuffer[p.x + width * p.y] = depth;*/
			pixels[p.x + width * p.y] = (color.b << 24) + (color.g << 16) + (color.r << 8) + (color.a);
			/*pixelsV[width * 4 * p.y + p.x * 4 + 0] = color.a;
			pixelsV[width * 4 * p.y + p.x * 4 + 1] = color.b;
			pixelsV[width * 4 * p.y + p.x * 4 + 2] = color.g;
			pixelsV[width * 4 * p.y + p.x * 4 + 3] = color.r;*/
		}
	}
};