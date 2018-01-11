#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "RingBuf.h"

static int RingBufPut(RingBuf *pObj, const char*buf, int bufSize)
{
    int leftSize = 0;
    int rightSize = 0;
    int part1    = 0;
    char outChange = 0;
    
    if(pObj->wIdx >= pObj->rIdx)
    {
        leftSize = pObj->total - pObj->wIdx + pObj->rIdx;
        rightSize = pObj->total - pObj->wIdx;

        if(bufSize <= leftSize)
        {
            if(bufSize <= rightSize)
            {
                part1 = bufSize;
            }
            else
            {
                part1 = rightSize;
            }
        }
        else
        {
            part1 = rightSize;
            outChange = 1;
        }
    }
    else
    {
        leftSize = pObj->rIdx - pObj->wIdx;
        rightSize = pObj->total - pObj->wIdx;
        if(bufSize <= rightSize)
        {
            if(bufSize <= leftSize)
            {
                part1 = bufSize;
            }
            else
            {
                part1 = bufSize;
                outChange = 1;
            }
        }
        else
        {
            part1 = rightSize;
            outChange = 1;
        }

    }

    /*  */
    memcpy(pObj->buffer + pObj->wIdx, buf, part1);
    memcpy(pObj->buffer, buf + part1, bufSize - part1);
    
    /* 修改读写指针 */    
    pObj->wIdx = (pObj->wIdx + bufSize) % (pObj->total);
    if(outChange){
        pObj->rIdx = pObj->wIdx;
    }
    pObj->wLen += bufSize;
    if(pObj->wLen > pObj->total)
    {
        pObj->wLen = pObj->total;
    }

    return 0; 
}


static int RingBufGet(RingBuf *pObj, char *buf, int getSize)
{
    int rightSize = 0;
    int part1 = 0;
    
    if(getSize > pObj->wLen)
    {
        printf("Not enough data\n");
        return -1;
    }

    rightSize = pObj->total - pObj->rIdx;

    if(getSize > rightSize)
    {
        part1 = rightSize;
    }
    else
    {
        part1 = getSize;
    }
    
    memcpy(buf, pObj->buffer + pObj->rIdx, part1);
    memcpy(buf + part1, pObj->buffer, getSize - part1);
    
    pObj->rIdx = (pObj->rIdx + getSize) % pObj->total;
    pObj->wLen -= getSize;

    return 0;
}


RingBuf* CreateRingBuf(unsigned int bufSize)
{
    RingBuf *pObj = NULL;
   
    pObj = (RingBuf *)malloc(sizeof(*pObj));
    if (pObj == NULL)
    {
        return NULL;
    }
    
    
    pObj->rIdx = 0;
    pObj->wIdx = 0;
    pObj->wLen = 0;
    pObj->total = bufSize;
    pObj->buffer = NULL;
    pObj->get = NULL;
    pObj->put = NULL;

    pObj->buffer = (char *)malloc(bufSize);
    if (pObj->buffer == NULL)
    {
        free(pObj);
        return NULL;
    }
    
    memset(pObj->buffer, 0, bufSize);

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
