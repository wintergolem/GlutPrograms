/*  =====================================
    SU-Camera.cpp - GL view control
    Copyright (c) Jules Bloomenthal, 2011
    All rights reserved
    =====================================

    Contents
        Transformations
            MatrixTransform
        Camera
            SetViewRotate
            GetModelRotate
            SetModelView
        Mouse
            MouseUp
            MouseDown
            MouseDrag
*/

#include <string>
#include <float.h>
#include <gl/glut.h>
#include "Camera.h"


//========== Transformations ===============

static void MatrixTransform(double *p, double m[][4], double *r)
{
    r[0] = p[0]*m[0][0]+p[1]*m[1][0]+p[2]*m[2][0]+m[3][0];
    r[1] = p[0]*m[0][1]+p[1]*m[1][1]+p[2]*m[2][1]+m[3][1];
    r[2] = p[0]*m[0][2]+p[1]*m[1][2]+p[2]*m[2][2]+m[3][2];
}


//========== Camera ===============

Camera::Camera()
{
	Set(0, 0, 0);
}

Camera::Camera(int w, int h, double fov)
{
	Set(w, h, fov);
}

void Camera::Set(int w, int h, double fov)
{
    SetView(w, h, fov);
    eventMode    = NoEvent;
    nViewChanges = -1;
    zoom         = 1;
    dolly        = 5.;
    for (int i = 0; i < 2; i++)
        mouseRef[i] = mouseNow[i] = screenShift[i] = 0;
    for (int j = 0; j < 3; j++)
        rotate[j] = rotateCenter[j] = rotateOffset[j] =
        viewCenter[j] = viewOffset[j] = 0;
}

void Camera::SetView(int w, int h, double fov)
{
    double aspect = (double) w / (double) h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    screenHeight = h;
    perspective = fov > 0;
    if (perspective)
        gluPerspective(fov, aspect, 0.001, 100.);
    else {
        if (aspect > 1)
            glOrtho(-aspect, aspect, -1, 1, -100, 100);
        else
            glOrtho(-1, 1, -1/aspect, 1/aspect, -100, 100);
    }
	SetModelView();
}

void Camera::SetViewRotate(double *r)
{
    double m[4][4], rXformedWithOldRotateCenter[3], rXformedWithNewRotateCenter[3];
    GetModelRotate(m);
    MatrixTransform(r, m, rXformedWithOldRotateCenter);
    memcpy(rotateCenter, r, 3*sizeof(double));
    GetModelRotate(m); // *** depends on rotateCenter
    MatrixTransform(r, m, rXformedWithNewRotateCenter);
    for (int i = 0; i < 3; i++)
        rotateOffset[i] += rXformedWithOldRotateCenter[i]-rXformedWithNewRotateCenter[i];
    nViewChanges++;
}

void Camera::GetModelRotate(double m[][4])
{
    // as SetModelView except no scale (zoom) and no screen screenShift
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    double t[3];
    for (int i = 0; i < 3; i++)
        t[i] = rotateOffset[i]+rotateCenter[i];
    glTranslatef(t[0], t[1], t[2]);                                     // c, d
    glRotatef(rotate[0], 1, 0, 0);                                      // b)
    glRotatef(rotate[1], 0, 1, 0);                                      // b)
    glTranslatef(-rotateCenter[0], -rotateCenter[1], -rotateCenter[2]); // a)
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble*) m);
    glPopMatrix();
}

void Camera::SetModelView()
{
    // called by MouseDrag
    nViewChanges++;
    // initialize ModelView matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); 
    // GL transformations accumulate upon the local coordinate system, ie, the reverse
    // order that transformations accumulate on the object in the global coord system
    // read software in reverse to understand these geometric transformations:
    //   a) shift to center of rotation
    //   b) rotate about arbitrary axis in local coord system (world y-axis is screen-up)
    //   c) shift back from center of rotation
    //   d) shift by rotateOffset, to compensate for change in center of rotation, ie, 
    //      permit change in center of rotation without translation (see SetViewRotate)
    //   e) translate camera to scene center
    //      **** replace with gluLookAt?
    //   f) orthographic: scale x,y to effect telephoto, perspective: dolly in z
    if (perspective)
        glTranslated(screenShift[0], screenShift[1], -dolly);
    else {
        // orthographic
        glScaled(zoom, zoom, 1);                                        // f
            // **** can cause bad lighting?
        double d[] = {screenShift[0], screenShift[1], 0.}, s[3];
        for (int i = 0; i < 3; i++)
            s[i] = d[i]-viewCenter[i]-viewOffset[i];
        glTranslated(s[0], s[1], s[2]);                                 // e
    }
    double t[3];
    for (int i = 0; i < 3; i++)
        t[i] = rotateOffset[i]+rotateCenter[i];
    glTranslated(t[0], t[1], t[2]);                                     // c, d
    glRotated(rotate[0], 1, 0, 0);                                      // b)
    glRotated(rotate[1], 0, 1, 0);                                      // b)
    glTranslated(-rotateCenter[0], -rotateCenter[1], -rotateCenter[2]); // a)
}

void Camera::SetZoom(double z)
{
    zoom = z;
    SetModelView();
}

double Camera::GetDolly()
{
    return dolly;
}


//========== Mouse ===============

void Camera::MouseUp()
{
    eventMode = NoEvent;
}

void Camera::MouseDown(int x, int y, bool shft, bool ctrl, bool alt)
{
    // up to the app y = screenHeight-y; // want y-axis upwards

    mouseNow[0] = mouseRef[0] = x;
    mouseNow[1] = mouseRef[1] = y;
    dollyRef = dolly;
    speed = alt? 0.1 : 0.5;
    eventMode = 
        !shft && !ctrl? Rotate    :
         shft && !ctrl? Translate :
        !shft &&  ctrl? Zoom      :
                        NoEvent;
}

bool Camera::MouseDrag(int x, int y)
{
    // up to the app y = screenHeight-y; // wish y-axis upwards

    mouseNow[0] = x;
    mouseNow[1] = y;

    if (eventMode == NoEvent)
        return false;

    int dx = x-mouseRef[0];
    int dy = y-mouseRef[1];

    if (eventMode == Rotate) {
        mouseRef[0] = x;
        mouseRef[1] = y;
        rotate[0] -= speed*dy;
        rotate[1] += speed*dx; // world y-axis points up
    }

    if (eventMode == Translate) {
        double f = .005*speed/zoom;
        mouseRef[0] = mouseNow[0];
        mouseRef[1] = mouseNow[1];
        screenShift[0] += f*dx;
        screenShift[1] += f*dy;
    }

    if (eventMode == Zoom) {
        if (perspective) {
            double a = -(double)dy/screenHeight;
            if (dollyRef > -.1 && dollyRef < .1)
                // dolly is miniscule so increment rather than scale
                dolly = dollyRef-.2*a;
            else {
                double f = 9*a+1.; // dy > 0? SlowInOut(1., 10., a) : SlowInOut(1., -2., a);
                dolly = f*dollyRef;
            }
        }
        else {
            enum {Linear, Quadratic, Quartic} mode = Quartic;
                // quadratic better small-valued control, but quartic smoother
            double d = .005*(double)dy;
            double v = 1+speed*(d < -1? -1 : d);  // +/- 200 pixels is -1 to 1
            double f = mode == Linear? v : mode == Quadratic? v*v : v*v*v*v;
            double z = f*zoom;
            zoom = z < DBL_EPSILON? DBL_EPSILON : z;
        }
    }

    SetModelView();
    return true;
}
