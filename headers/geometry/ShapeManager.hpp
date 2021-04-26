#pragma once
#include <iomanip>
#include <sstream>
class ShapeManager
{
private:
	struct MTL {
		std::string name;
		Color Ka = { 0,0,0 };
		Color Kd = { 0,0,0 };
		Color Ks = { 0,0,0 };
		float Ns{ 0 };
		float Tr{ 0 };
		Color Tf = { 0,0,0 };
		float Ni{ 0 };
		std::string bmpPath;
		Point2D<float> bmpDim{ 0,0 };
		MTL() = delete;
		MTL(const std::string& name) :name(name) {}
	};

	void caseMtllib(std::istringstream& iss, std::vector<MTL>& mtls, const std::string& path) {
		std::string mtlpath;
		iss >> mtlpath;
		std::string tmp;
		while (iss >> tmp) {
			mtlpath += " " + tmp;
		}

		std::ifstream mtlfile(path + mtlpath);
		if (!mtlfile.is_open()) {
			mtlfile = std::ifstream(path + mtlpath);
		}
		if (!mtlfile.is_open()) {
			std::cout << "no such mtl file" << std::endl;
			return;
		}
		std::string line;
		while (std::getline(mtlfile, line)) {
			std::istringstream iss(line);
			std::string type;
			iss >> type;
			if (type.size() <= 0 || type[0] == '#') {
				continue;
			}
			if (!type.compare("newmtl")) {
				std::string mtlname;
				iss >> mtlname;
				mtls.emplace_back(mtlname);
			}
			if (!type.compare("Kd") && mtls.size() > 0) {
				float r;
				float g;
				float b;
				iss >> r >> g >> b;
				mtls.back().Kd.r = r * 255;
				mtls.back().Kd.g = g * 255;
				mtls.back().Kd.b = b * 255;
			}
			if (!type.compare("map_Ka") && mtls.size() > 0) {
				std::string imgpath;
				iss >> imgpath;
				Bitmap::newBitmap(imgpath, path + imgpath);
				mtls.back().bmpDim.x = Bitmap::getBitmap(imgpath)->surface->w;
				mtls.back().bmpDim.y = Bitmap::getBitmap(imgpath)->surface->h;
				mtls.back().bmpPath = imgpath;
			}
			if (!type.compare("map_Kd") && mtls.size() > 0) {
				std::string imgpath;
				iss >> imgpath;
				Bitmap::newBitmap(imgpath, path + imgpath);
				mtls.back().bmpDim.x = Bitmap::getBitmap(imgpath)->surface->w;
				mtls.back().bmpDim.y = Bitmap::getBitmap(imgpath)->surface->h;
				mtls.back().bmpPath = imgpath;
			}
		}
	}

	void caseUsemtl(std::istringstream& iss, std::vector<MTL>& mtls) {
		if (mtls.size() <= 0) { std::cout << "There is no loaded mtl file." << std::endl; return; }
		std::string mtlname;
		iss >> mtlname;
		for (size_t i = 0; i < mtls.size(); i++) {
			if (!mtls[i].name.compare(mtlname)) {
				mtls.push_back(mtls[i]);
				mtls.erase(mtls.begin() + i);
				mtls.shrink_to_fit();
				break;
			}
		}
	}

	void caseO(std::istringstream& iss, std::vector<MTL>& mtls, std::vector<Triangle>& trs, std::string& currentObject, const std::string& shapeName) {
		Bitmap* bmp = nullptr;
		if (mtls.size() > 0) {
			bmp = mtls.back().bmpPath.length() > 0 ? Bitmap::getBitmap(mtls.back().bmpPath) : nullptr;
		}
		if (currentObject.length() > 0 && trs.size() > 0)
			addShape(shapeName + currentObject, trs, { 0,0,0 }, bmp);
		iss >> currentObject;
		trs.clear();
	}

