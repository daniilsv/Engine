#include "main.h"

Physics::Physics() {
	is_gravity = false;
	g = 9.8f;
}

Physics::~Physics() {

}

void Physics::toggleGravity() {
	is_gravity = !is_gravity;
}

V3 Physics::getCollision(Object *o1, PolygonS *p2) {
	//if (o1->divType == DIV_TYPE_SPHERE)
	return SpherePolygonCollision(p2, o1->pobj->position, 2.0f);
}
V3 Physics::getCollision(PObject *o1, PolygonS *p2) {
	//if (o1->divType == DIV_TYPE_SPHERE)
	return SpherePolygonCollision(p2, o1->position, 2.0f);
}

PObject::PObject() {
	mass = 1.0f;
	position = { 0.0f,0.0f,0.0f };
	velocity = { 0.0f, 0.0f, 0.0f };
	force = { 0.0f, 0.0f, 0.0f };
}
PObject::PObject(float _mass) {
	mass = _mass;
	position = { 0.0f,0.0f,0.0f };
	velocity = { 0.0f, 0.0f, 0.0f };
	force = { 0.0f, 0.0f, 0.0f };
}
PObject::PObject(float _mass, V3 _pos) {
	mass = _mass;
	position = _pos;
	velocity = { 0.0f, 0.0f, 0.0f };
	force = { 0.0f, 0.0f, 0.0f };
}

void PObject::init() {
	force = { 0.0f, 0.0f, 0.0f };
}

void PObject::applyVelocity(V3 _velocity) {
	velocity += _velocity;
}
void PObject::applyForce(V3 _force) {
	force += _force;
}
void PObject::applyGravity() {
	if (physics->is_gravity)
		applyForce({ 0.0f, -mass * g, 0.0f });
}
void PObject::applyFriction() {

}

void PObject::applyCollision() {
	for (Object *mobj : objs)
		if (mobj->pobj != this)
			for (ModelPart mp : mobj->model->parts)
				for (PolygonS poly : mp.polygons) {
					poly.p1 += mobj->pobj->position;
					poly.p2 += mobj->pobj->position;
					poly.p3 += mobj->pobj->position;
					V3 col = getCollision(this, &poly);
					position -= col;
				}
}

void PObject::tick(float dt) {
	velocity += ((force) / mass) * dt;
	position += velocity * dt;

	if (position.y < 0.0f) {
		position.y = 0.0f;
		velocity.y = -velocity.y + velocity.y / 3;
	}

	if (Magnitude(velocity) < 0.2f) {
		velocity = { 0.0f,0.0f,0.0f };
	}
}
