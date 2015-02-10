#ifndef SHAPE_SOLVER_H
#define SHAPE_SOLVER_H

#include "polygon.h"
#include <math.h>
#include <vector>

class CLevel;
class CShapeSolver
{
public:
	enum{LEFT,RIGHT,WC};
    std::vector<Vector*> finalPath;
    CLevel* l;
	CPolygon *polygon;
	Vector prevPos;

	int pathToLeaderLenght;
	int prevDir;
	int currDir;
	int flockingType;
    int ID;
    int tstep;
    int pathLenght;
	int allowUpdate;
    int xsign, ysign, xs, ys;
	int currentNode;
	int totalDistanceTraveled;
	int totalTimeTraveled;
	int prevTime;

    bool leader;
	bool clearable; 
	bool processed;
	bool updatePath;
	bool sleep;
    float speed;

	CShapeSolver(int v_count, int radius,CLevel *_l=NULL,  int x=0, int y=0);
	void CorrectPath();
	void UpdatePath();
	void FollowLeaderPath();
	void FollowClosestPath();
	void FollowNoOnePath();
	void UpdatePosition();
	void Update();
	void Draw();
};
#include "level.h"

#endif
