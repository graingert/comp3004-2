#include "pixel.h"
#include <svl/svl.h>

#define NO_FLAGS 0
#define Z_BUFFER_FLAG 1

/*

Geometric conversions and transforms.

*/

Vec4 Convert4(Vec4);
Vec4* Convert4(Vec4*, int);
void Init3D(void);
void Rotatey(float);
void Rotatex(float);
void Rotatez(float);
void Scale(float);
void Scale3(float, float, float);
void Translate(float, float, float);

/*

Intermediate Level Graphics Code

*/


void GWire2(Vec4 *, int, Vec4 *); // draws a line loop, joing back to beging
void GLine2(Vec4 *, int, Vec4 *); // draws a set of lines connected head to toe
void GPolygon2(Vec4 *, int, Vec4); // draws a polygon with constant colour
void GPolygon2(Vec4 *, int, Vec4 *); // draws a polygon with shaded colour
void GPolygon3(Vec4 *, int, Vec4 *); // draws a 3D polygon with shaded colour


/*

High Level Imediate mode constructs, 3D only.


*/
void gBegin(int);
void gVertex(Vec4);
void gEnd(void);
void gColour(int, int, int);
void gColour(Vec4);

/*

Types of objects to draw.

*/

#define gPOLYGON 1
#define gWIRE 2
#define gLINES 3

/*

Vector Colours.

*/

#define gWHITE (Vec4(255,255,255, 255))
#define gBLACK (Vec4(0,0,0, 255))
#define gRED (Vec4(255,0,0, 255))
#define gGREEN (Vec4(0,255,0, 255))
#define gBLUE (Vec4(0,0,255, 255))
#define gMAGENTA (Vec4(255,0,255, 255))
#define gCYAN (Vec4(0,255,255, 255))
#define gYELLOW (Vec4(255,255,0, 255))

/*

Z-buffer

*/
#define MAXDEPTH 1.e20;

void ZInit(void);
void ZClear(void);
void ZSet(int x, int y, Vec4 z);


