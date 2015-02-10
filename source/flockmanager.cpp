#include "flockmanager.h"	
#include "circle.h"
#include <algorithm>
#include <functional>

std::vector<Vector*> CFlockManager::nodesPath(0);
std::vector<Vector*> CFlockManager::nodesTracePath(0);
std::vector<Vector> CFlockManager::graph(0);
std::vector<CNode*> CFlockManager::eightway(0);

bool DataSort1(const CShapeSolver* d1, const CShapeSolver* d2)
{
	return d1->pathLenght < d2->pathLenght;
}

bool DataSort2(const CShapeSolver* d1, const CShapeSolver* d2)
{
	return d1->pathLenght > d2->pathLenght;
}

void CFlockManager::DrawPaths()
{
	if(level->doneWithUserInput)
	{    
		for(int i=0; i < flock.size(); i++)
		{
			for(int j=0; j < flock[i]->finalPath.size(); j++)
			{
				glColor3f(0.5,1,1);

				if(i==leader)
					glColor3f(1,0,1);

				DrawBall(flock[i]->finalPath[j]->x, flock[i]->finalPath[j]->y,10, 5);

				if(j!=0)
					DrawLine(flock[i]->finalPath[j-1], flock[i]->finalPath[j]);
			}
		}
	}    
}

void CFlockManager::Draw()
{
	for(int i=0; i < flock.size(); i++)
	{       
		glColor3f(1,0,1); 

		if(level->doneWithUserInput)
			if(leader == i)
				glColor3f(0,1,0);

		flock[i]->Draw();
	}     

	glColor3f(0,0,1);
	DrawBall(vectarget->x, vectarget->y,10, 10);
	DrawPaths();

	glColor3f(1,1,0);

	if(fgrouping)
	{
		if(graph.size() > 0)
		{
			glTranslatef(level->moveGraphLeft,level->moveGraphUp-graph[graph.size()-1].y+400,0);
			glLineWidth(1);
			DrawLines(&graph);
			glLoadIdentity();
		}
	}
}

void CFlockManager::Get8Way(CNode ***nodes, CNode* n, Vector *orig=NULL)
{ 	
	int c= n->c;
	int r= n->r;
	int G;

	Vector a,b;

	for(int i=0; i < 8; i++)
	{
		c = n->c;
		r = n->r; 
		G=Get8WayColRowValues(i,&c,&r);

		if( (c<0 || c >= ncols || r < 0 || r >= nrows) || !nodes[c][r]->active)
			continue;

		b.Set(nodes[c][r]->x, nodes[c][r]->y);

		if(orig!=NULL) 
			if(PolygonsLineCollision(&level->polygons,*orig,b,5))
				continue;

		a.Set(n->x, n->y);

		CPolygon tempP = GenerateBallUpdatedPoly(10, 10, &b);
		if(PolygonsPolyCollision(&level->polygons, &tempP) ) 
			continue;

		if(PolygonsLineCollision(&level->polygons,a,b,10))
			continue;

		if(!nodes[c][r]->open)
		{
			nodes[c][r]->open=true;
			nodes[c][r]->parrent=n;
			nodes[c][r]->G= n->G + G;
		}
		else
		{
			int g= n->G + G;
			if(nodes[c][r]->G > g)
			{
				nodes[c][r]->parrent=n;
				nodes[c][r]->F=0;
			}
		}
		eightway.push_back(nodes[c][r]);
	}
}

bool CFlockManager::StabilityEvaluation(int offset, int range)
{
	std::vector<int> eval;
	int val;

	if(graph.size()<range)
		return false;

	for(int i=1; i < range+1; i++)
	{
		eval.push_back(graph[graph.size()-i].y);
	}

	for(int i=0; i < eval.size(); i++)
	{
		for(int j=0; j < eval.size(); j++)
		{
			if(i!=j)
			{
				if(abs(eval[i]-eval[j]) > offset )
					return false;
			}
		}
	}
	return true;
}

