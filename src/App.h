#pragma once

#include "ofMain.h"

class App  
{
	
public:
	
	static App * getInstance();
	
	bool testMode;
	
private:
	
	// singleton stuff
	App();  
	App(App const&){};   
	App& operator=(App const&){};
	static App* m_pInstance;
};