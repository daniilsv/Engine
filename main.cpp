#include "main.h"

Camera *camera;
Player *player;
Physics *physics;
Console *cout;
Functions *function;

CFont *arial_16;

float dT = 1 / 16.0f;
V3 tmpV3;
vector<Object *> objs;

bool Pause = false;

bool kbd[256] = { false };
bool is_alt = false;
bool is_ctrl = false;
bool is_shift = false;

V3 yy1 = { -0.5f, 0.0f, 0.0f };
V3 yy2 = { 0.0f, 1.0f, 0.0f };
V3 yy3 = { 0.5f, 0.0f, 0.0f };

Quaternion *qq = new Quaternion({ 0.0f,1.0f,0.0f }, 0.025f);

wstring tmpStr;

void ReDisplay(int v) {
	if (v == 1)
		glutPostRedisplay();
}

void Render() {
	camera->Look();

	for (int i = -5; i < 6; ++i) {
		for (int k = -5; k < 5; ++k) {
			glBegin(GL_POLYGON);
			glColor3f((i + 5) / 10.0f, (10 - i - k) / 20.0f, (k + 5) / 10.0f);
			glVertex3f(i*2.0f, 0.0f, k*2.0f);
			glVertex3f(i*2.0f, 0.0f, k*2.0f + 1.5f);
			glVertex3f(i*2.0f + 1.5f, 0.0f, k*2.0f + 1.5f);
			glVertex3f(i*2.0f + 1.5f, 0.0f, k*2.0f);
			glEnd();

			glBegin(GL_TRIANGLES);
			glColor3f((10 - i - k) / 15.0f, (k + 5) / 5.0f, (i + 5) / 10.0f);
			glVertex3f(i*2.0f + yy1.x, yy1.y, k*2.0f + yy1.z);
			glVertex3f(i*2.0f + yy2.x, yy2.y, k*2.0f + yy2.z);
			glVertex3f(i*2.0f + yy3.x, yy3.y, k*2.0f + yy3.z);
			glEnd();
		}
	}

	for (Object *obj : objs)
		obj->draw();

}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	Render();
	player->draw();
	to2D();
	cout->draw();
	arial_16->Printf({ 300.0f, 13.0f }, { 1.0f,0.0f,1.0f }, L"%f - %f - %f", V3toxyz(player->obj->pobj->position));
	arial_16->Printf({ 300.0f, 30.0f }, { 0.0f,1.0f,1.0f }, L"%s", (physics->is_gravity ? L"ON" : L"OFF"));
	arial_16->Printf({ 300.0f, 45.0f }, { 0.0f,1.0f,0.0f }, L"%f - %f - %f", V3toxyz(tmpV3));

	backTo3D();

	glutSwapBuffers();
	glutTimerFunc(16, ReDisplay, 1);
}

void Idle(int v) {
	if (!Pause) {
		player->tick();

		yy1 = qq->rotate(Normalize(yy1));
		yy2 = qq->rotate(Normalize(yy2));
		yy3 = qq->rotate(Normalize(yy3));

		for (Object *obj : objs)
			obj->tick();
	}
	glutTimerFunc(16, Idle, 0);
}

void Motion(int mx, int my) {
	if (Pause)
		return;
	camera->SetViewByMouse(mx, my);
}
void PassiveMotion(int mx, int my) {
	if (Pause)
		return;
	camera->SetViewByMouse(mx, my);
}

void Reshape(int width, int height) {
	if (height == 0)
		height = 1;

	float ratio = width * 1.0f / height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void KbdSpecial(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_ALT_L:
	case GLUT_KEY_ALT_R:
		is_alt = true;
		break;
	case GLUT_KEY_CTRL_L:
	case GLUT_KEY_CTRL_R:
		is_ctrl = true;
		break;
	case GLUT_KEY_SHIFT_L:
	case GLUT_KEY_SHIFT_R:
		is_shift = true;
		break;
	}
	switch (key) {
	case GLUT_KEY_F1:
		Pause = !Pause;
		break;
	case GLUT_KEY_F2:
		cout->println(L"F2");
		break;
	case GLUT_KEY_F3:
		cout->println(L"F3");
		break;
	case GLUT_KEY_F4:
		cout->println(L"F4");
		if (is_alt) {
			is_alt = false;
			exit(0);
		}
		break;
	case GLUT_KEY_F11:
		glutFullScreenToggle();
		cout->println(L"Toggle full screen...");
		break;
	case GLUT_KEY_F12:
		cout->toggle();
		break;
	case GLUT_KEY_PAGE_UP:
		for (Object *obj : objs)
			obj->applyForce({ 0.0f,500.0f,0.0f });
		break;
	case GLUT_KEY_PAGE_DOWN:
		for (Object *obj : objs)
			obj->applyForce({ 0.0f,-500.0f,0.0f });
		break;
	}
}
void KbdSpecialReleased(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_ALT_L:
	case GLUT_KEY_ALT_R:
		is_alt = false;
		break;
	case GLUT_KEY_CTRL_L:
	case GLUT_KEY_CTRL_R:
		is_ctrl = false;
		break;
	case GLUT_KEY_SHIFT_L:
	case GLUT_KEY_SHIFT_R:
		is_shift = false;
		break;
	}
}

void KpdPresed(byte ch, int x, int y) {
	if (is_alt) {
		switch (ch) {
		case 'g':
			physics->toggleGravity();
			is_alt = false;
		}
	}
	if (Pause) {
		if (ch == 13) {
			if (tmpStr.empty())
				return;
			cout->println(tmpStr);
			cout->parseln(tmpStr);
			tmpStr.clear();
		}
		else if (ch == '\b') {
			tmpStr.pop_back();
		}
		else {
			wchar_t c = (ch >= 192 && ch <= 255) ? ch + 848 : ch;
			tmpStr.push_back(c);
		}
	}
	else {
		if (ch >= 65 && ch <= 90 || ch >= 192 && ch <= 223)
			ch += 32;
		if (ch < 256)
			kbd[ch] = true;
	}

}
void KbdReleased(byte ch, int x, int y) {
	if (ch >= 65 && ch <= 90 || ch >= 192 && ch <= 223)
		ch += 32;
	if (ch < 256)
		kbd[ch] = false;
}

void InitGL() {
	tmpStr.clear();
	cout = new Console;
	function = new Functions;
	cout->println(L"Начинаем!");

	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotion);
	glutKeyboardFunc(KpdPresed);
	glutKeyboardUpFunc(KbdReleased);
	glutSpecialFunc(KbdSpecial);
	glutSpecialUpFunc(KbdSpecialReleased);
	cout->println(L"Glut func has been setted.");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	cout->println(L"Glut vars has been setted.");

	arial_16 = new CFont("Arial.ttf", 12, 16.0f);

	camera = new Camera;
	cout->println(L"Camera.");
	player = new Player;
	cout->println(L"Engine.");
	physics = new Physics;
	cout->println(L"Physics.");

	addBox(2.0f, { 0.0f, 7.0f, 16.0f });
	addBox(500.0f, { 0.0f, 0.0f, 18.0f });

	Idle(0);
	cout->println(L"GO.");
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	int argc = 2;
	char *argv[2] = { "Engine.exe", lpCmdLine };
	glutInit_ATEXIT_HACK(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(START_WIDTH, START_HEIGHT);
	glutCreateWindow_ATEXIT_HACK(APP_TITLE);

	InitGL();
	glutMainLoop();
	////////////////////////PLACE EXIT CODE HERE

	return 0;
}