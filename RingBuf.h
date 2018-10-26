#ifndef __RING_BUF_H__
#define __RING_BUG_H__

typedef unsigned char ElementType;

typedef struct tagRingBuf RingBuf;
struct tagRingBuf{
    unsigned int  in;             //写索引
    unsigned int  out;             //读索引
    unsigned int  size;            //数据缓存区大小
    ElementType   *buffer;          //数据缓存

    unsigned int (*get) (RingBuf *pObj, char *buf, unsigned int getSize);
    unsigned int (*put) (RingBuf *pObj, const char *buf, unsigned int bufSize);
};

extern RingBuf* CreateRingBuf(unsigned int bufSize);
extern int ReleaseRingBuf(RingBuf *pObj);
#endif
