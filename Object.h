#pragma once
class Object{
public:
	Object(float _mass, V3 _pos, char *modelfile);
	void tick();
	void draw();
	void applyForce(V3 _force);

	unsigned short divType;

	PObject *pobj;
	Model *model;
	V3 force;
};

