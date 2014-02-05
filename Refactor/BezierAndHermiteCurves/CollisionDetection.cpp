// a.	Circle-Circle Intersection (Boby & Max)

double DistanceBetweenPoints(double p1[2], double p2[2])
{
	double dx = p2[0] - p1[0];
	double dy = p2[1] - p1[1];
	return sqrt(dx*dx + dy*dy);
}

bool Collision(double center1[2], double radius1, double center2[2], double radius2)
{

	return (DistanceBetweenPoints(center1, center2) < r1+r2);
	
}

// b.	AABB-AABB Intersection (Nate & John)

class BoundingBox
{
public:
	double xMin, xMax, yMin, yMax;

	BoundingBox(double sXMin, double sXMax, double sYMin, double sYMax)
	{
		xMin = sXMin;
		xMax = sXMax;
		yMin = sYMin;
		yMax = sYMax;
	}

	bool Collision(BoundingBox b)
	{
		if(xMin > b.xMax || b.xMin > xMax)
			return false;
		if(yMin > b.yMax || b.yMin > yMax)
			return false;
		return true;
	}
};

// c.	Ray-Plane Intersection (Jeff)

double DistanceToPlane(double ray[3], double plane[4])
{
	double d = plane[4];
	for( int i = 0; i < 4; i++ )
		d += ray[i]*plane[i];
	return d;
}

double Dot( double *v1, double *v2 )
{
	double DOT = 0.0;
	for( int i = 0; i < 3; i++ )
		DOT += v1[i]*v2[i];
	return DOT;
}

bool RayPlaneIntersect(double rayP[3], double rayV[3], double plane[4])
{
	bool PositiveSide = DistanceToPlane(rayP,plane) > 0;
	bool PositiveDot = Dot(rayV,plane) > 0;
	return	PositiveSide != PositiveDot;
}

// d.	Sphere-Plane Intersection (Jules)

double DistanceToPlane(double a[3], double plane[4])
{
	double d = plane[3];
	for (int i  = 0; i  < 3; i++)
		d += a[i]*plane[i];
	return d;
}

bool SpherePlane(double center[3], double radius, double plane[4])
{
	// doese sphere with given center and radius intersect the given plane?
	return DistanceToPlane(center, plane) < radius;
}

// e.	AABB-Plane Intersection (Jules)

bool AABBPlaneIntersection(double min[3], double max[3], double plane[4])
{
	double l = min[0], b = min[1], n = min[2];
	double r = max[0], t = max[1], f = max[2];
	double corners[][3] = {{l,b,n}, {l,b,f}, {l,t,n}, {l,t,f},
						   {r,b,n}, {r,b,f}, {r,t,n}, {r,t,f}};
	bool pos1 = DistanceToPlane(corners[0], plane) > 0;
	for (int i = 1; i < 8; i++) {
		bool pos2 = DistanceToPlane(corners[i], plane) > 0;
		if (pos1 != pos2)
			return true;
	}
	return false;
}
