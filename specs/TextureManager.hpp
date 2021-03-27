/**
 * @class TextureManager pour encapsuler la partie rastérisation et texture mapping
 */
class TextureManager {

	// le constructeur de base est supprimé car la classe est un singleton
	TextureManager() = delete;

private:

	// concaténer une valeur de pixel et un flottant
	static inline Uint8 concat(const float& a, const Uint8& b) {}

	// fonction de signe pour les coordonnées barycentriques
	static inline const int sign(const Point2& p1, const Point2& p2, const Point2& p3) {
		return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
	}

public:

	// rasteriser et colorier un triangle avec une couleur unique tout en respectant la perspective
	static inline void rasterize(const Color& color,
		const Point2& triA, const Point2& triB, const Point2& triC,
		const float& depthA, const float& depthB, const float& depthC,
		GlobalTexture& globalTexture, const float& light) {
		// extraire les informations en variables locales avant la boucle par soucis d'optimisation
		// calculer la bounding box du triangle dans la frame / clipping
		// précalculer des valeurs pour les barycentres
		// 2 boucles imbriquées pour parcourir tous les pixels de la bounding box
		{
			// calculer coordonnées barycentriques
			// vérifier à l'aide des coordonnées barycentriques si le pixel se trouve à l'intérieur du triangle
			// calculer les coordonnées barycentriques correctes pour la profondeur du pixel par rapport au plan de la caméra
			// calculer la profondeur du pixel par rapport au plan de la caméra
			// check et mise-à-jour du z-buffer
			// colorier le pixel en prenant en compte la lumière et la transparence
			// copier le pixel calculé dans la frame
		}
	}

	// rasteriser et colorier un triangle avec une texture tout en respectant la perspective
	static inline void rasterize(const Bitmap& bmp,
		const Point2& triA, const Point2& triB, const Point2& triC,
		const Point2& bmpA, const Point2& bmpB, const Point2& bmpC,
		const float& srcadepth, const float& srcbdepth, const float& srccdepth,
		GlobalTexture& globalTexture, const float& light) {
		// extraire les informations en variables locales avant la boucle par soucis d'optimisation
		// calculer la bounding box du triangle dans la frame / clipping
		// précalculer des valeurs pour les barycentres
		// 2 boucles imbriquées pour parcourir tous les pixels de la bounding box
		{
			// calculer coordonnées barycentriques
			// vérifier à l'aide des coordonnées barycentriques si le pixel se trouve à l'intérieur du triangle
			// calculer les coordonnées barycentriques correctes de la texture
			// calculer les coordonnées du point sur la texture
			// calculer la profondeur du pixel par rapport au plan de la caméra
			// check et mise-à-jour du z-buffer
			// colorier le pixel en prenant en compte la lumière et la transparence
			// copier le pixel calculé dans la frame
		}	
	}
};