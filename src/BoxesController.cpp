#include "BoxesController.h"

/*	Constructor
________________________________________________________________ */

BoxesController::BoxesController(float x, float y)
{
	_loc.set(x, y);
	_boxSize = 50;
	
	_curId = 1;
	
	createBoxes();
}

/*	Create Boxes
 ________________________________________________________________ */

void BoxesController::createBoxes()
{
	// create left shape (id 1-16)
	int left[20] = {	0, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 
		0, 1, 1, 1, 0
	};
	
	createShape(0, _boxSize, left, 20, 5);
	
	// create right shape (id 17-32)
	int right[20] = {	0, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 
		0, 1, 1, 1, 0
	};
	
	createShape(_boxSize * 14, _boxSize, left, 20, 5);
	
	// create small box left (id 33)
	int leftSingle[1] = {1};
	createShape(_boxSize * 5, _boxSize * 2.5, leftSingle, 1, 1);
	
	// create small box right (id 34)
	createShape(_boxSize * 13, _boxSize * 2.5, leftSingle, 1, 1);
	
	// create center shape (id 35-64)
	int center[42] = {	0, 0, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 0, 0
	};
	
	createShape(_boxSize * 6, 0, center, 42, 7);
}

/*	Update
 ________________________________________________________________ */

void BoxesController::update()
{	
	for(int i = 0; i < _boxes.size(); i++)
	{
		_boxes[i]->update();
	}
}

/*	Draw
 ________________________________________________________________ */

void BoxesController::draw()
{	
	ofTranslate(_loc.x, _loc.y);
	
	for(int i = 0; i < _boxes.size(); i++)
	{
		_boxes[i]->draw();
	}
}

/*	Create shape
 ________________________________________________________________ */

void BoxesController::createShape(int startX, int startY, int slots[], int slotlength, int numCols)
{
	float xPos = startX;
	float yPos = startY;
	
	for(int i = 0; i < slotlength; i++)
	{
		if(slots[i] == 1)
		{
			_boxes.push_back(new Box(_curId, xPos, yPos, _boxSize));
			
			_curId++;
		}
		
		xPos += _boxSize;
		
		if (i % numCols == numCols - 1) 
		{
			xPos = startX;
			yPos += _boxSize;
		}
	}
}

/*	Update box
 ________________________________________________________________ */


void BoxesController::updateBox(int boxid, bool state, ofColor color, int userid)
{
	Box * box = getBox(boxid);
	
	if (box != NULL) 
	{
		if (box->getState() != state && state) 
		{			
			Box * partner = findBoxWithoutPatner();
				
			box->setPartner(partner);
			//partner->setPartner(box);
		}
		
		box->updateState(state, color, userid);
	}
	else 
	{
		cout << "Box not found \n";
	}
}

/*	Get box without partner
 ________________________________________________________________ */

Box * BoxesController::findBoxWithoutPatner()
{
	// create vector with ids
	vector <int> ids;
	for (int i = 35; i <= 64; i++) 
	{
		ids.push_back(i);
	}
	
	// loop though all ids and grab random one each pass
	while (ids.size() > 0) 
	{
		bool partnered = false;
		int ranIndex = ofRandom(0, ids.size());
		
		for (int j = 1; j <= 32; j++) 
		{
			Box * box = getBox(j);
			
			if(box->getPartner() != NULL)
			{
				if(box->getPartner()->getId() == ids[ranIndex])
				{
					partnered = true;
				}
			}
		}
		
		if(!partnered)
		{
			return getBox(ids[ranIndex]);
		}
									
		ids.erase(ids.begin() + ranIndex);
	}
									
	return NULL;
	
	
	/*for(int i = 0; i < ids.size(); i++)
	{
		bool partnered = false;
		
		for (int j = 1; j <= 32; j++) 
		{
			Box * box = getBox(j);
			
			if(box->getPartner() != NULL)
			{
				if(box->getPartner()->getId() == i)
				{
					partnered = true;
				}
			}
		}
		
		if(!partnered)
		{
			return getBox(i);
		}
	}
	
	return NULL;*/
}

/*	Getter / Setter
 ________________________________________________________________ */

void BoxesController::setProperty(string p, float add)
{
	if(p == "x")
	{
		_loc.x += add;
	}
	else if(p == "y")
	{
		_loc.y += add;
	}
	else if(p == "size")
	{
		_boxSize += add;
		
		_boxes.clear();
		
		createBoxes();
	}
}

Box * BoxesController::getBox(int boxid)
{
	for(int i = 0; i < _boxes.size(); i++)
	{
		if(boxid == _boxes[i]->getId())
		{
			return _boxes[i];
		}
	}
	
	return NULL;
}