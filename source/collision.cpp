#include "collision.h"
#include "math.h"

CollisionInfo::CollisionInfo()
{
	m_mtdLengthSquared = 0.0f;
	m_mtd = Vector(0, 0);
	m_overlapped = false;
}	

void AxisProjection(const Vector& axis, float& min, float& max, const CPolygon& p) 
{
	min = max = (p.vertex[0] * axis);

	for(int i = 1; i < p.v_count; i ++)
	{
		float d = (p.vertex[i] * axis);
		if (d < min) 
			min = d; 
		else if (d > max) 
			max = d;
	}
}

bool CollisionTest(const CPolygon &_A,const CPolygon &_B) 
{
	CPolygon A=_A; 
	CPolygon B=_B;

	for(int j = A.v_count-1, i = 0; i < A.v_count; j = i, i ++)
	{
		Vector v0 = A.vertex[j];
		Vector v1 = A.vertex[i];

		Vector edge = v1 - v0;
		Vector axis = edge.perp(); 

		float mina, maxa;
		float minb, maxb;

		AxisProjection(axis, mina, maxa,A);

		AxisProjection(axis, minb, maxb,B);

		float d0 = (maxb - mina);
		float d1 = (minb - maxa);

		if(d0 < 0.0f || d1 > 0.0f) 
		{
			return false;
		}  
	}

	for(int j = B.v_count-1, i = 0; i < B.v_count; j = i, i ++)
	{
		Vector v0 = B.vertex[j];
		Vector v1 = B.vertex[i];

		Vector edge = v1 - v0;
		Vector axis = edge.perp(); 

		float mina, maxa;
		float minb, maxb;

		AxisProjection(axis, mina, maxa,A);

		AxisProjection(axis, minb, maxb,B);

		float d0 = (maxb - mina);
		float d1 = (minb - maxa);

		if(d0 < 0.0f || d1 > 0.0f) 
		{
			return false;
		}  
	}
	return true;
}

bool CollisionTestAndResponse(const CPolygon &_A,const CPolygon &_B, Vector &vc ) 
{
	CollisionInfo info;
	info.m_mtdLengthSquared = -1.0f;

	CPolygon A=_A; 
	CPolygon B=_B;

	for(int j = A.v_count-1, i = 0; i < A.v_count; j = i, i ++)
	{
		Vector v0 = A.vertex[j];
		Vector v1 = A.vertex[i];

		Vector edge = v1 - v0;
		Vector axis = edge.perp(); 

		float mina, maxa;
		float minb, maxb;

		AxisProjection(axis, mina, maxa,A);

		AxisProjection(axis, minb, maxb,B);

		float d0 = (maxb - mina);
		float d1 = (minb - maxa);

		if(d0 < 0.0f || d1 > 0.0f) 
		{
			return false;
		}  

		float overlap = (d0 < -d1)? d0 : d1;

		float axis_length_squared = (axis * axis);

		Vector sep = axis * (overlap / axis_length_squared);


		float sep_length_squared = (sep * sep);

		if(sep_length_squared < info.m_mtdLengthSquared || info.m_mtdLengthSquared < 0.0f)
		{
			info.m_mtdLengthSquared = sep_length_squared;
			info.m_mtd = sep;
		}
	}

	for(int j = B.v_count-1, i = 0; i < B.v_count; j = i, i ++)
	{
		Vector v0 = B.vertex[j];
		Vector v1 = B.vertex[i];

		Vector edge = v1 - v0;
		Vector axis = edge.perp(); 

		float mina, maxa;
		float minb, maxb;

		AxisProjection(axis, mina, maxa,A);

		AxisProjection(axis, minb, maxb,B);

		float d0 = (maxb - mina);
		float d1 = (minb - maxa);

		if(d0 < 0.0f || d1 > 0.0f) 
		{
			return false;
		}  

		float overlap = (d0 < -d1)? d0 : d1;

		float axis_length_squared = (axis * axis);

		Vector sep = axis * (overlap / axis_length_squared);

		float sep_length_squared = (sep * sep);

		if(sep_length_squared < info.m_mtdLengthSquared || info.m_mtdLengthSquared < 0.0f)
		{
			info.m_mtdLengthSquared = sep_length_squared;
			info.m_mtd = sep;
		}
	}
	vc=info.m_mtd;
	return true;
}

bool PolygonsPolygonCollision(std::vector<CPolygon*> *polygons, CPolygon* p)
{
    CPolygon a,b;
    a=*p;
    a.UpdateVertices();
    for(int j=0; j < polygons->size(); j++)
	{
       b=*(*polygons)[j];
       b.UpdateVertices();
       if(CollisionTest(&a, &b))
       {  
           return true;
       }
    }

    return false;
}

bool CircleCollision(CCircle* a, CCircle *b)
{
	Vector dist = a->position - b->position;
	float len= dist.Length();
	if( len > a->radius + b->radius )
		return false;

	return true;
}

void GetCircleCollisionVector(CCircle* a, CCircle *b, Vector *m)
{
	*m = a->position - b->position;
	Vector t=*m;
	t=t.Direction();
	t *= a->radius+b->radius;
	*m= t - *m ;
}