void CFlockManager::Calculate8WayValues(int c2 , int r2)
{
	for(int i=0; i <eightway.size(); i++)
	{
		if(eightway[i]->F==0)
		{
			int h= GetManhattanDistance(eightway[i]->c,eightway[i]->r,c2,r2);
			eightway[i]->H=h;
			eightway[i]->F= eightway[i]->G + h;
		}	
	}
}

void CFlockManager::CalcLeader()
{
	for(int i=0; i < flock.size(); i++)
	{
		FindPath(&flock[i]->finalPath, &level->polygons, level->nodes, flock[i]->polygon->position , *vectarget, &flock[i]->pathLenght);
		ClearNodes(level->nodes);
		flock[i]->leader=false;
	}

	int min=flock[0]->pathLenght;
	int dist;
	int indx=0;

	for(int i=1; i < flock.size(); i++)
	{
		dist = flock[i]->pathLenght;
		if(min > dist)
		{
			min=dist;
			indx=i;       
		}
	}

	leader=indx;
	flock[leader]->leader=true;
	flock[leader]->finalPath.clear();
	FindPath(&flock[leader]->finalPath, &level->polygons, level->nodes, flock[leader]->polygon->position , *vectarget, &flock[leader]->pathLenght);
	ClearNodes(level->nodes);

	for(int i=0; i < flock.size(); i++)
	{
		if(i!=leader)
		{            
			flock[i]->finalPath.clear();
			FindPath(&flock[i]->finalPath, &level->polygons, level->nodes, flock[i]->polygon->position , flock[leader]->polygon->position, &flock[i]->pathLenght);
			ClearNodes(level->nodes);
			flock[i]->pathToLeaderLenght = flock[i]->pathLenght;
		}
	}
}

int CFlockManager::FindPathLenght(std::vector<CPolygon*> *polygons , CNode ***nodes, Vector a,  Vector b)
{
	eightway.clear();

	int c,r,c2,r2;

	CalculateNodeLocation(&c, &r, &a);
	CalculateNodeLocation(&c2, &r2, &b);

	if(!ExpandNodePosition(&c, &r, &a) || !ExpandNodePosition(&c2, &r2, &b))
		return -1;

	eightway.clear();
	ClearNodes(nodes);

	CNode *currentNode=nodes[c][r];

	while(true)
	{
		currentNode->active=false;
		Get8Way(nodes, currentNode);
		Calculate8WayValues(c2 ,r2);

		if(eightway.size()==0)
			return -1;

		int min=100000;
		int indx=-1;
		for(int i=0; i < eightway.size(); i++)
		{
			if(!eightway[i]->active)
			{
				eightway.erase(eightway.begin() + i);
				i--;
				continue;
			}

			if(eightway[i]->F < min && eightway[i]->active)
			{
				indx=i;
				min=eightway[i]->F;
			}
		}

		if(indx!=-1 && indx < eightway.size())
			currentNode=eightway[indx];
		else
			return -1;

		if(currentNode->c==c2 && currentNode->r==r2)
			return currentNode->G;
	}
}

