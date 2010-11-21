#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "Box.h"

class BoxesController
{
	
public:
	
	BoxesController(float x, float y);
	void draw();
	void setProperty(string p, float add);
	
	Box * getBox(int boxid);
	
private:
	
	ofPoint _loc;
	float _boxSize;
	
	vector <Box *> _boxes;
	
	void createBoxes();
	void createShape(int startX, int startY, int slots[], int slotlength, int numCols);
	
	int _curId;
};