#include "Box.h"

Box::Box(float x, float y, int boxSize)
{
	_loc.x = x;
	_loc.y = y;
	_boxSize = boxSize;
}

void Box::draw()
{		
	ofFill();
	ofRect(_loc.x, _loc.y, _boxSize, _boxSize);
}