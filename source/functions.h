#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <math.h>
#include <vector>
#include "vector.h"
#include "polygon.h"
#include "node.h"

int GetDistance(int x1, int y1, int x2, int y2);
int GetDistance(Vector *a, Vector *b);
int GetDistanceSquared(int x1, int y1, int x2, int y2);
int GetDistanceSquared(Vector *a, Vector *b);
float GetfDistance(float x1, float y1, float x2, float y2);
int GetManhattanDistance(int x1, int y1, int x2, int y2);
Vector GetCenterPoint(std::vector<Vector> *vectors);
void DrawLines(std::vector<Vector> *vectors);
void CenterPoly(Vector cpoint, std::vector<Vector> *vectors);
bool get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, float p2_x, float p2_y, float p3_x, float p3_y);
bool PolygonsLineCollision(std::vector<CPolygon*> *polygons, Vector a, Vector b, int exp);
bool polyLineCollision(CPolygon* p, Vector a, Vector b, int exp);
bool ExpandLineCollision(Vector a, Vector b, Vector c, Vector d, int exp);
bool PolygonsPolyCollision(std::vector<CPolygon*> *polygons, CPolygon *p);
void ClearNodes(CNode ***nodes);
void FindPath(std::vector<Vector*> *vectors, Vector a,  Vector b);
void DrawBall(int x,int y,int vertices, int radius);
void DrawLine(Vector* a, Vector* b);
int Get8WayColRowValues(int i,int *c, int *r);
void CalculateNodeLocation(int *c, int *r, Vector *orig);
CPolygon GenerateBallUpdatedPoly(int vertices, int radius, Vector* position);
CPolygon GenerateBallPoly(int vertices, int radius, Vector* position);


#endif
