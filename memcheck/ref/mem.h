/*
*******************************************************************************************
*
*                             Heap Of The Abstract OS Layer
*                                   抽象操作系统层的堆
*
* File : MyOSHeap.h
* By   : Lin Shijun(http://blog.csdn.net/lin_strong)
* Date:  2019/09/08
* version: V1.0
* NOTE(s):a. This file is to adapt default heap interfaces(malloc/calloc/realloc/free) to
*     thread safe interfaces, which use MyOSMutex as lock.
*         b. force include this file to redirect all the memory interfaces
*         c. the malloc shouldn't be a macro in stdlib.h
*********************************************************************************************
*/

#ifndef _MYOSHEAP_H
#define _MYOSHEAP_H

#include <stdlib.h>
#include <stdbool.h>

#define malloc MyOSHeap_malloc
#define calloc MyOSHeap_calloc
#define realloc MyOSHeap_realloc
#define free MyOSHeap_free

// see the corresponding interface in stdlib.h but thread-safe
void *MyOSHeap_malloc(size_t _Size);
void *MyOSHeap_realloc(void *_Memory, size_t _NewSize);
void *MyOSHeap_calloc(size_t _Count, size_t _Size);
void MyOSHeap_free(void *_Memory);

// for free the memory used by this module, not thread-safe.
void MyOSHeap_Destroy(void);
#endif
