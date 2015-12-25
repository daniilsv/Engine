#include "main.h"

Object::Object(float _mass, V3 _pos, char *modelfile) {
	pobj = new PObject(_mass, _pos);
	model = new Model(modelfile);
	model->color = { 0.5f, 0.0f, 0.3f };
	//is_static ?
	divType = DIV_TYPE_SPHERE;
}

void Object::tick() {
	if (pobj->mass == 0.0f)
		return;
	pobj->init();
	pobj->applyForce(force);
	force = { 0.0f,0.0f,0.0f };
	pobj->applyGravity();
	pobj->applyCollision();
	//	pobj->applyFriction();
	pobj->tick(dT);
}
void Object::draw() {
	model->drawModel(pobj->position);

	glPushMatrix();
	glTranslatef(V3toxyz(pobj->position));
	glBegin(GL_LINES);
	for (ModelPart mp : model->parts)
		for (PolygonS p : mp.polygons) {
			V3 tmpvstart = (p.p1 + p.p2 + p.p3) / 3;
			V3 tmlvend = tmpvstart + p.normal;
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(V3toxyz(tmpvstart));
			glVertex3f(V3toxyz(tmlvend));
		}
	glEnd();
	glPopMatrix();

}

void Object::applyForce(V3 _force) {
	force += _force;
}

