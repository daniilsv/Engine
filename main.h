#pragma once
#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_WCSTOK
#include <windows.h>
#include <math.h>
#include <gl/freeglut.h>
#include <map>
#include <string>
#include <vector>

using namespace std;

#define DIV_TYPE_BOX 0x01
#define DIV_TYPE_SPHERE 0x02

#define V3toxyz(t) t.x, t.y, t.z
#define V3toxy(t) t.x, t.y

#define START_WIDTH 1280
#define START_HEIGHT 720
#define APP_TITLE "EngineTest"

#define FONT_DIR "Resources/Fonts/%s"
#define MODEL_DIR "Resources/Models/%s"

inline string format(const char* fmt, ...);

#include "structs.h"

V3 Cross(V3 vVector1, V3 vVector2);
V3 Normalize(V3 v);
float Magnitude(V3 v);
short sign(int a);
float Dot(V3 vVector1, V3 vVector2);
float Distance(V3 vPoint1, V3 vPoint2);
float PlaneDistance(V3 Normal, V3 Point);
float AngleBetweenVectors(V3 Vector1, V3 Vector2);
bool LineSphereCollision(V3 linep1, V3 linep2, V3 sphereCenter, float sphereRadius);
bool EdgeSphereCollision(V3 vCenter, PolygonS *poly, float radius);
V3 SpherePolygonCollision(PolygonS *poly, V3 vCenter, float radius);

void to2D();
void backTo3D();
void draw2DTexturedSquare(V3 p1, V3 p2, V3 c, V3 tp1 = { 0.0f ,0.0f ,0.0f }, V3 tp2 = { 0.0f ,0.0f ,0.0f });

void addBox(float mass, V3 pos);

void split(wstring &s, const wchar_t* delim, vector<wstring> & v);
inline string format(const char* fmt, ...);

class Quaternion;
class Physics;
class PObject;
class Object;
class Model;
class Player;
class Camera;
class CFont;
class Functions;
class Console;

#include "Classes.h"
#include "Camera.h"
#include "Physics.h"
#include "Model.h"
#include "Object.h"
#include "Player.h"

extern Camera *camera;
extern Player *player;
extern Physics *physics;
extern Console *cout;
extern Functions *function;
extern bool kbd[];
extern bool is_alt;
extern bool is_ctrl;
extern bool is_shift;
extern bool Pause;

extern CFont *arial_16;

extern wstring tmpStr;

extern vector<Object *> objs;

extern float dT;
extern V3 tmpV3;