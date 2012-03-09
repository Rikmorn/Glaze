/*
 * GlazeMesh.h
 *
 *  Created on: 2/Mai/2009
 *      Author: Daemoniorum
 */

/*
 * animations go through the same function over and over
 * just some can have values changing in the middle of the animation
 * while other don't.
 */

#ifndef GLAZEMESH_H_
#define GLAZEMESH_H_

#include "GlazeAnimation.h"
#include "GlazeTextures.h"
#include "GlazeCommon.h"

#include <GLES/egl.h>

class CGlazeMesh: public MAnimationObserver {
public:
	static CGlazeMesh* NewL();
	virtual ~CGlazeMesh();

protected:
	CGlazeMesh();
	void ConstructL(void);

public:
	void renderMesh(CGlazeTextures* aTex);
	void updateVelocity(TReal aVelx, TReal aVely, TReal aVelz);
	void updateInitialPosition(TReal iPx, TReal iPy, TReal iPz);

private:
	virtual void updateAnimation(TInt aAnim, TInt64 aDTime);
	virtual TInt getNumAminations();

	void animation1(TInt64 aDTime);

private:
	TUint iAnimNum;
	TReal iVelx, iVely, iVelz;
	TReal iPosx, iPosy, iPosz;
	TReal iPosix, iPosiy, iPosiz;
};

#define HEIGHT -20
#define CANTINAHEIGHT 50
#define TECHEIGHT 500
#define TECHADH 250
#define TECHADH1 50

static GLfloat streetVertTemp[] = { 
		0, HEIGHT, 0, 
		150, HEIGHT, -50,
		1400, HEIGHT, 600, 
		1300, HEIGHT, 800
		};

static GLbyte streetTextTemp[] = { -128, -128, 127, -128, 127, 127, -128, 127 };

static GLubyte streetFaceTemp[] = { 1, 0, 3, 1, 3, 2 };

// ground
static GLfloat groundVert[] = { 
		0, HEIGHT, 0, 
		1080, HEIGHT, 800,
		2530, HEIGHT, -2150, 
		1730, HEIGHT, -2750
		};

static GLbyte groundTex[] = { // -18
		127, 127,
		127, -128,
		-128, -128,
		-128, 127 
		};

static GLbyte tempTex[] = {
		-128, -128,
		127, -128,
		127, 127,
		-128, 127 
		};

static GLubyte streetFace[] = { 1, 0, 3, 1, 3, 2 };

static GLfloat campusVertData[] = {
		// front face
		920,TECHEIGHT,-600,
		580,TECHEIGHT,250,
		580,HEIGHT,250,
		920,HEIGHT,-600,
		
		// top face
		920,TECHEIGHT,-600,
		580,TECHEIGHT,250,
		930,TECHEIGHT,510,
		1280,TECHEIGHT,-250,
		
		// left 
		920,TECHEIGHT,-600,
		1280,TECHEIGHT,-250,
		1280,HEIGHT,-250,
		920,HEIGHT,-600,
		
		// right
		580,TECHEIGHT,250,
		930,TECHEIGHT,510,
		930,HEIGHT,510,
		580,HEIGHT,250,
		
		// back
		1280,TECHEIGHT,-250,
		930,TECHEIGHT,510,
		930,HEIGHT,510,
		1280,HEIGHT,-250,
		
		// bottom
		920,HEIGHT,-600,
		580,HEIGHT,250,
		930,HEIGHT,510,
		1280,HEIGHT,-250
};

static GLbyte campusTexdata[] =
{
    // front
    -128, -128,
    0, -128,
    0, 0,
    -128, 0,
    
    // top
    -128, -128,
     127, -128,
     127, 127,
    -128, 127,
    
    // left
    0,0,
    -128,0,
    -128,127,
    0,127,
   
    // right
    0,0,
    127,0,
    127,127,
    0,127,
    
    // back
    127, -128,
    0, -128,
    0, 0,
    127, 0
};

