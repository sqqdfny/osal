/**
 * @file osal_mem.c
 * @brief 内存管理,支持指定字节对齐,回收时会将相邻的内存块合并
 * @date 2021-06-09
 * @author sqqdfny
 */

#include "osal_typedef.h"
#include "osal_config.h"
#include "../hal/osal_hal.h"


#define OSAL_MEM_DEBUG_EN 
#undef OSAL_MEM_DEBUG_EN
//===================================================================

#define OsalMemEnterCritical()			  OsalEnterCritical()		
#define OsalMemExitCritical()			  OsalExitCritical()		
				
#define HEAP_ALIGNMENT				16
#define HEAP_ALIGNMENT_SIZE         ((sizeof(osal_mem_t) < HEAP_ALIGNMENT) ? HEAP_ALIGNMENT : ((sizeof(osal_mem_t) + HEAP_ALIGNMENT - 1) & ~(HEAP_ALIGNMENT - 1)))

typedef struct OSAL_MEM_T
{
	struct OSAL_MEM_T *next;
	size_t size;
}osal_mem_t;

typedef union
{
	osal_mem_t mem;
	uint8_t reserved[HEAP_ALIGNMENT_SIZE];
}osal_mem_head_t;


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

#if (0)
osal info:OsalMemInit: 0x006030c0 0x006030d0
Init hal timer ok !

osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x006030c0 0x00003ff0 0x00000000
osal info:OsalMemAlloc: 0x006030c0 0x006030e0 0x0010
osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x006030e0 0x00003fd0 0x00000000
osal info:OsalMemAlloc: 0x006030e0 0x00603100 0x0010
osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x00603100 0x00003fb0 0x00000000
osal info:OsalMemAlloc: 0x00603100 0x00603120 0x0010
osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x00603120 0x00003f90 0x00000000
osal info:OsalMemAlloc: 0x00603120 0x00603140 0x0010

osal info:OsalMemFree: 0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000

osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000
osal info:OsalMemAlloc: 0x00603100 0x00603140 0x0010

osal info:OsalMemFree: 0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000

osal info:OsalMemFree: 0x00603120 0x00000010 0x00603100
osal info:OsalMemPrintList:0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000
osal info:OsalMemFree: merge prev 0x00603100 0x00000010 0x00603120
osal info:OsalMemFree: merge next 0x00603100 0x00000030 0x00603140

osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x00603100 0x00003fb0 0x00000000
osal info:OsalMemAlloc: 0x00603100 0x00603120 0x0010

osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x00603120 0x00003f90 0x00000000
osal info:OsalMemAlloc: 0x00603120 0x00603140 0x0010

osal info:OsalMemFree: 0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000

osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000
osal info:OsalMemAlloc: 0x00603100 0x00603140 0x0010

osal info:OsalMemFree: 0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000

osal info:OsalMemFree: 0x00603120 0x00000010 0x00603100
osal info:OsalMemPrintList:0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000
osal info:OsalMemFree: merge prev 0x00603100 0x00000010 0x00603120
osal info:OsalMemFree: merge next 0x00603100 0x00000030 0x00603140

osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x00603100 0x00003fb0 0x00000000
osal info:OsalMemAlloc: 0x00603100 0x00603120 0x0010

osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x00603120 0x00003f90 0x00000000
osal info:OsalMemAlloc: 0x00603120 0x00603140 0x0010

osal info:OsalMemFree: 0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000

osal info:OsalMemFree: 0x006030c0 0x00000010 0x00603100
osal info:OsalMemPrintList:0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000

osal info:OsalMemFree: 0x00603120 0x00000010 0x006030c0
osal info:OsalMemPrintList:0x006030c0 0x00000010 0x00603100
osal info:OsalMemPrintList:0x00603100 0x00000010 0x00603140
osal info:OsalMemPrintList:0x00603140 0x00003f70 0x00000000
osal info:OsalMemFree: merge prev 0x00603100 0x00000010 0x00603120
osal info:OsalMemFree: merge next 0x00603100 0x00000030 0x00603140

osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x006030c0 0x00000010 0x00603100
osal info:OsalMemPrintList:0x00603100 0x00003fb0 0x00000000
osal info:OsalMemAlloc: 0x006030c0 0x00603100 0x0010

osal info:OsalMemFree: 0x006030c0 0x00000010 0x00603100
osal info:OsalMemPrintList:0x00603100 0x00003fb0 0x00000000

osal info:OsalMemAlloc: size_req=0x0004
osal info:OsalMemPrintList:0x006030c0 0x00000010 0x00603100
osal info:OsalMemPrintList:0x00603100 0x00003fb0 0x00000000
osal info:OsalMemAlloc: 0x006030c0 0x00603100 0x0010

osal info:OsalMemFree: 0x006030c0 0x00000010 0x00603100
osal info:OsalMemPrintList:0x00603100 0x00003fb0 0x00000000

osal info:OsalMemFree: 0x006030e0 0x00000010 0x006030c0
osal info:OsalMemPrintList:0x006030c0 0x00000010 0x00603100
osal info:OsalMemPrintList:0x00603100 0x00003fb0 0x00000000
osal info:OsalMemFree: merge prev 0x006030c0 0x00000010 0x006030e0
osal info:OsalMemFree: merge next 0x006030c0 0x00000030 0x00603100

osal info:OsalMemAlloc: size_req=0x0064
osal info:OsalMemPrintList:0x006030c0 0x00003ff0 0x00000000
osal info:OsalMemAlloc: 0x006030c0 0x00603140 0x0070
#endif 
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
			prev = p;
			p = (osal_mem_head_t*)(prev->mem.next);
		}
	}while(NULL != p);
	OsalMemExitCritical();
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
		g_pMemHead = p;
		if((g_pMemHead->mem.size + sizeof(osal_mem_head_t)) == (((uint8_t*)(g_pMemHead->mem.next)) - ((uint8_t*)g_pMemHead)))
		{
			OsalMemDebugPrintf("%s: merge head 0x%08lx 0x%08lx 0x%08lx\n", __func__, (size_t)(g_pMemHead), g_pMemHead->mem.size, (size_t)(p));
			g_pMemHead->mem.next = g_pMemHead->mem.next->next;
			g_pMemHead->mem.size = g_pMemHead->mem.size + sizeof(osal_mem_head_t);
		}
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
