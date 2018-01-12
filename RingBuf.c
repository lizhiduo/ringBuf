#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "RingBuf.h"

static int RingBufPut(RingBuf *pObj, const char*buf, int bufSize)
{
    unsigned int part1        = 0;
    unsigned int part2        = 0;
    unsigned int w            = 0;

    if (pObj == NULL || (bufSize > pObj->total))
    {
        return -1;
    }
    
    w = pObj->wIdx;
    part1 = pObj->total - w; 

    if (bufSize <= part1)
    {
        part1 = bufSize;
    }
    part2 = bufSize - part1;
    

    memcpy(pObj->buffer + w, buf, part1);
    memcpy(pObj->buffer, buf + part1, part2);

    pObj->wIdx = (pObj->wIdx + bufSize) % pObj->total;
    pObj->wLen += bufSize;
    if(pObj->wLen > pObj->total)
    {
        pObj->wLen = pObj->total;
        pObj->rIdx = pObj->wIdx;
    }
    
    return 0; 
}


static int RingBufGet(RingBuf *pObj, char *buf, int getSize)
{
    unsigned int r     = 0;
    unsigned int part1 = 0;
    unsigned int part2 = 0;

    if (pObj == NULL || (getSize > pObj->wLen))
    {
        return -1;
    }
    
    r = pObj->rIdx;
    part1 = pObj->total - r;

    if (getSize <= part1)
    {
        part1 = getSize;
    }
    part2 = getSize - part1;

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
