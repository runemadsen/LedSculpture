#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxHttpUtils.h"
#include "json/json.h"
#include "Constants.h"
#include "BoxesController.h"

class testApp : public ofBaseApp
{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void newResponse(ofxHttpResponse & response);
		void newError(string error);
		void parseJSON(string s);
		void createBoxesFromData();
	
		ofColor getColorFromString(string color);
	
		BoxesController * boxes;
	
		Json::Value root;   // will contains the root value after parsing.
		Json::Reader reader;
		Json::Value newData;
	
		ofxHttpUtils httpUtils;
		string url;
	
		ofSerial serial;
};



#endif
