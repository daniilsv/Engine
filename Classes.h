#pragma once

#pragma comment(lib, "ftgl_dynamic_MT.lib")
#include <ftgl/FTGLBitmapFont.h>
#include <ftgl/FTGLOutlineFont.h>

#pragma comment(lib,"DevIL.lib")
#pragma comment(lib,"ilut.lib")
#pragma comment(lib,"ilu.lib")
#include <il/ilu.h>

class Quaternion {
public:
	Quaternion();
	Quaternion(V3 v);
	Quaternion(V3 v, float degrees, bool is_radian = true);
	void Quaternion::CreateFromAxisAngle(V3 v, float degrees);
	void Quaternion::CreateFromAxisRadian(V3 v, float angle);

	Quaternion* Quaternion::mul(Quaternion*);
	Quaternion* Quaternion::sum(Quaternion*);
	Quaternion* Quaternion::Inverse();
	float Quaternion::QMagnitude();

	Quaternion rotate(Quaternion q);
	Quaternion rotate(V3 q);

	static Quaternion rotate(Quaternion *q, Quaternion *p);
	static Quaternion rotate(V3 qv, Quaternion *p);
	static Quaternion rotate(Quaternion *q, V3 v, float a, bool i = true);
	static Quaternion rotate(V3 qv, V3 pv, float a, bool i = true);

	operator V3();
	V3 v;
	float w;
};

class CFont {
public:
	CFont(char *ttf, byte s, float FDepth);
	FTFont *Font;
	void Printf(V3 p, V3 c, wstring fmt, ...);
	void Print(V3 p, V3 c, wstring text);
};

class Functions {
public:
	vector<wstring> param;
	void exitA();
	void toggle();
	void toXYZ();
	void toHome();
};

class Console {
public:
	Console();

	void toggle();
	void draw();
	void println(wstring string);
	void parseln(wstring string);

	bool is_open = false;
	wstring stack[10];
	std::map <wstring, void(Functions::*)()> funcs;
};
