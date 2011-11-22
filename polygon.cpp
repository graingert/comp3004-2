#include "gPackage.h"
#include <math.h>

/* Polygon drawing code, based on a modified line drawing algorithm.
 *
 * Vertices are specified by a 4 vector
 *
 * It uses a 4 vector to hold colour and floating point arithmetic to do the Colour
 * interpolation.
 *
 * One obvious improvement would be to use integer arithmetic.
 *
 * However this must be done with care as it is easy to get a step of less than 1.
 */

/*

Low level Code.

*/

/*

 Global variables.

*/


Vec4 c1[HEIGHT], c2[HEIGHT];  // Edge Colour table.

int EArray[HEIGHT][3]; //THE EDGE TABLE, COUNT, X1 X2.

/*

Helper Function

*/

int vcolour(Vec4 c) { // converts a 4 vector to a colour.
    return Colour((int)(c[0]), (int)(c[1]), (int)(c[2]));
}

int vcolour(Vec3 c) { // converts a 4 vector to a colour.
    return Colour((int)(c[0]), (int)(c[1]), (int)(c[2]));
}

/*
 *
 * Render a 2d polygon with constant colour.
 *
 */

void EAClear() { //clear the edge table
    int i;
    for (i=0;i<HEIGHT;i++) {
        EArray[i][0] = 0;
    }
}
void EASet(int x, int y, int *pigment) { //add to the edge table
    switch (EArray[y][0]) {
    case 0:
        EArray[y][0] = 1;
        EArray[y][1] = x;
        return;
    case 1:
        EArray[y][0] = 2;
        EArray[y][2] = x;
        if (EArray[y][1] > EArray[y][2]) { // fix order on 2nd insertion, to keep start, stop.
            swap(&EArray[y][1], &EArray[y][2]);
        }
        return;
    case 2:
        if ( x < EArray[y][1]) {
            EArray[y][1] = x;
        } else if ( x > EArray[y][2]) {
            EArray[y][2] = x;
        }
        return;
    default:
        return;
    }
}
void EdgeScan(Uint32 colour) { //scan te edge table
    int i, j;
    int start, stop;
    for (i=0; i < HEIGHT; i++) {
        if (EArray[i][0] == 0)
            continue;
        switch (EArray[i][0]) {
        case 1:
            SetPixel(EArray[i][1], i, colour);
            continue;
        case 2: // should clip here, but need to interpolate z or colour to boundary
            start = EArray[i][1];
            stop  = EArray[i][2];
            if (stop==start) { // same value
                SetPixel(EArray[i][1], i, colour);
                continue;
            }
            for (j = start; j < stop; j++)
                SetPixel(j, i, colour);
            continue;
        default:
            continue;
        }
    }
}
void EdgeSet(int x0, int y0, int x1, int y1) { //modified line drawing
    int dx, dy, incrE, incrNE, d, x, y, x_end;
    int incrN, incrEN, y_end;
    int Xinc, Yinc;
    int tone[3];
    dx = x1-x0;
    dy = y1-y0;
    if (abs(dx) > abs(dy)) {
        if (dx<0) {
            swap(&x0, &x1);
            swap(&y0, &y1);
        }
        if (y1>y0)
            Yinc = 1;
        else
            Yinc = -1;
        dx =abs(dx);
        dy =abs(dy);
        d = 2*dy-dx;
        incrE = 2*dy;
        incrNE = 2 * (dy-dx);
        x_end = x1;
        x = x0;
        y = y0;
        // interpolate z along the edge
        EASet(x, y, tone);
        while (x < x_end) {
            if ( d <= 0 ) {
                d = d + incrE;
                x = x + 1;
            } else {
                d = d + incrNE;
                x = x + 1;
                y = y +Yinc;
            }
            EASet(x, y, tone);
        }
    } else {
        if (dy<0) {
            swap(&x0, &x1);
            swap(&y0, &y1);
        }
        if (x1>x0)
            Xinc = 1;
        else
            Xinc = -1;
        dx =abs(dx);
        dy =abs(dy);
        d = 2*dx-dy;
        incrN = 2*dx;
        incrEN = 2 * (dx-dy);
        y_end = y1;
        x = x0;
        y = y0;
        // interpolate z along y axis

        EASet(x, y, tone);
        while (y < y_end) {
            if ( d <= 0 ) {
                d = d + incrN;
                y = y + 1;
            } else {
                d = d + incrEN;
                y = y + 1;
                x = x + Xinc;
            }
            EASet(x, y, tone);
        }
    }
}

/*

Render a polygon with colour interpolation.

*/

