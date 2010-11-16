#include "BoxesController.h"

/*	Constructor
________________________________________________________________ */

BoxesController::BoxesController(float x, float y)
{
	_loc.x = x;
	_loc.y = y;
	
	// create left shape
	int left[20] = {	0, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 
		0, 1, 1, 1, 0
	};
	
	createShape(0, BOX_SIZE, left, 20, 5);
	
	// create small box left
	int leftSingle[1] = {1};
	createShape(BOX_SIZE * 5, BOX_SIZE * 2.5, leftSingle, 1, 1);
	
	// create center shape
	int center[42] = {	0, 0, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 0, 0
	};
	
	createShape(BOX_SIZE * 6, 0, center, 42, 7);
	
	// create small box right
	createShape(BOX_SIZE * 13, BOX_SIZE * 2.5, leftSingle, 1, 1);
	
	// create right shape
	int right[20] = {	0, 1, 1, 1, 0, 
		1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 
		0, 1, 1, 1, 0
	};
	
	createShape(BOX_SIZE * 14, BOX_SIZE, left, 20, 5);
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
	int xPos = startX;
	int yPos = startY;
	
	for(int i = 0; i < slotlength; i++)
	{
		if(slots[i] == 1)
		{
			_boxes.push_back(new Box(xPos, yPos));
		}
		
		xPos += BOX_SIZE;
		
		if (i % numCols == numCols - 1) 
		{
			xPos = startX;
			yPos += BOX_SIZE;
		}
	}
}