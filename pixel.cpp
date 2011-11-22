/*

Low Level gaphics


*/
#include "pixel.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


SDL_Surface *screen; /* global variable for the screen, bad programing practice */
SDL_Joystick *joystick;
int mouse_x;
int mouse_y;

/* **************************** System Operations ************************** */

//~ int buffer[HEIGHT/8][WIDTH/8];

Uint32 DefaultColour;


void Lock() { /* Lock the screen for direct access to the pixels */
    if ( SDL_MUSTLOCK(screen) ) {
        if ( SDL_LockSurface(screen) < 0 ) {
            fprintf(stderr, "Can't lock screen: %s\n", SDL_GetError());
            return;
        }
    }
}

void Unlock() { /* Unlock and update the whole screen */
    if ( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
    /* Update just the part of the display that we've changed */
    SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void StartUp(const char *title) { //Start up SDL and other stuff
    int w = WIDTH;
    int h = HEIGHT;
    int desired_bpp = 0;
    Uint32 video_flags = 0;
    int bpp;
    mouse_x = 0;
    mouse_y = 0;
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);
    atexit(SDL_Quit);			/* Clean up on exit */

    screen = SDL_SetVideoMode(w, h, desired_bpp, video_flags);	/* Initialize the display */
    if ( screen == NULL ) {
        fprintf(stderr, "Couldn't set %dx%dx%d video mode: %s\n",
                w, h, desired_bpp, SDL_GetError());
        exit(1);
    }
    SDL_WM_SetCaption(title, "testwin");   /* Set the window manager title bar */
    printf("SDL Setp Done\n");
    DefaultColour = Colour(255, 255, 255);
    bpp = screen->format->BytesPerPixel;
    printf("BPP is %d\n", bpp);
    printf("WIDTH is %d\n", WIDTH);
    printf("HEIGHT is %d\n", HEIGHT);

}

void ShutDown(void) {
    Lock();
    ClearScreen();
    Unlock();
    SDL_FreeSurface(screen);// Shut down the SDL library, freeing resources.
    SDL_Quit();
    // This is how to quit back to the GP2X menu.  Just doing a return(0) will look like the GP2X hung.
    // chdir("/usr/gp2x");
    // execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
    // execl will go run the other program INSTEAD of this one.
}



/* ****************************  Pixel Operations ************************** */


void SET16(int x, int y, Uint32 c) { /* sets pixel if 16-bit display */
    Uint8 *p;
    p  = (Uint8 *)screen->pixels + y * screen->pitch + x * 2;
    *(Uint16 *)p = c; // dont clean it up.
}

void SET32(int x, int y, Uint32 c) { /* sets pixel if 32-bit display */
    Uint8 *p;
    p = (Uint8 *)screen->pixels + y * screen->pitch + x * 4;
    *(Uint32 *)p = c;
}

void AutoSetPixel(int x, int y, Uint32 pixel) { /* set a single pixel independnt of resolution */
    /* int bpp = screen->format->BytesPerPixel; */
    /* Here p is the address to the pixel we want to set */
    //Uint16 *p = (Uint16 *) ((Uint8 *)screen->pixels + y * screen->pitch + x * 2);
    //*p = pixel;
    //return;
    int bpp = screen->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

    switch (bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }


}


int Colour(int r, int g, int b) /* Converts RGB to screen colour */
{ /* generate a colour value */
    return SDL_MapRGB(screen->format, (Uint8) r, (Uint8) g, (Uint8) b);
}



void SetDefaultColour(int r, int g, int b) { //Sets the default Colour
    DefaultColour = Colour(r, g, b);
}


void ClearScreen() { // Clears the screen
    SDL_FillRect(screen, NULL, BLACK);
}


void SetScreen(void) { // Draws a cross on the screen so you can see its extent
    printf("Starting Configuration"); // test code
    Line(0,0, WIDTH-1, HEIGHT-1, Colour(255, 255, 255));
    Line(0, HEIGHT-1, WIDTH-1, 0, Colour(255, 0, 0));
}

/* *********** Diagnostic cross ************************** */
void test_cross(void) {
    Line(0, 0, WIDTH-1, HEIGHT-1, RED);
    Line(0, HEIGHT-1, WIDTH-1, 0, BLUE);
}

/* ****************************  Line drawing ************************** */

void Line(int x0, int y0, int x1, int y1, Uint32 colour) // low-level line draw
{ //draw a real line
    int dx, dy, incrE, incrNE, d, x, y, x_end;
    int incrN, incrEN, y_end;
    int Xinc, Yinc;
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

        SetPixel(x, y, colour);
        while (x < x_end) {
            if ( d <= 0 ) {
                d = d + incrE;
                x = x + 1;
            } else {
                d = d + incrNE;
                x = x + 1;
                y = y +Yinc;
            }
            SetPixel(x, y, colour);
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

        SetPixel(x, y, colour);
        while (y < y_end) {
            if ( d <= 0 ) {
                d = d + incrN;
                y = y + 1;
            } else {
                d = d + incrEN;
                y = y + 1;
                x = x + Xinc;
            }
            SetPixel(x, y, colour);
        }
    }
}

/* ------------------------------------------------------------------------ */

/* ****************************  Line clipping ************************** */

#define CHECK(w, v) (v >= 0 && v < w)
#define FALSE 0
#define TRUE 1

int outcode(int wxl, int wxr, int wyb, int wyt, int x, int y) {
    int out = 0;
    if (x < wxl)
        out = out | 1;
    if (x > wxr)
        out = out | 2;
    if (y < wyb)
        out = out | 4;
    if (y > wyt)
        out = out | 8;
    return out;
}

int Clip(int wxl, int wxr, int wyb, int wyt, int *x0, int *y0, int *x1, int *y1)
// clips line from v1 to v2 to wxl < x < wxr,
// wyb < y < wyt.  If line outside window then
// reject is set true, otherwise the visible
// portion is from the amended v1 to v2.
{
    int c,c1,c2,done, reject;
// const int wyb =0;
// const int wxl = 0;
//~ double t,x,y;
    float t = 0;
    float x = 0;
    float y = 0;
    wxr--; // set proper bounds
    wyt--;
    reject = FALSE;
    done = FALSE;
    c1 = outcode(wxl, wxr, wyb, wyt, *x0, *y0);
    c2 = outcode(wxl, wxr, wyb, wyt, *x1, *y1);
//printf("Codes %x %x\n", c1, c2);
    while (!done && (c1 != 0 || c2 != 0)) {
        if ((c1 & c2) != 0) {
            reject = TRUE;
            done = TRUE;
        } else if (c1 == c2 && c1 != 0) {
            reject= TRUE;
            done = TRUE;
        } else {
            c = c1;
            if (c == 0) c = c2;    // c is outside
            if ((c & 0x0001) != 0) {// intersects left edge
                // finds point of intersecion
                t = (double)(wxl - *x0)/(double)(*x1 - *x0);
                x = wxl;
                y = (1.0-t)*((double)*y0) + t*((double)*y1);
            } else
                if ((c & 0x0002) != 0) {// intersects right edge
                    t = (double)(wxr - *x0)/(double)(*x1 - *x0);
                    x = wxr;
                    y = (1.0-t)*((double)*y0) + t*((double)*y1);
                } else
                    if ((c & 0x0004) != 0) {// intersects bottom edge
                        t = (double)(wyb - *y0)/(double)(*y1 - *y0);
                        y = wyb;
                        x = (1.0-t)*((double)*x0) + t*((double)*x1);
                    } else
                        if ((c & 0x0008) != 0) {// intersects top edge
                            t = (double)(wyt - *y0)/(double)(*y1 - *y0);
                            y = wyt;
                            x = (1.0-t)*((double)*x0) + t*((double)*x1);
                        }
            if (c == c1) {  // v1 was outside, replace it by intersection
                *x0 = (int) x;
                *y0 = (int) y;
                c1 = outcode(wxl, wxr, wyb, wyt, *x0, *y0);
            } else {        // v2 was outside, replace it by intersection
                *x1 = (int) x;
                *y1 = (int) y;
                c2 = outcode(wxl, wxr, wyb, wyt, *x1, *y1);
            }
        }
    }
    return reject;
}


/* **************************** Helper Operations ************************** */


void swap( int *x, int *y) { /* swap two integers */
    int t;
    t = *x;
    *x = *y;
    *y = t;
}
void fswap( float *x, float *y) { /* swap two floats */
    float t;
    t = *x;
    *x = *y;
    *y = t;
}


