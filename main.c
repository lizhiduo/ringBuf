#include <stdio.h>
#include <string.h>

#define MaxBuffSize 256

typedef struct tagRingBuf{
    unsigned int in;                    //写入地址  
    unsigned int out;                     //读取地址  
    unsigned int count;                 //有效未读数据大小  
                                                //读缓冲相关
    unsigned int size;                          //数据缓存区大小
    char buffer[MaxBuffSize];          //数据缓存区 
}RingBuf;

static RingBuf RingBuf1;

int buf_put(RingBuf *pObj, const char*buf, int bufSize)
{
    int leftSize = 0;
    int rightSize = 0;
    int part1    = 0;
    char outChange = 0;

    if(pObj->in >= pObj->out)
    {
        leftSize = pObj->size - pObj->in + pObj->out;
        rightSize = pObj->size - pObj->in;

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
        leftSize = pObj->out - pObj->in;
        rightSize = pObj->size - pObj->in;
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
    memcpy(pObj->buffer + pObj->in, buf, part1);
    memcpy(pObj->buffer, buf, bufSize - part1);
    
    /* 修改读写指针 */    
    pObj->in = (pObj->in + bufSize) % (pObj->size);
    if(outChange){
        pObj->out = pObj->in;
    }
    pObj->count += bufSize;
    if(pObj->count > pObj->size)
    {
        pObj-> count = pObj->size;
    }

    return 0; 
}

int main()
{
    

    return 0;
}
