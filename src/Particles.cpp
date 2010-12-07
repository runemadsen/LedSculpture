#include "Particles.h"
#include "Box.h"

/* Constructor
 _______________________________________________________________________ */

Particles::Particles(Box * model) 
{	
	_model = model;
	
	PSetting setting1;
	setting1.percent = 1;
	setting1.sizeMin = 10;
	setting1.sizeMax = 30;
	setting1.dirMin = 0.5;
	setting1.dirMax = 1;
	//setting1.dirMin = 0.2;
	//setting1.dirMax = 0.9;
	setting1.lifeMin = 0.8;
	setting1.lifeMax = 2;
	settings.push_back(setting1);
	
	/*PSetting setting2;
	setting2.percent = 0.6;
	setting1.sizeMin = 5;
	setting1.sizeMax = 10;
	setting2.dirMin = 2;
	setting2.dirMax = 3;
	setting2.lifeMin = 0.15;
	setting2.lifeMax = 0.25;
	settings.push_back(setting2);*/
}

/* Init
 _______________________________________________________________________ */

void Particles::init()
{			
	cout << "Init" << endl;
	
	numParticles = 0;
	
	// load texture
	ofImage img;
	img.loadImage("particleGrid0.png");
	setTexture(img, 2, 2);
	
	texW = texture.getWidth();
	texH = texture.getHeight();
	
	// Setup the VBO
	
	glGenBuffersARB(3, &particleVBO[0]);	// Create 3 vertex buffer object. Give address to GLUint array where ID's are stored
	
	// color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);	// Init VBO 0 as a vertex array VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*4*sizeof(float), color, GL_STREAM_DRAW_ARB);	// Copy data into VBO 0: 
	// Second parameter is the number of bytes to allocate
	// Third parameter is pointer to the actual data
	// Fourth parameter is for performance and says how the VBO is going to be used: Stream means that it will be changed every frame. Draw means data is sent to GPU
	
	// vertices
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*3*sizeof(float), pos, GL_STREAM_DRAW_ARB);
	
	// texture coords
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, (MAX_PARTICLES*4)*2*sizeof(float), texcords, GL_STREAM_DRAW_ARB);
}

/* Update
 _______________________________________________________________________ */

void Particles::update() 
{	
	if(_model->visible())	
	{
		if(numParticles < MAX_PARTICLES)
		{
			for(int i = 0; i < 1; i++)
			{
				// take this out and fix to better solution
				if(ofRandomuf() > 0.5)
				{
					spawn(numParticles);
				
					numParticles++;
				
					if(numParticles >= MAX_PARTICLES)
						break;
				}
			}
		}
	}
	
	for(int i=0; i<numParticles; i++) 
	{
		// check if position is outside destination
		if(fabs(pos[(i*4)+0].x - dest[i].x) > 10 || fabs(pos[(i*4)+0].y - dest[i].y) > 10)
		{
			vel[i][0] += acc[i][0];
			vel[i][1] += acc[i][1];
			
			addPosition(i, vel[i][0], vel[i][1], vel[i][2]);
		}
		
		vel[i][0] *= damping[i];
		vel[i][1] *= damping[i];
		vel[i][2] *= damping[i];
		
		// fade by the life rate
		life[i][0] -= life[i][1];
		
		//setParticleColor(i, 1.0, 1.0, 1.0, life[i][0]);
		//setParticleAlpha(i, life[i][0]);
		
		if(life[i][0] <= 0.0) 
		{
			// make sure the particle disappears when life is over
			setParticleColor(i, 1.0, 1.0, 1.0, 0);
			
			if(_model->visible())	
			{
				spawn(i);
			}
		}
	}
}

/* Create / Reset Particle
 _______________________________________________________________________ */

