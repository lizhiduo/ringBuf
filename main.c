#include <stdio.h>
#include <string.h>

#include "RingBuf.h"

#define MaxBuffSize 256


void show_char(const unsigned char *buf, int count)
{
    int k = 0;
    
    /*  */
    for(k=0; k<count; k++)
    {
        if(k%16 == 0){ printf("\n"); }
        printf("%d ", buf[k]);
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    unsigned char buf[5];
    unsigned char i = 1;
    unsigned char rdat[16];
    RingBuf *pRingBuf = NULL;
    int stop = 0;

    pRingBuf = CreateRingBuf(MaxBuffSize);

    while(1)
    {
        
        /*  */
        while(1)
        {
            if(i%6 == 0)
            {
                i++;
                break;
            }

            buf[i%6 - 1] = i;
            i++;
        }
        
        pRingBuf->put(pRingBuf, buf, 5);

        if(pRingBuf->wLen >= 16)
        {
            pRingBuf->get(pRingBuf, rdat, 16);
            printf("[%d]=====================\n", stop);
            show_char(rdat, 16);
            printf("=========================\n");
            stop++;
        }
        
        /*stop*/
        if(stop >= 60)
        {
            break;
        }
    }

    ReleaseRingBuf(pRingBuf);
    
    return 0;
}

