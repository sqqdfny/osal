
#include "osal_typedef.h"
#include "osal_config.h"
#include "../hal/osal_hal.h"



typedef osal_ucpu_t portHeap;
//===================================================================

#define HeapEnterCritical()			OsalEnterCritical()		
#define HeapExitCritical()			OsalExitCritical()		
				
#define HEAP_ALIGNMENT				4
#define HEAP_ALIGNMENT_MASK			0x00000003UL

typedef struct HEAP
{
	struct HEAP * pNext;
	portHeap size;
}struct_HEAP;

static struct_HEAP *g_pHeapStart;
static uint32_t g_freeSize = 0;
//==================================================================================================
uint32_t OsalGetFreeHeapSize(void)
{
	return (g_freeSize);
}

void * OsalMemAlloc(uint32_t size_req)
{
	struct_HEAP *pHeap1, *pHeap2, *pHeap3;
	void *pReturn;
	
	if(0 == size_req) return NULL;
	
	HeapEnterCritical();
	
	size_req = (size_req + HEAP_ALIGNMENT - 1) & (~HEAP_ALIGNMENT_MASK);
	
	pHeap1 = g_pHeapStart;
	pHeap3 = g_pHeapStart;
	
	while (1)
	{
		if(pHeap1->size >= size_req)
		{//当前的空闲块 >= 请求的块
			pReturn = (void*)pHeap1;
			if(pHeap1->size > (size_req + sizeof(struct_HEAP)))
			{
				pHeap2 = (struct_HEAP*)( ((portHeap)pHeap1) + size_req + HEAP_ALIGNMENT);
				pHeap2->pNext = pHeap1->pNext;
				pHeap2->size = pHeap1->size - size_req - HEAP_ALIGNMENT;
				pHeap3->pNext = pHeap2;
				
				if(pReturn == (void*)g_pHeapStart)
				{
					g_pHeapStart = pHeap2;
				}
				
				*((portHeap*)pReturn) = size_req;
				g_freeSize = g_freeSize - (size_req + sizeof(struct_HEAP));
				pReturn = (void*)(((portHeap)pReturn) + HEAP_ALIGNMENT);
			}
			else
			{
				pHeap3->pNext = pHeap1->pNext;
	
				if(pReturn == (void*)g_pHeapStart)
				{
					g_pHeapStart = pHeap3->pNext;
				}
				
				*((portHeap*)pReturn) = pHeap1->size;
				g_freeSize = g_freeSize - ((pHeap1->size) + sizeof(struct_HEAP));
				pReturn = (void*)( ((portHeap)pReturn) + HEAP_ALIGNMENT);
			}
			
			HeapExitCritical();
			return pReturn; 
		}
			
		if(NULL == pHeap1->pNext) 
		{//已经到最后一个空闲的块
			HeapExitCritical();
			OsalDebugErr("%s\n", __func__);
			return NULL;
		} 
		
		pHeap3 = pHeap1;
		pHeap1 = pHeap1->pNext;		//查找下一个空闲块
	}
}

void OsalMemFree(void* p)
{
	struct_HEAP *pHeap1, *pHeap2, *pHeap3;
	
	if(NULL == p) return;
	
	HeapEnterCritical();	
	
	p = (void*)( ((portHeap)p) - HEAP_ALIGNMENT);
	g_freeSize = g_freeSize + (((struct_HEAP*)p)->size) + sizeof(struct_HEAP);
	if(p < (void*)g_pHeapStart) 
	{
		pHeap1 = (struct_HEAP*)p;
		pHeap3 = g_pHeapStart;
		pHeap1->size = *((portHeap*)p);
		if( ( ((portHeap)pHeap1) + pHeap1->size + HEAP_ALIGNMENT) == (portHeap)pHeap3)
		{
			pHeap1->pNext = pHeap3->pNext;
			pHeap1->size += pHeap3->size + HEAP_ALIGNMENT;
		}
		else
		{
			pHeap1->pNext = pHeap3;
		}
		g_pHeapStart = pHeap1;
	}
	else
	{
		pHeap1 = g_pHeapStart;
		pHeap3 = g_pHeapStart;
		
		while(p > (void*)pHeap1)
		{
			if(NULL == pHeap1->pNext) 
			{
			    OSAL_HAL_REBOOT();
			}
			pHeap3 = pHeap1;
			pHeap1 = pHeap1->pNext;		//查找下一个空闲块
		}
	
		pHeap2 = (struct_HEAP*)p;
		pHeap2->size = *((portHeap*)p);
		if( ( ((portHeap)pHeap2) + pHeap2->size + HEAP_ALIGNMENT) == ((portHeap)pHeap1) )
		{
			pHeap2->size += pHeap1->size + HEAP_ALIGNMENT;
			pHeap2->pNext = pHeap1->pNext;
		}
		else
		{
			pHeap2->pNext = pHeap1;
		}
		
		if( ( ((portHeap)pHeap3) + pHeap3->size + HEAP_ALIGNMENT) == ((portHeap)pHeap2) )
		{
			pHeap3->size += pHeap2->size + HEAP_ALIGNMENT;
			pHeap3->pNext = pHeap2->pNext;
		}
		else
		{
			pHeap3->pNext = pHeap2;
		}
	}
	HeapExitCritical();
}

/**
 * @brief 内存管理初始化,传入的addr要注意对齐的问题
 */
void OsalMemInit(uint32_t * addr, size_t size_bytes)
{	
    if(NULL == addr) return;
	g_pHeapStart = (struct_HEAP *)addr;
	g_pHeapStart->pNext = NULL;
	g_pHeapStart->size = size_bytes - HEAP_ALIGNMENT;
	g_freeSize = g_pHeapStart->size;
}
//==================================================================================================
//end files
