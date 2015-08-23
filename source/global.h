#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef __linux__
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"
#elif defined _WIN32 || defined _WIN64
#include "SDL.h"
#include "SDL_opengl.h"
#else
#error "unknown platform"
#endif
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern int ncols;
extern int nrows;
extern int TIME;
extern int REAL_TIME;
extern int gfmType;

extern double xstep;
extern double ystep;

extern Uint8 *keystates;

extern SDL_Event event;

#endif

