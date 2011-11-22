#include "gPackage.h"

static int mode = 0;    //Type of object to draw.
static int cnt = 0;     //Number of vertices
static Vec4 hue;        //Current colour.
/*
 * Arrays to hold vrtex properties.
 * Allow up to 20 vertices in a polygon
 */
static Vec4 p[20];
static Vec4 colour[20];

void gBegin(int type) { // set type and vertex count to zero
    mode = type;
    cnt = 0;
}

void gColour(int r, int g, int b) { // set current colour
    hue = Vec4(r, g, b, 1.);;
}
void gColour(Vec4 c) { //set current colour
    hue = c;
}
void gColour(Vec3 c) { //set current colour
    hue = Vec4(c[0], c[1], c[2], 1.);
}

void gVertex(Vec4 x) { // add a new vertex, set its colour to current value.
    float z = x[3];
    p[cnt] = Convert4(x); // convert to (x, y)
    p[cnt][3] = z; // restore origional z value.
    colour[cnt] = hue;
    cnt++;
}

void gEnd(void) { // draw current graphics object.
    switch (mode) {
    case gWIRE:
        GWire2(p, cnt, colour);
        break;
    case gLINES:
        GLine2(p, cnt, colour);
        break;
    case gPOLYGON:
        GPolygon3(p, cnt, colour);
        break;
    default:
//            This is an error
        break;
    }
    mode = 0;
    cnt = 0;
}



