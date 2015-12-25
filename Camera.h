#pragma once
class Camera
{
public:
	Camera();
	void Look();
	void SetViewByMouse(int mx, int my);
	V3 Position;
	V3 View;
	V3 Up;
	V3 Strafe;
	float Sensitivity;
	Quaternion *Pitch, *Heading;
	float PitchDegrees, HeadingDegrees, PitchSum;
};

