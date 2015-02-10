#ifndef OBJECT_H
#define OBJECT_H

#include "vector.h"
class CObject
{
public:
	virtual void Draw()=0;
	virtual void Logic()=0;
	virtual void Update()=0;

	Vector position;
	Vector *vertex;
	Vector vel;

    int TYPE;
	float angle;
	int v_count;
	float v_angle;
};

#endif
