/*
*******************************************************************************************
*
*                             Heap Of The Abstract OS Layer
*                                   抽象操作系统层的堆
*
* File : MyOSHeap.c
* By   : Lin Shijun(http://blog.csdn.net/lin_strong)
* Date:  2019/09/08
* version: V1.0
* NOTE(s): 
*********************************************************************************************
*/

/*
******************************************************************************************
*                                     INCLUDES
******************************************************************************************
*/

#include "mem.h"
#include <stdio.h>

// resume malloc define
#undef malloc
#undef calloc
#undef realloc
#undef free

/*
******************************************************************************************
*                             INTERFACE IMPLEMENTATIONS
******************************************************************************************
*/

void *MyOSHeap_malloc(size_t _Size)
{
    void *ret = NULL;
    ret = malloc(_Size);
    printf("malloc\n");
    return ret;
}

void *MyOSHeap_realloc(void *_Memory, size_t _NewSize)
{
    void *ret = NULL;
    ret = realloc(_Memory, _NewSize);
    printf("realloc\n");
    return ret;
}

void *MyOSHeap_calloc(size_t _Count, size_t _Size)
{
    void *ret = NULL;
    ret = calloc(_Count, _Size);
    printf("calloc\n");
    return ret;
}

void MyOSHeap_free(void *_Memory)
{
    free(_Memory);
    printf("free\n");
}
