

#ifndef __OSAL_MEM_H__
#define __OSAL_MEM_H__

#include "osal.h"

#ifdef __cplusplus
extern "C"
{
#endif
//==================================================================================================

uint32_t OsalGetFreeHeapSize(void);
void * OsalMemAlloc(uint32_t size_req);
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