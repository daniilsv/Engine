#include "main.h"


Camera::Camera() {
	Position = { -2.0f, 0.75f, -1.0f };
	View = Normalize({ 1.0f, 0.0f, 0.0f });
	Up = Normalize({ 0.0f, 1.0f, 0.0f });
	Strafe = Normalize(Cross(View, Up));
	Pitch = new Quaternion();
	Heading = new Quaternion();
	HeadingDegrees = PitchDegrees = PitchSum = 0.0f;
	Sensitivity = 75.0f;
}
void Camera::SetViewByMouse(int mx, int my) {
	int middleX = START_WIDTH >> 1;
	int middleY = START_HEIGHT >> 1;

	if ((mx == middleX) && (my == middleY)) return;

	glutWarpPointer(middleX, middleY);
	if (my != middleY) {
		PitchDegrees = (float)atan((middleY - my) / Sensitivity);
		V3 View1 = Normalize(Quaternion::rotate(View, Strafe, PitchDegrees));
		float angle = AngleBetweenVectors(View1, Up);
		if (angle > M_PI_4 / 2 && angle < M_PI - M_PI_4 / 2)
			View = View1;
	}
	if (mx != middleX) {
		HeadingDegrees = (float)atan((middleX - mx) / Sensitivity);
		float ty = View.y;
		View = Normalize(Quaternion::rotate(View, Up, HeadingDegrees));
		View.y = ty;
	}
	Strafe = Normalize(Cross(View, Up));
}

void Camera::Look() {
	gluLookAt(Position.x, Position.y, Position.z,
		View.x + Position.x, View.y + Position.y, View.z + Position.z,
		Up.x, Up.y, Up.z);
}