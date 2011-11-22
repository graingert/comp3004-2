#include "gPackage.h"

/*
 *
 * This module is the main controler.
 *
 * This version draws in Device Independant coordinates.
 *
 */


static int first_call = 1;
static Uint32 lasttick; //Time last time the clock was read
static Uint32 tick; //Global variable used to control time.
static Uint32 deltaT; //Time between ticks,



void poly(Vec4 *m, int i, int j, int k, int l, Vec4 h) {
//    int mode = gPOLYGON;
    int mode = gWIRE;
    gColour(h);
    gBegin(mode);
    gColour(gRED);
    gVertex(m[i]);
    gColour(gBLUE);
    gVertex(m[j]);
    gColour(gGREEN);
    gVertex(m[k]);
    gColour(gBLACK);
    gVertex(m[l]);
    gEnd();
}

void Sphere(float t) {
    return;
}
void test4(float t) { //Tests z-buffer and draws Rotaing cube
    Vec4 cube[8] = { Vec4(0., 0., 0., 1.), Vec4(0., 1., 0., 1.), Vec4(1., 1., 0., 1.), Vec4(1., 0., 0., 1.),
                     Vec4(0., 0., 1., 1.), Vec4(0., 1., 1., 1.), Vec4(1., 1., 1., 1.), Vec4(1., 0., 1., 1.)
                   };
    float th = vl_halfPi * t/(10.);
    ClearScreen();
    ZClear();
    Init3D();
    Translate(-0.5, -0.5, -0.5);
    Rotatex(th);
    Rotatez(th/2.);
    Rotatey(th/3.);
    poly(cube, 0, 1, 2, 3, gRED);
    poly(cube, 4, 5, 6, 7, gGREEN);
    poly(cube, 0, 4, 5, 1, gBLUE);
    poly(cube, 1, 5, 6, 2, gMAGENTA);
    poly(cube, 2, 6, 7, 3, gYELLOW);
    poly(cube, 3, 7, 4, 0, gCYAN);
}

void test3(float t) { // tests drawing shaded 2D polygon
    Vec4 box[4] = { Vec4(WIDTH / 4., HEIGHT / 4., 0., 1.), Vec4(WIDTH /4, 3. * HEIGHT /4., 0., 1.), Vec4(3. * WIDTH /4., 3. * HEIGHT /4., 0., 1.), Vec4(3. * WIDTH /4., HEIGHT /4, 0., 1.)};
    Vec4 colours[4] = {gRED, gGREEN, gBLUE, gWHITE};
    ClearScreen();
    GPolygon2(box, 4, colours);
}

void test2(float t) {  // tests drawing  2D polygon
    Vec4 box[4] = { Vec4(WIDTH / 4., HEIGHT / 4., 0., 1.), Vec4(WIDTH /4, 3. * HEIGHT /4., 0., 1.), Vec4(3. * WIDTH /4., 3. * HEIGHT /4., 0., 1.), Vec4(3. * WIDTH /4., HEIGHT /4, 0., 1.)};
    ClearScreen();
    GPolygon2(box, 4, gRED);
}

void test1(float t) { // tests drawing lines
    Vec4 box[4] = { Vec4(WIDTH / 4., HEIGHT / 4., 0., 1.), Vec4(WIDTH /4, 3. * HEIGHT /4., 0., 1.), Vec4(3. * WIDTH /4., 3. * HEIGHT /4., 0., 1.), Vec4(3. * WIDTH /4., HEIGHT /4, 0., 1.)};
    Vec4 colours[4] = {gRED, gGREEN, gBLUE, gWHITE};
    ClearScreen();
    GWire2(box, 4, colours);
}

#define TIMING 0

#define TEST test4

#if TIMING
/*

This code can be used to time the graphics operation.

define TIMING as 1 and MAXDELAY to a suitable value for the processor in use.

*/
#define MAXDELAY 10
void Drawing(SDL_Event test_event) { // Use this code to time the graphics.
    int delay, timetick;
    if (first_call) { // provision for some 1st call setup
        first_call = 0;
    }
    tick = SDL_GetTicks();
    deltaT = tick - lasttick;
    lasttick = tick;
    ClearScreen();
    for (delay = 0; delay < MAXDELAY; delay++)
        TEST((float) tick/ 1000.);
    timetick = SDL_GetTicks();
    printf("Time taken %d for %d loops\n", timetick-tick, delay);
}
#else
void Drawing(SDL_Event test_event) { // User interface.
    if (first_call) { // provision for some 1st call setup
        first_call = 0;
    }
    tick = SDL_GetTicks();
    deltaT = tick - lasttick;
    lasttick = tick;
    ClearScreen();
    TEST((float) tick/ 1000.);
}
#endif



void StartDrawing(void) { //Set up
    ClearScreen();
    ZInit();
    first_call = 1;
}



void MainLoop() { // this does the work
    int done=0;
    SDL_Event test_event;
    printf("Starting Main Loop\n");
    while (!done) { /* wait for quit event */
        if (SDL_PollEvent(&test_event)) {
            switch (test_event.type) {
#if PC_PLATFORM
            case SDL_KEYDOWN: //Keys pressed
                if ( test_event.key.keysym.sym == SDLK_ESCAPE || test_event.key.keysym.sym == SDLK_q) {
                    printf("Exiting Game on command\n");
                    return;
                } else if (test_event.key.keysym.sym == SDLK_c) {
                    Lock();
                    StartDrawing();
                    Unlock();
                }
                break;
            case SDL_QUIT: // Window exit
                return;
                break;
#endif
#ifdef       GP2X_PLATFORM
            case  SDL_JOYBUTTONDOWN:
                switch (test_event.jbutton.button) { //these keys override the game
                case GP2X_BUTTON_START:
                    done = 1;
                    continue;
                case GP2X_BUTTON_SELECT: // Clear the screen and restart
                    Lock();
                    StartDrawing();
                    Unlock();
                    continue;
                default:
                    Lock();
                    Drawing(test_event);
                    Unlock();
                }
#endif
            }
        }
        test_event.type = SDL_USEREVENT;
        test_event.user.code = 0;
        Lock();
        Drawing(test_event); // User event is hardwired
        Unlock();
#ifdef  GP2X_PLATFORM
        SDL_Flip(screen);
        SDL_Delay(10);
#endif
    }
}


int main(int argc, char *argv[]) {
#ifdef PC_PLATFORM
    printf("PC Version\n");
#else
    printf("GP2X Version\n");
#endif
    StartUp("SDL Coursework 2008");
    Lock();
    StartDrawing();  // screen must be locked here.
    Unlock();
    MainLoop();
    printf("Exit Main Loop\n");
    ShutDown();
    return 0;
}
