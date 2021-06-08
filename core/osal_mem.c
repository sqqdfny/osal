
#include "osal_typedef.h"
#include "osal_config.h"
#include "../hal/osal_hal.h"



//===================================================================

#define OsalMemEnterCritical()			  OsalEnterCritical()		
#define OsalMemExitCritical()			  OsalExitCritical()		
#define OsalMemDebugPrintf(fmt, args...)  OsalDebugInfo(fmt, ##args)
				
#define HEAP_ALIGNMENT				8

typedef struct OSAL_MEM_T
{
	struct OSAL_MEM_T *next;
	size_t size;
}osal_mem_t;

typedef union
{
	osal_mem_t mem;
	uint8_t reserved[sizeof(osal_mem_t) > HEAP_ALIGNMENT ? ((sizeof(osal_mem_t) + HEAP_ALIGNMENT - 1) & ~(HEAP_ALIGNMENT - 1)) : HEAP_ALIGNMENT];
}osal_mem_head_t;


static osal_mem_head_t *g_pMemHead;
//==================================================================================================
void * OsalMemAlloc(size_t size_req)
{
	osal_mem_head_t *p, *prev;

	OsalMemEnterCritical();
	p = g_pMemHead;
	prev = p;
	do{
		if(p->mem.size > (size_req + sizeof(osal_mem_head_t) * 2))
		{
			prev->mem.next = (osal_mem_t*)(((uint8_t*)(p + 1)) + size_req);
			prev->mem.next->next = p->mem.next;
			prev->mem.next->size = p->mem.size - (size_req + sizeof(osal_mem_head_t));
			p->mem.size = size_req;
			OsalMemExitCritical();
			OsalMemDebugPrintf("%s: 0x%08lx\n", __func__, (size_t)(p + 1));
			return (p + 1);
		}
		else
		if(p->mem.size >= (size_req + sizeof(osal_mem_head_t)))
		{
			prev->mem.next = p->mem.next;
			OsalMemExitCritical();
			OsalMemDebugPrintf("%s: 0x%08lx\n", __func__, (size_t)(p + 1));
			return (p + 1);
		}
		else
		{
			if(p != g_pMemHead)
			{
				prev = p;
			}
			p = (osal_mem_head_t*)(p->mem.next);
		}
	}while(NULL != p->mem.next);
	OsalMemExitCritical();
	return NULL;
}

void OsalMemFree(void* pMem)
{
	osal_mem_head_t *p, *prev;
	p = (osal_mem_head_t*)pMem;
	p --;
    OsalMemDebugPrintf("%s: 0x%08lx 0x%08lx  0x%08lx\n", __func__, (size_t)(p), p->mem.size, (size_t)(g_pMemHead));
	OsalMemEnterCritical();
	if(p < g_pMemHead)
	{
		p->mem.next = g_pMemHead->mem.next;
		p->mem.size += g_pMemHead->mem.size + sizeof(osal_mem_head_t);
		g_pMemHead = p;
		OsalMemExitCritical();
		return;
	}

    prev = g_pMemHead;
	do{
		if(p < (osal_mem_head_t*)(prev->mem.next))
		{
			p->mem.next = prev->mem.next;
			prev->mem.next = (osal_mem_t*)p;
			if(prev->mem.size == (((uint8_t*)p) - ((uint8_t*)prev)))
			{
				prev->mem.size += p->mem.size + sizeof(osal_mem_head_t);
				prev->mem.next = p->mem.next;
			}

			if(prev->mem.size == (((uint8_t*)prev->mem.next) - ((uint8_t*)prev)))
			{
				prev->mem.size += prev->mem.next->size + sizeof(osal_mem_head_t);
				prev->mem.next = prev->mem.next->next;
			}
			OsalMemExitCritical();
			return;
		}
		prev = (osal_mem_head_t*)prev->mem.next;
	}while(NULL != prev);
	OsalMemExitCritical();
	OsalDebugErr("%s err\n", __func__);
}

/**
 * @brief 内存管理初始化,
 *        传入的addr要注意对齐的问题,必须按HEAP_ALIGNMENT对齐
 */
void OsalMemInit(uint32_t * addr, size_t size_bytes)
{	
    if(NULL == addr) return;
	g_pMemHead = (osal_mem_head_t *)addr;
	g_pMemHead->mem.next = NULL;
	g_pMemHead->mem.size = size_bytes - sizeof(osal_mem_head_t);
	OsalMemDebugPrintf("%s: 0x%08lx 0x%08lx\n", __func__, (size_t)(addr), (size_t)(g_pMemHead + 1));
}
//==================================================================================================
//end files
