#ifndef _BOX_
#define _BOX_

#include "ofMain.h"
#include "Constants.h"
#include "App.h"
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
	Box * getNeighbour() { return _neighbour; }
	bool visible() { return _state; }
	ofPoint getLoc() { return _loc; }
	int getSize() { return _boxSize; }
	
	void makeConnection() { _connectionMade = true; }
	void stopConnection() { _connectionMade = false; }
	
	ofImage * getTexture() { return _texture; }

	void setPartner(Box * partner);
	void setNeighbour(Box * neighbour);
	
private:
	
	bool _connectionMade;
	
	int _id;
	bool _state;
	ofColor _color;
	int _userid;
	
	ofPoint _loc;
	int _boxSize;
	
	Box * _partner;
	Box * _neighbour;
	
	Particles * _particles;
	
	ofImage * _texture;
};

#endif