#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "Box.h"

class BoxesController
{
	
public:
	
	BoxesController(float x, float y);
	void draw();
	
private:
	
	ofPoint _loc;
	
	vector <Box *> _boxes;
	
	void createShape(int startX, int startY, int slots[], int slotlength, int numCols);
};