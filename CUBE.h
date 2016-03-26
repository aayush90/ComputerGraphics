#ifndef CUBE_H
#define CUBE_H

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>

#include <map>
#include <vector>

using namespace std;

// extern GLfloat **n;
// extern GLint **faces;
// extern GLfloat **v;

//extern int cubes;

void init(void);

void drawBox(void);

void display(void);

void reshape(int, int);

void project(int,char **,GLfloat **,GLfloat **,GLint **,int);


#endif




//g++ -std=c++11 -c VoxelMergeGreedy.cpp CUBE.cpp -lGL -lGLU -lglut
//g++ -std=c++11 VoxelMergeGreedy.o CUBE.o -o VoxelMergeGreedy -lGL -lGLU -lglut
