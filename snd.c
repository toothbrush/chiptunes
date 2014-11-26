#include <stdio.h>
void main(){
    /* this generates some nifty sound. */
    int t;
    fprintf(stderr, "size of variable = %d bytes\n", sizeof(t));
    for(t=0;      ;t++)
        putchar(t * (((t>>12)|(t>>8)) & (63&(t>>4))));
}
