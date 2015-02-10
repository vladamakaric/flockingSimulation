#include "level.h"
#include <stdlib.h>
#include "functions.h"

CLevel::~CLevel()
{
	polygons.clear();
};

void CLevel::Draw()
{
	for(int i=0; i < polygons.size(); i++)
	{
		CPolygon g=polygons[i];
		g.UpdateVertices();
		glColor3f(0,0.6,0);
		g.Draw();
	}

	if(doneWithUserInput)
	{
        glDisable(GL_LINE_SMOOTH);
		glLineWidth(1.0);

        int x,y;

	/*	
		for(int i=0; i < ncols; i++)
		{
			glColor3f(0,1,1);
			x = xstep * (i+1);
			glBegin(GL_LINES);
			glVertex2i(x, 0);
			glVertex2i(x, SCREEN_HEIGHT);
			glEnd();
		}

		for(int i=0; i < nrows; i++)
		{
			glColor3f(0,1,1);
			y= ystep * (i+1);
			glBegin(GL_LINES);
			glVertex2i(0, y);
			glVertex2i(SCREEN_WIDTH, y);
			glEnd();
		}

		CShapeSolver *ball = new CShapeSolver(10, 5);
		
		for(int i=0; i < ncols; i++)
		{
			x = xstep * (i+1);
			for(int j=0; j < nrows; j++)
			{
				y= ystep * (j+1);
				if(!nodes[i][j]->active)
				{
					ball->polygon->position.x=x;
					ball->polygon->position.y=y;
					CPolygon b = ball->polygon;
					b.UpdateVertices();
					glColor3f(0,0,0.75);
					b.Draw();
				}
			}
		}*/
	}
	else
	{        
        glEnable(GL_LINE_SMOOTH);
		glLineWidth(2);

		glColor3f(1,0,1);
		if(tempVec.size()==1)
		{
			glEnable(GL_POINT_SMOOTH);
			glPointSize(4);
			glBegin(GL_POINTS);
			glVertex2i(tempVec[0].x, tempVec[0].y);
			glEnd();
		}
		else if(tempVec.size()>0)
		{
			for(int i=0; i < tempVec.size(); i++)
			{
				glBegin(GL_LINES);
				glVertex2i(tempVec[i].x, tempVec[i].y);

				if(i+1<tempVec.size())
					glVertex2i(tempVec[i+1].x, tempVec[i+1].y);
				else
					glVertex2i(tempVec[tempVec.size()-1].x, tempVec[tempVec.size()-1].y);

				glEnd();
			}
		}
	}	
    if(fmanager!=NULL)
    {
       fmanager->Draw();               
    }
}

void CLevel::SetUpFmanager(char *fmanfile)
{
    int pcount;
    FILE *F;
   	F=fopen(fmanfile,"r+");
	if(F==NULL)
		exit(1);  
		
	int x,y;
	fscanf(F,"%d",&pcount);
	
	fscanf(F,"%d",&x);
	fscanf(F,"%d",&y);
	fmanager = new CFlockManager(new Vector(x, y), this);
    
	for(int i=0; i < pcount; i++)
	{
    	fscanf(F,"%d",&x);
    	fscanf(F,"%d",&y);
        fmanager->flock.push_back(new CShapeSolver(10, 10,this , x, y));
    }
    
	if(gfmType!=CFlockManager::FOLLOW_NO_ONE)
	fmanager->CalcLeader();

	fclose(F);
}

