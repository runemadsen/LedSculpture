#include "Box.h"

Box::Box(int id, float x, float y, int boxSize)
{
	_id = id;
	_loc.x = x;
	_loc.y = y;
	_boxSize = boxSize;
	
	_state = false;
	_userid = DISABLED;
}

void Box::update(bool state, ofColor color, int userid)
{
	_state = state;
	_color = color;
	_userid = userid;
}

void Box::draw()
{		
	ofFill();
	
	if(_state)
		ofSetColor(_color.r, _color.g, _color.b);
	else 
		ofSetColor(0, 0, 0);

	ofRect(_loc.x, _loc.y, _boxSize, _boxSize);
}