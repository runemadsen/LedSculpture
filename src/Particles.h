#ifndef _PARTICLES_
#define _PARTICLES_

#include "ofMain.h"
#include "ofxVec2f.h"
#include "Tools.h"
#include "Constants.h"

class Box;

class Particles 
{
	
public:
	
	Particles(Box * model);

	void setTexture(ofImage newTexture, int cellsInRow, int cellsInCol);
	
	void init();
	void update();
	void render();
	
	int getTotal();
	
	ofImage		texture;
	ofImage		flare;
	
private:
	
	void spawn(int i);
	
	void setParticleColor(int i, float r, float g, float b, float a=0.0);
	void setParticleAlpha(int i, float a);
	void setParticlePos(int i, float px, float py, float pz=0.0);
	void setParticleSize(int i, float particleDim);
	void addPosition(int i, float x, float y, float z);
	void setParticleTexCoords(int i, float columnID, float rowID);
	
	int			cellRows;
	int			cellColls;
	float		texW, texH;
	
	GLuint		particleVBO[3];
	
	
	float		dim[MAX_PARTICLES];			// particle size (w/h)
	PVector		pos[MAX_PARTICLES*4];		// vertex (quad) of particle
	PTexture	texcords[MAX_PARTICLES*4];	// texture coords
	PColor		color[MAX_PARTICLES*4];		// particle color rgba
	PVector		dest[MAX_PARTICLES];
	
	float		vel[MAX_PARTICLES][3];		
	float		acc[MAX_PARTICLES][3];
	float		damping[MAX_PARTICLES];
	
	float		life[MAX_PARTICLES][2];		//	[life] [life rate] 
	
	vector <PSetting> settings;
	
	int numParticles;
	
	Box * _model;
};

#endif