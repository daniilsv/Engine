#include "main.h"

Player::Player() {
	height = 0.75f;
	obj = new Object(60.0f, { 0.0f,0.0f,0.0f }, "");
	objs.push_back(obj);
	View = Normalize({ 1.0f,0.0f,0.0f });
	Strafe = Normalize({ 1.0f,0.0f,0.0f });
	Speed = 0.3f;
}

Player::~Player() {

}

void Player::tick() {
	View = camera->View;
	Strafe = camera->Strafe;

	if (kbd['w'])
		obj->pobj->position += View * Speed;
	else if (kbd['s'])
		obj->pobj->position -= View * Speed;

	if (kbd['a'])
		obj->pobj->position -= Strafe * Speed;
	else if (kbd['d'])
		obj->pobj->position += Strafe * Speed;


	if (!physics->is_gravity) {
		if (kbd[' ']) {
			obj->pobj->position += camera->Up * Speed;
		}
		else if (is_ctrl) {
			obj->pobj->position -= camera->Up * Speed;
		}
	}
	else {
		if (kbd[' ']) {
			obj->applyForce({ 0.0f,600.0f,0.0f });
		}
		else if (is_ctrl) {
			obj->pobj->position -= camera->Up * Speed;
		}
	}
	camera->Position = obj->pobj->position + V3{ 0.0f, height, 0.0f };
}


void Player::draw() {
	glPushMatrix();
	glTranslatef(V3toxyz(obj->pobj->position));
	glutWireSphere(2.0, 10, 10);
	glPopMatrix();
}
