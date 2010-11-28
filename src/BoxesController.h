#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "Box.h"

class BoxesController
{
	
public:
	
	BoxesController(float x, float y);
	
	void update();
	void draw();
	void setProperty(string p, float add);
	
	Box * getBox(int boxid);
	
	void updateBox(int boxid, bool state, ofColor color, int userid);
	
	
private:
	
	ofPoint _loc;
	float _boxSize;
	
	vector <Box *> _boxes;
	
	void createBoxes();
	void createShape(int startX, int startY, int slots[], int slotlength, int numCols);
	
	Box * findBoxWithoutPatner();
	
	int _curId;
};