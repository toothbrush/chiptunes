#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define N 11

/*
 * note that as a starting point these tunes have been
 * lifted from <http://countercomplex.blogspot.fr/2011/10/algorithmic-symphonies-from-one-line-of.html>
 * Thanks viznut!
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
        return (t >> 6|t|t>>(t>>16))*10+((t>>11)&7);
};
unsigned int tune5(unsigned int t){
    //quiet thing.
        return (t*5&t>>7)|(t*3&t>>10);
};
unsigned int tune6(unsigned int t){
    //dubious transcription?
        return (((int)(t/1e7*t*t+t)%127) | (t>>4) | (t>>5) | (t%127)) + ((t>>16)|t);
};
unsigned int tune7(unsigned int t){
    //syncopation?
        return (t&t%255)-(t*3&t>>13&t>>6);
};
unsigned int tune8(unsigned int t){
        return ((t*("36364689"[t>>13&7]&15))/12&128)
            +((((((t>>12)^(t>>12))-2)%11*t)/4
                        |t>>13)&127);
};
unsigned int tune9(unsigned int t){
    // pretty slow and nice
        return ((t*9&t>>4)|(t*5&t>>7)|((t*3)&(t/1024)))-1;
};
unsigned int tune10(unsigned int t){
    //dante's inferno by oasiz & viznut
        return ((t*(t>>12)&(201*t/100)&(199*t/100))
                &(t*(t>>14)&(t*301/100)&(t*399/100)))
            +((t*(t>>16)&(t*202/100)&(t*198/100))
                    -(t*(t>>17)&(t*302/100)&(t*298/100)));
};
unsigned int tune11(unsigned int t){
    // originally mu6k's javascript expression.. proceed with caution?
    unsigned int y,x;
    // http://youtu.be/tCRPUv8V22o

    y=t&16383;
    x=t*"6689"[t>>16&3]/24&127;

    return ((int)(3e3/y)&1)*35
        +x*y/4e4
        +(((t>>8^t>>10)|(t>>14)|x)&63);
    return 1;

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

    printw("Press <RET> to advance to next tune, C-c to quit.\n");
    refresh();            /* Print it on to the real screen */

    /* put together a list of function pointers to our "tunes" */

    unsigned int (**tunes)(unsigned int);

    tunes = malloc(N * sizeof(&tune1));
    if (tunes == NULL)
    {
        fprintf(stderr, "omigod, couldn't allocate tunes array!\n");
        exit(1);
    }

    tunes[0] = &tune1;
    tunes[1] = &tune2;
    tunes[2] = &tune3;
    tunes[3] = &tune4;
    tunes[4] = &tune5;
    tunes[5] = &tune6;
    tunes[6] = &tune7;
    tunes[7] = &tune8;
    tunes[8] = &tune9;
    tunes[9] = &tune10;
    tunes[10] = &tune11;



    /* sanity check, are all tunes filled? */
    for(int i = 0; i<N; i++)
    {
        if (tunes[i] == NULL)
        {
            fprintf(stderr, "oops, fill in tune %d please!\n", i);
            exit(4);
        }}

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

