/*  =====================================
    SU-Widgets.cpp - GL view control
    Copyright (c) Jules Bloomenthal, 2011
    All rights reserved
    ===================================== */

#include "MiscGL.h"
#include "Vector.h"
#include "Widgets.h"

static double textScale = .1;


/// Widgets ///

double Widget::DistanceTo(int x, int y)
{
    double xdif = xscreen-x, ydif = yscreen-y;
    return sqrt(xdif*xdif+ydif*ydif);
}


/// Buttons ///

Button::Button() {
    xscreen = yscreen = size = 0;
    pressed = false;
    textPlacement = TP_Left;
    namePressed[0] = 0;
}

Button::Button(int x, int y, int s, char *nameUnpressed, char *namePressed, bool pressed)
{
    Set(x, y, s, nameUnpressed, namePressed, pressed);
    textPlacement = TP_Left;
}

void Button::Set(int x, int y, int s, char *nameUnpressed, char *namePressedA, bool pressed)
{
    xscreen = x;
    yscreen = y;
    size = s;
    this->pressed = pressed;
    if (nameUnpressed)
        strncpy(name, nameUnpressed, 255);
    if (namePressedA)
        strncpy(namePressed, namePressedA, 255);
    name[255] = namePressed[255] = 0;
}

void Button::MouseDown()
{
    pressed = !pressed;
}

void Button::Draw(double *onColor, double *offColor)
{
    // save GL state, disable z-buf, enter screen mode
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glmScreenMode();

    // text to display, and its length
    const char *n = pressed && namePressed[0]? namePressed : name;
    double glutlen = glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char *) n);
    double len = textScale*glutlen;

    // display name
    glColor3d(0, 0, 0);
    glLineWidth(1.5);
    double xText, yText;
    if (textPlacement == TP_Left || textPlacement == TP_Right) {
        yText = yscreen-4;
        xText = xscreen+(textPlacement == TP_Left? -5-len : 4+size/2);
    }
    if (textPlacement == TP_Top || textPlacement == TP_Bottom) {
        xText = xscreen-5-len/2;
        yText = yscreen+(textPlacement == TP_Top? 8+size/2 : -13-size/2);
    }
    if (textPlacement != TP_None) {
        glLineWidth(1);
        glmLabel(xText, yText, textScale, n);
    }

    // draw button
    double s[] = {xscreen, yscreen};
    double red[] = {1,0,0}, gry[] = {.3,.3,.3};
    onColor = onColor? onColor : red;
    offColor = offColor? offColor : gry;
    glmSoftDisk(s, size);
    glmSoftDisk(s, size-3/*4.5*/, pressed? onColor : offColor);

    // restore view and state
    glmRestoreView();
    glPopAttrib();
}

Button *ClosestButton(int x, int y, Button **buttons, int nButtons, int proximity)
{
    Button *closest = NULL;
    for (int i = 0; i < nButtons; i++) {
        Button *b = buttons[i];
        if (b->DistanceTo(x, y) < proximity)
            closest = b;
    }
    return closest;
}

void DrawButtons(Button **buttons, int nButtons, double *onColor, double *offColor)
{
    for (int i = 0; i < nButtons; i++)
        buttons[i]->Draw(onColor, offColor);
}

/// Sliders ///

Slider::Slider(int     xLocation,
               int     yLocation,
               int     dotSize,
               int     pixHeight,
               double  min,
               double  max,
               double  val,
               char   *text,
               bool    roundToInteger,
               bool    displayVal)
{
    xscreen      = xLocation;
    yscreen      = yLocation;
    size         = dotSize;
    height       = pixHeight;
    minValue     = min;
    maxValue     = max;
    enabled      = true;
    integer      = roundToInteger;
    displayValue = displayVal;
    strncpy(name, text, 255);
    SetValue(val);
}

double Slider::SetValue(double val)
{
    double old = value;
    value = val;
    if (integer)
        value = Round(value);
    slider = (int) ((maxValue-value)*height/(maxValue-minValue));
    return old;
}

double Slider::DistanceTo(int x, int y)
{
    double xdif = xscreen-x, ydif = yscreen+slider-y;
    return enabled? sqrt(xdif*xdif+ydif*ydif) : 1000;
}

