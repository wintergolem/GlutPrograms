
#ifndef CURVES_HEADER
#define CURVES_HEADER


#include <stdio.h>
#include <stdlib.h>
#include "SimpleFramework.h"
#include "gl/freeglut.h"
#include <time.h>
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

extern int AppSize;



void Line(float x1, float y1, float x2, float y2);//, bool round = true);
void Line(int x1, int y1, int x2, int y2);//, bool round = true);

enum AppState { MenuState, BezierState, HermiteState};

class Point{
public:
	float pos[2];
	float tang[2];
	bool set;

	Point(float x, float y);
	void draw();
};

class Widget {
public:
    // a horizontal button between x and x+w, centered at y and h thick
    int x, y, w, h;
    string name;
	Widget();
    Widget(int x, int y, int w, int h, string name);
    //void WidgetInit(int x, int y, int w, int h, string name);
    bool Hit(int xx, int yy);
};

class Slider : public Widget {
public:
	bool init_ed;

    float loc, min, max, init; // slider position, min, max
	Slider();
	Slider(int x, int y, int w, string name, float min, float max, float init) {}
	void Init(int x, int y, int w, string name, float min, float max, float init);
    void Draw();
    float GetValue();
    bool Hit(int x, int y);
};

class HermiteClass{
public:
	//varibles
	static int selected;

	HermiteClass();

	vector<Point> points;
	void Line(float x1, float y1, float x2, float y2);//, bool round = true);
	void Line(int x1, int y1, int x2, int y2);//, bool round = true);
	//Slider cSlider;
	//Slider cSlider (int i = 50,int j = AppSize -50,int h = 155, string n = "C Slider", int a = 0,int b =1,float c= .5);
	void DrawDots();
	void CalCurve(float p1[],float p2[], float v1[], float v2[], float *result, float t);
	void DrawCurve();
	void DrawLines();
	void AllocDot(int a, int b);
	void AllocTangent(Slider &cSlider);
	void Draw(Slider &cSlider);
	void Idle(Slider &cSlider);
	void MouseButton(int button, int state, int x, int y, AppState &appState,Slider &cSlider);
	void MouseDrag(int x, int y,Slider &cSlider);
};

class BezierClass{
public:
	static int selected;
	static float points[4][2];

	BezierClass();
	void Line(float x1, float y1, float x2, float y2);//, bool round = true);
	void Line(int x1, int y1, int x2, int y2);//, bool round = true);
	//Slider tSlider;
	//Slider tSlider (int i =50,int inp = AppSize -50, int z =155, string na = "T Slider", int b =0,int c = 1, float g=.5);
	void Lerp(float *result, float *p0, float *p1, float t);
	void DrawPolygon();
	void DrawDot(float x, float y);
	void DrawDots(float a0[2],float a1[2],float a2[2],float b0[2],float b1[2],float c0[2]);
	void DrawBezierConstruct(float t);
	void MakeBezierCurve(float *result,float *a, float *b,float c[], float d[], float t);
	void DrawBezierCurve();
	void Draw(Slider &tSlider);
	void Idle(Slider &tSlider);
	void MouseButton(int button, int state, int x, int y, AppState &appState,Slider &tSlider);

	void MouseDrag(int x, int y, Slider &tSlider);

};

class MenuClass{
public:
	char *chooseText;

	MenuClass();
	void Draw();
	void Idle();
	void MouseButton(int button, int state, int x, int y, AppState &appState);
	void MouseDrag(int x, int y);
};

#endif