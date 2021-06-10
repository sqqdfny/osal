/**
 * @file osal_mem.c
 * @brief 内存管理,支持指定字节对齐,回收时会将相邻的内存块合并
 * @date 2021-06-09
 * @author sqqdfny
 */

#include "osal_typedef.h"
#include "osal_config.h"
#include "osal_mem.h"
#include "../hal/osal_hal.h"


#define OSAL_MEM_DEBUG_EN 
#undef OSAL_MEM_DEBUG_EN


#define OsalMemEnterCritical()		OsalEnterCritical()		
#define OsalMemExitCritical()		OsalExitCritical()		
				

static osal_mem_head_t *g_pMemHead;
//==================================================================================================
#ifdef OSAL_MEM_DEBUG_EN
	#define OsalMemDebugPrintf(fmt, args...)  OsalDebugInfo(fmt, ##args)
	static void OsalMemPrintList(void)
	{
		osal_mem_head_t *p;
		p = g_pMemHead;
		do{
			OsalMemDebugPrintf("%s:0x%08lx 0x%08lx 0x%08lx\n", __func__, (size_t)(p), p->mem.size, (size_t)(p->mem.next));
			p = (osal_mem_head_t*)(p->mem.next);
		}while(p);
	}
#else
	#define OsalMemDebugPrintf(...)
	#define OsalMemPrintList()
#endif //OSAL_MEM_DEBUG_EN
//==================================================================================================
void * OsalMemAlloc(size_t size_req)
{
	osal_mem_head_t *p, *prev;
    OsalMemDebugPrintf("%s: size_req=0x%04lx\n", __func__, size_req);
	size_req = (size_req + HEAP_ALIGNMENT_SIZE - 1) & ~(HEAP_ALIGNMENT_SIZE - 1);
	OsalMemEnterCritical();
	OsalMemPrintList();
	if(g_pMemHead->mem.size > (size_req + sizeof(osal_mem_head_t) * 2))
	{
		p = g_pMemHead;
		g_pMemHead = (osal_mem_head_t*)(((uint8_t*)(g_pMemHead + 1)) + size_req);
		g_pMemHead->mem.next = p->mem.next;
		g_pMemHead->mem.size = p->mem.size - (size_req + sizeof(osal_mem_head_t));
		p->mem.size = size_req;
		p->mem.next = NULL;
		OsalMemExitCritical();
		OsalMemDebugPrintf("%s: 0x%08lx 0x%08lx 0x%04lx\n", __func__, (size_t)(p), (size_t)(g_pMemHead), size_req);
		return (p + 1);
	}
	else
	if(g_pMemHead->mem.size >= (size_req))
	{
		p = g_pMemHead;
		g_pMemHead = (osal_mem_head_t*)(g_pMemHead->mem.next);
		p->mem.next = NULL
		OsalMemExitCritical();
		OsalMemDebugPrintf("%s: 0x%08lx 0x%08lx 0x%04lx\n", __func__, (size_t)(p), (size_t)(g_pMemHead), size_req);
		return (p + 1);
	}
	else
	{
	#if (0)
		if(NULL == g_pMemHead->mem.next)
		{
			OsalMemExitCritical();
			OsalMemDebugPrintf("%s: no mem\n", __func__);
			return NULL;
		}
	#endif 
	}


	prev = g_pMemHead;
	p = (osal_mem_head_t*)(prev->mem.next);
	do{
		if(p->mem.size > (size_req + sizeof(osal_mem_head_t) * 2))
		{
			prev->mem.next = (osal_mem_t*)(((uint8_t*)(p + 1)) + size_req);
			prev->mem.next->next = p->mem.next;
			prev->mem.next->size = p->mem.size - (size_req + sizeof(osal_mem_head_t));
			p->mem.size = size_req;
			p->mem.next = NULL;
			OsalMemExitCritical();
			OsalMemDebugPrintf("%s: 0x%08lx 0x%08lx 0x%04lx\n", __func__, (size_t)(p), (size_t)(g_pMemHead), size_req);
			return (p + 1);
		}
		else
		if(p->mem.size >= (size_req))
		{
			prev->mem.next = p->mem.next;
			p->mem.next = NULL;
			OsalMemExitCritical();
			OsalMemDebugPrintf("%s: 0x%08lx 0x%08lx 0x%04lx\n", __func__, (size_t)(p), (size_t)(g_pMemHead), size_req);
			return (p + 1);
		}
		else
		{
			OsalMemDebugPrintf("%s: next 0x%08lx 0x%08lx\n", __func__, (size_t)(prev), (size_t)(p));
			prev = p;
			p = (osal_mem_head_t*)(prev->mem.next);
		}
	}while(NULL != p);
	OsalMemExitCritical();
	OsalMemDebugPrintf("%s: no mem 1\n", __func__);
	return NULL;
}

void OsalMemFree(void* pMem)
{
	osal_mem_head_t *p, *prev;
	p = (osal_mem_head_t*)pMem;
	p --;
    OsalMemDebugPrintf("%s: 0x%08lx 0x%08lx 0x%08lx\n", __func__, (size_t)(p), p->mem.size, (size_t)(g_pMemHead));
	OsalMemEnterCritical();
	OsalMemPrintList();
	if(p < g_pMemHead)
	{
		p->mem.next = (osal_mem_t*)g_pMemHead;
		if((p->mem.size + sizeof(osal_mem_head_t)) == (((uint8_t*)(p->mem.next)) - ((uint8_t*)p)))
		{
			OsalMemDebugPrintf("%s: merge head 0x%08lx 0x%08lx 0x%08lx\n", __func__, (size_t)(p), p->mem.size, (size_t)(g_pMemHead));
			p->mem.next = g_pMemHead->mem.next;
			p->mem.size += g_pMemHead->mem.size + sizeof(osal_mem_head_t);
		}
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
			if((prev->mem.size + sizeof(osal_mem_head_t)) == (((uint8_t*)p) - ((uint8_t*)prev)))
			{
				OsalMemDebugPrintf("%s: merge prev 0x%08lx 0x%08lx 0x%08lx\n", __func__, (size_t)(prev), prev->mem.size, (size_t)(p));
				prev->mem.size += p->mem.size + sizeof(osal_mem_head_t);
				prev->mem.next = p->mem.next;
			}

			if((prev->mem.size + sizeof(osal_mem_head_t)) == (((uint8_t*)prev->mem.next) - ((uint8_t*)prev)))
			{
				OsalMemDebugPrintf("%s: merge next 0x%08lx 0x%08lx 0x%08lx\n", __func__, (size_t)(prev), prev->mem.size, (size_t)(prev->mem.next));
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
	uint8_t * addr_tmp;
    if(NULL == addr) return;
	
	addr_tmp = (uint8_t *)addr;
	
	if(((uint32_t)addr_tmp) & (HEAP_ALIGNMENT_SIZE - 1))
	{
		addr_tmp += HEAP_ALIGNMENT_SIZE - (((uint32_t)addr_tmp) & (HEAP_ALIGNMENT_SIZE - 1));
		size_bytes -= (((uint32_t)addr_tmp) & (HEAP_ALIGNMENT_SIZE - 1));
	}

	g_pMemHead = (osal_mem_head_t *)addr_tmp;
	g_pMemHead->mem.next = NULL;
	g_pMemHead->mem.size = size_bytes - sizeof(osal_mem_head_t);
	OsalMemDebugPrintf("%s: 0x%08lx 0x%08lx\n", __func__, (size_t)(addr), (size_t)(g_pMemHead + 1));
}
//==================================================================================================
//end files
