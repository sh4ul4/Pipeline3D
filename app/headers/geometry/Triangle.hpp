#pragma once

#ifdef _CUDA
#include "../display/Cuda.cu"

float* m_dev;
float* m_host;
bool* clip_dev;
bool* clip_host;
float* res_dev;
float* res_host;
float* vm;
float* pm;
size_t size = 0;

#endif // _CUDA


/** 
 * @file Triangle.hpp
 * @brief La classe Triangle permet d'encapsuler les données d'un triangle, à la fois sous sa forme 3D
 * dans l'environnement et sous sa forme 2D dans la frame.
 */ 
/**
 * @class Triangle permet d'encapsuler les données d'un triangle, à la fois sous sa forme 3D
 * dans l'environnement et sous sa forme 2D dans la frame.
 */
class Triangle {

public:

	// sommets du triangle en world-coordinates
	Vertex a, b, c;

	// sommets du triangle en screen-coordinates (clip-space)
	Vertex aScreen, bScreen, cScreen;

	// pointeur vers la texture bitmap du triangle
	Bitmap* bmp = nullptr;

	// sommets du triangle en coordonnées de texture
	Point2D<float> bmpA, bmpB, bmpC;

	// couleur du triangle
	Color color = black;

	// distance du triangle à la caméra
	float distanceToCamera = INFINITY;

	// sommet qui indique le vecteur normal du triangle
	Vector normalVec;

	// booléen pour indiquer la visibilité du triangle
	bool visible = false;
	bool rendered = true;

	// booléen pour indiquer si le triangle est rempli
	bool fill = true;

	// booléen pour indiquer si le triangle est contourné
	bool contour = true;

	// variables propres au clipping
	bool farclipA = false;
	bool farclipB = false;
	bool farclipC = false;
	bool nearclipA = false;
	bool nearclipB = false;
	bool nearclipC = false;
	
public:

	Triangle(){}

	// constructeur sans texture
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Vector& normalVec, const Color& color = black, const bool& fill = true)
		: a(a), b(b), c(c), normalVec(normalVec), color(color), fill(fill) {}

	// constructeur avec texture
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c, const Vector& normalVec,
		const Color& color, Bitmap* bmp, const Point2D<float>& bmpA, const Point2D<float>& bmpB, const Point2D<float>& bmpC)
		: a(a), b(b), c(c), normalVec(normalVec), color(color),
		bmp(bmp), bmpA(bmpA), bmpB(bmpB), bmpC(bmpC) {}
	
	// comparaison de deux triangles en fonction de leurs distance à la caméra
	inline bool operator<(const Triangle& node) const {
		return (this->distanceToCamera < node.distanceToCamera);
	}
	inline bool operator>(const Triangle& node) const {
		return (this->distanceToCamera > node.distanceToCamera);
	}

	// bouger tout le triangle sur un vecteur
	inline void move(const Vector& movement) {
		a += movement;
		b += movement;
		c += movement;
	}

	void setNormalVector() {
		Vector v1 = a - c;
		Vector v2 = b - c;
		normalVec = v1.cross(v2);
		normalVec.normalize();
	}

	// set screen coordinates and handle clipping
	void setScreenCoord(const Window& window, const Point2D<int>& center, Camera& camera) {
		visible = true;
		// far-clipping
		distanceToCamera = a.distance(camera.getCameraPosition()) / 3 +
			b.distance(camera.getCameraPosition()) / 3 +
			c.distance(camera.getCameraPosition()) / 3;
		visible = visible && distanceToCamera < camera.far;
		if (!visible)return;
		// near-clipping
		nearclipA = nearclipB = nearclipC = false;
		const float rela = camera.relationToClipPlane(a);
		const float relb = camera.relationToClipPlane(b);
		const float relc = camera.relationToClipPlane(c);
		nearclipA = rela <= 0;
		nearclipB = relb <= 0;
		nearclipC = relc <= 0;
		visible = visible && !nearclipA && !nearclipB && !nearclipC;
		if (!visible)return;
		// 2D points calculation
		Matrix<4, 4> ma;
		ma.m[0] = { a.x,a.y,a.z,1 };
		Matrix<4, 4> mb;
		mb.m[0] = { b.x,b.y,b.z,1 };
		Matrix<4, 4> mc;
		mc.m[0] = { c.x,c.y,c.z,1 };
		aScreen = camera.get2D(ma, farclipA, center);
		bScreen = camera.get2D(mb, farclipB, center);
		cScreen = camera.get2D(mc, farclipC, center);
		visible = visible && !farclipA && !farclipB && !farclipC;
	}

	// dessiner le triangle dans la frame
	void render(const Window& window, GlobalTexture& globalTexture, const Point2D<int>& center, bool showDepth = false, bool showMesh = false)  {
		if (!visible) return;
		if (abs(aScreen.x) > 10000 || abs(aScreen.y) > 10000 ||
			abs(bScreen.x) > 10000 || abs(bScreen.y) > 10000 ||
			abs(cScreen.x) > 10000 || abs(cScreen.y) > 10000) {
			//clip here
			return;
		}
		Camera::getCurrent().lightSource.normalize();
		float light = normalVec.dot(Camera::getCurrent().lightSource);
		light *= Camera::getCurrent().lightIntensity;
		light = Maths::clamp(light, 0.0f, 1.0f);
		if (showDepth && bmp != nullptr) {
			drawDepth(*bmp, globalTexture, light, Camera::getCurrent().lightColor);
		}
		else if (fill && !showMesh) {
			if (bmp != nullptr) draw(*bmp, globalTexture, light, Camera::getCurrent().lightColor);
			else draw(color, globalTexture, light, Camera::getCurrent().lightColor);
		}
		else if (contour || showMesh) {
			globalTexture.drawLine(globalTexture, Point2D<int>{ aScreen.x,aScreen.y }, aScreen.z, Point2D<int>{ bScreen.x,bScreen.y }, bScreen.z, color);
			globalTexture.drawLine(globalTexture, Point2D<int>{ bScreen.x,bScreen.y }, bScreen.z, Point2D<int>{ cScreen.x,cScreen.y }, cScreen.z, color);
			globalTexture.drawLine(globalTexture, Point2D<int>{ aScreen.x,aScreen.y }, aScreen.z, Point2D<int>{ cScreen.x,cScreen.y }, cScreen.z, color);
		}
	}

	// afficher l'intérieur du triangle
	inline void fillIt(const bool& b) { this->fill = b; }

	// afficher les contours du triangle
	inline void contourIt(const bool& b) { this->contour = b; }