void Particles::spawn(int i) 
{
	//cout << "Spawn" << endl;
	
	float percent = 0.0;
	float random = ofRandomuf();
	
	for (int j = 0; j < settings.size(); j++) 
	{
		percent += settings[j].percent;
		
		if (random < percent) 
		{
			// size
			float size = ofRandom(settings[j].sizeMin, settings[j].sizeMax);
			setParticleSize(i, size);
			
			// color
			ofColor c = _model->getColor();
			setParticleColor(i, (float) c.r / 255.0, (float) c.g / 255.0, (float) c.b / 255.0, 1);
			
			ofPoint thePoint = _model->getLoc();
			
			// adjust for image width
			float margin = size / 2; // divide by 2 if on the edges
			thePoint.x -= margin;
			thePoint.y -= margin;
			
			int side = ofRandom(0, 4);
			float ran = ofRandomuf();
			
			
			if(side < 1)
			{
				thePoint.y += (float) _model->getSize() * ran;
			}
			else if(side < 2)
			{
				thePoint.x += (float) _model->getSize() * ran;
			}
			else if(side < 3)
			{
				thePoint.x += _model->getSize();
				thePoint.y += (float) _model->getSize() * ran;
			}
			else if(side < 4)
			{
				thePoint.x += (float) _model->getSize() * ran;
				thePoint.y += _model->getSize();
			}
			
			// choose where the particle goes
			ofxVec2f direction;
			
			if(_model->getPartner() != NULL)
			{
				ofxVec2f partnerLoc;
				partnerLoc.x = _model->getPartner()->getLoc().x;
				partnerLoc.y = _model->getPartner()->getLoc().y;
				
				// make them end in the middle of the box
				partnerLoc.x += (_model->getPartner()->getSize() / 2) - (size / 2);
				partnerLoc.y += (_model->getPartner()->getSize() / 2) - (size / 2);
				
				// save destination for later
				dest[i].x = partnerLoc.x;
				dest[i].y = partnerLoc.y;
				
				direction.set(partnerLoc - thePoint);
			}
			
			direction.normalize();
			
			setParticlePos(i, thePoint.x, thePoint.y);
			setParticleTexCoords(i, (int)ofRandom(0, 2), (int)ofRandom(0, 2));
			
			direction *= ofRandom(settings[j].dirMin, settings[j].dirMax);
			
			life[i][0] = ofRandom(settings[j].lifeMin, settings[j].lifeMax);
			life[i][1] = ofRandom(settings[j].lifeSubMin, settings[j].lifeSubMax);
			
			acc[i][0] = direction.x;
			acc[i][1] = direction.y;
			acc[i][2] = 0;
			
			damping[i] = settings[j].damping;
			
			break;
		}
	}	
}


/* Render
 _______________________________________________________________________ */

void Particles::render() 
{		
	//cout << "Render" << endl;
	
	glEnable(GL_TEXTURE_2D);	// Tells OpenGL that we want ot draw a 2d teture
	
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); // Enables the Texture coordinate array for drawing with glDrawArrays or glDrawElements calls 
	glEnableClientState(GL_VERTEX_ARRAY); // Enables the Vertex array for drawing with glDrawArrays or glDrawElements calls 
	glEnableClientState(GL_COLOR_ARRAY); // Enables the Color array for drawing with glDrawArrays or glDrawElements calls 
	
	// bind tex coors
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[2]); // init VBO (see above)
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*2*sizeof(float), texcords); // put data in VBO
	glTexCoordPointer(2, GL_FLOAT, 0, 0); // Tell OpenGL that we have 2 coordinates and the coordinates in the array are floats
	
	
	// bind color
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[0]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*4*sizeof(float), color);
	glColorPointer(4, GL_FLOAT, 0, 0);
	
	
	// bind vertices [these are quads]
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, particleVBO[1]);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, (MAX_PARTICLES*4)*3*sizeof(float), pos);
	glVertexPointer(3, GL_FLOAT, 0, 0); // Tell OpenGL that we have 3 coordinates (x, y, z) and the coordinates are stored as floats in the array
	
	// draw the vbo
	glDisable(GL_DEPTH_TEST); // disable depth test (whatever it is)
	ofEnableArbTex();	
	
	ofEnableAlphaBlending();
	
	texture.getTextureReference().bind();
	
	glDrawArrays(GL_QUADS, 0, MAX_PARTICLES*4);  // render all array data we putted in VBO's as quads, at index 0, and to the end of the arrays
	
	texture.getTextureReference().unbind();
	
	ofDisableAlphaBlending();
	
	ofDisableArbTex();
	
	glEnable(GL_DEPTH_TEST);
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glDisable(GL_TEXTURE_2D);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}



/* Set Particle Texture Coordinates
 _______________________________________________________________________ */

