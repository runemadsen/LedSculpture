#include "BoxesController.h"

/*	Constructor
________________________________________________________________ */

BoxesController::BoxesController(float x, float y)
{
	_loc.set(x, y);
	_boxSize = 40;
	
	createBoxes();
}

/*	Create Boxes
 ________________________________________________________________ */

void BoxesController::createBoxes()
{
	// create left shape
	int left[20] = {	0, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 
		0, 1, 1, 1, 0
	};
	
	createShape(0, _boxSize, left, 20, 5);
	
	// create small box left
	int leftSingle[1] = {1};
	createShape(_boxSize * 5, _boxSize * 2.5, leftSingle, 1, 1);
	
	// create center shape
	int center[42] = {	0, 0, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 0, 0
	};
	
	createShape(_boxSize * 6, 0, center, 42, 7);
	
	// create small box right
	createShape(_boxSize * 13, _boxSize * 2.5, leftSingle, 1, 1);
	
	// create right shape
	int right[20] = {	0, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 
		0, 1, 1, 1, 0
	};
	
	createShape(_boxSize * 14, _boxSize, left, 20, 5);
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
			_boxes.push_back(new Box(xPos, yPos, _boxSize));
		}
		
		xPos += _boxSize;
		
		if (i % numCols == numCols - 1) 
		{
			xPos = startX;
			yPos += _boxSize;
		}
	}
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