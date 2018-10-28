#ifndef __RING_BUF_H__
#define __RING_BUG_H__

typedef struct tagRingBuf RingBuf;
struct tagRingBuf{
    unsigned int  in;             //写索引
    unsigned int  out;             //读索引
    unsigned int  size;            //数据缓存区大小
    void   *buffer;          //数据缓存

    unsigned int (*get) (RingBuf *pObj, char *buf, unsigned int getSize);
    unsigned int (*put) (RingBuf *pObj, const char *buf, unsigned int bufSize);
};

/*****************************************************************************
*   Prototype    : CreateRingBuf
*   Description  :  
*   Input        : unsigned int bufSize  
*   Output       : None
*   Return Value : extern RingBuf*
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2018/10/26
*           Author       : lizhiduo
*           Modification : Created function
*
*****************************************************************************/
extern RingBuf* CreateRingBuf(unsigned int bufSize);

/*****************************************************************************
*   Prototype    : ReleaseRingBuf
*   Description  :  
*   Input        : RingBuf *pObj  
*   Output       : None
*   Return Value : extern int
*   Calls        : 
*   Called By    : 
*
*   History:
* 
*       1.  Date         : 2018/10/26
*           Author       : lizhiduo
*           Modification : Created function
*
*****************************************************************************/
extern int ReleaseRingBuf(RingBuf *pObj);
#endif