bool CFlockManager::FindPath(std::vector<Vector*> *finalPath, std::vector<CPolygon*> *polygons , CNode ***nodes, Vector a,  Vector b, int *plenght)
{
	nodesTracePath.clear();
	nodesPath.clear();
	eightway.clear();

	int c,r,c2,r2;

	CalculateNodeLocation(&c, &r, &a);
	CalculateNodeLocation(&c2, &r2, &b);

	if(!ExpandNodePosition(&c, &r, &a) || !ExpandNodePosition(&c2, &r2, &b))
		return false;

	eightway.clear();
	ClearNodes(nodes);

	CNode *currentNode=nodes[c][r];
	Vector* v=new Vector(currentNode->x,currentNode->y);
	CFlockManager::nodesPath.push_back(v);

	while(true)
	{
		currentNode->active=false;
		Get8Way(nodes, currentNode);
		Calculate8WayValues(c2 ,r2);

		if(eightway.size()==0)
			return false;

		int min=100000;
		int indx=-1;
		for(int i=0; i < eightway.size(); i++)
		{
			if(!eightway[i]->active)
			{
				eightway.erase(eightway.begin() + i);
				i--;
				continue;
			}

			if(eightway[i]->F < min && eightway[i]->active)
			{
				indx=i;
				min=eightway[i]->F;
			}
		}

		if(indx!=-1 && indx < eightway.size())
			currentNode=eightway[indx];
		else
			return false;

		Vector* v=new Vector(currentNode->x,currentNode->y);
		nodesPath.push_back(v);

		if(currentNode->c==c2 && currentNode->r==r2)
		{
			*plenght = currentNode->G;
			while(true)
			{

				Vector* v=new Vector(currentNode->x,currentNode->y);
				nodesTracePath.push_back(v);

				if(currentNode->parrent==NULL)
				{
					std::vector<Vector*>::iterator vstart, vend;
					vstart=nodesTracePath.begin();
					vend = nodesTracePath.end();
					reverse(vstart, vend);
					break;
				}
				currentNode=currentNode->parrent;
			}

			*finalPath = nodesTracePath;
			finalPath->insert(finalPath->begin(), new Vector(a.x, a.y));

			int a1=0; int b1=0;
			for(int i=0; i < finalPath->size(); i++)
			{
				b1=i+1;
				if(i!=0 && i!=finalPath->size()-1 && !PolygonsLineCollision(&level->polygons, *(*finalPath)[a1], *(*finalPath)[b1],5))
				{
					finalPath->erase(finalPath->begin()+i);
					i--;
				}
				else if(i!=0 && i!=finalPath->size()-1)
					a1=i;
			}

			Vector g(b.x, b.y);
			if(!PolygonsLineCollision( &level->polygons, *(*finalPath)[finalPath->size()-2], g,5))
			{
				finalPath->erase(finalPath->begin()+finalPath->size()-1);
			}

			finalPath->push_back(new Vector(b.x, b.y));
			break;
		}
	}

	nodesTracePath.clear();
	nodesPath.clear();
	eightway.clear();
	//*plenght = GetFinalPathLenght(finalPath);
	return true;
}

int CFlockManager::GetFinalPathLenght(std::vector<Vector*> *path)
{
	int lenght=0;

	if(path->size()==0)
		return -1;

	Vector a,b;
	a = *(*path)[0];

	for(int i=0; i < path->size(); i++)
	{   
		if(i!=path->size()-1)
			b=*(*path)[i+1];
		a=*(*path)[i];

		lenght+=GetDistanceSquared(a.x, a.y, b.x, b.y);
	}

	return lenght;
}

void CFlockManager::Update()
{
	/////////////////////////////////////////////////////////

	if(fgrouping)
	{
		if(stable)
		{
			graphcount+=3;
			graph.push_back(Vector(graphcount,graph[graph.size()-1].y));
		}

		if(fgrouping && TIME%2==0 && !stable)
		{
			float val=GetCurrentFlockDistance();

			val= val/flock.size()*30;
			graph.push_back(Vector(graphcount,val));
			graphcount+=3;

			if(StabilityEvaluation(40, 40))
			{
				stable=true;
				CalculateStats();
			}
		}
	}

	if(!stable)
	{


		if(TIME%30==0 && gfmType==FOLLOW_CLOSEST)
		{
			for(int i=0; i < flock.size(); i++)
			{
				if(i!=leader)
				{            
					flock[i]->finalPath.clear();
					flock[i]->pathToLeaderLenght = FindPathLenght(&level->polygons, level->nodes, flock[i]->polygon->position , flock[leader]->polygon->position);
					ClearNodes(level->nodes);
				}
			}
		}

		for(int i=0; i < flock.size(); i++)
		{  
			flock[i]->Update();      
		}

		//PrintSolverStats();

		for(int i=0; i < flock.size(); i++)
		{
			if(flock[i]->leader)
				continue;

			flock[i]->allowUpdate=-45;
		}
		
		ClearFlockPermissions();
		for(int i=0; i < flock.size(); i++)
		{   
			if(flock[i]->leader)
				continue;

			flock[i]->allowUpdate = ExpandCollision(flock[i], -1);
			ClearFlockClearability();
			ClearFlockPermissions();

			for(int i=0; i < flock.size(); i++)
			{  
				if(!flock[i]->leader)
					flock[i]->allowUpdate = Solve3Way(flock[i]);
			}
		}


		for(int i=0; i < flock.size(); i++)
		{  
			flock[i]->UpdatePosition();      
		}
	}
}

