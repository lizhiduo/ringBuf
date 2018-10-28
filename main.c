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
#include <pthread.h>
#include <unistd.h>

#include "RingBuf.h"

/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/
#define BUF_SIZE  (64 * 3)
#define PUT_DATA_LEN (5)
#define GET_DATA_LEN (16)


#define TASK_EXIT       \
        do              \
        {               \
            if(bExit)   \
                return; \
        }while(0)

/*==============================================*
 *      project-wide global variables           *
 *----------------------------------------------*/
static int bExit = 0;
static RingBuf *pRingBuf = NULL;



/*==============================================*
 *      routines' or functions' implementations *
 *----------------------------------------------*/
#if 1
void show_char(const unsigned char *buf, int count)
{
    int k = 0;
    
    for(k = 0; k < count; k++)
    {
        if((k % 16) == 0){ printf("\n"); }
        printf("%d ", buf[k]);
    }
    printf("\n");
}

void fillData(unsigned char *buf, unsigned int len)
{   
    static unsigned char count = 1;

    while(1)
    {
        if(count % len == 0)
        {
            count++;
            break;
        }

        buf[count % len - 1] = count;
        count++;
    }
}

void put_thread(void)
{
    unsigned char wDat[PUT_DATA_LEN] = {0};
    unsigned int retSize = 0;
    
    while(1)
    {
        TASK_EXIT;
        fillData(wDat, PUT_DATA_LEN + 1);
        retSize = pRingBuf->put(pRingBuf, wDat, PUT_DATA_LEN);
        if (0 == retSize)
        {
            printf("no space...\n");
        }
        printf("in:%d out:%d\n", pRingBuf->in, pRingBuf->out);
        usleep(500 * 1000);
    }
}

void get_thread(void)
{
    unsigned char rDat[GET_DATA_LEN] = {0};
    unsigned int retSize = 0;
    
    while(1)
    {   
        TASK_EXIT;
        retSize = pRingBuf->get(pRingBuf, rDat, 16);
        if (retSize != 0)
        {
            printf("=========================\n");
            show_char(rDat, retSize);
            printf("=========================\n");
        }
        else
        {
            printf("no data...\n");
        }
        usleep(1000 * 1000);
    }

}


int main(int argc, char* argv[])
{
    int ret = 0;
    pthread_t id[2] = {0};
    char op = 0;
    
    pRingBuf = CreateRingBuf(BUF_SIZE);
    
    ret = pthread_create(&id[0], NULL, (void *) put_thread, NULL);
    if(ret != 0)
    {
        printf ("Create pthread error!\n");
        return -1;
    }
    
    ret = pthread_create(&id[1], NULL, (void *) get_thread, NULL);
    if(ret != 0)
    {
        printf("Create pthread error!\n");
        return -1;
    }
    

    while (1)
    {
        
        op = getchar();
        if ('Q' == op || 'q' == op)
        {
            bExit = 1;
            printf("EXIT...\n");
            break;
        }
    }


    pthread_join(id[0],NULL);
    pthread_join(id[1],NULL);

    ReleaseRingBuf(pRingBuf);
    
    return 0;
}
#endif
