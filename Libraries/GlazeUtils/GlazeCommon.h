/*
============================================================================
 Name        : Common.h
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Common.h - header file
============================================================================
*/
#include <GLES/egl.h>

#ifndef COMMON_H
#define COMMON_H

#define TEX_WIDTH     256 // Texture size must be power of two and max size
#define TEX_HEIGHT    256 
#define FRUSTUM_LEFT   -1.f     //left vertical clipping plane
#define FRUSTUM_RIGHT   1.f     //right vertical clipping plane
#define FRUSTUM_BOTTOM -1.f     //bottom horizontal clipping plane
#define FRUSTUM_TOP     1.f     //top horizontal clipping plane
#define FRUSTUM_NEAR    3.f     //near depth clipping plane
#define FRUSTUM_FAR  100000.f     //far depth clipping plane
#define NUM_VERTICES_CUBE     24
#define NUM_FACES_CUBE        12
#define NUM_VERTICES_POLY     4
#define NUM_FACES_POLY        2

static GLfloat objVertexdata[] =
{
    /* top */
    -1,  1,  1,
    1,  1,  1,
    1, -1,  1,
    -1, -1,  1,

    /* front */
    1,  1,  1,
    1,  1, -1,
    1, -1, -1,
    1, -1,  1,

    /* right */
    -1,  1,  1,
    -1,  1, -1,
    1,  1, -1,
    1,  1,  1,

    /* left */
    1, -1,  1,
    1, -1, -1,
    -1, -1, -1,
    -1, -1,  1,

    /* back */
    -1, -1,  1,
    -1, -1, -1,
    -1,  1, -1,
    -1,  1,  1,

    /* bottom */
    -1,  1, -1,
    1,  1, -1,
    1, -1, -1,
    -1, -1, -1,

	/* char */
    -1,  1,  1,
    1,  1,  1,
    1, -1,  1,
    -1, -1,  1
};

static GLbyte objNormaldata[] =
{
    /* top */
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1,

    /* front */
    1,0,0,
    1,0,0,
    1,0,0,
    1,0,0,

    /* right */
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,

    /* left */
    0,-1,0,
    0,-1,0,
    0,-1,0,
    0,-1,0,

    /* back */
    -1,0,0,
    -1,0,0,
    -1,0,0,
    -1,0,0,

    /* bottom */
    0,0,-1,
    0,0,-1,
    0,0,-1,
    0,0,-1,

	/* chars */
    0,0,1,
    0,0,1,
    0,0,1,
    0,0,1
};


static GLbyte objTexdata[] =
{
    /* top, whole texture nasa_hubble.h */
    -128, -128,
     127, -128,
     127, 127,
    -128, 127,

    /* front, spiral with tail */
    -128, -128,
     127, -128,
     127, 127,
    -128, 127,

    /* right, red nebulae */
    -128, -128,
     127, -128,
     127, 127,
    -128, 127,

    /* left, plasma cloud */
   -128, -128,
     127, -128,
     127, 127,
    -128, 127,

    /* back, 2 spirals */
    -128, -128,
     127, -128,
     127, 127,
    -128, 127,

    /* bottom, whole texture ogles.jpg */
    -128, -128,
     127, -128,
     127, 127,
    -128, 127,
	
	/* chars */
	-128, -128,
     127, -128,
     127, 127,
    -128, 127,
};

static GLubyte objFacedataCube[] =
{
    /* top */
     1,0,3,
     1,3,2,

     /* front */
     5,4,7,
     5,7,6,

     /* right */
     9,8,11,
     9,11,10,

     /* left */
     13,12,15,
     13,15,14,

     /* back */
     17,16,19,
     17,19,18,
	 
	 /* bottom */
	 21,22,23,
	 21,23,20
};

static GLubyte objFacedataPoly[] =
{
	1,0,3,
	1,3,2
};

static GLubyte objFacedataChar[] =
{
	25,24,27,
    25,27,26
};

#endif // COMMON_H

// End of File
