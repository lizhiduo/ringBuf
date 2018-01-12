#ifndef __RING_BUF_H__
#define __RING_BUG_H__

#if 0
#define lenth  16
#define count  20

#define FORWARD(_idex, _total) ((_idex * FrameLen > _total) ? 0 : _idex)
#define GO(idex, total) (idex = FORWARD((idex + 1),  total))

typedef struct tag_A_RingBuf{
    unsigned int  wIdx;             //写索引  
    unsigned int  rIdx;             //读索引
    ElementType   buf[lenth * count];
}A_RingBuf;
#endif /* 0 */


typedef unsigned char ElementType;

typedef struct tagRingBuf RingBuf;
struct tagRingBuf{
    unsigned int  wIdx;             //写索引  
    unsigned int  rIdx;             //读索引  
    unsigned int  wLen;             //有效未读数据大小  
    unsigned int  total;            //数据缓存区大小
    ElementType   *buffer;          //数据缓存区

    int (*get) (RingBuf *pObj, char *buf, int getSize);
    int (*put) (RingBuf *pObj, const char*buf, int bufSize);
};

extern RingBuf* CreateRingBuf(unsigned int bufSize);
extern int ReleaseRingBuf(RingBuf *pObj);
#endif
