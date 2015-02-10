#include "polygon.h"

CPolygon::CPolygon(const CPolygon* p) 
{
	angle=p->angle;
	position=p->position;
	vel=p->vel;
	v_count=p->v_count;
	colided=p->colided;
	vertex=(Vector*)malloc(sizeof(Vector)*v_count);

	for(int i=0; i < v_count; i++)
	{
		vertex[i]=p->vertex[i];
	}
};

CPolygon::CPolygon(const CPolygon& p) 
{
	angle=p.angle;
	position=p.position;
	vel=p.vel;
	v_count=p.v_count;
	colided=p.colided;
	vertex=(Vector*)malloc(sizeof(Vector)*v_count);

	for(int i=0; i < v_count; i++)
	{
		vertex[i]=p.vertex[i];
	}
};

CPolygon::CPolygon(std::vector<Vector> *vectors ,int _angle, int x=0,int y=0)
{
	vertex=new Vector[vectors->size()];

	for(int br=0; br < vectors->size();br++)
	{
		vertex[br]=(*vectors)[br];
	}     

	position=Vector(x,y);
	angle=_angle;
	v_count=vectors->size(); 
	vel.Set(0,0);
}

CPolygon::CPolygon(int n,int *i,int _angle, int x=0,int y=0)
{
	vertex=new Vector[n];

	for(int br=0; br < n*2;br++)
	{
		int df=br+1;
		vertex[0+br/2]=Vector(i[br],i[df]);
		br++;
	}     

	position=Vector(x,y);
	angle=_angle;
	v_count=n; 
	vel.Set(0,0);
}
void CPolygon::UpdateVertices(const Vector& position, float angle)
{
	for(int i = 0; i < v_count; i ++)
		vertex[i].transform(position, angle);
}

void CPolygon::UpdateVertices()
{
	for(int i = 0; i < v_count; i ++)
		vertex[i].transform(position, angle);
}

void CPolygon::Draw()
{

	CPolygon p= *this;

	glBegin(GL_TRIANGLE_FAN);
	for(int i=0; v_count > i; i++ )
		glVertex2f( p.vertex[i].x,p.vertex[i].y );
	glVertex2f(p.vertex[0].x,p.vertex[0].y);
	glEnd();     
}


