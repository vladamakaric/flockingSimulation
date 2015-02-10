#include "shapesolver.h"	
#include "collision.h"
#include <algorithm>

CShapeSolver::CShapeSolver(int v_count, int radius, CLevel *_l ,  int x, int y)
{
	prevDir=WC;
	currDir=WC;
	processed=false;
	sleep=false;
	leader=false;
	updatePath=true;
	clearable = false;
	allowUpdate=true;

	pathToLeaderLenght = 0;
	tstep = 30;
	currentNode=0;
	speed=2.5;

	totalDistanceTraveled=0;
	totalTimeTraveled=0;
	prevTime = SDL_GetTicks();
	
	l=_l;

	if(l!=NULL)
	{
		ID = l->fmanager->ID++;
		flockingType = gfmType;
	}

	polygon = new CPolygon();
	polygon->v_count=v_count;
	polygon->vertex = new Vector[v_count];

	for(int i = 0; i < v_count; i ++)
	{
		float a = 2.0f * PI * (i / (float) v_count);
		polygon->vertex[i] = Vector(cos(a), sin(a)) * radius;
	}

	polygon->vel.x=0;
	polygon->vel.y=0;
	polygon->position.x=x;
	polygon->position.y=y;
	prevPos.Set(polygon->position.x, polygon->position.y);
}

void CShapeSolver::CorrectPath()
{
	if(!PolygonsLineCollision(&l->polygons,polygon->position,*finalPath[currentNode], 5 ))
		return;

	while(true)
	{
		currentNode--;

		if(currentNode < 0)
		{
			updatePath = true;
			tstep= 1;
			break;
		}
		else
		{
			if(GetDistanceSquared(l->fmanager->vectarget, finalPath[currentNode]) > GetDistanceSquared(l->fmanager->vectarget, &polygon->position) )
			{
				updatePath = true;
				tstep= 1;
				break;
			}
			else 
			if(!PolygonsLineCollision(&l->polygons,polygon->position,*finalPath[currentNode], 5 ))
				break;
		}
	}
}

void CShapeSolver::Update()
{   
	if(TIME%30==0)
	{
		int dist=GetDistance(&prevPos, &polygon->position);
		if(dist>speed)
		{
			totalTimeTraveled+=SDL_GetTicks() - prevTime;
			totalDistanceTraveled+=dist;
		}

		prevTime = SDL_GetTicks();
		prevPos = polygon->position;
	}

	if(leader && sleep)
		return;

	if(TIME%tstep == 0 && !leader && updatePath)
	{
		if(gfmType==CFlockManager::FOLLOW_NO_ONE || (l->fmanager->flock[l->fmanager->leader]->sleep && gfmType!= CFlockManager::FOLLOW_CLOSEST))
			updatePath=false;

		if(tstep==1)
			tstep=30;

		UpdatePath();
		sleep=false;
	}

	if(!sleep && finalPath.size()>0)
	{
		Vector bvec;
		xs = abs(finalPath[currentNode]->x - polygon->position.x);
		ys = abs(finalPath[currentNode]->y - polygon->position.y);
		xsign;
		ysign;

		if(finalPath[currentNode]->x > polygon->position.x)
			xsign=1;
		else
			xsign=-1;

		if(finalPath[currentNode]->y > polygon->position.y)
			ysign=1;
		else
			ysign=-1;

		bvec.Set(xs,ys);
		bvec = bvec.Direction();
		bvec*=speed;
		bvec.x*=xsign;
		bvec.y*=ysign;

		polygon->vel.x=bvec.x;
		polygon->vel.y=bvec.y;

		CorrectPath();

		if(leader)
		{
			if(ys < 4 && xs < 4)
			{
				if(currentNode > finalPath.size()-2)
				{
					//polygon->vel.Set(0,speed);
					l->fmanager->fgrouping=true;
					sleep=true;
				}
				else
					currentNode++;    
			}
		}
		else
		{
			if( currentNode < finalPath.size()-1)
			{
				bool next = !PolygonsLineCollision(&l->polygons,polygon->position,*finalPath[currentNode+1], 5 );

				if( next )  
				{
					currentNode++;                    
				}
				else
					if(ys < 4 && xs < 4)
					{
						currentNode++;      
					}
			}
			else
			{
				if(ys < 4 && xs < 4)
				{
					if( GetDistance(polygon->position.x, polygon->position.y, l->fmanager->vectarget->x, l->fmanager->vectarget->y ) < 5)
					{
						polygon->vel.Set(0,speed);
						l->fmanager->fgrouping=true;
						//leader=true;
						//l->fmanager->leader = ID;
					}
					
					//sleep=true;
				}
			}
		}

		if(!leader)
		sleep=false;

		
	}
	else if(!leader)
	{
		Vector bvec;
		int tysign,txsign;
		int txs = abs(l->fmanager->vectarget->x - polygon->position.x);
		int tys = abs(l->fmanager->vectarget->y - polygon->position.y);

		if(l->fmanager->vectarget->x > polygon->position.x)
			txsign=1;
		else
			txsign=-1;

		if(l->fmanager->vectarget->y > polygon->position.y)
			tysign=1;
		else
			tysign=-1;

		bvec.Set(xs,ys);
		bvec = bvec.Direction();
		bvec*=speed;
		bvec.x*=xsign;
		bvec.y*=ysign;

		polygon->vel.x=bvec.x;
		polygon->vel.y=bvec.y;
		sleep=false;
	}
}

