/******************************************************************************
*
*  Copyright (C), 2001-2022
*
*******************************************************************************
*  File Name     : RingBuf.c
*  Version       : Initial Draft
*  Author        : lizhiduo
*  Created       : 2018/10/26
*  Last Modified :
*  Description   :  
*  Function List :
*
*       1.                CreateRingBuf
*       2.                fls
*       3.                ReleaseRingBuf
*       4.                RingBufGet
*       5.                RingBufPut
*       6.                roundupOfTwo
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
    
#include "RingBuf.h"


/*==============================================*
 *      constants or macros define              *
 *----------------------------------------------*/
#define MIN(a, b) ((a)<(b))?(a):(b)


/*==============================================*
 *      project-wide global variables           *
 *----------------------------------------------*/



/*==============================================*
 *      routines' or functions' implementations *
 *----------------------------------------------*/
static inline int fls(int x)
{
    int position = 0;
    int i = 0;

    if(0 != x)
    {
        for (i = (x >> 1), position = 0; i != 0; ++position)
        {
            i >>= 1;
        }
    }
    else
    {
        position = -1;
    }
    
    return position + 1;
}

static inline unsigned int roundupOfTwo(unsigned int x)
{
    return 1UL << fls(x - 1);
}


static unsigned int RingBufPut(RingBuf *pObj, const char *buf, unsigned int bufSize)
{
    unsigned int part1 = 0;

    /* in-out 表示空间使用大小； size-(in-out)表示未使用空间大小 */
    bufSize = MIN(bufSize, pObj->size - pObj->in + pObj->out);

    /* size-in表示in到尾端的大小 */
    part1 = MIN(bufSize, pObj->size - (pObj->in & (pObj->size - 1)));
//    part1 = MIN(bufSize, pObj->size - (pObj->in % pObj->size));
    
    memcpy(pObj->buffer + (pObj->in & (pObj->size - 1)), buf, part1);
//    memcpy(pObj->buffer + (pObj->in % pObj->size), buf, part1);
    memcpy(pObj->buffer, buf + part1, bufSize - part1);
    
    pObj->in += bufSize;
    
    return bufSize;
}

static unsigned int RingBufGet(RingBuf *pObj, char *buf, unsigned int getSize)
{
    unsigned int part1 = 0;

    getSize = MIN(getSize, pObj->in - pObj->out);

    /* size-out表示out到尾端大小 */
    part1 = MIN(getSize, pObj->size - (pObj->out & (pObj->size - 1)));
//    part1 = MIN(getSize, pObj->size - (pObj->out % pObj->size));

    /* 第二次copy可能是0个字节，0个字节不做任何copy */
    memcpy(buf, pObj->buffer + (pObj->out & (pObj->size - 1)), part1);
//    memcpy(buf, pObj->buffer + (pObj->out % pObj->size), part1);
    memcpy(buf + part1, pObj->buffer, getSize - part1);
    
    pObj->out += getSize;

    return getSize;
}


RingBuf* CreateRingBuf(unsigned int bufSize)
{
    RingBuf *pObj = NULL;
   
    pObj = (RingBuf *)malloc(sizeof(*pObj));
    if (pObj == NULL)
    {
        return NULL;
    }
    
    memset(pObj, 0, sizeof(*pObj));

    /* 判断size是否是2的幂 */
    if (bufSize & (bufSize - 1)) 
    {
        /* 不是则向上扩展成2的幂 */
        /*
        优点：
            1. 保证in/out是size的整数倍 (size:2^x in:2^32)
            2. 取模运算可以转换成与运算 in%size   ->in&(size-1)
        */
        bufSize = roundupOfTwo(bufSize);
    }
    
    pObj->buffer = (char *)malloc(bufSize);
    if (pObj->buffer == NULL)
    {
        free(pObj);
        return NULL;
    }
    
    memset(pObj->buffer, 0, bufSize);

    pObj->size = bufSize;
    pObj->get = RingBufGet;
    pObj->put = RingBufPut;

    return pObj;
}


int ReleaseRingBuf(RingBuf *pObj)
{
    if (pObj != NULL)
    {
        if (pObj->buffer != NULL)
        {
            free(pObj->buffer);
            pObj->buffer == NULL;
        }
        free(pObj);
        pObj = NULL;
    }

    return 0;
}
