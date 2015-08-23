#include <math.h>
#include "polygon.h"
#include "level.h"
#include "global.h"
#include "flockmanager.h"
#include <iostream>
#include <stdio.h>

bool init_GL()
{
	int error;
	SDL_Surface* drawContext;
	error = SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_WM_SetCaption("Flocking simulation",NULL);
	Uint32 flags;
	flags = SDL_OPENGL | SDL_HWSURFACE;
	drawContext = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,32 ,flags);

	glShadeModel(GL_SMOOTH);									
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);	
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);							
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	glEnable(GL_POLYGON_SMOOTH);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT,-500,600);

	glMatrixMode(GL_MODELVIEW);
	REAL_TIME=SDL_GetTicks();
	return true;
}

int main( int argc, char *argv[] )
{
	printf("\n**************************************************");
	printf("\nThis is a flocking simulation.");
	printf("\nIt simulates a collection of entities moving towards a goal position.");
	printf("\nThe entities are moving through a \"terrain\" with obstacles.");
	printf("\nThe movement of the entire group as a whole is called flocking (e.g. flock of birds).");
	printf("\n--------------Vladimir Makaric 2010---------------");
	printf("\n**************************************************");

	printf("\n*******Simulation parameters**********************");
	printf("\nChoose flocking algorithm: \n");
	printf("\nenter 0 if you want all balls to follow the leader");
	printf("\nenter 1 if you want every ball to follow its nearest neighbor");
	printf("\nenter 2 if you want every ball to move autonomously\n");
	scanf("%d", &gfmType);

	printf("\nenter 0 if you want to create a new simulation");
	printf("\nenter 1 if you want to load the previous one you created\n");
	int simul;
	scanf("%d", &simul);

	CLevel *l1;

	printf("\nEvery simulation needs a nodex.txt file which specifies the");
	printf("\ndimensions of the matrix according to which the entites will calculate");
	printf("\ntrajectories using the A* search algorithm, the default is 30 30");

	printf("\nNow you will enter the simulation, and if you selected to create a new ona");
			printf("\nyou will get a blank window. First you must define convex polygons to act as");
			printf("\nobstacles you define them by consecutive left clicks, when you finish one polygon");
			printf("\npress ENTER and	start making another one, when you are done press the right mouse"); 
			printf("\nbutton to specify the goal position for the flock. Next right click to place entities");
			printf("\nwhen you are done press ENTER to run the simulation.");

	printf("\n**********************************\nPress ENTER to launch simulation!");

	getchar();
	getchar();

	if(simul==1)
		l1=new CLevel("level1.txt", "nodes.txt");
	else
		l1=new CLevel("nodes.txt");
	bool quit = false;

	if( init_GL() == false )
		return 1;

	while(1)
	{
		if(l1->HandleEvents())
		{
			delete l1;
			break;
		}

        l1->Update();
		l1->Collision();
		glColor3f(0.7, 0.5, 0.8);
		glClear(GL_COLOR_BUFFER_BIT);

		l1->Draw();
		glLoadIdentity();
		SDL_GL_SwapBuffers();
		TIME++;
	}

	SDL_Quit();
	return 0;
}

