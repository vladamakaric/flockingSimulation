#ifndef FLOCK_MANAGER
#define FLOCK_MANAGER



#include "shapesolver.h"
#include "functions.h"
#include "node.h"
#include <vector>

void clearChildrenPermissions(std::vector<CShapeSolver*> *children);

class CLevel;
class CFlockManager
{
public:
    CFlockManager(Vector *_target, CLevel* l) : graphcount(0), vectarget(_target), ID(0), fgrouping(false), stable(false), level(l) {};

	enum{FOLLOW_LEADER, FOLLOW_CLOSEST, FOLLOW_NO_ONE};
    int leader;
    int ID;
	int graphcount;
	bool stable;
    bool fgrouping;
    Vector *vectarget;
	CLevel* level;
	std::vector<CShapeSolver*> flock;

    void Draw();
    void Update();
    void CalcLeader();
    void DrawPaths();
	void CalculateStats();
	void PrintSolverStats();
	bool StabilityEvaluation(int offset, int range);
    bool PolyFlockCollision(CPolygon *a);
    bool LineFlockCollision(Vector a, Vector b, int exp, int tindx, int ID);
    bool CrossablePath(std::vector<Vector*> *path, int exp, int tindx, int ID);
    bool ExpandNodePosition(int *c, int *r, Vector *orig);
	int GetFinalPathLenght(std::vector<Vector*> *finalPath);
	float GetCurrentFlockDistance();
	static std::vector<Vector> graph;
	static std::vector<Vector*> nodesPath;
	static std::vector<Vector*> nodesTracePath;
	static std::vector<CNode*> eightway;

	bool RecursiveIsGoodVel(CShapeSolver* ss, int ID);
	bool GetChildren(std::vector<CShapeSolver*> *children, CShapeSolver* ss, int ID);
	bool ExpandCollision(CShapeSolver* ss, int ID);
	bool IsGoodVel(CShapeSolver *ss, Vector vel);
	bool Solve3Way(CShapeSolver* ss);
	bool ShapeSolverHitWall(CShapeSolver* ss);
	void ClearFlockPermissions();
	void ClearFlockClearability();
	void ClearUnallowed();

	void Get8Way(CNode ***nodes, CNode* n, Vector *orig);
	void Calculate8WayValues(int c2 , int r2);
    bool FindPath(std::vector<Vector*> *finalPath, std::vector<CPolygon*> *polygons , CNode ***nodes, Vector a,  Vector b, int *plenght);
	int FindPathLenght(std::vector<CPolygon*> *polygons, CNode ***nodes, Vector a,  Vector b);
};
#include "level.h"

#endif
