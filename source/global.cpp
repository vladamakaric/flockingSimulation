#include "global.h"

extern const int SCREEN_WIDTH=1024;
extern const int SCREEN_HEIGHT=768;

double xstep;
double ystep;
int gfmType;
int ncols;
int nrows;
int TIME=0;
int REAL_TIME=0;

Uint8 *keystates = SDL_GetKeyState( NULL );

SDL_Event event;

