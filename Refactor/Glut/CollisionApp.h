

#include <time.h>
#include <string>
#include <vector>
#include <gl/freeglut.h>

#ifndef COLLISION_HEADER
#define COLLISION_HEADER

using std::string;
using std::vector;

float Random(int min, int max);


class Circle {
public:
    float range, x, y, dx, dy, radius;
    bool collided;
    void Init(int xyRange);
    bool Collide(Circle &c);
    void Draw();
    void Update(float dt);
};

//****** Collision Detection

int SetCircleCollisionsViaBruteForce(vector<Circle> &circles);


//****** Grid

class FixedGrid {
public:
    int res, nCells;
    float cellSize;
    vector<vector<int>> cells;
    FixedGrid(int res, int pixelSize);
    void Init(int res, int pixelSize) ;
    int Index(float v);
    int SetCircleCollisionsViaSpatialPartition(vector<Circle> &circles);
};


//****** Graphics

void ColWidth(float *color, float width);
void Line(float x1, float y1, float x2, float y2, bool round = true);

void Line(int x1, int y1, int x2, int y2, bool round = true);

//****** UI

class Widget {
public:
    // a horizontal button between x and x+w, centered at y and h thick
    int x, y, w, h;
    string name;
    Widget(int x, int y, int w, int h, string name) ;
	bool Hit (int xx, int yy);
};

class Button : public Widget {
public:
	Button(int x, int y, int w, int h, string name);
    void Draw();
};

class Slider : public Widget {
public:
    float loc, min, max; // slider position, min, max
    Slider(int x, int y, int w, string name, float min, float max, float init);
    void Draw() ;
    float GetValue() ;
    bool Hit(int x, int y);
} ;

//****** Mouse Handlers

void MouseButton(int butn, int state, int x, int y);
void MouseDrag(int x, int y);

#endif