bool CFlockManager::PolyFlockCollision(CPolygon *a)
{
	for(int i=0; i < flock.size(); i++)
	{       
		CPolygon fp=*flock[i]->polygon;
		fp.UpdateVertices();
		CPolygon h=*a;
		if(CollisionTest(h,fp))
			return true;
	}
	return false;
}

bool CFlockManager::LineFlockCollision(Vector a, Vector b, int exp, int tindx, int _ID)
{
	for(int i=0; i < flock.size(); i++)
	{
		if(flock[i]->ID==_ID || i==tindx || flock[i]->leader)
			continue;

		CPolygon fp=*flock[i]->polygon;
		if(polyLineCollision(&fp, a, b, exp))
			return true;
	}
	return false;
}

bool CFlockManager::ExpandNodePosition(int *c, int *r, Vector *orig)
{
	Vector nvec;
	if(level->nodes[*c][*r]->active)
		return true;

	for(int i=0; i < 8; i++)
	{
		Get8WayColRowValues(i,c,r);

		if(*c<0 || *c >= ncols || *r < 0 || *r >= nrows)
			continue;

		nvec.Set(level->nodes[*c][*r]->x, level->nodes[*c][*r]->y);

		CPolygon tempP = GenerateBallUpdatedPoly(10, 10, &nvec);
		if(level->nodes[*c][*r]->active &&
		!PolygonsLineCollision(&level->polygons,*orig,nvec,5) && 
		!PolygonsPolyCollision(&level->polygons, &tempP ))
			return true;

		Get8Way(level->nodes, level->nodes[*c][*r], orig);

		if(eightway.size()>0)
		{
			*c=eightway[0]->c;
			*r=eightway[0]->r;
			eightway.clear();
			return true;
		}
		eightway.clear();

		CalculateNodeLocation(c,r,orig);
	}
	return false;
}

bool CFlockManager::CrossablePath(std::vector<Vector*> *path, int exp, int tindx, int _ID)
{
	if(path->size()==0)
		return false;

	Vector a,b;
	a = *(*path)[0];

	for(int i=0; i < path->size(); i++)
	{   
		if(i!=path->size()-1)
			b=*(*path)[i+1];
		a=*(*path)[i];

		if(LineFlockCollision(a,b,exp,tindx, _ID))
		{
			return false;
		}   
	}
	return true;
}

float CFlockManager::GetCurrentFlockDistance()
{
	float totaldist=0;
	for(int i=0; i < flock.size(); i++)
	{
		if(flock[i]->leader)
			continue;

		float dist = GetfDistance(vectarget->x ,vectarget->y, flock[i]->polygon->position.x ,flock[i]->polygon->position.y);
		totaldist+=dist;
	}
	return totaldist;
}

void CFlockManager::ClearFlockPermissions()
{
	for(int i=0; i < flock.size(); i++)
	{
		flock[i]->processed=false;
	}
}

void CFlockManager::ClearFlockClearability()
{
	for(int i=0; i < flock.size(); i++)
	{
		flock[i]->clearable=false;
	}
}

void CFlockManager::ClearUnallowed()
{
	for(int i=0; i < flock.size(); i++)
	{
		if(flock[i]->clearable)
			flock[i]->processed = false;
	}
}

