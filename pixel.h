/*

header for pixel library

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SDL/SDL.h"

#define WHITE (Colour(255,255,255))
#define BLACK (Colour(0,0,0))
#define RED (Colour(255,0,0))
#define GREEN (Colour(0,255,0))
#define BLUE (Colour(0,0,255))
#define MAGENTA (Colour(255,255,0))
#define CYAN (Colour(255,0,255))
#define YELLOW (Colour(0,255,255))

#ifndef HEIGHT
#define HEIGHT SCREEN_HEIGHT
#endif
#ifndef WIDTH
#define WIDTH SCREEN_WIDTH
#endif

#define FALSE 0
#define TRUE 1

#include "gp2x.h"


extern int dumpit;
extern SDL_Surface *screen;
extern Uint32 plot_colour;
extern Uint32 DefaultColour;
extern SDL_Joystick *joystick;
extern int mouse_x;
extern int mouse_y;

void swap(int *, int *);
void fswap(float *, float *);
void Lock();
void Unlock();
void SetSefaultColour(int, int, int);
void StartUp(const char *);
void ClearScreen(void);
void ShutDown(void);
void SetScreen(void);
int Colour(int, int, int); /* generate a colour value */

void Line(int, int, int, int, Uint32);
void AutoSetPixel(int, int, Uint32);
void SET16(int, int, Uint32);
void SET32(int, int, Uint32);
void Circle(int, int, int, Uint32);
int Clip(int , int , int , int , int *, int *, int *, int *);
void fFillPoly(int *, int *, int, Uint32);
void test_cross(void);



