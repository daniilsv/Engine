#include "main.h"

Model::Model(char *modelfile) {
	loadModel(modelfile);
	is_static = true;
}

void Model::loadModel(char *modelname) {
	byte maxObjModelCount = 1;
	char Mname[100];
	sprintf(Mname, MODEL_DIR, modelname);
	FILE *F = fopen(Mname, "rb");
	char textureName[20];////////////
	if (F == NULL)return;
	bool isPart;
	unsigned short polygonCount;
	fread(&isPart, sizeof(bool), 1, F);
	while (isPart) {
		polygonCount = 0;
		ModelPart tmpmp;
		fread(&tmpmp.name, sizeof(char), 40, F);
		fread(&polygonCount, sizeof(unsigned short), 1, F);
		for (int i = 0; i < polygonCount; ++i) {
			PolygonS tmppo;

			fread(&tmppo.p1, sizeof(V3), 1, F);
			fread(&tmppo.p2, sizeof(V3), 1, F);
			fread(&tmppo.p3, sizeof(V3), 1, F);

			fread(&tmppo.normal, sizeof(V3), 1, F);

			fread(&textureName, sizeof(char), 20, F);

			fread(&tmppo.tp1, sizeof(V3), 1, F);
			fread(&tmppo.tp2, sizeof(V3), 1, F);
			fread(&tmppo.tp3, sizeof(V3), 1, F);
			
			tmpmp.polygons.push_back(tmppo);
		}
		buildPart(&tmpmp);
		parts.push_back(tmpmp);
		fread(&isPart, sizeof(bool), 1, F);
	}

	fclose(F);
}
void Model::drawPart(ModelPart *mpart) {

	glBegin(GL_TRIANGLE_STRIP);
	for (PolygonS poly : mpart->polygons) {

		//glBindTexture(GL_TEXTURE_2D, Textures[models[modelId]->polygon[i].texId].texID);

		glNormal3f(V3toxyz(poly.normal));

		glTexCoord2f(V3toxy(poly.tp1));
		glVertex3f(V3toxyz(poly.p1));

		glTexCoord2f(V3toxy(poly.tp2));
		glVertex3f(V3toxyz(poly.p2));

		glTexCoord2f(V3toxy(poly.tp3));
		glVertex3f(V3toxyz(poly.p3));
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, -1);
}

void Model::buildPart(ModelPart *mpart) {
	int list_id = glGenLists(1);
	glNewList(list_id, GL_COMPILE);
	drawPart(mpart);
	glEndList();
	mpart->listId = list_id;
}
void Model::drawModel(V3 position) {
	glPushMatrix();

	glTranslatef(V3toxyz(position));
	glColor3f(V3toxyz(color));
	if (is_static)
		for (ModelPart mp : parts)
			glCallList(mp.listId);
	else
		for (ModelPart mpart : parts)
			drawPart(&mpart);

	glPopMatrix();
}

void Model::setStatic() {
	is_static = true;
	for (ModelPart mpart : parts)
		buildPart(&mpart);
}
void Model::setDynamic() {
	is_static = false;
	glDeleteLists(parts[0].listId, parts.size());
}