bool CFlockManager::Solve3Way(CShapeSolver* ss)
{
	Vector curVel = ss->polygon->vel;

	if(IsGoodVel(ss, curVel))
	{
		ss->processed=true;
		ss->currDir=CShapeSolver::WC;
		return true;
	}

	bool vel1b=false;
	bool vel2b=false;
	Vector vel1=curVel.perp();
	Vector vel2=-vel1;

	if(ss->prevDir==CShapeSolver::WC)
	{
		if(IsGoodVel(ss,vel1) && IsGoodVel(ss,vel2))
		{
			if(rand()%2==0)
			{
				ss->polygon->vel=vel1;
				ss->currDir=CShapeSolver::LEFT;
			}
			else
			{
				ss->polygon->vel=vel2;
				ss->currDir=CShapeSolver::RIGHT;
			}

			ss->processed=true;
			return true;
		}

		if(IsGoodVel(ss,vel1))
		{
			ss->polygon->vel=vel1;
			ss->processed=true;
			ss->currDir=CShapeSolver::LEFT;
			return true;
		}

		if(IsGoodVel(ss,vel2))
		{
			ss->polygon->vel=vel2;
			ss->processed=true;
			ss->currDir=CShapeSolver::RIGHT;
			return true;
		}
	}


	if(ss->prevDir==CShapeSolver::LEFT)
	{
		if(IsGoodVel(ss,vel1))
		{
			ss->polygon->vel=vel1;
			ss->processed=true;
			ss->currDir=CShapeSolver::LEFT;
			return true;
		}

		if(IsGoodVel(ss,vel2))
		{
			ss->polygon->vel=vel2;
			ss->processed=true;
			ss->currDir=CShapeSolver::RIGHT;
			return true;
		}
	}

	if(ss->prevDir==CShapeSolver::RIGHT)
	{
		if(IsGoodVel(ss,vel2))
		{
			ss->polygon->vel=vel2;
			ss->processed=true;
			ss->currDir=CShapeSolver::RIGHT;
			return true;
		}

		if(IsGoodVel(ss,vel1))
		{
			ss->polygon->vel=vel1;
			ss->processed=true;
			ss->currDir=CShapeSolver::LEFT;
			return true;
		}
	}

	ss->processed=true;
	return false;
}

bool CFlockManager::ShapeSolverHitWall(CShapeSolver* ss)
{
	CPolygon q=ss->polygon;
	q.position+=ss->polygon->vel;
	q.UpdateVertices();

	return PolygonsPolyCollision(&level->polygons, &q);
}

void clearChildrenPermissions(std::vector<CShapeSolver*> *children)
{
	for(int i=0; i < children->size(); i++)
	{
		//if(!(*children)[i]->allowUpdate)
		(*children)[i]->allowUpdate = false;
		(*children)[i]->processed = false;
	}
}

bool CFlockManager::RecursiveIsGoodVel(CShapeSolver* ss, int ID)
{
	std::vector<CShapeSolver*> children;
	bool velFlag = GetChildren(&children, ss, ID) && !ShapeSolverHitWall(ss);

	if(velFlag)
	{
		for(int i=0; i < children.size(); i++)
		{
			children[i]->allowUpdate = ExpandCollision(children[i], ss->ID);

			if(!children[i]->allowUpdate)
			{
				velFlag = false;
			}
		}

		if(!velFlag)
		{
			clearChildrenPermissions(&children);	
		}
	}

	if(!velFlag)
		ClearUnallowed();

	return velFlag;
}

