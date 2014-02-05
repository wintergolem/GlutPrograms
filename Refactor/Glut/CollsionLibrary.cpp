//--INCLUDE FILES--
#include <stdio.h>
#include <stdlib.h>
#include "SimpleFramework.h"
#include "gl/freeglut.h"
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>
#include "CollisionLibraryHeader.h"
using namespace std;


class Point2d{
public:
	float x,y;
};
class Point3d{
	float x,y,z;
};
class Ray{
public:
	float x,y,z;
	float dir;
};
class plane{
public:
	float a,b,c,d;

	float dot(float rx,float ry,float rd){
		return (rx*a + ry*b);
	}
};
class sph{
public:
	Point3d pos;
};
bool AABBAABB(Point2d c1, Point2d c2){
	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;
	return (dx <c1.x && dy <c2.y);
}

bool circlecircle(Point2d c1, float r1, Point2d c2, float r2){
	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;

	float Dsq = dx * dx + dy * dy;
	float Rsq = (r1 + r2) * (r1 + r2);
	return Dsq < Rsq;
}

bool RayPlane(Ray r, plane p){
	//float d = p.dot(r.x,r.y,r.dir) / p.dot(r.dir);
	return true;
}

//bool SpherePlane(Sph s, plane p){
//	return false;
//}