#ifndef __RING_BUF_H__
#define __RING_BUG_H__

typedef struct tagRingBuf RingBuf;
struct tagRingBuf{
    unsigned int  wIdx;                //写索引  
    unsigned int  rIdx;               //读索引  
    unsigned int  wLen;             //有效未读数据大小  
    unsigned int  total;            //数据缓存区大小
    unsigned char *buffer;          //数据缓存区

    int (*get) (RingBuf *pObj, char *buf, int getSize);
    int (*put) (RingBuf *pObj, const char*buf, int bufSize);
};

extern RingBuf* CreateRingBuf(unsigned int bufSize);
extern int ReleaseRingBuf(RingBuf *pObj);
#endif