public:

	// renvoie le point du triangle le plus proche du point
	Vertex ClosestPoint(const Vertex& point) const {
		return Maths::ClosestPointOnTriangle(point, a, b, c);
	}

private:

	// dessiner le triangle dans la frame
	void draw(const Bitmap& src, GlobalTexture& dst, const float& lightIntensity, const Color& lightColor) const {
		TextureManager::rasterize(src,
			Point2D<int>(aScreen.x, aScreen.y),
			Point2D<int>(bScreen.x, bScreen.y),
			Point2D<int>(cScreen.x, cScreen.y),
			Point2D<int>(bmp->surface->w * bmpA.x, std::abs(bmp->surface->h * bmpA.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpB.x, std::abs(bmp->surface->h * bmpB.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpC.x, std::abs(bmp->surface->h * bmpC.y - bmp->surface->h)),
			aScreen.z, bScreen.z, cScreen.z,
			dst, lightIntensity, lightColor);
	}

	// dessiner le triangle dans la frame
	void draw(const Color& color, GlobalTexture& dst, const float& lightIntensity, const Color& lightColor) const {
		TextureManager::rasterize(color,
			Point2D<int>(aScreen.x, aScreen.y),
			Point2D<int>(bScreen.x, bScreen.y),
			Point2D<int>(cScreen.x, cScreen.y),
			aScreen.z, bScreen.z, cScreen.z,
			dst, lightIntensity, lightColor);
	}

	void drawDepth(const Bitmap& src, GlobalTexture& dst, const float& lightIntensity, const Color& lightColor) const {
		TextureManager::rasterizeDepth(src,
			Point2D<int>(aScreen.x, aScreen.y),
			Point2D<int>(bScreen.x, bScreen.y),
			Point2D<int>(cScreen.x, cScreen.y),
			Point2D<int>(bmp->surface->w * bmpA.x, std::abs(bmp->surface->h * bmpA.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpB.x, std::abs(bmp->surface->h * bmpB.y - bmp->surface->h)),
			Point2D<int>(bmp->surface->w * bmpC.x, std::abs(bmp->surface->h * bmpC.y - bmp->surface->h)),
			aScreen.z, bScreen.z, cScreen.z,
			dst, lightIntensity, lightColor);
	}
};

#ifdef _CUDA
void updateDev(std::vector<Triangle*>& tr) {
	// free
	delete[] m_host;
	cudaFree(m_dev);
	delete[] clip_host;
	cudaFree(clip_dev);
	delete[] res_host;
	cudaFree(res_dev);
	// allocate
	size = tr.size();
	m_host = new float[size * 4 * 4];
	memset(m_host, 0, size * 16);
	gpuErrchk(cudaMalloc((void**)&m_dev, size * 4 * 4 * sizeof(float)));
	res_host = new float[size * 3];
	memset(res_host, 0, size * 3);
	gpuErrchk(cudaMalloc((void**)&res_dev, size * 3 * sizeof(float)));
	clip_host = new bool[size];
	memset(clip_host, 0, size);
	gpuErrchk(cudaMalloc((void**)&clip_dev, size * sizeof(bool)));
}
void copyDevice_Host_A(std::vector<Triangle*>& tr) {
	gpuErrchk(cudaMemcpy(res_host, res_dev, size * 3 * sizeof(float), cudaMemcpyDeviceToHost));
	gpuErrchk(cudaMemcpy(clip_host, clip_dev, size * sizeof(bool), cudaMemcpyDeviceToHost));
	for (int i = 0; i < size; i++) {
		tr[i]->aScreen.x = res_host[i * 3];
		tr[i]->aScreen.y = res_host[i * 3 + 1];
		tr[i]->aScreen.z = res_host[i * 3 + 2];
		tr[i]->farclipA = clip_host[i];
	}
}
void copyDevice_Host_B(std::vector<Triangle*>& tr) {
	gpuErrchk(cudaMemcpy(res_host, res_dev, size * 3 * sizeof(float), cudaMemcpyDeviceToHost));
	gpuErrchk(cudaMemcpy(clip_host, clip_dev, size * sizeof(bool), cudaMemcpyDeviceToHost));
	for (int i = 0; i < size; i++) {
		tr[i]->bScreen.x = res_host[i * 3];
		tr[i]->bScreen.y = res_host[i * 3 + 1];
		tr[i]->bScreen.z = res_host[i * 3 + 2];
		tr[i]->farclipB = clip_host[i];
	}
}
void copyDevice_Host_C(std::vector<Triangle*>& tr) {
	gpuErrchk(cudaMemcpy(res_host, res_dev, size * 3 * sizeof(float), cudaMemcpyDeviceToHost));
	gpuErrchk(cudaMemcpy(clip_host, clip_dev, size * sizeof(bool), cudaMemcpyDeviceToHost));
	for (int i = 0; i < size; i++) {
		tr[i]->cScreen.x = res_host[i * 3];
		tr[i]->cScreen.y = res_host[i * 3 + 1];
		tr[i]->cScreen.z = res_host[i * 3 + 2];
		tr[i]->farclipC = clip_host[i];
	}
}
void copyHost_Device_A(std::vector<Triangle*>& tr) {
	//Uint32 t0 = SDL_GetTicks();
	for (int i = 0; i < size; i++) {
		clip_host[i] = tr[i]->farclipA;
	}
	for (int i = 0; i < size; i++) {
		m_host[i * 16 + 0 + 0] = tr[i]->a.x;
		m_host[i * 16 + 0 + 1] = tr[i]->a.y;
		m_host[i * 16 + 0 + 2] = tr[i]->a.z;
		m_host[i * 16 + 0 + 3] = 1;
	}
	//Uint32 t1 = SDL_GetTicks();
	gpuErrchk(cudaMemcpy(m_dev, m_host, size * 4 * 4 * sizeof(float), cudaMemcpyHostToDevice));
	gpuErrchk(cudaMemcpy(clip_dev, clip_host, size * sizeof(bool), cudaMemcpyHostToDevice));
	//Uint32 t2 = SDL_GetTicks();
	//std::cout << "\t" << t1 - t0 << " " << t2 - t1 << std::endl;
}
void copyHost_Device_B(std::vector<Triangle*>& tr) {
	for (int i = 0; i < size; i++) {
		clip_host[i] = tr[i]->farclipB;
	}
	for (int i = 0; i < size; i++) {
		m_host[i * 16 + 0 + 0] = tr[i]->b.x;
		m_host[i * 16 + 0 + 1] = tr[i]->b.y;
		m_host[i * 16 + 0 + 2] = tr[i]->b.z;
		m_host[i * 16 + 0 + 3] = 1;
	}
	gpuErrchk(cudaMemcpy(m_dev, m_host, size * 4 * 4 * sizeof(float), cudaMemcpyHostToDevice));
	gpuErrchk(cudaMemcpy(clip_dev, clip_host, size * sizeof(bool), cudaMemcpyHostToDevice));
}
void copyHost_Device_C(std::vector<Triangle*>& tr) {
	for (int i = 0; i < size; i++) {
		clip_host[i] = tr[i]->farclipC;
	}
	for (int i = 0; i < size; i++) {
		m_host[i * 16 + 0 + 0] = tr[i]->c.x;
		m_host[i * 16 + 0 + 1] = tr[i]->c.y;
		m_host[i * 16 + 0 + 2] = tr[i]->c.z;
		m_host[i * 16 + 0 + 3] = 1;
	}
	gpuErrchk(cudaMemcpy(m_dev, m_host, size * 4 * 4 * sizeof(float), cudaMemcpyHostToDevice));
	gpuErrchk(cudaMemcpy(clip_dev, clip_host, size * sizeof(bool), cudaMemcpyHostToDevice));
}
void setScreenCoord(const Window& window, const Point2D<int>& center, Camera& camera, std::vector<Triangle*>& tr) {
	bool _UPDATE = false;
	if (size != tr.size()) {
		_UPDATE = true;
		updateDev(tr);
	}

	for (int i = 0; i < tr.size(); i++) {
		if (tr[i]->rendered == false)continue;
		tr[i]->visible = true;
		// far-clipping
		tr[i]->distanceToCamera = tr[i]->a.distance(camera.getCameraPosition()) / 3 +
			tr[i]->b.distance(camera.getCameraPosition()) / 3 +
			tr[i]->c.distance(camera.getCameraPosition()) / 3;
		tr[i]->visible = tr[i]->visible && tr[i]->distanceToCamera < camera.far;
		if (!tr[i]->visible)continue;
		// near-clipping
		tr[i]->nearclipA = tr[i]->nearclipB = tr[i]->nearclipC = false;
		const float rela = camera.relationToClipPlane(tr[i]->a);
		const float relb = camera.relationToClipPlane(tr[i]->b);
		const float relc = camera.relationToClipPlane(tr[i]->c);
		tr[i]->nearclipA = rela <= 0;
		tr[i]->nearclipB = relb <= 0;
		tr[i]->nearclipC = relc <= 0;
		tr[i]->visible = tr[i]->visible && !tr[i]->nearclipA && !tr[i]->nearclipB && !tr[i]->nearclipC;
	}
	//Uint32 t0 = SDL_GetTicks();
	//updateDev(tr);
	int threadsPerBlock = 256;
	int blocksPerGrid = (size + threadsPerBlock - 1) / threadsPerBlock;
	vm = camera.viewMatrix.generateArray();
	pm = camera.projectionMatrix.generateArray();
	copyHost_Device_A(tr);
	//Uint32 t1 = SDL_GetTicks();
	get2dCuda<<<threadsPerBlock, blocksPerGrid >>>(size, m_dev, clip_dev, center.x, center.y, camera.far, vm, pm, res_dev);
	//Uint32 t2 = SDL_GetTicks();
	copyDevice_Host_A(tr);
	//Uint32 t3 = SDL_GetTicks();
	copyHost_Device_B(tr);
	//Uint32 t4 = SDL_GetTicks();
	get2dCuda<<<threadsPerBlock, blocksPerGrid >>>(size, m_dev, clip_dev, center.x, center.y, camera.far, vm, pm, res_dev);
	//Uint32 t5 = SDL_GetTicks();
	copyDevice_Host_B(tr);
	//Uint32 t6 = SDL_GetTicks();
	copyHost_Device_C(tr);
	//Uint32 t7 = SDL_GetTicks();
	get2dCuda<<<threadsPerBlock, blocksPerGrid >>>(size, m_dev, clip_dev, center.x, center.y, camera.far, vm, pm, res_dev);
	//Uint32 t8 = SDL_GetTicks();
	copyDevice_Host_C(tr);
	//Uint32 t9 = SDL_GetTicks();
	//std::cout << t1 - t0 + t4 - t3 + t7 - t6 << " " << t3 - t2 + t6 - t5 + t9 - t8 << " " << t2 - t1 + t5 - t4 + t8 - t7 << std::endl;
	camera.viewMatrix.deleteArray(vm);
	camera.projectionMatrix.deleteArray(pm);
	for (int i = 0; i < size; i++) {
		if (tr[i]->rendered == false)continue;
		tr[i]->visible = tr[i]->visible && !tr[i]->farclipA && !tr[i]->farclipB && !tr[i]->farclipC;
	}
}
#endif // _CUDA