static GLfloat adj2VertData[] = {
	// front face
	1080,TECHADH1,-880,
	920,TECHADH1,-600,
	920,HEIGHT,-600,
	1080,HEIGHT,-880,
	
	// top face
	1080,TECHADH1,-880,
	920,TECHADH1,-600,
	1080,TECHADH1,-450,
	1230,TECHADH1,-740,
	
	// left 
	1080,TECHADH1,-880,
	1230,TECHADH1,-740,
	1230,HEIGHT,-740,
	1080,HEIGHT,-880,
	
	// right
	920,TECHADH1,-600,
	1080,TECHADH1,-450,
	1080,HEIGHT,-450,
	920,HEIGHT,-600,
	
	// back
	1230,TECHADH1,-740,
	1080,TECHADH1,-450,
	1080,HEIGHT,-450,
	1230,HEIGHT,-740,
	
	// bottom
	1080,HEIGHT,-880,
	920,HEIGHT,-600,
	1080,HEIGHT,-450,
	1230,HEIGHT,-740
};


static GLbyte adj2Texdata[] =
{
    // front
	-128, 0,
	43, 0,
	43, 127,
	-128, 127,
    
    // top
	-128, -128,
	 127, -128,
	 127, 127,
	-128, 127,
    
    // left
	127, -53,
	 43, -53,
	 43, 127,
	 127, 127,
	    
    // right
	-128, 73,
	127, 73,
	127, 127,
	-128, 127,
    
    // back
	127, -128,
	-128, -128,
	-128, -54,
	 127, -54
};

static GLfloat adj1VertData[] = {
	// front face
	1080,TECHADH1,-880,
	1140,TECHADH1,-800,
	1140,HEIGHT,-800,
	1080,HEIGHT,-880,
	
	// top face
	1080,TECHADH1,-880,
	1140,TECHADH1,-800,
	1506,TECHADH1,-1320,
	1420,TECHADH1,-1420,
	
	// left 
	1080,TECHADH1,-880,
	1420,TECHADH1,-1420,
	1420,HEIGHT,-1420,
	1080,HEIGHT,-880,
	
	// right
	1140,TECHADH1,-800,
	1506,TECHADH1,-1320,
	1506,HEIGHT,-1320,
	1140,HEIGHT,-800,
	
	// back
	1506,TECHADH1,-1320,
	1420,TECHADH1,-1420,
	1420,HEIGHT,-1420,
	1506,HEIGHT,-1320,
	
	// bottom
	1080,HEIGHT,-880,
	1140,HEIGHT,-800,
	1506,HEIGHT,-1320,
	1420,HEIGHT,-1420
};

static GLbyte adj1Texdata[] =
{
    // front
	-128, -128,
	 127, -128,
	 127, 127,
	-128, 127,
    
    // top
	-128, -128,
	 127, -128,
	 127, 127,
	-128, 127,
    
    // left
    -128, 73,
    127, 73,
    127, 127,
    -128, 127,
   
    // right
	-128, -128,
	 127, -128,
	 127, -60,
	-128, -60,
    
    // back
	-128, -128,
	 127, -128,
	 127, 127,
	-128, 127
};

static GLfloat adjVertData[] = {
	// front face
	1420,TECHADH,-1420,
	1610,TECHADH,-1340,
	1610,HEIGHT,-1340,
	1420,HEIGHT,-1420,
	
	// top face
	1420,TECHADH,-1420,
	1610,TECHADH,-1340,
	1890,TECHADH,-1690,
	1690,TECHADH,-1850,
	
	// left 
	1420,TECHADH,-1420,
	1690,TECHADH,-1850,
	1690,HEIGHT,-1850,
	1420,HEIGHT,-1420,
	
	// right
	1610,TECHADH,-1340,
	1890,TECHADH,-1690,
	1890,HEIGHT,-1690,
	1610,HEIGHT,-1340,
	
	// back
	1890,TECHADH,-1690,
	1690,TECHADH,-1850,
	1690,HEIGHT,-1850,
	1890,HEIGHT,-1690,
	
	// bottom
	1420,HEIGHT,-1420,
	1690,HEIGHT,-1850,
	1890,HEIGHT,-1690,
	1610,HEIGHT,-1340
};

static GLbyte adjTexdata[] =
{
	// front
	0, -128,
	127, -128,
	127, 0,
	0, 0,
	
	// top
	-128, -128,
	 127, -128,
	 127, 127,
	-128, 127,
	
	// left
	0, -128,
	-128, -128,
	-128, 0,
	0, 0,
	
	// right
	-128,0,
	0,0,
	0,127,
	-128,127,
	
	// back
	0,0,
	127,0,
	127,127,
	0,127
};

