#include "testApp.h"
#include <iostream>
#include <fstream>

/* Setup
 _________________________________________________________________ */

void testApp::setup()
{
	ofSetFrameRate(60);
	ofBackground(0, 0, 0);
	
	ofEnableSmoothing();
		
	boxes = new BoxesController();
	
	url = "http://versionize.com/ledsculpture/displaycells.php";
	ofAddListener(httpUtils.newResponseEvent, this, &testApp::newResponse);
	httpUtils.start();
	
	serial.enumerateDevices();
	
	if(serial.setup("/dev/tty.usbserial-A800euZ6", 9600))
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
	
	// check serial response
	if (serial.available() > 0)
	{
		while(serial.available() > 0)
		{
			cout << serial.readByte();
		}
		
		cout << "\n";
	}
}

/* Draw
 _________________________________________________________________ */

void testApp::draw()
{
	ofSetColor(255, 255, 255);
	ofDrawBitmapString(ofToString(ofGetFrameRate(), 0), 10, 10);
	
	if(!enableHTTP)
	{
		ofDrawBitmapString("Press space to start HTTP calls", 10, 30);
	}
	
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
	else if(key == 'Y')
	{
		// send test
		cout << "Sending test \n";
		
		bool byteWritten = serial.writeByte('y');
		if(!byteWritten)	cout << "y was not written to serial port \n";
		
		byteWritten = serial.writeByte('y');
		if(!byteWritten)	cout << "y was not written to serial port \n";
		
		byteWritten = serial.writeByte('y');
		if(!byteWritten)	cout << "y was not written to serial port \n";
		
		byteWritten = serial.writeByte('y');
		if(!byteWritten)	cout << "y was not written to serial port \n";
	}
	else if(key == 'p')
	{
		boxes->printVars();
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
	else if(key == ' ')
	{
		enableHTTP = !enableHTTP;
		
		if(enableHTTP)
		{
			lastReceived = ofGetElapsedTimeMillis();
			
			makeHTTPCall();
		}
	}
	else if(key == 't')
	{
		App::getInstance()->testMode = !App::getInstance()->testMode;
	}
}

void testApp::keyReleased(int key){}
void testApp::mouseMoved(int x, int y ){}
void testApp::mouseDragged(int x, int y, int button){}

void testApp::mousePressed(int x, int y, int button)
{
	for (int i = 0; i < 64; i++) 
	{
		Box * box = boxes->getBox(i);
		
		if(x > box->getLoc().x && x < box->getLoc().x + box->getSize() && y > box->getLoc().y && y < box->getLoc().y + box->getSize())
		{
			bool updated = boxes->updateBox(box->getId(), !box->getState(), box->getColor(), box->getUserId());
			
			if(updated)
			{
				sendBoxToArduino(i);
			}
		}
	}
}

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
	for(int i = 0; i < newData.size(); i++)
	{
		Json::Value cell = newData[i];		
		
		int id = cell["cellid"].asInt();
		int state = cell["state"].asInt();
		ofColor color = getColorFromString(cell["color"].asString());
		int userid = cell["userid"].asInt();
		
		bool updated = boxes->updateBox(cell["cellid"].asInt(), cell["state"].asInt(), getColorFromString(cell["color"].asString()), cell["userid"].asInt());
		
		if(updated)
		{
			sendBoxToArduino(id);
		}
	}
	
	newData = NULL;
}

/* Serial communication
 _________________________________________________________________ */

void testApp::sendBoxToArduino(int boxid)
{
	Box * box = boxes->getBox(boxid);
	int idToSend = getIDToSend(boxid);
	
	cout << "> Sending Box Serial Data \n";
	
	// start send to arduino
	bool byteWritten = serial.writeByte('*');
	if(!byteWritten)	cout << "* was not written to serial port \n";
	else				cout << "*";	
	
	// send id
	byteWritten = serial.writeByte(idToSend);
	if(!byteWritten)	cout << "Id was not written to serial port \n";
	else				cout << idToSend;
	
	// send state
	byteWritten = serial.writeByte(box->getState());
	if(!byteWritten)	cout << "State was not written to serial port \n";
	else				cout << box->getState();
	
	// send color (must convert to number)
	byteWritten = serial.writeByte(getIntFromColor(box->getColor()));
	if(!byteWritten)	cout << "Color was not written to serial port \n";
	else				cout << getIntFromColor(box->getColor()) << "\n";
}

int testApp::getIDToSend(int boxid)
{
	int order[64] = { 
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 
		32, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
		51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 33, 16, 17, 18, 19, 20, 21,
		22, 23, 24, 25, 26, 27, 28, 29, 30, 31
	};
	
	for (int i = 0; i < 64; i++) 
	{
		if(boxid == order[i])
		{
			return i;
		}
	}
	
	return DISABLED;
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

int testApp::getIntFromColor(ofColor c)
{
	// red
	if(c.r == 255 && c.g == 0 && c.b == 0)
	{
		return 0;
	}
	// green
	else if(c.r == 0 && c.g == 104 && c.b == 55)
	{
		return 1;
	}
	// blue
	else if(c.r == 46 && c.g == 49 && c.b == 146)
	{
		return 2;
	}
	// yellow
	else if(c.r == 252 && c.g == 238 && c.b == 33)
	{
		return 3;
	}
	// purple
	else if(c.r == 102 && c.g == 45 && c.b == 145)
	{
		return 4;
	}
	// cyan
	else if(c.r == 41 && c.g == 171 && c.b == 226)
	{
		return 5;
	}
	// pink
	else if(c.r == 255 && c.g == 0 && c.b == 255)
	{
		return 6;
	}
	// orange
	else if(c.r == 251 && c.g == 176 && c.b == 59)
	{
		return 7;
	}
	// white
	else 
	{
		return DISABLED;
	}
}

