#include "Box.h"

Box::Box(float x, float y)
{
	_loc.x = x;
	_loc.y = y;
}

void Box::draw()
{	
	ofFill();
	ofRect(_loc.x, _loc.y, BOX_SIZE, BOX_SIZE);
}