#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "RingBuf.h"

#define MIN(a, b) ((a)<(b))?(a):(b)

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
    unsigned int l = 0;

    bufSize = MIN(bufSize, pObj->size - pObj->in + pObj->out);
    
    l = MIN(bufSize, pObj->size - (pObj->in & (pObj->size - 1)));
    
    memcpy(pObj->buffer + (pObj->in & (pObj->size - 1)), buf, l);
    memcpy(pObj->buffer, buf + l, bufSize - l);
    
    pObj->in += bufSize;
    
    return bufSize;
}

static unsigned int RingBufGet(RingBuf *pObj, char *buf, unsigned int getSize)
{
    unsigned int l = 0;

    getSize = MIN(getSize, pObj->in - pObj->out);

    l = MIN(getSize, pObj->size - (pObj->out & (pObj->size - 1)));
    
    memcpy(buf, pObj->buffer + (pObj->out & (pObj->size - 1)), l);
    memcpy(buf + l, pObj->buffer, getSize - l);
    
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
    
    if (bufSize & (bufSize - 1)) 
    {
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