void ShadeEdgeArrayClear() { //clear the edge table
    int i;
    for (i=0;i<HEIGHT;i++) {
        EArray[i][0] = 0;
    }
}

void ShadeEdgeArraySet(int x, int y, Vec4 Colour) { //add to the edge table
    Vec4 t;
    switch (EArray[y][0]) {
    case 0:
        EArray[y][0] = 1;
        EArray[y][1] = x;
        c1[y] = Colour;
        return;
    case 1:
        EArray[y][0] = 2;
        EArray[y][2] = x;
        c2[y] = Colour;
        if (EArray[y][1] > EArray[y][2]) { // fix order on 2nd insertion, to keep start, stop.
            swap(&EArray[y][1], &EArray[y][2]);
            t = c1[y];
//                t[0] = c1[y][0];
//                t[1] = c1[y][1];
//                t[2] = c1[y][2];
            c1[y] = c2[y];
//                c1[y][0] = c2[y][0]; // Should replace this with a swap function.
//                c1[y][1] = c2[y][1];
//                c1[y][2] = c2[y][2];
            c2[y] = t;
//                c2[y][0] = t[0];
//                c2[y][1] = t[1];
//                c2[y][2] = t[2];
        }
        return;
    case 2:
        if ( x < EArray[y][1]) {
            EArray[y][1] = x;
            c1[y] = Colour;
        } else if ( x > EArray[y][2]) {
            EArray[y][2] = x;
            c2[y] = Colour;
        }
        return;
    default:
        return;
    }
}

void ShadeEdgeScan() { //scan the edge table and draw spans
    int i, j;
    int start, stop;
    Vec4 diff, base, a, b;
    for (i=0; i < HEIGHT; i++) {
        if (EArray[i][0] == 0)
            continue;
        switch (EArray[i][0]) {
        case 1:
            SetPixel(EArray[i][1], i, vcolour(c1[i]));
            continue;
        case 2: // should clip here, but need to interpolate z or colour to boundary
            start = EArray[i][1];
            stop  = EArray[i][2];
            diff = c2[i]-c1[i];
            if (stop==start) // same value
                SetPixel(EArray[i][1], i, vcolour(c1[i]));
            else {
                a = c1[i];
                b = c2[i];
                diff = diff / (stop - start);
                base = c1[i];
                for (j = start; j < stop; j++) {
                    base += diff;
                    SetPixel(j, i, vcolour(base));
                }
            }
            continue;
        default:
            continue;
        }
    }
}

void ShadeEdgeSet(Vec4 vx0, Vec4 v1, Vec4 vx1, Vec4 v2) { //modified line drawing
    int dx, dy, incrE, incrNE, d, x, y, x_end;
    int incrN, incrEN, y_end;
    int Xinc, Yinc;
    int x0, x1, y0, y1;
    Vec4 diff, delta, base;
    Vec4 a, b;
    /*
     *     Interpolation across polygon.
     *     4 parameters are interploated
     *     Red channel
     *     Green channel
     *     Blue channel
     *     and distance
     *
     *     vx0 and vx1 are in eye cordiates.
     *     parallel or perspective projection have
     *     not been applied, so z value is correct.
     *
     *     if required apply projection to x, y components only.
     */
    x0 = (int) vx0[0]; // start x pixels
    x1 = (int) vx1[0]; // Stop x pixels.
    y0 = (int) vx0[1];
    y1 = (int) vx1[1];
    /*
     *     Fill a and b as vectors to interpolate.
     *     [0:2] are colour.
     */
    a[0] = v1[0]; //red
    b[0] = v2[0];
    a[1] = v1[1]; //green
    b[1] = v2[1];
    a[2] = v1[2]; //blue
    b[2] = v2[2];
    a[3] = b[3] = 1;
    /*
     *     Ths code interpolates between x0, y0 and x1, y1 over a, b
     */
    dx = x1-x0;
    dy = y1-y0;
    diff = b - a;
    if (abs(dx) > abs(dy)) {
        base = a;
        if (dx<0) {
            swap(&x0, &x1);
            swap(&y0, &y1);
            base = b;
        }
        if (y1>y0)
            Yinc = 1;
        else
            Yinc = -1;
        delta = diff/dx;
        dx =abs(dx);
        dy =abs(dy);
        d = 2*dy-dx;
        incrE = 2*dy;
        incrNE = 2 * (dy-dx);
        x_end = x1;
        x = x0;
        y = y0;
        ShadeEdgeArraySet(x, y, base);
        while (x < x_end) {
            base = base + delta;
            if ( d <= 0 ) {
                d = d + incrE;
                x = x + 1;
            } else {
                d = d + incrNE;
                x = x + 1;
                y = y +Yinc;
            }
            ShadeEdgeArraySet(x, y, base);
        }
    } else {
        base = a;
        if (dy<0) {
            swap(&x0, &x1);
            swap(&y0, &y1);
            base = b;
        }
        if (x1>x0)
            Xinc = 1;
        else
            Xinc = -1;
        delta = diff/dy;
        dx =abs(dx);
        dy =abs(dy);
        d = 2*dx-dy;
        incrN = 2*dx;
        incrEN = 2 * (dx-dy);
        y_end = y1;
        x = x0;
        y = y0;
        ShadeEdgeArraySet(x, y, base);
        while (y < y_end) {
            base = base + delta;
            if ( d <= 0 ) {
                d = d + incrN;
                y = y + 1;
            } else {
                d = d + incrEN;
                y = y + 1;
                x = x + Xinc;
            }
            ShadeEdgeArraySet(x, y, base);
        }
    }
}

