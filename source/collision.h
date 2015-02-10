#ifndef COLLISION_H
#define COLLISION_H



#include "circle.h"
#include "polygon.h"

struct CollisionInfo
{
	CollisionInfo();
	float			m_mtdLengthSquared;
	Vector			m_mtd;
	bool			m_overlapped;
};

void AxisProjection(const Vector& axis, float& min, float& max, const CPolygon& p); 
bool PolygonsPolygonCollision(std::vector<CPolygon*> *polygons, CPolygon*);
bool CollisionTest(const CPolygon &_A,const CPolygon &_B);
bool CollisionTestAndResponse(const CPolygon &_A,const CPolygon &_B,Vector &vc ); 
bool intersect2DSegPoly( Vector sp0, Vector sp1, Vector* V, int n, Vector* isp0=NULL, Vector* isp1=NULL);
bool CircleCollision(CCircle* a, CCircle *b);
void GetCircleCollisionVector(CCircle* a, CCircle *b, Vector *m);

#endif