CLevel::CLevel(char *polyfile, char *nodesfile)
{
	moveGraphUp=0;
	moveGraphLeft=0;
	doneWithUserInput=true;
	FILE *F;
	int poly_count;
	F=fopen(polyfile,"r+");

	if(F==NULL)
		exit(1);

	fscanf(F,"%d",&poly_count);

	CPolygon *currentPolygon;
	for(int i=0; i < poly_count; i++)
	{
		int x,y;
		int angle;
		int v_count;

		fscanf(F,"%d",&v_count);
		int *vertex_array=new int[v_count*2];

		fscanf(F,"%d",&x);
		fscanf(F,"%d",&y);
		fscanf(F,"%d",&angle);

		for(int j=0; j < v_count*2; j++)
			fscanf(F,"%d",&vertex_array[j]);

		currentPolygon = new CPolygon(v_count,vertex_array,angle,x,y);
		polygons.push_back(currentPolygon);
		delete[] vertex_array;
	}

	fclose(F);

	FILE *F1;
	F1 = fopen(nodesfile, "r+");

	if(F1==NULL)
		exit(1);

	fscanf(F1, "%d", &ncols);
	fscanf(F1, "%d", &nrows);

	nodes = new CNode**[ncols];
	for(int i=0; i < ncols; i++)
	{
		nodes[i]= new CNode*[nrows];
	}

	int x,y;
	xstep=double(double(SCREEN_WIDTH)/(double(ncols+1)));
	ystep=double(double(SCREEN_HEIGHT)/(double(nrows+1)));

	CShapeSolver *ball = new CShapeSolver(10, 10);
	bool empty=true;

	nodes = new CNode**[ncols];
	for(int i=0; i < ncols; i++)
	{
		nodes[i]= new CNode*[nrows];
	}

	for(int i=0; i < ncols; i++)
	{
		x = xstep * (i+1);
		for(int j=0; j < nrows; j++)
		{
			y= ystep * (j+1);
			ball->polygon->position.x=x;
			ball->polygon->position.y=y;
			CPolygon b=ball->polygon;
			b.UpdateVertices();
			for(int k=0; k < polygons.size(); k++)
			{
				CPolygon h=polygons[k];	
				h.UpdateVertices();

				if(CollisionTest(h,b))
				{
					empty=false;
					break;
				}
				else
					empty=true;
			}

			nodes[i][j]=new CNode(empty);
			nodes[i][j]->c=i;
			nodes[i][j]->r=j;
			nodes[i][j]->x=x;
			nodes[i][j]->y=y;

		}
	}
    SetUpFmanager("fmanager.txt");
	fclose(F1);
}

void CLevel::SetNodes()
{
    int x,y;

	CShapeSolver *ball = new CShapeSolver(10, 10);
	bool empty=true;
	clickTarget=true;

	nodes = new CNode**[ncols];
	for(int i=0; i < ncols; i++)
	{
		nodes[i]= new CNode*[nrows];
	}

	for(int i=0; i < ncols; i++)
	{
		x = xstep * (i+1);
		for(int j=0; j < nrows; j++)
		{
			y= ystep * (j+1);
			ball->polygon->position.x=x;
			ball->polygon->position.y=y;
			CPolygon b=ball->polygon;
			b.UpdateVertices();
			for(int k=0; k < polygons.size(); k++)
			{
				CPolygon h=polygons[k];	
				h.UpdateVertices();

				if(CollisionTest(h,b))
				{
					empty=false;
					break;
				}
				else
					empty=true;
			}

			nodes[i][j]=new CNode(empty);
			nodes[i][j]->c=i;
			nodes[i][j]->r=j;
			nodes[i][j]->x=x;
			nodes[i][j]->y=y;

		}
	}
}

CLevel::CLevel(char *nodesfile)
{
	moveGraphUp=0;
	moveGraphLeft=0;
    fmanager=NULL;
    clickTarget=true;
    tempVec.clear();
    
	doneWithUserInput=false;

	FILE *F1;
	F1 = fopen(nodesfile, "r+");

	if(F1==NULL)
		exit(1);

	fscanf(F1, "%d", &ncols);
	fscanf(F1, "%d", &nrows);

	xstep=double(double(SCREEN_WIDTH)/(double(ncols+1)));
	ystep=double(double(SCREEN_HEIGHT)/(double(nrows+1)));

	fclose(F1);
}

