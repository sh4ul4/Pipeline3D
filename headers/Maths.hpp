#pragma once

namespace Maths{
    /*@brief Renvoie le barycentre d'un triangle
     *@param t un triangle
    */
    //Vertex getBarycentre(Triangle t);

    /* @brief Renvoie le vecteur normale au plan du triangle
     * @param t un triangle
    */
    //Vertex getNormalVector(Triangle t);

    /*@brief Renvoie la distance entre les deux sommets
     *@param s1 premier sommet
     *@param s2 deuxième sommet
    */
    float getDistancePoint(const Vertex& s1,const Vertex& s2);

    /*
     * @brief renvoie le sommet qui est l'intersection entre un segment et un plan
     * @param s1 le premier sommet du segment
     * @param s2 le deuxième sommet du segment 
     * @param p un point de plan
     * @param v1 un vecteur du plan
     * @param v2 un vexteur du plan
    */
    Vertex intersectionSegmentPlane(const Vertex& s1,const Vertex& s2,const Vertex& p,const Vector& v1,const Vector& v2);

	void mult3x3_3x1(const Matrix<3,3>& m, Vertex v, Vertex& res) {
		res.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z;
		res.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z;
		res.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z;
	}

	// rotate around X-axis (world-axis)
	// loss of precision due to int-float conversion
	void rotateX(Vertex& p, const float& angle) {
		Matrix<3, 3> a;
		a.m[0] = { 1,0,0 };
		a.m[1] = { 0,cos(angle),-sin(angle) };
		a.m[2] = { 0,sin(angle),cos(angle) };
		mult3x3_3x1(a, p, p);
	}

	// rotate around Y-axis (world-axis)
	// loss of precision due to int-float conversion
	void rotateY(Vertex& p, const float& angle) {
		Matrix<3, 3> a;
		a.m[0] = { cos(angle),0,sin(angle) };
		a.m[1] = { 0,1,0 };
		a.m[2] = { -sin(angle),0,cos(angle) };
		mult3x3_3x1(a, p, p);
	}

	// rotate around Z-axis (world-axis)
	// loss of precision due to int-float conversion
	void rotateZ(Vertex& p, const float& angle) {
		Matrix<3, 3> a;
		a.m[0] = { cos(angle),-sin(angle),0 };
		a.m[1] = { sin(angle),cos(angle),0 };
		a.m[2] = { 0,0,1 };
		mult3x3_3x1(a, p, p);
	}
}