#include "gPackage.h"

#define ZLENGTH (HEIGHT * WIDTH)
#define ACCESS(x, y) ((int) y * WIDTH + (int) x)

static float  *zbuffer;

void ZInit(void) {
    /*
    Z-Buffer must be created dynamically as it is too big for propper static allocation
    */
}

void ZClear() {
}

void ZSet(int x, int y, Vec4 value) {
}

void ZSetDepth(float z, int x, int y) {
}