/*
 *
 *
 *Draw a 3D polygon using interpolation to calculate the Z value a well as the colour.
 *
 *
*/

void ZEdgeArrayClear() { //clear the edge table
}

void ZEdgeArraySet(int x, int y, Vec4 Colour) { //add to the edge table
}

void ZEdgeScan() { //scan the edge table and draw spans
}

void ZEdgeSet(Vec4 vx0, Vec4 v1, Vec4 vx1, Vec4 v2) { //Draws a shaded polygon with a z-buffer.
}




/*

High Level Interface based on arrays of vertices and colours.

*/
void GPolygon2(Vec4 *x, int n, Vec4 col) {  //draw a 2D polygon with constant colour.
    int i;
    int colour = vcolour(col);
    EAClear();
    for (i=0; i < n; i++) {
        int j= (i+1) % n;
        EdgeSet((int) x[j][0], (int) x[j][1], (int) x[i][0], (int) x[i][1]);
    }
    EdgeScan(colour);
}

void GPolygon2(Vec4 *x, int n, Vec4 *colours) {  //draw a 2D polygon with colour per vertex.
    int i;
    ShadeEdgeArrayClear();
    for (i=0; i < n; i++) {
        int j= (i+1) % n;
        ShadeEdgeSet( x[j], colours[j], x[i], colours[i]);
    }
    ShadeEdgeScan();
}

void GPolygon3(Vec4 *x, int n, Vec4 *colours) {  //draw a 2D polygon with colour per vertex.
    int i;
    ZEdgeArrayClear();
    for (i=0; i < n; i++) {
        int j= (i+1) % n;
        ZEdgeSet( x[j], colours[j], x[i], colours[i]);
    }
    ZEdgeScan();
}

/*

drawing a line loop

*/

void GWire2(Vec4 *p, int n, Vec4* colour) {
    int i;
    int x1, y1, x2, y2;
    int reject, col;
    Vec4 value;
    for (i=1;i < n ; i++) {
        x1 = (int) p[i-1][0];
        y1 = (int) p[i-1][1];
        x2 = (int) p[i][0];
        y2 = (int) p[i][1];
        value = colour[i-1];
        col = Colour((int) value[0], (int) value[1], (int) value[2]);
        reject = Clip(0, WIDTH-1, 0, HEIGHT-1, &x1, &y1, &x2, &y2);
        if (!reject)
            Line(x1, y1, x2, y2, col);
    }
    x1 = (int) p[0][0];
    y1 = (int) p[0][1];
    x2 = (int) p[n-1][0];
    y2 = (int) p[n-1][1];
    value = colour[n-1];
    col = Colour((int) value[0], (int) value[1], (int) value[2]);
    reject = Clip(0, WIDTH, 0, HEIGHT, &x1, &y1, &x2, &y2);
    if (!reject)
        Line(x1, y1, x2, y2, col);
}
void GLine2(Vec4 *p, int n, Vec4*  colour) {
    int i;
    int x1, y1, x2, y2;
    int reject, col;
    Vec4 value;
    for (i=1;i < n ; i++) {
        x1 = (int) p[i-1][0];
        y1 = (int) p[i-1][1];
        x2 = (int) p[i][0];
        y2 = (int) p[i][1];
        value = colour[i-1];
        col = Colour((int) value[0], (int) value[1], (int) value[2]);
        reject = Clip(0, WIDTH, 0, HEIGHT, &x1, &y1, &x2, &y2);
        if (!reject)
            Line(x1, y1, x2, y2, col);
    }
}



