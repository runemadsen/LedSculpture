#include "Box.h"
#include "Particles.h"

/* Constructor
 _________________________________________________________________ */

Box::Box(int id, float x, float y, int boxSize)
{
	_id = id;
	_loc.x = x;
	_loc.y = y;
	_boxSize = boxSize;
	
	_state = false;
	_userid = DISABLED;
	
	_color.r = 255;
	_color.g = 0;
	_color.b = 0;
	
	_partner = NULL;
	_particles = NULL;
}

/* Update
 _________________________________________________________________ */

void Box::updateState(bool state, ofColor color, int userid)
{
	// if first on, create particles
	if(!_state && state && _particles == NULL)
	{
		cout << "Created particles \n";
		_particles = new Particles(this);
		_particles->init();
	}
	
	_state = state;
	_color = color;
	_userid = userid;
	
	if(_partner != NULL)
	{
		_partner->updateState(_state, _color, _userid);
	}
	
	if(!_state && _partner != NULL)
	{
		_partner = NULL;
	}
}


/* Update
 _________________________________________________________________ */

void Box::update()
{
	if(_particles != NULL)
	{
		cout << "Updating particles \n";
		_particles->update();
	}
}

/* Draw
 _________________________________________________________________ */

void Box::draw()
{		
	ofFill();
	
	if(_particles != NULL)
	{
		cout << "Rendering particles \n";
		_particles->render();
	}
	
	if(_state)
		ofSetColor(_color.r, _color.g, _color.b);
	else 
		ofSetColor(0, 0, 0);

	ofRect(_loc.x, _loc.y, _boxSize, _boxSize);
}

/* Getter / Setter
 _________________________________________________________________ */

void Box::setPartner(Box * partner)
{	
	_partner = partner;
}