	void caseV(std::istringstream& iss, std::vector<Vertex>& v, const float& scale) {
		float n;
		std::vector<float> vals;
		while (iss >> n) {
			vals.push_back(n);
		}
		if (vals.size() == 3 || vals.size() == 6) {
			Vertex tmp(vals[0] * scale, vals[1] * scale, vals[2] * scale);
			v.push_back(tmp);
		}
		else {
			std::cerr << "wrong input values v" << std::endl;
			return;
		}
	}

	void caseVt(std::istringstream& iss, std::vector<Point2D<float>>& vt) {
		float n;
		std::vector<float> vals;
		while (iss >> n) {
			vals.push_back(n);
		}
		if (vals.size() == 2 || vals.size() == 3) {
			//Point2D tmp(mtls.back().bmpDim.x * vals[0], std::abs(mtls.back().bmpDim.y * vals[1] - mtls.back().bmpDim.y));
			vt.push_back(Point2D<float>(vals[0], vals[1]));
		}
		else {
			std::cerr << "wrong input values vt" << std::endl;
			return;
		}
	}

	void caseVn(std::istringstream& iss, std::vector<Vector>& vn) {
		float n;
		std::vector<float> vals;
		while (iss >> n) {
			vals.push_back(n);
		}
		if (vals.size() == 3) {
			Vector tmp(vals[0], vals[1], vals[2]);
			vn.push_back(tmp);
		}
		else {
			std::cerr << "wrong input values vn" << std::endl;
			return;
		}
	}

