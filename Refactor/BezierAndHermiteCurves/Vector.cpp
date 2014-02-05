/* ========================================
   SU-Vector.cpp - Simple Vector Operations
   Copyright (c) Jules Bloomenthal, 2011
   All rights reserved
   ======================================== */

#include "Vector.h"


// ================ Miscellaneous Operations

int Round(double d)
{
    return (int)(d < 0? ceil(d-.5) : floor(d+.5));
};

double Blend(double x)
{
    // f(r): f(0)=1, f(R)=0, f'(0)=f'(1)=0
    if (x < 0)
        x = -x;
    if (x < DBL_EPSILON)
        return 1;
    if (x >= 1)
        return 0;
    double x2 = x*x, x4 = x2*x2;
    return (-4./9.)*x2*x4+(17./9.)*x4+(-22./9.)*x2+1;
};


// ================ N-Dimensional Vector Operations

double MagSq(double *v, int dim)
{
    return DotProduct(v, v, dim);
}

double DistSq(double *p1, double *p2, int dim)
{
    double distSq = 0;
    for (int i = 0; i < dim; i++) {
        double d = p2[i]-p1[i];
        distSq += d*d;
    }
    return distSq;
}

double Normalize(double *v, int dim)
{
    double mag = sqrt(MagSq(v, dim));
    for (int i = 0; i < dim; i++)
        v[i] /= mag;
    return mag;
}

void Sub(double *v1, double *v2, double *r, int dim)
{
    for (int i = 0; i < dim; i++)
        r[i] = v1[i]-v2[i];
}

void Add(double *v1, double *v2, double *r, int dim)
{
    for (int i = 0; i < dim; i++)
        r[i] = v1[i]+v2[i];
}

void Mul(double *v, double a, double *r, int dim)
{
    for (int i = 0; i < dim; i++)
        r[i] = a*v[i];
}

double DotProduct(double *v1, double *v2, int dim)
{
    double dot = 0.;
    for (int i = 0; i < dim; i++)
        dot += v1[i]*v2[i];
    return dot;
}

void Invert(double *v, int dim)
{
    for (int i = 0; i < dim; i++)
        v[i] = -v[i];
}

void Copy(double *vSrc, double *vDst, int dim)
{
    memcpy(vDst, vSrc, dim*sizeof(double));
    // for (int i = 0; i < dim; i++)
    //     vDst[i] = vSrc[i];
}


// ================ 3-Dimensional Vector Operations

