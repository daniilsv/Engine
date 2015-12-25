#include "main.h"


V3 Cross(V3 vVector1, V3 vVector2) {
	V3 vNormal;
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));
	return vNormal;
}
float Magnitude(V3 v) {
	return (float)sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}
V3 Normalize(V3 v) {
	return v / Magnitude(v);
}

short sign(int a) {
	return (a > 0) ? 1 : ((a < 0) ? -1 : a);
}


/////   Вычисляет растояние между точками
float Distance(V3 vPoint1, V3 vPoint2) {
	return sqrt((vPoint2.x - vPoint1.x) * (vPoint2.x - vPoint1.x) +
		(vPoint2.y - vPoint1.y) * (vPoint2.y - vPoint1.y) +
		(vPoint2.z - vPoint1.z) * (vPoint2.z - vPoint1.z));
}
/////   Вычисляет растояние от точки до плоскости
float PlaneDistance(V3 Normal, V3 Point) {
	return -((Normal.x * Point.x) + (Normal.y * Point.y) + (Normal.z * Point.z));
}

/////   Вычисляет скалярное произведение
float Dot(V3 vVector1, V3 vVector2) {
	return ((vVector1.x * vVector2.x) + (vVector1.y * vVector2.y) + (vVector1.z * vVector2.z));
}

/////   Ищет ближайшую точку на линии
V3 ClosestPointOnLine(V3 vA, V3 vB, V3 vPoint) {
	V3 vV = Normalize(vB - vA);
	float d = Distance(vA, vB);
	float t = Dot(vV, vPoint - vA);
	if (t <= 0)
		return vA;
	if (t >= d)
		return vB;
	return vA + vV * t;
}

/////   Вычисляет угол между векторами
float AngleBetweenVectors(V3 Vector1, V3 Vector2) {
	float dotProduct = Dot(Vector1, Vector2);
	float vectorsMagnitude = Magnitude(Vector1) * Magnitude(Vector2);

	float angle = acos(dotProduct / vectorsMagnitude);
	if (_isnan(angle))
		return 0;

	return angle;
}

bool InsidePolygon(V3 vIntersection, PolygonS *poly) {
	const double MATCH_FACTOR = 0.9999;
	double Angle = 0.0;
	Angle += AngleBetweenVectors(poly->p1 - vIntersection, poly->p2 - vIntersection);
	Angle += AngleBetweenVectors(poly->p2 - vIntersection, poly->p3 - vIntersection);
	Angle += AngleBetweenVectors(poly->p3 - vIntersection, poly->p1 - vIntersection);

	if (Angle >= (MATCH_FACTOR * (2.0 * M_PI)))
		return true;
	return false;
}

/////   Вычисляет положение сферы относительно плоскости, а так же расстояние
int ClassifySphere(PolygonS *poly, V3 sphereCenter, float radius, float &distance) {
	float pD = PlaneDistance(poly->normal, poly->p1);
	distance = abs((poly->normal.x * sphereCenter.x + poly->normal.y * sphereCenter.y + poly->normal.z * sphereCenter.z + pD));

	if (distance < radius)
		return 2;
	else if (distance >= radius)
		return 1;
	return 0;
}

/////   Определяет, пересекает ли сфера какое-либо ребро треугольника
bool LineSphereCollision(V3 linep1, V3 linep2, V3 sphereCenter, float sphereRadius) {
	if (Distance(ClosestPointOnLine(linep1, linep2, sphereCenter), sphereCenter) < sphereRadius)
		return true;
	return false;
}

/////   Определяет, пересекает ли сфера какое-либо ребро треугольника
bool EdgeSphereCollision(V3 vCenter, PolygonS *poly, float radius) {

	if (LineSphereCollision(poly->p1, poly->p2, vCenter, radius))
		return true;
	if (LineSphereCollision(poly->p2, poly->p3, vCenter, radius))
		return true;
	if (LineSphereCollision(poly->p3, poly->p1, vCenter, radius))
		return true;

	return false;
}

V3 SpherePolygonCollision(PolygonS *poly, V3 vCenter, float radius) {
	float distance = 0.0f;
	int classification = ClassifySphere(poly, vCenter, radius, distance);

	if (classification == 2) {
		V3 vOffset = poly->normal * distance;
		V3 vPosition = vCenter - vOffset;
		if (InsidePolygon(vPosition, poly) || EdgeSphereCollision(vCenter, poly, radius)) {
			V3 rad = Normalize(vOffset) * radius;
			return vOffset - rad;
		}
	}
	return{ 0.0f,0.0f,0.0f };
}