/******************************************************************************
*
*  Copyright (C), 2001-2022
*
*******************************************************************************
*  File Name     : main.c
*  Version       : Initial Draft
*  Author        : lizhiduo
*  Created       : 2018/10/26
*  Last Modified :
*  Description   : xx
*  Function List :
*
*       1.                main
*       2.                show_ch3ar
*
*  History:
* 
*       1.  Date         : 2018/10/26
*           Author       : lizhiduo
*           Modification : Created file
*
******************************************************************************/

/*==============================================*
 *      include header files                    *
 *----------------------------------------------*/
#include <string.h>
#include <stdio.h>

#include "RingBuf.h"




/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/
#define BUF_SIZE  (256)


/*==============================================*
 *      project-wide global variables           *
 *----------------------------------------------*/



/*==============================================*
 *      routines' or functions' implementations *
 *----------------------------------------------*/

#if 1
void show_char(const unsigned char *buf, int count)
{
    int k = 0;
    
    
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
    unsigned char rdat[16];
    RingBuf       *pRingBuf = NULL;
    unsigned char i         = 1;
    int           stop      = 0;
    char          ret       = 0;

    
    pRingBuf = CreateRingBuf(BUF_SIZE);
#if 1
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

        printf("in:%d out:%d\n", pRingBuf->in, pRingBuf->out);
  
        ret = pRingBuf->put(pRingBuf, buf, 5);
        if (ret == 0)
        {
            printf("no space..\n");
            break;
        }
       
        #if 0
        ret = pRingBuf->get(pRingBuf, rdat, 16);
        if (ret != 0)
        {
            printf("[%d]=====================\n", stop);
            show_char(rdat, ret);
            printf("=========================\n");
        }
        #endif /* 0 */
        
        /*stop*/
        stop++;
        if(stop >= 160)
        {
            break;
        }
    }
#endif
    ReleaseRingBuf(pRingBuf);
    
    return 0;
}
#endif