void Set(double *v, double x, double y, double z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

static double xaxis[] = {1,0,0}, yaxis[] = {0,1,0}, zaxis[] = {0,0,1};

bool GoodCrosser(double *v1, double *v2)
{
    return fabs(DotProduct(v1, v2)) < 1;
}

void Ortho(double v[3], double o[3])
{
    double *crosser = GoodCrosser(v, xaxis)? xaxis :
                      GoodCrosser(v, yaxis)? yaxis :
                                             zaxis;
        // a vector that is not colinear with (x,y,z)
    CrossProduct(crosser, v, o);
}

void CrossProduct(double *v1, double *v2, double *r)
{
    r[0] =  v1[1]*v2[2]-v1[2]*v2[1];
    r[1] = -v1[0]*v2[2]+v1[2]*v2[0];
    r[2] =  v1[0]*v2[1]-v1[1]*v2[0];
}

void TriangleCenter(double *p1, double *p2, double *p3, double *r)
{
    for (int i = 0; i < 3; i++)
        r[i] = (p1[i]+p2[i]+p3[i])/3.;
}

void TriangleNormal(double *p1, double *p2, double *p3, double *r)
{
    // best results if pivot is vertex with greatest angle (ie, opposite longest side)
    double dif1[3], dif2[3], dif3[3];
    Sub(p3, p2, dif1);
    Sub(p1, p3, dif2);
    Sub(p2, p1, dif3);
    double sqMag1 = MagSq(dif1);
    double sqMag2 = MagSq(dif2);
    double sqMag3 = MagSq(dif3);
    int pivot = sqMag1 > sqMag2? (sqMag1 > sqMag3? 1 : 3) : (sqMag2 > sqMag3? 2 : 3);
    switch (pivot) {
        case 1: CrossProduct(dif2, dif3, r); break;
        case 2: CrossProduct(dif3, dif1, r); break;
        case 3: CrossProduct(dif1, dif2, r); break;
    }
    Normalize(r);
}

void RotateAbout(double v[3], double axis[3], double radians, double result[3])
{
  // axis presumed normalized, angle in radians
  double s = sin(radians), c = cos(radians), c1 = 1-c;
  double sqx = axis[0]*axis[0], sqy = axis[1]*axis[1], sqz = axis[2]*axis[2];
  double xycos1 = axis[0]*axis[1]*c1, xsin = axis[0]*s;
  double yzcos1 = axis[1]*axis[2]*c1, ysin = axis[1]*s;
  double zxcos1 = axis[0]*axis[2]*c1, zsin = axis[2]*s;
  double m00 = sqx+(1-sqx)*c, m01 = xycos1+zsin,   m02 = zxcos1-ysin;
  double m10 = xycos1-zsin,   m11 = sqy+(1-sqy)*c, m12 = yzcos1+xsin;
  double m20 = zxcos1+ysin,   m21 = yzcos1-xsin,   m22 = sqz+(1-sqz)*c;
  result[0] = v[0]*m00+v[1]*m01+v[2]*m02;
  result[1] = v[0]*m10+v[1]*m11+v[2]*m12;
  result[2] = v[0]*m20+v[1]*m21+v[2]*m22;
}

int QuadraticRoots(double a, double b, double c, double *r1, double *r2)
{
    // solve for ax**2+bx+c
    double discriminant = b*b-4*a*c;
    if (discriminant < 0)
        return 0; // no real roots
    double den = 1/(2*a);
    if (IsZero(discriminant)) {
        *r1 = -b*den;
        return 1; // single root
    }
    double sqroot = sqrt(discriminant);
    *r1 = (-b+sqroot)*den;
    *r2 = (-b-sqroot)*den;
    return 2;
}

int SpherePtFromLine(double p1[3], double p2[3], double center[3], double radius, double roots[2])
{
    // given a 3D line defined by point p1 and p2, compute roots such that
    // p1+root*(p2-p1) is a point of intersection with a sphere of given center and radius
    // whose distance to center (of the sphere) equals radius
    double d[3], q[3]; // two points on line, line vector, two roots for t
    Sub(p2, p1, d);            // set d (line vector)
    // vector from point on line, p+t*d, to center is p+td-center, or q+td (q = p-center)
    // magnitude-squared of this vector is (q+td)**2 = (q**2)+2tdq+(t**2)(d**2)
    // in terms of the quadratic equation, a is d**2, b is 2dq, and c is (q**2)-(radius**2)
    Sub(p1, center, q);
    double a = MagSq(d);
    double b = 2*DotProduct(d, q);
    double c = MagSq(q)-radius*radius;
    return QuadraticRoots(a, b, c, &roots[0], &roots[1]);
}

void NearestToLine(double p[3], double p1[3], double p2[3], double nearest[3])
{
    // set nearest to be point on line p1p2 that is closest to p
    double v[] = {p2[0]-p1[0], p2[1]-p1[1], p2[2]-p1[2]};
    double dif[] = {p[0]-p1[0], p[1]-p1[1], p[2]-p1[2]};
    double vMagSq = v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
    double alpha = DotProduct(v, dif)/vMagSq;
    for (int i = 0; i < 3; i++)
        nearest[i] = p1[i]+alpha*v[i];
}

void ReferenceFrame(double v[3], double s[3], double t[3])
{
    Ortho(v, s);
    CrossProduct(v, s, t);
    Normalize(s);
    Normalize(t);
}


// ================ Distance

double DistSq2d(double x, double y, double *p)
{
    double dx = x-p[0], dy = y-p[1];
    return dx*dx+dy*dy;
}

double DistSq3d(double x, double y, double z, double *p)
{
    double dx = x-p[0], dy = y-p[1], dz = z-p[2];
    return dx*dx+dy*dy+dz*dz;
}


// ================ Planes

double DistanceToPlane(double *p, double *pln)
{
    return p[0]*pln[0]+p[1]*pln[1]+p[2]*pln[2]+pln[3];
}

bool IntersectPlane(double *p1, double *p2, double *pln, double *pInt, double *alpha)
{
    double d1 = DistanceToPlane(p1, pln);
    double d2 = DistanceToPlane(p2, pln);
    double d = d1-d2;
    if (IsZero(d))
        return false;
    double a = d1/d;
    if (alpha)
        *alpha = a;
    for (int i = 0; i < 3; i++)
        pInt[i] = p1[i]+a*(p2[i]-p1[i]);
    return true;
}

void SetPlane(double *p1, double *p2, double *p3, double *pln)
{
    // set plane through points p1, p2, p3
    double cen[3];
    TriangleNormal(p1, p2, p3, pln);
    TriangleCenter(p1, p2, p3, cen);
    pln[3] = -DotProduct(cen, pln);
}


// ================ Barycentric coordinates

enum PrincipalPlane {XY=0, YZ, ZX};

double Bary2Pts(double *p, double *p1, double *p2, PrincipalPlane pp) {
    return pp == XY? (p1[0]-p2[0])*(p[1]-p2[1])-(p1[1]-p2[1])*(p[0]-p2[0]) :
           pp == YZ? (p1[1]-p2[1])*(p[2]-p2[2])-(p1[2]-p2[2])*(p[1]-p2[1]) :
                     (p1[2]-p2[2])*(p[0]-p2[0])-(p1[0]-p2[0])*(p[2]-p2[2]);
}

void BarycentricFromPoint(double *p, double *p1, double *p2, double *p3, double *n, double *b)
{
    // set 3D barycentric coords b for a point p within triangle p1p2p3; n is triangle normal
    double a[] = {fabs(n[0]), fabs(n[1]), fabs(n[2])};
    PrincipalPlane pp = a[0] > a[1]? a[0] > a[2]? YZ : XY : a[1] > a[2]? ZX : XY;
    b[0] = Bary2Pts(p, p3, p2, pp);
    b[1] = Bary2Pts(p, p1, p3, pp);
    b[2] = Bary2Pts(p, p2, p1, pp);
    double sum = b[0]+b[1]+b[2];
    for (int i = 0; i < 3; i++)
        b[i] /= sum;
}


// ================ Transformations

void TransformPoint(float p[3], double m[][4], float result[3])
{
    result[0] = (float) (p[0]*m[0][0]+p[1]*m[1][0]+p[2]*m[2][0]+m[3][0]);
    result[1] = (float) (p[0]*m[0][1]+p[1]*m[1][1]+p[2]*m[2][1]+m[3][1]);
    result[2] = (float) (p[0]*m[0][2]+p[1]*m[1][2]+p[2]*m[2][2]+m[3][2]);
}

void TransformPoint(double p[3], double m[][4], double result[3])
{
    result[0] = p[0]*m[0][0]+p[1]*m[1][0]+p[2]*m[2][0]+m[3][0];
    result[1] = p[0]*m[0][1]+p[1]*m[1][1]+p[2]*m[2][1]+m[3][1];
    result[2] = p[0]*m[0][2]+p[1]*m[1][2]+p[2]*m[2][2]+m[3][2];
}

