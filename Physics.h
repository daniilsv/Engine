#pragma once
class Physics {
public:
	Physics();
	~Physics();
	void toggleGravity();
	V3 getCollision(Object *o1, PolygonS *p2);
	V3 getCollision(PObject *o1, PolygonS *p2);
	bool is_gravity;
	float g;
};

class PObject : Physics {
public:
	PObject();
	PObject(float _mass);
	PObject(float _mass, V3 _pos);
	void init();
	void applyVelocity(V3 _velocity);
	void applyForce(V3 _force);
	void applyGravity();
	void applyFriction();

	void applyCollision();

	void tick(float dt);

	float mass;
	V3 position;
	V3 velocity;
	V3 force;
};
