
#ifndef LEVEL_H
#define LEVEL_H



#include "node.h"
#include "polygon.h"
#include "global.h"

#include "collision.h"
#include <vector>

class CFlockManager;
class CShapeSolver;
class CLevel
{
public:
	
	CLevel(char *file, char *nodesfile);
	CLevel(char *nodesfile);
	void SaveLevel(char *file, char *nodesfile);
	void Collision();
	void Draw();
	~CLevel();
	void Update();
	void SetNodes();
	void SetUpFmanager(char *fmanfile);
	bool HandleEvents();

    bool clickTarget;
	int moveGraphLeft;
	int moveGraphUp;
	CFlockManager *fmanager;
	CNode ***nodes;
	std::vector<CPolygon*> polygons;
	std::vector<Vector> tempVec; 
	Vector t;

	bool doneWithUserInput;
};
#include "flockmanager.h"

#endif