bool CFlockManager::ExpandCollision(CShapeSolver* ss, int ID)
{
	///////////////////////////////////////////////////////////////
	if(Solve3Way(ss)) return true;
	Vector formerVel = ss->polygon->vel;

	Vector vel1 = ss->polygon->vel.perp();
	Vector vel2 = -vel1;
	Vector vel3 = -ss->polygon->vel;

	int randnum= rand()%2;

	if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::WC; return true;}

	if(ss->prevDir==CShapeSolver::WC)
	{
		if(randnum==0)
		{
			ss->polygon->vel = vel1;
			if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::LEFT; return true;}

			ss->polygon->vel = vel2;
			if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::RIGHT; return true; }
		}
		else
		{
			ss->polygon->vel = vel2;
			if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::RIGHT; return true; }

			ss->polygon->vel = vel1;
			if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::LEFT; return true;}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
	if(ss->prevDir==CShapeSolver::LEFT)
	{
		ss->polygon->vel = vel1;
		if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::LEFT; return true;}

		ss->polygon->vel = vel2;
		if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::RIGHT; return true; }
	}

	if(ss->prevDir==CShapeSolver::RIGHT)
	{
		ss->polygon->vel = vel2;
		if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::RIGHT; return true; }

		ss->polygon->vel = vel1;
		if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::LEFT; return true;}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	ss->polygon->vel = vel3;
	if(RecursiveIsGoodVel(ss, ID)) { ss->currDir=CShapeSolver::WC; return true;}

	ss->polygon->vel= formerVel;

	return false;
}

bool CFlockManager::IsGoodVel(CShapeSolver *ss, Vector tvel)
{
	CCircle test(ss->polygon->position + tvel, 20);

	CPolygon p=ss->polygon;
	p.position+=tvel;
	p.UpdateVertices();

	if(PolygonsPolyCollision(&level->polygons, &p))
		return false;   

	for(int i=0; i < flock.size(); i++)
	{
		if(i==ss->ID)
			continue;

		CCircle flockmember;

		if(flock[i]->allowUpdate!=-45 && !flock[i]->allowUpdate)
			flockmember = CCircle(flock[i]->polygon->position, 20);
		else
			flockmember = CCircle(flock[i]->polygon->position + flock[i]->polygon->vel, 20);

		if(CircleCollision(&test, &flockmember))
		{
			return false;
		}
	}

	return true;
}

bool CFlockManager::GetChildren(std::vector<CShapeSolver*> *children, CShapeSolver* ss, int ID)
{
	CCircle parrent(ss->polygon->position + ss->polygon->vel ,20);

	for(int i=0; i < flock.size(); i++)
	{
		if(i==ss->ID)
			continue;

		CCircle flockmember;

		if(flock[i]->allowUpdate!=-45 && !flock[i]->allowUpdate)
			flockmember = CCircle(flock[i]->polygon->position, 20);
		else
			flockmember = CCircle(flock[i]->polygon->position + flock[i]->polygon->vel, 20);

		if(CircleCollision(&parrent,&flockmember))
		{
			if(!flock[i]->processed && !flock[i]->leader && i!=ID && !flock[i]->sleep) 
			{
				flock[i]->processed = true;
				flock[i]->clearable = true;
				children->push_back(flock[i]);
			}
			else
				return false;
		}
	}

	return true;
}


void CFlockManager::PrintSolverStats()
{
	for(int i=0; i < flock.size(); i++)
	{	
		printf("BR %d:predjena razdaljina i vreme: %d, %d \n", i, flock[i]->totalDistanceTraveled,  flock[i]->totalTimeTraveled/1000);
	}
	printf("////////////////////////////////////////\n");
}
void CFlockManager::CalculateStats()
{
	int totalDistanceTraveled=0;
	int totalTimeTraveled=0;
	int averageDistanceTraveled=0;
	int averageTimeTraveled=0;

	for(int i=0; i < flock.size(); i++)
	{
		totalDistanceTraveled+=flock[i]->totalDistanceTraveled;
		totalTimeTraveled+=flock[i]->totalTimeTraveled;
	}

	averageDistanceTraveled = totalDistanceTraveled/ flock.size();
	averageTimeTraveled = totalTimeTraveled/ flock.size();

	printf("\n\n/////////STATISTIKA CELOG JATA//////////\n");
	printf("ukupna predjena razdaljina: %d \n", totalDistanceTraveled);
	printf("prosecna predjena razdaljina: %d \n", averageDistanceTraveled);
	printf("ukupno vreme kretanja: %d \n", totalTimeTraveled/1000);
	printf("prosecno vreme kretanja: %d \n", averageTimeTraveled/1000);
}