void Slider::MouseDown(bool control)
{
    if (enabled) {
        vernier = control;
        oldSlider = slider;
    }
}

void Slider::MouseDrag(int xmouse, int ymouse)
{
    if (enabled) {
        slider = vernier?
            oldSlider+Round((ymouse-yscreen)/10.) :
            Max(Min(ymouse-yscreen, height), 0);
        double alpha = (height-slider)/(double)height;
        value = minValue+alpha*(maxValue-minValue);
        if (integer)
            value = Round(value);
    }
}

void Slider::Draw(double *color)
{
    // double slidePos[] = {xscreen, yscreen+slider};
    double grey[] = {.6, .6, .6}, blk[] = {0, 0, 0}, red[] = {1, 0, 0};
    glmScreenMode();
    glColor3dv(enabled? blk : grey);
    glBegin(GL_LINES);
    glVertex2d(xscreen, yscreen);
    glVertex2d(xscreen, yscreen+height);
    glEnd();
    glmRect(xscreen-5, yscreen+slider-3, xscreen+5, yscreen+slider+3, blk);
    glmRect(xscreen-4, yscreen+slider-2, xscreen+4, yscreen+slider+2, enabled? (color? color : red) : grey);
    if (displayValue) {
        double len = textScale*glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char *) name);
        glLineWidth(1.);
        glColor3dv(enabled? blk : grey);
        glmLabel(xscreen-5-len/2, yscreen-15, textScale, name);
        char dbuf[100], ibuf[100];
        sprintf(dbuf, "%4.2f", value);
        sprintf(ibuf, "%i", (int) value);
        glmLabel(xscreen-10, yscreen+height+4, .8*textScale, integer? ibuf : dbuf);
    }
    glmRestoreView();
}

Slider *ClosestSlider(int x, int y, Slider **sliders, int nSliders, int proximity)
{
    Slider *closest = NULL;
    for (int i = 0; i < nSliders; i++) {
        Slider *s = sliders[i];
        if (s->DistanceTo(x, y) < proximity)
            closest = s;
    }
    return closest;
}

void DrawSliders(Slider **sliders, int nSliders, double *color)
{
    for (int i = 0; i < nSliders; i++)
        sliders[i]->Draw(color);
}


/// Movers ///

void Mover::SetPlane(int x, int y)
{
    double p1[3], p2[3];
    glmScreenLine(x, y, p1, p2);
        // get two points that transform to pixel x,y
	for (int i = 0; i < 3; i++)
        plane[i] = p2[i]-p1[i];
        // set plane normal to that of line p1p2
    Normalize(plane);
        // not strictly needed
    plane[3] = -DotProduct(plane, point);
        // pass plane through point
}

void Mover::Pick(const double *p, int x, int y)
{
    memcpy(point, p, 3*sizeof(double));
    SetPlane(x, y);
}

void Mover::Pick(const float *p, int x, int y)
{
    for (int i = 0; i < 3; i++)
        point[i] = p[i];
    SetPlane(x, y);
}

void Mover::Drag(int x, int y)
{
    double p1[3], p2[3], axis[3];
    glmScreenLine(x, y, p1, p2);
        // get two points that transform to pixel x,y
    for (int i = 0; i < 3; i++)
        axis[i] = p2[i]-p1[i];
        // direction of line through p1
    double pdDot = DotProduct(axis, plane);
        // project onto plane normal
    double a = (-plane[3]-DotProduct(p1, plane))/pdDot;
    for (int j = 0; j < 3; j++)
        point[j] = p1[j]+a*axis[j];
        // intersection of line with plane
}

void Mover::Drag(int x, int y, double p[3])
{
    Drag(x, y);
    memcpy(p, point, 3*sizeof(double));
}

void Mover::Drag(int x, int y, float p[3])
{
    Drag(x, y);
    for (int i = 0; i < 3; i++)
        p[i] = point[i];
}


/// Joysticks ///

Joystick::Joystick(double p[3], double v[3], double a, double c[3]) {
    memcpy(base, p, 3*sizeof(double));
    memcpy(vector, v, 3*sizeof(double));
    memcpy(color, c, 3*sizeof(double));
    arrowLen = a;
    Normalize(vector);
    SetArrowHead();
}

void Joystick::SetArrowHead() {
    for (int i = 0; i < 3; i++)
        arrowHead[i] = base[i]+arrowLen*vector[i];
}