static GLfloat tecnVertData[] = {
		// front face
		1480,TECHEIGHT,-2300,
		2066,TECHEIGHT,-1550,
		2066,HEIGHT,-1550,
		1480,HEIGHT,-2300,
		
		// top face
		1480,TECHEIGHT,-2300,
		2066,TECHEIGHT,-1550,
		2410,TECHEIGHT,-2080,
		1850,TECHEIGHT,-2530,
		
		// left 
		1480,TECHEIGHT,-2300,
		1850,TECHEIGHT,-2530,
		1850,HEIGHT,-2530,
		1480,HEIGHT,-2300,
		
		// right
		2066,TECHEIGHT,-1550,
		2410,TECHEIGHT,-2080,
		2410,HEIGHT,-2080,
		2066,HEIGHT,-1550,
		
		// back
		2410,TECHEIGHT,-2080,
		1850,TECHEIGHT,-2530,
		1850,HEIGHT,-2530,
		2410,HEIGHT,-2080,
		
		// bottom
		1480,HEIGHT,-2300,
		1850,HEIGHT,-2530,
		2410,HEIGHT,-2080,
		2066,HEIGHT,-1550
};

static GLbyte tecnTexdata[] =
{
    // front
    -128, -128,
    0, -128,
    0, 0,
    -128, 0,
    
    // top
    -128, -128,
     127, -128,
     127, 127,
    -128, 127,
    
    // left
    0,0,
    -128,0,
    -128,127,
    0,127,
   
    // right
    127, -128,
	0, -128,
	0, 0,
	127, 0,
    
    // back
	0,0,
	127,0,
	127,127,
	0,127
};

static GLfloat cantinaVertData[] = {
		// front face
		0,CANTINAHEIGHT,0,
		230,CANTINAHEIGHT,200,
		230,HEIGHT,200,
		0,HEIGHT,0,
		
		// top face
		0,CANTINAHEIGHT,0,
		230,CANTINAHEIGHT,200,
		380,CANTINAHEIGHT,-200,
		180,CANTINAHEIGHT,-300,
		
		// left 
		0,CANTINAHEIGHT,0,
		180,CANTINAHEIGHT,-300,
		180,HEIGHT,-300,
		0,HEIGHT,0,
		
		// right
		230,CANTINAHEIGHT,200,
		380,CANTINAHEIGHT,-200,
		380,HEIGHT,-200,
		230,HEIGHT,200,
		
		// back
		380,CANTINAHEIGHT,-200,
		180,CANTINAHEIGHT,-300,
		180,HEIGHT,-300,
		380,HEIGHT,-200,
		
		// bottom
		0,HEIGHT,0,
		180,HEIGHT,-300,
		380,HEIGHT,-200,
		230,HEIGHT,200,
		
};

static GLbyte cantinaTexdata[] =
{
    // front
	-128, -128,
	 127, -128,
	 127, 127,
	-128, 127,
    
    // top
	-128, -128,
	 127, -128,
	 127, 127,
	-128, 127,
    
    // left
	-128, -128,
	 127, -128,
	 127, 127,
	-128, 127,
   
    // right
    -128, -128,
     127, -128,
     127, -75,
    -128, -75,
    
    // back
    -128, 62,
    127, 62,
    127, 127,
    -128, 127
};

static GLbyte objCubeTexdata[] =
{
    /* whole texture all faces */
    -128, -128,
     127, -128,
     127, 127,
    -128, 127,

    -128, -128,
     127, -128,
     127, 127,
    -128, 127,

    -128, -128,
     127, -128,
     127, 127,
    -128, 127,

   -128, -128,
     127, -128,
     127, 127,
    -128, 127,

    -128, -128,
     127, -128,
     127, 127,
    -128, 127,

    -128, -128,
     127, -128,
     127, 127,
    -128, 127
};

static GLubyte objCubeFaceData[] =
{
     1,0,3,
     1,3,2,

     5,4,7,
     5,7,6,

     9,8,11,
     9,11,10,

     13,12,15,
     13,15,14,

     17,16,19,
     17,19,18,
	 
	 21,22,23,
	 21,23,20
};

#endif /* GLAZEMESH_H_ */
