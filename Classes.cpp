#include "main.h"

Quaternion::Quaternion() {
}
Quaternion::Quaternion(V3 v3) {
	v.x = v3.x;
	v.y = v3.y;
	v.z = v3.z;
	w = 0.0f;
}
Quaternion::Quaternion(V3 v, float degrees, bool is_radian) {
	if (is_radian)CreateFromAxisRadian(v, degrees);
	else CreateFromAxisAngle(v, degrees);
}
void Quaternion::CreateFromAxisAngle(V3 v3, float degrees) {
	float angle = float(degrees / 180.0f * M_PI);

	float result = float(sin(angle / 2.0f));

	v.x = float(v3.x * result);
	v.y = float(v3.y * result);
	v.z = float(v3.z * result);
	w = float(cos(angle / 2.0f));
}
void Quaternion::CreateFromAxisRadian(V3 v3, float angle) {
	float result = float(sin(angle / 2.0f));

	v.x = float(v3.x * result);
	v.y = float(v3.y * result);
	v.z = float(v3.z * result);
	w = float(cos(angle / 2.0f));
}
Quaternion* Quaternion::mul(Quaternion *q) {
	Quaternion *r = new Quaternion();

	float A = (w + v.x)*(q->w + q->v.x);
	float B = (v.z - v.y)*(q->v.y - q->v.z);
	float C = (v.x - w)*(q->v.y + q->v.z);
	float D = (v.y + v.z)*(q->v.x - q->w);
	float E = (v.x + v.z)*(q->v.x + q->v.y);
	float F = (v.x - v.z)*(q->v.x - q->v.y);
	float G = (w + v.y)*(q->w - q->v.z);
	float H = (w - v.y)*(q->w + q->v.z);

	r->w = B + (-E - F + G + H) * 0.5f;
	r->v.x = A - (E + F + G + H) * 0.5f;
	r->v.y = -C + (E - F + G - H) * 0.5f;
	r->v.z = -D + (E - F - G + H) * 0.5f;

	return r;
}
Quaternion* Quaternion::sum(Quaternion *q) {
	return new Quaternion(v + q->v, w + q->w);
}
float Quaternion::QMagnitude() {
	return (float)sqrt(Magnitude(v) + w*w);
}
Quaternion* Quaternion::Inverse() {
	return new Quaternion({ -v.x,-v.y,-v.z }, w);/////////////////////////
}
Quaternion::operator V3() { return v; }

Quaternion Quaternion::rotate(Quaternion *q, Quaternion *p) {
	Quaternion *mul1 = p->mul(q);
	Quaternion *invQ = p->Inverse();
	Quaternion *mul2 = mul1->mul(invQ);
	Quaternion ret = *mul2;
	delete(invQ);
	delete(mul1);
	delete(mul2);
	return ret;
}

Quaternion Quaternion::rotate(V3 qv) {
	Quaternion *q = new Quaternion(qv);
	Quaternion ret = rotate(q, this);
	delete(q);
	return ret;
}
Quaternion Quaternion::rotate(Quaternion q) {
	return rotate(q, this);
}

Quaternion Quaternion::rotate(Quaternion *q, V3 pv, float a, bool i) {
	Quaternion *p = new Quaternion(pv, a, i);
	Quaternion ret = rotate(q, p);
	delete(p);
	return ret;
}
Quaternion Quaternion::rotate(V3 qv, Quaternion *p) {
	Quaternion *q = new Quaternion(qv);
	Quaternion ret = rotate(q, p);
	delete(q);
	return ret;
}
Quaternion Quaternion::rotate(V3 qv, V3 pv, float a, bool i) {
	Quaternion *q = new Quaternion(qv);
	Quaternion *p = new Quaternion(pv, a, i);
	Quaternion ret = rotate(q, p);
	delete(q);
	delete(p);
	return ret;
}

///////////////////////////////////////////////////////////////////////////

Console::Console() {
	funcs[L"##"] = &Functions::toggle;
	funcs[L"toXYZ"] = &Functions::toXYZ;
	funcs[L"toHome"] = &Functions::toHome;
	funcs[L"exit"] = &Functions::exitA;

}
void Console::draw() {
	if (!is_open)
		return;
	draw2DTexturedSquare({ 0.0f,0.0f }, { (float)START_WIDTH, 150.0f }, { 0.7f,0.7f,0.7f });
	int j = 0;
	for (int i = 9; i >= 0; --i) {
		if (stack[i].empty()) {
			continue;
		}
		else {
			arial_16->Print({ 10.0f, (++j) * 13.0f }, { 1.0f,1.0f,1.0f }, stack[i]);
		}
	}
	arial_16->Print({ 10.0f, (++j) * 13.0f }, { 1.0f,0.0f,1.0f }, tmpStr);

}
void Console::toggle() {
	Pause = is_open = !is_open;
	glutWarpPointer(START_WIDTH >> 1, START_HEIGHT >> 1);
}
void Console::println(wstring s) {
	for (int i = 8; i >= 0; --i)
		if (stack[i].empty())
			continue;
		else
			stack[i + 1] = stack[i];
	stack[0] = s;
}
void Console::parseln(wstring s) {
	vector<wstring> v;
	split(s, L" ", v);
	function->param = v;
	if (funcs.find(v[0]) != funcs.end())
		(*function.*funcs[v[0]])();
}


CFont::CFont(char *ttf, byte s, float FDepth) {
	char filename[100];
	sprintf(filename, FONT_DIR, ttf);
	this->Font = new FTGLBitmapFont(filename);
	Font->Depth(FDepth);
	Font->CharMap(ft_encoding_unicode);
	Font->FaceSize(s);
}
void CFont::Printf(V3 p, V3 c, wstring fmt, ...) {
	if (fmt.empty())
		return;
	wchar_t text[256];
	va_list ap;
	va_start(ap, fmt);
	vswprintf(text, fmt.c_str(), ap);
	va_end(ap);
	Print(p, c, text);
}
void CFont::Print(V3 p, V3 c, wstring text) {
	if (text.empty())
		return;
	glColor3f(c.x, c.y, c.z);

	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glTranslatef(p.x, START_HEIGHT - p.y, 0.0f);
	glRasterPos2f(-1.0f, 0.5f);
	Font->Render(text.c_str());
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Functions::exitA() {
	exit(0);
}
void Functions::toggle() {
	cout->toggle();
}
void Functions::toXYZ() {
	float x = stof(param[1]);
	float y = stof(param[2]);
	float z = stof(param[3]);
	player->obj->pobj->position = { x,y,z };
	player->tick();
}
void Functions::toHome() {
	param.push_back(L"0.0");
	param.push_back(L"0.75");
	param.push_back(L"0.0");
	toXYZ();
}