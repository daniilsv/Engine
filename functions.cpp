#include "main.h"

void to2D() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, START_WIDTH, 0, START_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glLoadIdentity();
}
void backTo3D() {
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void draw2DTexturedSquare(V3 p1, V3 p2, V3 c, V3 tp1, V3 tp2) {
	glPushMatrix();
	glColor3f(c.x, c.y, c.z);
	glBegin(GL_POLYGON);
	glTexCoord2f(tp1.x, tp2.y); glVertex2f(p1.x, START_HEIGHT - p1.y);
	glTexCoord2f(tp2.x, tp2.y); glVertex2f(p2.x, START_HEIGHT - p1.y);
	glTexCoord2f(tp2.x, tp1.y); glVertex2f(p2.x, START_HEIGHT - p2.y);
	glTexCoord2f(tp1.x, tp1.y); glVertex2f(p1.x, START_HEIGHT - p2.y);
	glEnd();
	glPopMatrix();
}

void split(wstring &s, const wchar_t* delim, vector<wstring> & v) {
	wchar_t * dup = _wcsdup(s.c_str());
	wchar_t * token = wcstok(dup, delim);
	while (token != NULL) {
		v.push_back(wstring(token));
		token = wcstok(NULL, delim);
	}
	delete(dup);
}
inline string format(const char* fmt, ...) {
	int size = 512;
	char* buffer = 0;
	buffer = new char[size];
	va_list vl;
	va_start(vl, fmt);
	int nsize = vsnprintf(buffer, size, fmt, vl);
	if (size <= nsize) { //fail delete buffer and try again
		delete[] buffer;
		buffer = 0;
		buffer = new char[nsize + 1]; //+1 for /0
		nsize = vsnprintf(buffer, size, fmt, vl);
	}
	std::string ret(buffer);
	va_end(vl);
	delete[] buffer;
	return ret;
}

void addBox(float mass, V3 pos) {
	Object *obj = new Object(mass, pos, "box.dmodel");
	objs.push_back(obj);
}
