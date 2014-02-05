/*  =======================================================
    CollisionBruteVsGrid.cpp - test collision detection
    Copyright (c) Jules Bloomenthal, 2013
    All rights reserved

    Exercise: convert this file into three separate source
    (.cpp) files and any needed header (.h) files:
        1. CollisionApp.cpp
            app globals
                appSize ... wht
            application routines
                Idle
                Draw
                MouseButton
                MouseDrag
                main
        2. CollisionMisc.cpp
				support routines
				    ColWidth
                    Line
				    Random
                class Circle
				class Widget
				class Slider
				class Button
        3. CollisionMethods.cpp
                Collide
                SetCircleCollisionsViaBruteForce
				class FixedGrid
 =======================================================
*/

#include <time.h>
#include <string>
#include <vector>
#include <gl/freeglut.h>
#include "CollisionApp.h"

using std::string;
using std::vector;


//****** Collision Detection

int SetCircleCollisionsViaBruteForce(vector<Circle> &circles)
{
    int nTests = 0;
    // brute force: test all circles against all other circles
    for (size_t k = 0; k < circles.size(); k++) {
        Circle &c = circles[k];
        for (size_t kk = k+1; kk < circles.size(); kk++) {
            Circle &c2 = circles[kk];
            if (!c.collided || !c2.collided) {
                nTests++;
                if (c.Collide(c2))
                    c.collided = c2.collided = true;
            }
        }
    }
    return nTests;
}

// FixedGrid Class contents
FixedGrid::FixedGrid(int res, int pixelSize){
	Init(res,pixelSize);
}

void FixedGrid::Init(int res, int pixelSize) {
    this->res = res;
    cells.resize(nCells = res*res);
    cellSize = static_cast<float>(pixelSize)/res;
}
int FixedGrid::Index(float v) {
    int i = static_cast<int>(v/cellSize);
    return i < 0? 0 : i >= res? res-1 : i;
}
int FixedGrid::SetCircleCollisionsViaSpatialPartition(vector<Circle> &circles) {
    int nTests = 0;
    // clear cells
    for (int i = 0; i < nCells; i++)
        cells[i].resize(0);
    // set cells for each circle
    for (size_t k = 0; k < circles.size(); k++) {
        Circle &c = circles[k];
        for (int i = Index(c.x-c.radius); i <= Index(c.x+c.radius); i++)
            for (int j = Index(c.y-c.radius); j <= Index(c.y+c.radius); j++)
                cells[i+j*res].push_back(k);
    }
    // check cells
    for (int i = 0; i < nCells; i++) {
        vector<int> &ids = cells[i];
        int nids = ids.size();
        // test all circle pairings
        for (int k = 0; k < nids; k++) {
            Circle &c = circles[ids[k]];
            for (int kk = k+1; kk < nids; kk++) {
                Circle &c2 = circles[ids[kk]];
                if (!c.collided || !c2.collided) {
                    nTests++;
                    if (c.Collide(c2))
                        c.collided = c2.collided = true;
                }
            }
        }
    }
    return nTests;
}



//****** Graphics
