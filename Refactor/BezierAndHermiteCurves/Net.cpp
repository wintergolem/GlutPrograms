/*  =====================================
    Net.cpp
    Copyright (c) Jules Bloomenthal, 2011
    All rights reserved.
    =====================================
*/

#include "Math.h"
#include "MiscGL.h"
#include "Net.h"


//****** Net Class

#define SIZE             15
double  netWidth       = 1.5;
double  springConstant = 0.05;
double  restLen        = 1.5/(SIZE-1);
double  restLenSq      = restLen*restLen;

void Net::Set()
{
    for (int i = 0; i < SIZE; i++) {
        double x = -netWidth/2.+netWidth*(double)i/(SIZE-1);
        for (int j = 0; j < SIZE; j++)
            pts[i][j] = Point(x, -netWidth/2+netWidth*(double)j/(SIZE-1), 0, false);
    }
    regressAlpha = 0;
}

void Net::PinBorder()
{
    for (int n = 0; n < SIZE; n++) {
        pts[n][0].pinned      =
        pts[n][SIZE-1].pinned =
        pts[0][n].pinned      =
        pts[SIZE-1][n].pinned = true;
    }
}

void Net::Regress(double regressBeta)
{
    // assume current net locations represent regression of regressAlpha
    // wish to set regression to regressBeta
    double c = (1-regressBeta)/(1-regressAlpha);
    double d = (regressBeta-regressAlpha)/(1-regressAlpha);
    for (int i = 0; i < SIZE; i++) {
        double x = -netWidth/2.+netWidth*(double)i/(SIZE-1);
        for (int j = 0; j < SIZE; j++) {
            double y = -netWidth/2+netWidth*(double)j/(SIZE-1);
            double z = 0;
            Point &p = pts[i][j];
            p.x = c*p.x+d*x;
            p.y = c*p.y+d*y;
            p.z = c*p.z+d*z;
        }
    }
    regressAlpha = regressBeta;
}

void Net::Unpin()
{
    for (int k = 0; k < SIZE*SIZE; k++)
        pts[k/SIZE][k%SIZE].pinned = false;
}

void Net::PinPoint(int i, int j, double x, double y, double z)
{
    Point &p = pts[i][j];
    p.x = x;
    p.y = y;
    p.z = z;
    p.pinned = true;
}

Point *Net::ClosestPoint(double *q)
{
    Point *r = NULL;
    double dsqMin = DBL_MAX;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            Point *p = &pts[i][j];
            double dx = p->x-q[0], dy = p->y-q[1], dz = p->z-q[2];
            double dsq = dx*dx+dy*dy+dz*dz;
            if (dsq < dsqMin) {
                dsqMin = dsq;
                r = p;
            }
        }
    return r;
}

Point *Net::NearPixel(double x, double y)
{
    // which is closest grid point to pixel (x,y)?
    Point *r = NULL;
    double dsqmin = DBL_MAX, s[2], fullMatrix[4][4];
    glmFullMatrix(fullMatrix);
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            Point *p = &pts[i][j];
            glmScreenFromWorld((double *) &p->x, s, fullMatrix);
            double dx = x-s[0], dy = y-s[1], dsq = dx*dx+dy*dy;
            if (dsq < dsqmin && dsq < 100) {
                dsqmin = dsq;
                r = p;
            }
        }
    return r;
}

void Net::AddForce(Point &p, int i, int j)
{
    if (i >= 0 && j >= 0 && i < SIZE && j < SIZE) {
        Point &q = pts[i][j];
        double dx = q.x-p.x, dy = q.y-p.y, dz = q.z-p.z;
        double dsq = dx*dx+dy*dy+dz*dz;
        if (dsq > restLenSq) {
            double len = sqrt(dsq), nx = dx/len, ny = dy/len, nz = dz/len;
            double f = springConstant*(len-restLen);
                // f = i+kd, where:
                //   f = total force exerted on spring
                //   d = distance spring extended beyond rest
                //   i = initial tension on spring (assumed 0)
                //   k = spring constant
            p.fx += f*nx;
            p.fy += f*ny;
            p.fz += f*nz;
        }
    }
}

double Net::Relax(int nRelaxations)
{
    double maxSq = 0;
    for (int n = 0; n < nRelaxations; n++) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                Point &p = pts[i][j];
                if (!p.pinned) {
                    p.fx = p.fy = p.fz = 0;
                    AddForce(p, i-1, j);
                    AddForce(p, i+1, j);
                    AddForce(p, i, j-1);
                    AddForce(p, i, j+1);
                }
            }
        }
        for (int k = 0; k < SIZE*SIZE; k++) {
            Point &p = pts[k/SIZE][k%SIZE];
            if (!p.pinned) {
                p.x += p.fx;
                p.y += p.fy;
                p.z += p.fz;
                double dsq = p.fx*p.fx+p.fy*p.fy+p.fz*p.fz;
                if (dsq > maxSq)
                    maxSq = dsq;
            }
        }
    }
    return maxSq;
}

void Net::Draw(bool shaded)
{
    if (shaded) {
        glEnable(GL_COLOR_MATERIAL);
        glBegin(GL_QUADS);
        for (int i = 0; i < SIZE-1; i++) {
            for (int j = 0; j < SIZE-1; j++) {
                glVertex3dv((double *) &pts[i][j].x);
                glVertex3dv((double *) &pts[i+1][j].x);
                glVertex3dv((double *) &pts[i+1][j+1].x);
                glVertex3dv((double *) &pts[i][j+1].x);
            }
        }
        glEnd();
    }
    else {
        // glColor3d(1, 1, 0);
        // for (int k = 0; k < SIZE*SIZE; k++)
        //     glmDisk((double *) &pts[k/SIZE][k%SIZE], 4);
        glColor3d(1, .7, 0);
        for (int i = 0; i < SIZE; i++) {
            glBegin(GL_LINE_STRIP);
            for (int t = 0; t < SIZE; t++)
                glVertex3dv((double *) &(pts[i][t]));
            glEnd();
            glBegin(GL_LINE_STRIP);
            for (int s = 0; s < SIZE; s++)
                glVertex3dv((double *) &(pts[s][i]));
            glEnd();
        }
    }
}