void Particles::setParticleTexCoords(int i, float columnID, float rowID) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	if(columnID > cellColls) columnID = cellColls;
	if(rowID    > cellRows)	 rowID	  = cellRows;
	
	if(columnID < 0) columnID = 0;
	if(rowID < 0)	 rowID	  = 0;
	
	// get the cell image width
	float cellWidth  = texW / cellRows;
	float cellHeight = texH / cellColls;
	
	float row = rowID;
	float col = columnID;
	
	// P1
	texcords[(i*4)+0].u = (cellWidth * row)		/ texW;
	texcords[(i*4)+0].v = (cellHeight * col)	/ texH;
	
	// P2
	texcords[(i*4)+1].u = ((cellWidth * row)	+ cellWidth)	/ texW;
	texcords[(i*4)+1].v = (cellHeight * col)	/ texH;
	
	// P2
	texcords[(i*4)+2].u = ((cellWidth * row) + cellWidth)		/ texW;
	texcords[(i*4)+2].v = ((cellHeight * col) + cellHeight)	/ texH;	
	
	// P2
	texcords[(i*4)+3].u = (cellWidth * row)		/ texW;
	texcords[(i*4)+3].v = ((cellHeight * col)+cellHeight)	/ texH;	
}

/* Set Particle Color
 _______________________________________________________________________ */

void Particles::setParticleColor(int i, float r, float g, float b, float a) 
{
	if(i < 0) i = 0;
	if(i > MAX_PARTICLES) i = MAX_PARTICLES;
	
	// Color 1
	color[(i*4)+0].r = r;
	color[(i*4)+0].g = g;
	color[(i*4)+0].b = b;
	color[(i*4)+0].a = a;
	
	// Color 2
	color[(i*4)+1].r = r;
	color[(i*4)+1].g = g;
	color[(i*4)+1].b = b;
	color[(i*4)+1].a = a;
	
	// Color 3
	color[(i*4)+2].r = r;
	color[(i*4)+2].g = g;
	color[(i*4)+2].b = b;
	color[(i*4)+2].a = a;
	
	// Color 4
	color[(i*4)+3].r = r;
	color[(i*4)+3].g = g;
	color[(i*4)+3].b = b;
	color[(i*4)+3].a = a;
}

/* Set Particle Color
 _______________________________________________________________________ */

void Particles::setParticleAlpha(int i, float a) 
{
	if(i < 0) i = 0;
	if(i > MAX_PARTICLES) i = MAX_PARTICLES;
	
	// Color 1
	color[(i*4)+0].a = a;
	
	// Color 2
	color[(i*4)+1].a = a;
	
	// Color 3
	color[(i*4)+2].a = a;
	
	// Color 4
	color[(i*4)+3].a = a;
}


/* Set Particle Position
 _______________________________________________________________________ */

void Particles::setParticlePos(int i, float px, float py, float pz) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	// P1
	pos[(i*4)+0].x = px;
	pos[(i*4)+0].y = py;
	pos[(i*4)+0].z = pz;
	
	// P2
	pos[(i*4)+1].x = px + dim[i];
	pos[(i*4)+1].y = py;
	pos[(i*4)+1].z = pz;
	
	// P2
	pos[(i*4)+2].x = px + dim[i];
	pos[(i*4)+2].y = py + dim[i];
	pos[(i*4)+2].z = pz;
	
	// P2
	pos[(i*4)+3].x = px;
	pos[(i*4)+3].y = py + dim[i];
	pos[(i*4)+3].z = pz;
}

/* Set Particle Size
 _______________________________________________________________________ */

void Particles::setParticleSize(int i, float particleDim) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	dim[i] = particleDim;
}

/* Add Position
 _______________________________________________________________________ */

void Particles::addPosition(int i, float x, float y, float z) 
{	
	if(i < 0)				i = 0;
	if(i > MAX_PARTICLES)	i = MAX_PARTICLES;
	
	// P1
	pos[(i*4)+0].x += x;
	pos[(i*4)+0].y += y;
	pos[(i*4)+0].z += z;
	
	// P2
	pos[(i*4)+1].x += x;
	pos[(i*4)+1].y += y;
	pos[(i*4)+1].z += z;
	
	// P2
	pos[(i*4)+2].x += x;
	pos[(i*4)+2].y += y;
	pos[(i*4)+2].z += z;
	
	// P2
	pos[(i*4)+3].x += x;
	pos[(i*4)+3].y += y;
	pos[(i*4)+3].z += z;
}

/* Getter / Setter
 _______________________________________________________________________ */

int Particles::getTotal()
{
	return numParticles;
}

/* Set texture
 _______________________________________________________________________ */

void Particles::setTexture(ofImage newTexture, int cellsInRow, int cellsInCol) 
{
	ofDisableArbTex();
	texture = newTexture;
	ofEnableArbTex();
	
	texW = texture.getWidth();
	texH = texture.getHeight();
	
	cellRows  = cellsInRow;
	cellColls = cellsInCol; 
}