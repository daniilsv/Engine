#pragma once
class Player {
public:
	Player();
	~Player();
	void tick();
	void draw();
	Object *obj;
	V3 View;
	V3 Strafe;
	float Speed;
	float height;
};

