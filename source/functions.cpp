#include "functions.h"
#include "flockmanager.h"

float GetfDistance(float x1, float y1, float x2, float y2)
{
	return sqrt(float((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
}
int GetDistance(int x1, int y1, int x2, int y2)
{
	return sqrt(double(abs(x1-x2)*abs(x1-x2) + abs(y1-y2)*abs(y1-y2)));
}

int GetDistance(Vector *a, Vector *b)
{
	return GetDistance(a->x, a->y, b->x, b->y);
}

int GetDistanceSquared(int x1, int y1, int x2, int y2)
{
	return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

int GetDistanceSquared(Vector *a, Vector *b)
{
	return (a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y);
}

int GetManhattanDistance(int x1, int y1, int x2, int y2)
{
	return 10*(abs(x1-x2) + abs(y1-y2));
}

Vector GetCenterPoint(std::vector<Vector> *vectors)
{
	int ysum=0;
	int xsum=0;
	for(int i=0; i < vectors->size(); i++)
	{
		ysum+= (*vectors)[i].y;
		xsum+= (*vectors)[i].x;
	}
	xsum/=vectors->size();
	ysum/=vectors->size();


	return Vector(xsum, ysum);
}

void CenterPoly(Vector cpoint, std::vector<Vector> *vectors)
{
	for(int i=0; i < vectors->size(); i++)
	{
		(*vectors)[i].y-=cpoint.y;
		(*vectors)[i].x-=cpoint.x;
	}
}

bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, 
	float p2_x, float p2_y, float p3_x, float p3_y)
{
	float s1_x, s1_y, s2_x, s2_y;
	s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
	s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

	float s, t;
	s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
	t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		/*
		// Collision detected
		if (i_x != NULL)
		*i_x = p0_x + (t * s1_x);
		if (i_y != NULL)
		*i_y = p0_y + (t * s1_y);*/
		return 1;
	}

	return 0; // No collision
}

bool ExpandLineCollision(Vector a, Vector b, Vector c, Vector d, int exp)
{
	if(get_line_intersection(a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y ))
		return true;
	if(exp!=0)
	{      
		Vector extend = a - b;
		extend = extend.Direction();
		extend = extend.perp();
		extend*=exp;

		a+=extend;
		b+=extend;

		if(get_line_intersection(a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y))
			return true;

		extend*=-2;

		a+=extend;
		b+=extend;

		if(get_line_intersection(a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y))
			return true;
	}

	return false;
}

bool polyLineCollision(CPolygon* cp, Vector a, Vector b, int exp)
{
	CPolygon updatePoly=*cp;
	updatePoly.UpdateVertices();

	for(int j=0; j < cp->v_count-1; j++)
	{
		if(ExpandLineCollision(a,b,updatePoly.vertex[j],updatePoly.vertex[j+1], exp))
			return true;
	}

	if(ExpandLineCollision(a,b,updatePoly.vertex[0],updatePoly.vertex[cp->v_count-1], exp))
		return true;

	return false;
}

void ClearNodes(CNode ***nodes)
{
	for(int i=0; i < ncols; i++)
	{
		for(int j=0; j < nrows; j++)
		{
			nodes[i][j]->active = nodes[i][j]->empty;
			nodes[i][j]->open = false;
			nodes[i][j]->parrent = 0;
			nodes[i][j]->F = 0;
			nodes[i][j]->G = 0;         
			nodes[i][j]->H = 0;
		} 
	}                
}

bool PolygonsLineCollision(std::vector<CPolygon*> *polygons, Vector a, Vector b, int exp)
{
	for(int j=0; j < polygons->size(); j++)
	{
		if(polyLineCollision((*polygons)[j],a,b,exp))
		{
			return true;
		}
	}     
	return false;
}

bool PolygonsPolyCollision(std::vector<CPolygon*> *polygons, CPolygon *p)
{
	for(int g=0; g < polygons->size(); g++)
	{
		CPolygon h=(*polygons)[g];	
		h.UpdateVertices();
		if(CollisionTest(h,*p))
			return true;
	}
	return false;
}

void DrawBall(int x,int y,int vertices, int radius)
{
	CShapeSolver *ball= new CShapeSolver(vertices, radius, NULL, x, y);
	CPolygon polly= ball->polygon;
	polly.UpdateVertices();
	polly.Draw();
	delete ball;  
}

CPolygon GenerateBallUpdatedPoly(int vertices, int radius, Vector* position)
{
	CShapeSolver *bal= new CShapeSolver(vertices, radius, NULL , position->x, position->y);
	bal->polygon->UpdateVertices();
	return *bal->polygon;   
}

CPolygon GenerateBallPoly(int vertices, int radius, Vector* position)
{
	CShapeSolver *bal= new CShapeSolver(vertices, radius, NULL , position->x, position->y);
	return *bal->polygon;   
}
void DrawLine(Vector* a, Vector *b)
{
	glBegin(GL_LINES);
	glVertex2i(a->x, a->y);
	glVertex2i(b->x, b->y);
	glEnd();         
}

int Get8WayColRowValues(int i,int *c, int *r)
{
	switch(i)
	{
	case 0: {(*c)++;return 10;}
	case 1: {(*c)++;(*r)++;return 14;}
	case 2: {(*r)++;return 10;}
	case 3: {(*r)++;(*c)--;return 14;}
	case 4: {(*c)--;return 10;}
	case 5: {(*r)--;(*c)--;return 14;}
	case 6: {(*r)--;return 10;}
	case 7: {(*r)--;(*c)++;return 14;}
	}
}

void CalculateNodeLocation(int *c, int *r, Vector *orig)
{
	*c=orig->x/xstep-1;
	*r=orig->y/ystep-1;

	if(int(orig->x)%int(xstep) > int(xstep/2))(*c)++;
	if(int(orig->y)%int(ystep) > int(ystep/2))(*r)++;     
}

void DrawLines(std::vector<Vector> *vectors)
{
	Vector *a,*b;
	for(int i=0; i < vectors->size(); i++)
	{
		if(i!=0)
			DrawLine(&(*vectors)[i-1], &(*vectors)[i]);
	}
}


