

#ifndef __OSAL_MEM_H__
#define __OSAL_MEM_H__

#include "osal.h"

#ifdef __cplusplus
extern "C"
{
#endif

//==================================================================================================
#define HEAP_ALIGNMENT_SIZE				4
#define HEAP_HEAD_ALIGNMENT_SIZE        ((sizeof(osal_mem_t) < HEAP_ALIGNMENT_SIZE) ? HEAP_ALIGNMENT_SIZE : ((sizeof(osal_mem_t) + HEAP_ALIGNMENT_SIZE - 1) & ~(HEAP_ALIGNMENT_SIZE - 1)))
  
typedef struct OSAL_MEM_T
{
	struct OSAL_MEM_T *next;
	size_t size;
}osal_mem_t;

typedef union
{
	osal_mem_t mem;
	uint8_t reserved[HEAP_HEAD_ALIGNMENT_SIZE];
}osal_mem_head_t;
//==================================================================================================
void * OsalMemAlloc(size_t size_req);
void OsalMemFree(void* p);

/**
 * @brief 内存管理初始化,传入的addr要注意对齐的问题
 */
void OsalMemInit(uint32_t * addr, size_t size_bytes);
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_MEM_H__
//==================================================================================================
//end files