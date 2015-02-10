#ifndef POLYGON_H
#define POLYGON_H

#include "global.h"
#include "object.h"
#include "vector.h"
#include <stdlib.h>
#include <vector>

class CPolygon : public CObject
{
public:
	CPolygon(){};
	CPolygon(int n,int *i,int _angle, int x,int y);
	CPolygon(std::vector<Vector> *vectors ,int _angle, int x,int y);
	CPolygon(const CPolygon* p);
	CPolygon(const CPolygon& p);
	void Draw();
	void UpdateVertices(const Vector  &position, float angle);
	void UpdateVertices();
	void Update(){position+=vel;};
	void Logic(){};

	bool colided;
};

#endif

