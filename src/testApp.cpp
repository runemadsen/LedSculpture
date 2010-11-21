#include "testApp.h"
#include <iostream>
#include <fstream>

/* Setup
 _________________________________________________________________ */

void testApp::setup()
{
	ofSetFrameRate(12);
	ofBackground(255, 255, 255);
	
	ofEnableSmoothing();
		
	boxes = new BoxesController(20, 100);
	
	url = "http://versionize.com/ledsculpture/displaycells.php";
	ofAddListener(httpUtils.newResponseEvent, this, &testApp::newResponse);
	httpUtils.start();
}

/* Update
 _________________________________________________________________ */

void testApp::update()
{
	
}

/* Draw
 _________________________________________________________________ */

void testApp::draw()
{
	ofSetColor(0, 0, 0);
	boxes->draw();
}

/* JSON parse
 _________________________________________________________________ */

void testApp::parseJSON(string s) 
{
	cout << "Parsing JSON" << endl;
	
	bool parsingSuccessful = reader.parse(s, root);
	
	if (!parsingSuccessful) 
	{
		cout  << "Failed to parse JSON\n" << reader.getFormatedErrorMessages();
	}
	
	// See this page for all of the ways to access data in a Json::Value
	// http://jsoncpp.sourceforge.net/class_json_1_1_value.html
	Json::Value cells = root["cells"];
	
	for(int i = 0; i < cells.size(); i++)
	{
		Json::Value cell = cells[ofToString(i, 0)];
		int boxid = cell["cellid"].asInt();
		
		Box * box = boxes->getBox(boxid);
		
		if(box != NULL)
		{
			box->update(cell["state"].asInt(), getColorFromString(cell["color"].asString()), cell["userid"].asInt());
		}
		else 
		{
			cout << "Box with id: " << boxid << " was not found" << endl;
		}

	}
}

ofColor testApp::getColorFromString(string color)
{
	ofColor c;
	
	if(color == RED)
	{
		c.r = 255; c.g = 0; c.b = 0;
	}
	else if(color == GREEN)
	{
		c.r = 0; c.g = 104; c.b = 55;
	}
	else if(color == BLUE)
	{
		c.r = 46; c.g = 49; c.b = 146;
	}
	else if(color == YELLOW)
	{
		c.r = 252; c.g = 238; c.b = 33;
	}
	else if(color == PURPLE)
	{
		c.r = 102; c.g = 45; c.b = 145;
	}
	else if(color == CYAN)
	{
		c.r = 41; c.g = 171; c.b = 226;
	}
	else if(color == PINK)
	{
		c.r = 255; c.g = 0; c.b = 255;
	}
	else if(color == ORANGE)
	{
		c.r = 251; c.g = 176; c.b = 59;
	}
	else 
	{
		c.r = 255; c.g = 255; c.b = 255;
	}
	
	return c;

}

/* HTTP Events
 _________________________________________________________________ */

void testApp::newResponse(ofxHttpResponse & response)
{
	parseJSON(response.responseBody);
}

void testApp::newError(string error)
{
    printf("new error = %s\n", error.c_str());
}

/* Key Events
 _________________________________________________________________ */

void testApp::keyPressed(int key)
{
	if(key == 'x')
		boxes->setProperty("x", -1);
	else if(key == 'X')
		boxes->setProperty("x", 1);
	else if(key == 'y')
		boxes->setProperty("y", -1);
	else if(key == 'Y')
		boxes->setProperty("y", -1);
	else if(key == 's')
		boxes->setProperty("size", -1);
	else if(key == 'S')
		boxes->setProperty("size", 1);	
	else if(key == 'l')
	{
		httpUtils.addUrl(url);
	}
}

void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y ){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}

