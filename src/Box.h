#ifndef _BOX_
#define _BOX_

#include "ofMain.h"
class Constants;
class Particles;

class Box
{

public:
	
	Box(int id, float x, float y, int boxSize);
	
	void draw();
	
	int getId() { return _id; };
	
	void update();
	void updateState(bool state, ofColor color, int userid);
	
	bool getState() { return _state; }
	int getUserId() { return _userid; }
	ofColor getColor() { return _color; }
	Box * getPartner() { return _partner; }
	bool visible() { return _state; }
	ofPoint getLoc() { return _loc; }

	void setPartner(Box * partner);
	
private:
	
	int _id;
	bool _state;
	ofColor _color;
	int _userid;
	
	ofPoint _loc;
	int _boxSize;
	
	Box * _partner;
	
	Particles * _particles;
};

#endif