	void caseF(std::istringstream& iss, std::vector<MTL>& mtls, std::vector<Triangle>& trs, std::vector<Vertex>& v, std::vector<Point2D<float>>& vt, std::vector<Vector>& vn) {
		Color col = black;
		Bitmap* bmp = nullptr;
		if (mtls.size() > 0) {
			col = mtls.back().Kd;
			bmp = mtls.back().bmpPath.length() > 0 ? Bitmap::getBitmap(mtls.back().bmpPath) : nullptr;
		}
		std::string n;
		std::vector<std::vector<int>> ind;
		while (iss >> n) {
			std::vector<int> tmpvec;
			std::string tmpstr;
			for (char c : n) {
				if (c == '/') {
					if (tmpstr.length() == 0)tmpvec.push_back(0);
					else tmpvec.push_back(atoi(tmpstr.c_str()));
					tmpstr.clear();
				}
				else tmpstr += c;
			}
			tmpvec.push_back(atoi(tmpstr.c_str()));
			tmpstr.clear();
			ind.push_back(tmpvec);
		}

		if (ind.size() == 3) {
			if (ind[0].size() == 3 && ind[1].size() == 3 && ind[2].size() == 3) {
				// v vt vn
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[1][0] - 1],
					v[ind[2][0] - 1],
					vn[ind[0][2] - 1],
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[1][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0));
			}
			if (ind[0].size() == 2 && ind[1].size() == 2 && ind[2].size() == 2) {
				// v vt
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[1][0] - 1],
					v[ind[2][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[1][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0));
			}
			if (ind[0].size() == 1 && ind[1].size() == 1 && ind[2].size() == 1) {
				// v vt
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[1][0] - 1],
					v[ind[2][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					Point2D<float>(0, 0), Point2D<float>(0, 0), Point2D<float>(0, 0));
			}
		}
		if (ind.size() == 4) {
			if (ind[0].size() == 3 && ind[1].size() == 3 && ind[2].size() == 3 && ind[3].size() == 3) {
				// v vt vn
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[1][0] - 1],
					v[ind[2][0] - 1],
					vn[ind[0][2] - 1],
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[1][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0));
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[2][0] - 1],
					v[ind[3][0] - 1],
					vn[ind[0][2] - 1],
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[3][1] - 1] : Point2D<float>(0, 0));
			}
			if (ind[0].size() == 2 && ind[1].size() == 2 && ind[2].size() == 2 && ind[3].size() == 2) {
				// v vt
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[1][0] - 1],
					v[ind[2][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[1][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0));
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[2][0] - 1],
					v[ind[3][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[3][1] - 1] : Point2D<float>(0, 0));
			}
			if (ind[0].size() == 1 && ind[1].size() == 1 && ind[2].size() == 1 && ind[3].size() == 1) {
				// v vt
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[1][0] - 1],
					v[ind[2][0] - 1],
					Vector(1, 1, 1),
					col, bmp,
					Point2D<float>(0, 0), Point2D<float>(0, 0), Point2D<float>(0, 0));
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[2][0] - 1],
					v[ind[3][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					Point2D<float>(0, 0), Point2D<float>(0, 0), Point2D<float>(0, 0));
			}
		}
		if (ind.size() == 5) {
			if (ind[0].size() == 3 && ind[1].size() == 3 && ind[2].size() == 3 && ind[3].size() == 3) {
				// v vt vn
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[1][0] - 1],
					v[ind[2][0] - 1],
					vn[ind[0][2] - 1],
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[1][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0));
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[2][0] - 1],
					v[ind[3][0] - 1],
					vn[ind[0][2] - 1],
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[3][1] - 1] : Point2D<float>(0, 0));
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[3][0] - 1],
					v[ind[4][0] - 1],
					vn[ind[0][2] - 1],
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[3][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[4][1] - 1] : Point2D<float>(0, 0));
			}
			if (ind[0].size() == 2 && ind[1].size() == 2 && ind[2].size() == 2 && ind[3].size() == 2) {
				// v vt
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[1][0] - 1],
					v[ind[2][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[1][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0));
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[2][0] - 1],
					v[ind[3][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[2][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[3][1] - 1] : Point2D<float>(0, 0));
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[3][0] - 1],
					v[ind[4][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					vt.size() > 0 ? vt[ind[0][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[3][1] - 1] : Point2D<float>(0, 0),
					vt.size() > 0 ? vt[ind[4][1] - 1] : Point2D<float>(0, 0));
			}
			if (ind[0].size() == 1 && ind[1].size() == 1 && ind[2].size() == 1 && ind[3].size() == 1) {
				// v vt
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[1][0] - 1],
					v[ind[2][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					Point2D<float>(0, 0), Point2D<float>(0, 0), Point2D<float>(0, 0));
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[2][0] - 1],
					v[ind[3][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					Point2D<float>(0, 0), Point2D<float>(0, 0), Point2D<float>(0, 0));
				trs.emplace_back(
					v[ind[0][0] - 1],
					v[ind[3][0] - 1],
					v[ind[4][0] - 1],
					Vector(1, 1, 1),
					col,
					bmp,
					Point2D<float>(0, 0), Point2D<float>(0, 0), Point2D<float>(0, 0));
			}
		}
	}