double Joystick::HeadDistanceSq(int x, int y, double fullMatrix[4][4]) {
    return glmScreenDistSq(x, y, arrowHead, fullMatrix);
}

double Joystick::BaseDistanceSq(int x, int y, double fullMatrix[4][4]) {
    return glmScreenDistSq(x, y, base, fullMatrix);
}

void Joystick::SetBase(double p[3]) {
    memcpy(base, p, 3*sizeof(double));
    SetArrowHead();
}

void Joystick::SetVector(double v[3]) {
    memcpy(vector, v, 3*sizeof(double));
    SetArrowHead();
}

void Joystick::SetVectorFromScreen(int x, int y) {
    double p1[3], p2[3], roots[2];
    glmScreenLine(x, y, p1, p2);
    int nroots = SpherePtFromLine(p1, p2, base, arrowLen, roots);
    if (nroots > 0) {
        double spherePt[2][3], *p = spherePt[0];
        for (int r = 0; r < nroots; r++)
            for (int i = 0; i < 3; i++)
                spherePt[r][i] = p1[i]+roots[r]*(p2[i]-p1[i]);
        if (nroots > 1) {
            double d1Mag = DistSq(spherePt[0], arrowHead);
            double d2Mag = DistSq(spherePt[1], arrowHead);
            p = d1Mag < d2Mag? spherePt[0] : spherePt[1];
        }
        Sub(p, base, vector);
        Mul(vector, 1./arrowLen, vector); // normalize
    }
    else {
        // line doesn't intersect sphere, but still change vector heading
        double nearest[3];
        NearestToLine(base, p1, p2, nearest);
        Sub(nearest, base, vector);
        Normalize(vector);
    }
    SetArrowHead();
}

void Joystick::Draw(bool baseSelected, bool headSelected) {
    double cyn[] = {0, 1, 1};
    glmDisk(arrowHead, 3.5, headSelected? cyn : color);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex3dv(base);
    glVertex3dv(arrowHead);
    glEnd();
    if (baseSelected) {
        double fullMatrix[4][4];
        glmFullMatrix(fullMatrix);
        glLineWidth(1.5);
        glColor3dv(cyn);
        glmCircle(base, fullMatrix, 5, false);
    }
}


/// Menus ///

Menu::Menu(int x, int y, int s, char *title)
{
    nentries = 0;
    xscreen = x;
    yscreen = y;
    size = s;
    pressed = false;
    selected = -1;
    name = string(title);
}

void Menu::AddEntry(char *entry)
{
    entries[nentries++] = string(entry);
}

void Menu::MouseDown()
{
    pressed = true;
}

void Menu::MouseDrag(int x, int y)
{
    selected = Round((double)(yscreen-y)/20.);
    if (selected < 0 || selected > nentries-1)
        selected = -1;
}

void Menu::MouseUp()
{
    pressed = false;
}

void Menu::Draw()
{
    double gry[] = {.7, .7, .7}, blk[] = {0, 0, 0}, red[] = {1, 0, 0};
    const char *title = name.c_str();
    double len = glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char *) title);
    double xText = xscreen+12; // -len*textScale/2;
    double yText = yscreen-5; // 13-size/2;
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_DEPTH_TEST);
    glmScreenMode();
    glColor3dv(blk);
    glLineWidth(1.5);
    glmLabel(xText, yText, textScale, title);
    if (pressed) {
        double maxlen = 0;
        for (int i = 0; i < nentries; i++) {
            double len = glutStrokeLength(GLUT_STROKE_ROMAN, (const unsigned char *) entries[i].c_str());
            maxlen = Max(maxlen, len);
        }
        glmRect(xscreen+30, yscreen+5, xscreen+1.5*maxlen*textScale, yscreen-10-20*nentries, gry);
        for (i = 0; i < nentries; i++) {
            glColor3dv(i == selected? red : blk);
            glmLabel(xscreen+40, yscreen-15-i*20, textScale, entries[i].c_str());
        }
    }
    else {
        double s[] = {xscreen, yscreen};
        glmSoftDisk(s, size, blk);
        glmSoftDisk(s, size-4.5, red);
    }
    glmRestoreView();
    glPopAttrib();
}
