#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define N 4

/*
 * note that as a starting point these tunes have been 
 * lifted from <http://countercomplex.blogspot.fr/2011/10/algorithmic-symphonies-from-one-line-of.html>
 */
unsigned int tune1(unsigned int t){
        return (t * (((t>>12)|(t>>8)) & (63&(t>>4))));
};
unsigned int tune2(unsigned int t){
        return (t * (t>>8*(t>>15|t>>8) & (20|(t>>19)*5>>t|t>>3)));
};
unsigned int tune3(unsigned int t){
        return ((-t&4095)*(255&t*(t&t>>13))>>12) + (127&t*(234&t>>8&t>>3)>>(3&t>>14));
};
unsigned int tune4(unsigned int t){
        return t;
        //return (t*(t>>((t>>9|t>>8))&63&t>>4));
};

/* end tunes */

int main(int argc, char** argv){
    /* this generates some nifty sound. */
    unsigned int t;
    char ch;
    FILE *f = fopen("/dev/dsp", "w");

    int currentTune = 0;

    initscr();            /* Start curses mode           */
    if (f == NULL)
    {
        endwin();
        fprintf(stderr,"Error opening /dev/dsp!\n");
        fprintf(stderr,"You should probably invoke like this:\n");
        fprintf(stderr,"  padsp %s\n", argv[0]);

        exit(1);
    }

    printw("Size of variable t = %d bytes.\n", sizeof(t));
    printw("Press <RET> to advance to next tune, C-c to quit.\n");
    refresh();            /* Print it on to the real screen */

    /* put together a list of function pointers to our "tunes" */

    unsigned int (**tunes)(unsigned int);

    tunes = malloc(N * sizeof(&tune1));
    printw("size of tune = %d\n", (&tune1));
    printw("size of tune = %d\n", (&tune2));
    printw("size of tune = %d\n", (&tune3));
    if (tunes == NULL)
    {
        fprintf(stderr, "omigod, couldn't allocate tunes array!\n");
        exit(1);
    }

    tunes[0] = &tune1;
    tunes[1] = &tune2;
    tunes[2] = &tune3;
    tunes[3] = &tune4;

    t=0;
    timeout(0);
    while ( 1 ) {

        ch = getch();

        //if ( ch == 'q' ) {
        //    break;
        //}
        if (ch == '\n') {
            currentTune = (currentTune + 1) % N;
            t = 0; // restart at beginning of song.
            printw("Next tune! Will play tune #%d/%d.\n", currentTune + 1, N);
            printw("would be the output?? %d\n", tunes[currentTune](t));
            refresh();
        }

        /* the main loop code */
        putc(tunes[currentTune](t), f);
        t++;
    }
    free(tunes);
    refresh();
    printw("Okay, quitting.\n");
    fclose(f);
    endwin();
}