public:
	/*=============================================================================================
	 *		Attributs
	 *===========================================================================================*/

	 // Un vecteur de pointeurs vers les shapes
	std::vector<std::unique_ptr<Shape>> shapes;

	/*=============================================================================================
	 *		Méthodes
	 *===========================================================================================*/

	void imprtShapeObj(const std::string& path, const std::string& source, const std::string& shapeName, const float& scale = 1) {
		if (nameTaken(shapeName)) {
			PRINT_ON_ERR("Name for shape is already taken");
			return;
		}
		std::ifstream in(path + source);
		if (!in.is_open()) {
			in = std::ifstream(path + source);
		}
		if (!in.is_open()) {
			PRINT_ON_ERR(std::string("There's no file named ") + path + source);
			return;
		}
		std::string currentObject;
		std::vector<MTL> mtls;
		std::vector<Vertex> v;
		std::vector<Vector> vn;
		std::vector<Point2D<float>> vt;
		std::vector<Triangle> trs;
		std::string line;
		while (std::getline(in, line)) {
			std::istringstream iss(line);
			std::string type;
			iss >> type;
			if (!type.compare("mtllib")) {
				caseMtllib(iss, mtls, path);
			}
			if (!type.compare("usemtl")) {
				caseUsemtl(iss, mtls);
			}
			if (/*!type.compare("g") || */!type.compare("o")) {
				caseO(iss, mtls, trs, currentObject, shapeName);
			}
			if (!type.compare("v")) {
				caseV(iss, v, scale);
			}
			if (!type.compare("vt")) {
				caseVt(iss, vt);
			}
			if (!type.compare("vn")) {
				caseVn(iss, vn);
			}
			if (!type.compare("f")) {
				caseF(iss, mtls, trs, v, vt, vn);
			}
			if (type.size() <= 0 || type[0] == '#' || !type.compare("vp") || !type.compare("l")) {
				continue;
			}
		}
		Bitmap* bmp = nullptr;
		if (mtls.size() > 0) {
			bmp = mtls.back().bmpPath.length() > 0 ? Bitmap::getBitmap(mtls.back().bmpPath) : nullptr;
		}
		std::string lastShape(shapeName + currentObject);
		if (lastShape.length() > 0 && trs.size() > 0)
			addShape(lastShape, trs, { 0,0,0 }, bmp);
	}

	void exprtShapeObj(const std::string& shape) {
		if (!nameTaken(shape))return; // no shape named that way

		std::ofstream mtl(pth + std::string("OBJ/") + shape + ".mtl");
		mtl << "newmtl " << shape << "_mtl " <<
			"\n\tKd 0 0 0 " <<
			"\n\tmap_Ka " << getShape(shape).bmp->name.c_str() << ".png\n\tmap_Kd " << getShape(shape).bmp->name.c_str() << ".png\n";
		IMG_SavePNG(getShape(shape).bmp->surface, std::string(pth + std::string("OBJ/") + getShape(shape).bmp->name + ".png").c_str());


		std::ofstream out(pth + std::string("OBJ/") + shape + ".obj");
		out << "# " + shape + ".obj\n";
		out << "#\n\n";
		out << "o " + shape + "\n";
		out << "mtllib " << shape << ".mtl\n";
		for (size_t tr = 0; tr < getShape(shape).triangles.size(); tr++) {
			out << std::fixed << std::setprecision(8);
			out << "v "
				<< getShape(shape).triangles[tr].a.x << " "
				<< getShape(shape).triangles[tr].a.y << " "
				<< getShape(shape).triangles[tr].a.z << "\n";

			out << "v "
				<< getShape(shape).triangles[tr].b.x << " "
				<< getShape(shape).triangles[tr].b.y << " "
				<< getShape(shape).triangles[tr].b.z << "\n";

			out << "v "
				<< getShape(shape).triangles[tr].c.x << " "
				<< getShape(shape).triangles[tr].c.y << " "
				<< getShape(shape).triangles[tr].c.z << "\n";
		}
		out << "\n";
		for (size_t tr = 0; tr < getShape(shape).triangles.size(); tr++) {
			out << std::fixed << std::setprecision(8);
			out << "vt "
				<< (float)getShape(shape).triangles[tr].bmpA.x << " "
				<< (float)getShape(shape).triangles[tr].bmpA.y << "\n";

			out << "vt "
				<< (float)getShape(shape).triangles[tr].bmpB.x << " "
				<< (float)getShape(shape).triangles[tr].bmpB.y << "\n";

			out << "vt "
				<< (float)getShape(shape).triangles[tr].bmpC.x << " "
				<< (float)getShape(shape).triangles[tr].bmpC.y << "\n";
		}
		out << "\n";
		for (size_t tr = 0; tr < getShape(shape).triangles.size(); tr++) {
			out << std::fixed << std::setprecision(8);
			out << "vn "
				<< getShape(shape).triangles[tr].normalVec.x << " "
				<< getShape(shape).triangles[tr].normalVec.y << " "
				<< getShape(shape).triangles[tr].normalVec.z << "\n";
		}
		out << "\ng " + shape + "\n";
		out << "usemtl " << shape << "_mtl \n";
		for (size_t tr = 0; tr < getShape(shape).triangles.size(); tr++) {
			out << std::fixed << std::setprecision(8);
			out << "f " << std::to_string((tr + 1) * 3 - 2) << "/" << std::to_string((tr + 1) * 3 - 2) << "/" << std::to_string(tr + 1) << " ";
			out << std::to_string((tr + 1) * 3 - 1) << "/" << std::to_string((tr + 1) * 3 - 1) << "/" << std::to_string(tr + 1) << " ";
			out << std::to_string((tr + 1) * 3) << "/" << std::to_string((tr + 1) * 3) << "/" << std::to_string(tr + 1) << "\n";
		}
	}

	// pas terminé
	void exprt(const std::string& name)const {
		std::ofstream out(name + ".flanf");
		for (size_t s = 0; s < shapes.size(); s++) {
			out << "shape " + shapes[s]->name + "\n";
			for (size_t t = 0; t < shapes[s]->triangles.size(); t++) {
				out << "tr ";
				out << (Uint8)shapes[s]->triangles[t].color.r << " "
					<< (Uint8)shapes[s]->triangles[t].color.g << " "
					<< (Uint8)shapes[s]->triangles[t].color.b << " "
					<< (Uint8)shapes[s]->triangles[t].color.a << " ";
				out << (bool)shapes[s]->triangles[t].fill << " ";
				if (shapes[s]->triangles[t].bmp == nullptr) {
					out << "null null ";
				}
				else {
					out << shapes[s]->triangles[t].bmp->name << " "
						<< shapes[s]->triangles[t].bmp->path << " ";
				}
				out << std::fixed << std::setprecision(8);
				out << shapes[s]->triangles[t].a.x << " "
					<< shapes[s]->triangles[t].a.y << " "
					<< shapes[s]->triangles[t].a.z << " "
					<< shapes[s]->triangles[t].b.x << " "
					<< shapes[s]->triangles[t].b.y << " "
					<< shapes[s]->triangles[t].b.z << " "
					<< shapes[s]->triangles[t].c.x << " "
					<< shapes[s]->triangles[t].c.y << " "
					<< shapes[s]->triangles[t].c.z << " endtr\n";
			}
			out << "endshape\n";
		}
	}

	// pas terminé
	void imprt(const std::string& name) {
		std::ifstream in(name + ".flanf");
		std::string nxt;
		while (in >> nxt) {
			//std::cout << nxt << std::endl;
			if (!nxt.compare("shape")) {
				std::string name;
				in >> name;
				std::string tr;
				std::vector<Triangle> trs;
				while (in >> tr) {
					if (!tr.compare("tr")) {
						Vertex a, b, c;
						Vector n{};
						Color color = black;
						bool fill = true;
						std::string bmpName;
						std::string bmpPath;
						in >> color.r;
						in >> color.g;
						in >> color.b;
						in >> color.a;
						in >> fill;
						in >> bmpName; // handle "null"
						in >> bmpPath;
						in >> a.x;
						in >> a.y;
						in >> a.z;
						in >> b.x;
						in >> b.y;
						in >> b.z;
						in >> c.x;
						in >> c.y;
						in >> c.z;
						trs.push_back(Triangle(a, b, c, n, color, fill));
					}
					else if (!tr.compare("endtr")) continue;
					else if (!tr.compare("endshape")) break;
				}
				std::cout << name << std::endl;
				for (auto& tr : trs) {
					std::cout << tr.fill << std::endl;
					std::cout << (unsigned)tr.color.r << " " << (unsigned)tr.color.g << " " << (unsigned)tr.color.b << " " << (unsigned)tr.color.a << std::endl;
				}
				//Bitmap::newBitmap(bmpName)
				addShape(name, trs, { 0,0,0 }, nullptr);
			}
		}
	}

	/**
	 * @brief Vérifie si une shape avec le nom donnée existe déjà
	 *
	 * @param name Nom à vérifier
	 */
	bool nameTaken(const std::string& name) const {
		for (size_t i = 0; i < shapes.size(); i++) if (!name.compare(shapes[i]->name)) return true;
		return false;
	}

	/**
	 * @brief Donne le premier nom unique disponible
	 *
	 * @param name Nom à vérifier
	 */
	std::string giveUniqueName() const {
		std::string newName;
		return newName;
	}

	/**
	 * @brief Renvoie la shape demandée par reference
	 *
	 * @param name Nom de la shape désirée
	 */
	Shape& getShape(const std::string& name) {
		for (size_t i = 0; i < shapes.size(); i++) if (!name.compare(shapes[i]->name)) return *shapes[i];
		std::cout << "Error: there is no shape named " << name << std::endl;
		exit(1);
	}

	/**
	 * @brief Ajouter une nouvelle forme à partir de triangles, d'un centre et d'une texture
	 *
	 * @param Nom unique de la shape
	 * @param triangles Vecteur de triangles qui composent la forme
	 * @param center Le sommet centre de la forme
	 * @param bmp La texture de la forme
	 */
	void addShape(const std::string& name, const std::vector<Triangle>& triangles, const Vertex& center, Bitmap* bmp = nullptr) {
		if (nameTaken(name)) { std::cout << "Name taken" << std::endl; return; }
		shapes.emplace_back(new Shape(name, triangles, center, bmp));
	}

	/**
	 * @brief Ajouter une nouvelle forme à partir d'une forme existante
	 *
	 * @param shape Shape à copier
	 */
	void addShape(const Shape& shape) {
		if (nameTaken(shape.name)) { std::cout << "Name taken" << std::endl; return; }
		shapes.emplace_back(new Shape(shape));
	}

	/**
	 * @brief Ajoute une nouvelle sphere
	 *
	 * @param Nom unique de la shape
	 * @param center Sommet représentant le centre de la sphère
	 * @param radius Valeur numérique représentant l'angle de la sphère
	 * @param bmp Bitmap
	 * @param precision Degré de précision (par defaut : 20)
	 */
	void addSphere(const std::string& name, const Vertex& center, const double& radius, Bitmap* bmp, const int& precision = 20) {
		if (nameTaken(name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Sphere(name, center, radius, bmp, precision));
	}

	/**
	 * @brief Ajoute une nouvelle sphère
	 *
	 * @param Nom unique de la shape
	 * @param center Sommet représentant le centre de la sphère
	 * @param radius Valeur numérique représentant l'angle de la sphère
	 * @param color Indique la couleur de la sphère
	 * @param precision Degré de précision (par defaut : 20)
	 */
	void addSphere(const std::string& name, const Vertex& center, const double& radius, const Color& color, const int& precision = 20) {
		if (nameTaken(name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Sphere(name, center, radius, color, precision));
	}

	/**
	 * @brief Ajoute une sphère pré-existante
	 *
	 * @param sphere Sphère qui sera ajoutée
	 */
	void addSphere(const Sphere& sphere) {
		if (nameTaken(sphere.name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Sphere(sphere));
	}

	/**
	 * @brief Ajoute un cube
	 *
	 * @param Nom unique de la shape
	 * @param center Sommet représentant le centre de la sphère
	 * @param width indique la largeur du cube (Taille des arètes)
	 * @param bmp Bitmap
	 */
	void addCube(const std::string& name, const Vertex& center, const double& width, Bitmap* bmp = nullptr) {
		if (nameTaken(name)) { std::cout << "error" << std::endl; return; }
		if (bmp == nullptr) shapes.emplace_back(new Cube(name, center, width));
		else shapes.emplace_back(new Cube(name, center, width, bmp));
	}

	/**
	 * @brief Ajoute un cube pré-existant
	 *
	 * @param cube Cube qui sera ajouté
	 */
	void addCube(const Cube& cube) {
		if (nameTaken(cube.name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Cube(cube));
	}

	/**
	 * @brief Ajoute un rectangle
	 *
	 * @param Nom unique de la shape
	 * @param a,b,c,d Sommets du rectangle, permettant de le dessiner
	 * @param bmp Bitmap
	 */
	void addRectangle(const std::string& name, const Vertex& a, const Vertex& b, const Vertex& c, const Vertex& d, Bitmap* bmp = nullptr) {
		if (nameTaken(name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Rectangle(name, a, b, c, d, bmp));
	}

	/**
	 * @brief Ajoute un rectangle pré-existant
	 *
	 * @param rectangle Rectangle qui sera ajouté
	 */
	void addRectangle(const Rectangle& rectangle) {
		if (nameTaken(rectangle.name)) { std::cout << "error" << std::endl; return; }
		shapes.emplace_back(new Rectangle(rectangle));
	}

	/**
	 * @brief Retire la shape qui porte le nom donné
	 *
	 * @param name Nom de la shape recherchée
	 */
	void removeShape(const std::string& name) {
		for (size_t i = 0; i < shapes.size(); i++)
			if (!shapes[i]->name.compare(name)) {
				shapes.erase(shapes.begin() + i);
				shapes.shrink_to_fit();
				return;
			}
		std::cout << "Warning : A Shape named " << name << " does not exist" << std::endl;
	}

	/**
	 * @brief ???
	 *
	 * @param window Fenêtre avec laquelle on interagit
	 */
	void set2ds(const Window& window, const Point2D<int>& center) {
		for (int i = 0; i < static_cast<int>(shapes.size()); i++) {
			for (int j = 0; j < static_cast<int>(shapes[i]->triangles.size()); j++)
				shapes[i]->triangles[j].setScreenCoord(window, true, center);
		}
	}

	/**
	 * @brief ???
	 *
	 * @param startingPos
	 * @param movement
	 * @param intersectionZone
	 * @param intersectionPoint
	 * @param interactionDistance
	 * @return Retourne 'True' si... et 'False' si...
	 */
	bool getFirstInteraction(const Vertex& startingPos, const Vector& movement, Triangle& intersectionZone, Vertex& intersectionPoint, const float& interactionDistance) const {
		const Vertex goalPos = startingPos + movement;
		for (size_t i = 0; i < shapes.size(); i++) {
			for (size_t j = 0; j < shapes[i]->triangles.size(); j++) {
				/*if (shapes[i]->triangles[j].a.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].b.distance(startingPos) > interactionDistance + 10 &&
					shapes[i]->triangles[j].c.distance(startingPos) > interactionDistance + 10)continue;*/
				const Vector triangleNormal(Vector(shapes[i]->triangles[j].b - shapes[i]->triangles[j].a).cross(shapes[i]->triangles[j].c - shapes[i]->triangles[j].a));
				const Vertex planePoint(shapes[i]->triangles[j].a);
				const Vertex tmp(Maths::getIntersectionWithPlane(startingPos, movement, triangleNormal, planePoint)); // check if there is an intersection between movement-ray and triangle-plane
				if (tmp.getDistanceToOrigin() != 0) {
					intersectionPoint = tmp; // copy value (not address)
					if (abs(Maths::distancePointTriangle(intersectionPoint, shapes[i]->triangles[j].a, shapes[i]->triangles[j].b, shapes[i]->triangles[j].c)) > interactionDistance)
						continue; // check if point is on triangle
					if (abs(intersectionPoint.distanceToLine(startingPos, goalPos)) > interactionDistance)
						continue; // check if point is on segment
					intersectionZone = shapes[i]->triangles[j];
					return true;
				}
				else
					continue; // no intersection
			}
		}
		return false;
	}
};
