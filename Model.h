#pragma once

struct ModelPart {
	char name[40];
	unsigned int listId;
	vector<PolygonS> polygons;
};

class Model {
public:
	Model(char *modelfile);
	void loadModel(char *modelfile);
	void drawPart(ModelPart *mpart);
	void buildPart(ModelPart *mpart);
	void drawModel(V3 position);

	void setStatic();
	void setDynamic();

	bool is_static;
	vector<ModelPart> parts;
	V3 color;
};

