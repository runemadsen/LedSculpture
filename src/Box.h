#pragma once

#include "ofMain.h"
#include "Constants.h"

class Box
{

public:
	
	Box(float x, float y);
	void draw();
	
private:
	
	ofPoint _loc;
};