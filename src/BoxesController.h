#pragma once

#include "ofMain.h"
#include "Constants.h"
#include "Box.h"

class BoxesController
{
	
public:
	
	BoxesController();
	
	void update();
	void draw();
	void setProperty(string p, float add);
	
	int getX() { return _loc.x; }
	int getY() { return _loc.y; }
	
	Box * getBox(int boxid);
	
	int getBoxesLength() { return _boxes.size(); }
	
	bool updateBox(int boxid, bool state, ofColor color, int userid);
	
	void printVars();
	
	
private:
	
	ofPoint _loc;
	float _boxSize;
	
	vector <Box *> _boxes;
	
	void createBoxes();
	void createShape(int startX, int startY, int slots[], int slotlength, int numCols);
	
	Box * findBoxWithoutPatner();
	Box * findPartneredNeighbour(Box * box);
	
	int _curId;
};