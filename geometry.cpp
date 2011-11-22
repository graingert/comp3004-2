#include "gPackage.h"
/*

Master transform, applied to vertices to convert to normalised window coordinates

*/

static Mat4 transform;

/*
 * Convert from normalised window coordinates to pixels
 * Origin is in middle of screen
 */

inline float scalex(float x) {
    return (x * HEIGHT/2.0) + (WIDTH/2.0);
}
inline float scaley(float y) {
    return HEIGHT - ((y * HEIGHT/2.0) + (HEIGHT/2.0));
}
/*
 * Initialiseation function
 *
 */
void Init3D(void) {
    transform = vl_1; // svl short cut to create an identity matrix.
    return;
}

void SetTransform(Mat4 in){
    transform = in;
    }
/*

Rotating

*/
void Rotatex(float a) {
    Mat4 id = Mat4(vl_1);
    transform = id * transform;
    return;
}
void Rotatey(float a) {
    Mat4 id = Mat4(vl_1);
    transform = id * transform;
    return;
}
void Rotatez(float a) {
    Mat4 id = Mat4(vl_1);
    transform = id * transform;
    return;
}

/*

Scaling

*/
void Scale(float a) {
    Mat4 id = Mat4(vl_1);
    transform = id * transform;
    return;
}
void Scale3(float a, float b, float c) {
    Mat4 id = Mat4(vl_1);
    transform = id * transform;
    return;
}

/*

Translation

*/
void Translate(float x, float y, float z) {
    Mat4 id = vl_1;
    id[0][3] = x;
    id[1][3] = y;
    id[2][3] = z;
    transform = id * transform;
    return;
}

/*

Conversion routines - Yhis assumes that the camera is at the orign and a paralel projection is used.

*/

Vec4* Convert4(Vec4 *x, int n) { //converts an aray of Vec4
    Vec4 r, y, *z;
    z = new Vec4[n];
    for (int i =0; i< n; i++) {
        y = transform * x[i]; // converts from model coordinates to world coordinates.
        z[i][0] = scalex(y[0]); // Converts to pixels.
        z[i][1] = scaley(y[1]);
        z[i][2] = y[2]; // z-value left in world coordinates.
        z[i][3] = y[3];
    }
    return z;
}

Vec4 Convert4(Vec4 x) { // Converts a single Vec4
    Vec4 r, y;
    y = transform * x;
    r[0] = scalex(y[0]);
    r[1] = scaley(y[1]);
    r[2] = y[2];
    r[3] = y[3];
    return r;
}


