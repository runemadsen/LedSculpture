#pragma once

#include "ofMain.h"
#include "Constants.h"

class Box
{

public:
	
	Box(float x, float y, int boxSize);
	void draw();
	int _boxSize;
	
private:
	
	ofPoint _loc;
	
};