void CShapeSolver::UpdatePath()
{
	switch(flockingType)
	{
	case CFlockManager::FOLLOW_LEADER:
		FollowLeaderPath();
		break;
	case CFlockManager::FOLLOW_CLOSEST:
		FollowClosestPath();
		break;
	case CFlockManager::FOLLOW_NO_ONE:
		FollowNoOnePath();
		break;
	}
}

void CShapeSolver::FollowLeaderPath()
{
	finalPath.clear();
	l->fmanager->FindPath(&finalPath, &l->polygons, l->nodes, polygon->position , l->fmanager->flock[l->fmanager->leader]->polygon->position , &pathLenght);
	ClearNodes(l->nodes);
	currentNode=0;
	sleep=false;
}

void CShapeSolver::FollowClosestPath()
{
	int minDist=1000;
	int minIndx=-1;
	for(int i=0; i < l->fmanager->flock.size(); i++)
	{
		
		if(l->fmanager->flock[i]->ID==ID)
			continue;

		int dist= GetDistance(l->fmanager->flock[i]->polygon->position.x, l->fmanager->flock[i]->polygon->position.y
			,polygon->position.x, polygon->position.y );

		int dist2 = pathToLeaderLenght;

		int dist3 = l->fmanager->flock[i]->pathToLeaderLenght;

		if(dist3>dist2)
			continue;

		if(minDist > dist)
		{
			minDist=dist;
			minIndx=i;
		}
	}

	if(minIndx==-1)
		return;

	finalPath.clear();
	if(!l->fmanager->FindPath(&finalPath, &l->polygons, l->nodes, polygon->position , l->fmanager->flock[minIndx]->polygon->position , &pathLenght))
	{
		finalPath.clear();
		finalPath.push_back(new Vector(polygon->position.x, polygon->position.y));
		finalPath.push_back(new Vector( l->fmanager->flock[minIndx]->polygon->position.x, l->fmanager->flock[minIndx]->polygon->position.y));
	}
	ClearNodes(l->nodes);
	currentNode=0;
	sleep=false;
}

void CShapeSolver::FollowNoOnePath()
{
	finalPath.clear();
	l->fmanager->FindPath(&finalPath, &l->polygons, l->nodes, polygon->position , *l->fmanager->vectarget , &pathLenght);
	ClearNodes(l->nodes);
	currentNode=0;
	sleep=false;
}

void CShapeSolver::UpdatePosition()
{
	if(!sleep && allowUpdate)
	{
		prevDir=currDir;
		polygon->position+=polygon->vel; 
	}  
}

void CShapeSolver::Draw()
{
	CPolygon t= *polygon;
	t.UpdateVertices();

	if(!allowUpdate)
		glColor3f(1,1,0);

	if(l->fmanager->stable)
		glColor3f(1,0,0);

	t.Draw();     
}
