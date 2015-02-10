#ifndef CIRCLE_H
#define CIRCLE_H

#include "vector.h"

class CCircle
{
public:
	CCircle() {}
   CCircle(Vector _p, int r) : position(_p), radius(r) {}
   Vector position;
   int radius;
};

#endif