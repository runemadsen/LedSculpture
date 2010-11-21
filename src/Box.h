#pragma once

#include "ofMain.h"
#include "Constants.h"

class Box
{

public:
	
	Box(int id, float x, float y, int boxSize);
	void draw();
	
	int getId() { return _id; };
	
	void update(bool state, ofColor color, int userid);
	
private:
	
	int _id;
	bool _state;
	ofColor _color;
	int _userid;
	
	ofPoint _loc;
	int _boxSize;
	
};