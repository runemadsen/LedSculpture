#include "testApp.h"
#include <iostream>
#include <fstream>

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(12);
	ofBackground(255, 255, 255);
	myFont.loadFont("Verdana.ttf", 32);
	
	url = "http://www.runemadsen.com/zeven.php";
	ofAddListener(httpUtils.newResponseEvent, this, &testApp::newResponse);
	httpUtils.start();
}

void testApp::newResponse(ofxHttpResponse & response)
{
	parseJSON(response.responseBody);
}

void testApp::newError(string error)
{
    printf("new error = %s\n", error.c_str());
}

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
		cout << "CellNum: " << cells[ofToString(i, 0)];
		
		// i is cell number
		// cells[ofToString(i, 0)] is on or off
	}
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(0, 0, 0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	ofxHttpForm form;
	form.method = OFX_HTTP_GET;
	httpUtils.addUrl(url);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