bool CLevel::HandleEvents()
{

    if( keystates[ SDLK_RIGHT ] )
    {
			moveGraphLeft+=10;
	}

    if( keystates[ SDLK_LEFT ] )
    {
			moveGraphLeft-=10;
	}

    if( keystates[ SDLK_UP ] )
    {
			moveGraphUp+=10;
	}

    if( keystates[ SDLK_DOWN ] )
    {
			moveGraphUp-=10;
	}

	while (SDL_PollEvent( &event ))
	{
		if( event.type == SDL_QUIT )	
			return true;
			
		if(!doneWithUserInput)
		{
			if( event.type == SDL_MOUSEBUTTONDOWN)
			{
				if( event.button.button == SDL_BUTTON_LEFT )
				{
					Vector v;
					v.Set(event.button.x, SCREEN_HEIGHT - event.button.y);
					tempVec.push_back(v);
				}
		
				if( event.button.button == SDL_BUTTON_RIGHT )
				{
					if(clickTarget && tempVec.size()==0)
					{
                        clickTarget=false;
						fmanager = new CFlockManager(new Vector(event.button.x, SCREEN_HEIGHT - event.button.y), this);
                    }
					else if(clickTarget==false && fmanager!=NULL)
					    fmanager->flock.push_back(new CShapeSolver(10, 10,this , event.button.x, SCREEN_HEIGHT - event.button.y));
			    }
			}

			if( event.type == SDL_KEYDOWN )
			{
                if(event.key.keysym.sym == SDLK_ESCAPE)
                {
                   exit(1);
                }
                
                if( event.key.keysym.sym == SDLK_RSHIFT )
                {
                    if( fmanager!=NULL && fmanager->flock.size()>0)
                    {
                        doneWithUserInput=true;
                    }
                }
                
				if(	event.key.keysym.sym == SDLK_RETURN )
				{
					if(tempVec.size()>2)
					{
						Vector cpoint= GetCenterPoint(&tempVec);
						CenterPoly(cpoint, &tempVec);
						polygons.push_back(new CPolygon(&tempVec, 0,cpoint.x,cpoint.y));
						tempVec.clear();
					}
					else		
						if(tempVec.size()==0 && fmanager!=NULL && fmanager->flock.size()>0)
						{
							doneWithUserInput=true;
							
							SetNodes();

							if(gfmType!=CFlockManager::FOLLOW_NO_ONE)
							fmanager->CalcLeader();

							SaveLevel("level1.txt", "fmanager.txt");
							TIME=0;
						}	
				}
			}
		}
	}

	return false;
}

void CLevel::SaveLevel(char  *file, char  *fmanagerfile)
{
	FILE *F1;
	F1 = fopen(file, "w+");

	fprintf(F1, "%d ", polygons.size());

	for(int i=0; i < polygons.size(); i++)
	{

		if(i==polygons.size()-1)
		{
			int gh=543;
		}

		fprintf(F1, "%d ", polygons[i]->v_count);
		fprintf(F1, "%d ", int(polygons[i]->position.x));
		fprintf(F1, "%d ", int(polygons[i]->position.y));
		fprintf(F1, "%d ", int(polygons[i]->angle));

		for(int j=0; j < polygons[i]->v_count; j++)
		{
			int x=polygons[i]->vertex[j].x;
			int y=polygons[i]->vertex[j].y;

			fprintf(F1, "%d ", x);
			fprintf(F1, "%d ", y);
		}
	}
	fclose(F1);


	FILE *F2;
	F2 = fopen(fmanagerfile, "w+");


	fprintf(F2, "%d ", fmanager->flock.size());

	fprintf(F2, "%d ", int(fmanager->vectarget->x));
	fprintf(F2, "%d ", int(fmanager->vectarget->y));

	for(int i=0; i < fmanager->flock.size(); i++)
	{
		fprintf(F2, "%d ", int(fmanager->flock[i]->polygon->position.x));
		fprintf(F2, "%d ", int(fmanager->flock[i]->polygon->position.y));
	}
	fclose(F2);
}


void CLevel::Update()
{
     if(fmanager!=0 && doneWithUserInput)
     fmanager->Update();   
}

void CLevel::Collision()
{

}
