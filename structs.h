struct V3 {
public:
	V3() {

	}
	V3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	V3(float _x, float _y) {
		x = _x;
		y = _y;
		z = 0.0f;
	}
	V3 operator +(V3 v) {
		return{ x + v.x,y + v.y,z + v.z };
	}
	V3 operator -(V3 v) {
		return{ x - v.x,y - v.y,z - v.z };
	}
	void operator +=(V3 v) {
		x += v.x;
		y += v.y;
		z += v.z;
	}
	void operator -=(V3 v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	V3 operator +(float f) {
		return{ x + f,y + f,z + f };
	}
	V3 operator -(float f) {
		return{ x - f,y - f,z - f };
	}
	V3 operator /(float f) {
		return{ x / f,y / f,z / f };
	}
	V3 operator *(float f) {
		return{ x * f,y * f,z * f };
	}
	bool operator ==(V3 v) {
		return (x != v.x && y != v.y && z != v.z) ? false : true;
	}
	bool operator !=(V3 v) {
		return (x != v.x && y != v.y && z != v.z) ? true : false;
	}
	string toString() {
		return format("%f x %f x %f", x, y, z);
	}
	float x, y, z;
};


struct PolygonS {
	V3 p1, p2, p3;
	V3 normal;
	char *tex;
	V3 tp1, tp2, tp3;
};