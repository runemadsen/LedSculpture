#include "testApp.h"
#include <iostream>
#include <fstream>

/* Setup
 _________________________________________________________________ */

void testApp::setup()
{
	ofSetFrameRate(60);
	ofBackground(40, 40, 40);
	
	ofEnableSmoothing();
		
	boxes = new BoxesController(20, 100);
	
	url = "http://versionize.com/ledsculpture/displaycells.php";
	ofAddListener(httpUtils.newResponseEvent, this, &testApp::newResponse);
	httpUtils.start();
	
	if(serial.setup())
	{
		printf("serial is setup!");
	}
	
	lastReceived = ofGetElapsedTimeMillis();
	enableHTTP = false;
}

/* Update
 _________________________________________________________________ */

void testApp::update()
{
	if(newData != NULL)
	{
		createBoxesFromData();
	}
	
	boxes->update();
	
	// check if call didn't get through
	if(enableHTTP && ofGetElapsedTimeMillis() - lastReceived > HTTP_TIMEOUT)
	{
		lastReceived = ofGetElapsedTimeMillis();
		
		makeHTTPCall();
	}
}

/* Draw
 _________________________________________________________________ */

void testApp::draw()
{
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 0), 10, 10);
	ofSetColor(0, 0, 0);
	boxes->draw();
}

/* Key Events
 _________________________________________________________________ */

void testApp::keyPressed(int key)
{
	if(key == 'x')
	{
		boxes->setProperty("x", -1);
	}
	else if(key == 'X')
	{
		boxes->setProperty("x", 1);
	}
	else if(key == 'y')
	{
		boxes->setProperty("y", -1);
	}
	else if(key == 'Y')
	{
		boxes->setProperty("y", -1);
	}
	else if(key == 's')
	{
		boxes->setProperty("size", -1);
	}
	else if(key == 'S')
	{
		boxes->setProperty("size", 1);	
	}
	else if(key >= '0' && key <= '9')
	{
		Box * box = boxes->getBox(key - '0');
		
		boxes->updateBox(box->getId(), !box->getState(), box->getColor(), box->getUserId());
	}
	else if(key == ' ')
	{
		enableHTTP = !enableHTTP;
		
		if(enableHTTP)
		{
			lastReceived = ofGetElapsedTimeMillis();
			
			makeHTTPCall();
		}
	}
}

void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y ){}
void testApp::mouseDragged(int x, int y, int button){}
void testApp::mousePressed(int x, int y, int button){}
void testApp::mouseReleased(int x, int y, int button){}
void testApp::windowResized(int w, int h){}

void testApp::makeHTTPCall()
{
	cout << "HTTP Call requested" << endl;
	
	httpUtils.addUrl(url);
}


/* HTTP Events
 _________________________________________________________________ */

void testApp::newResponse(ofxHttpResponse & response)
{
	lastReceived = ofGetElapsedTimeMillis();
	
	parseJSON(response.responseBody);
	
	/*if(enableHTTP)
	{
		makeHTTPCall();
	}*/
	
	cout << "HTTP Call received" << endl;
}

void testApp::newError(string error)
{
    printf("new error = %s\n", error.c_str());
}

/* JSON parse
 _________________________________________________________________ */

void testApp::parseJSON(string s) 
{
	cout << "HTTP Call parsing" << endl;
	
	bool parsingSuccessful = reader.parse(s, root);
	
	if (!parsingSuccessful) 
	{
		cout  << "Failed to parse JSON\n" << reader.getFormatedErrorMessages();
	}
	
	newData = root["cells"];
}

/* Create boxes from data
 _________________________________________________________________ */

void testApp::createBoxesFromData()
{
	// start send to arduino
	//bool byteWritten = mySerial.writeByte("*");
	//if(!byteWritten)	cout << "* was not written to serial port \n";
	
	for(int i = 0; i < newData.size(); i++)
	{
		Json::Value cell = newData[i];		
		
		int id = cell["cellid"].asInt();
		int state = cell["state"].asInt();
		ofColor color = getColorFromString(cell["color"].asString());
		int userid = cell["userid"].asInt();
		
		/*cout << ":::::::::::::: CELL " << endl;
		cout << "Cell id: " << id << endl;
		cout << "Cell state: " << state << endl;
		cout << "Cell color red: " << color.r << endl;
		cout << "Cell color green: " << color.g << endl;
		cout << "Cell color blue: " << color.b << endl;
		cout << "Cell userid: " << userid << endl;*/
		
		// send id
		//byteWritten = mySerial.writeByte(id);
		//if(!byteWritten)	cout << "Id was not written to serial port \n";
		
		// send state
		//byteWritten = mySerial.writeByte(state);
		//if(!byteWritten)	cout << "State was not written to serial port \n";
		
		// send color (must convert to number)
		//byteWritten = mySerial.writeByte(color);
		//if(!byteWritten)	cout << "Color was not written to serial port \n";
		
		boxes->updateBox(cell["cellid"].asInt(), cell["state"].asInt(), getColorFromString(cell["color"].asString()), cell["userid"].asInt());
	}
	
	newData = NULL;
}

/* get color object from string name
 _________________________________________________________________ */

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

