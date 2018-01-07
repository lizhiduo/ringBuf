#include <stdio.h>
#include <string.h>

#define MaxBuffSize 256

typedef struct tagRingBuf{
    unsigned int in;                    //写入地址  
    unsigned int out;                     //读取地址  
    unsigned int count;                 //有效未读数据大小  
                                                //读缓冲相关
    unsigned int size;                          //数据缓存区大小
    unsigned char buffer[MaxBuffSize];          //数据缓存区 
}RingBuf;

static RingBuf ringBuf1;

void show_char(const unsigned char *buf, int count);

int init_buf()
{
    ringBuf1.in = 0;
    ringBuf1.out = 0;
    ringBuf1.count = 0;
    ringBuf1.size = MaxBuffSize;
    
    memset(ringBuf1.buffer, 0, MaxBuffSize);

    return 0;
}


int buf_get(RingBuf *pObj, char *buf, int getSize)
{
    int rightSize = 0;
    int part1 = 0;
    
    if(getSize > pObj->count)
    {
        printf("Not enough data\n");
        return -1;
    }

    rightSize = pObj->size - pObj->out;

    if(getSize > rightSize)
    {
        part1 = rightSize;
    }
    else
    {
        part1 = getSize;
    }
    
    memcpy(buf, pObj->buffer + pObj->out, part1);
    memcpy(buf + part1, pObj->buffer, getSize - part1);
    
    pObj->out = (pObj->out + getSize) % pObj->size;
    pObj->count -= getSize;

    return 0;
}

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
    memcpy(pObj->buffer, buf + part1, bufSize - part1);
    
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
    
    //show_char(pObj->buffer, 256);
    //printf("in:%d out:%d\n", pObj->in, pObj->out);

    return 0; 
}

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

int main()
{
    unsigned char buf[5];
    unsigned char i = 1;
    unsigned char rdat[16];
    
    int stop = 0;

    init_buf();

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
        
        buf_put(&ringBuf1, buf, 5);

        if(ringBuf1.count >= 16)
        {
            buf_get(&ringBuf1, rdat, 16);
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

    return 0;
}

