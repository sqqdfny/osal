/**
 * @file osal.c
 * @brief 消息队列管理,每个任务有一个独立的消息队列
 *        消息队列使用了osal_mem.c提供的接口函数,共用了相关的数据结构.
 * @date 2021-06-09
 * @author sqqdfny
 */

#include "osal_msg_queue.h"
#include "osal_mem.h"
#include "osal.h"
#include "osal_typedef.h"
#include "osal_config.h"
#include "../hal/osal_hal.h"



#define OSAL_MSG_DEBUG_EN 
#undef OSAL_MSG_DEBUG_EN

#ifdef OSAL_MSG_DEBUG_EN
	#define OsalMemDebugPrintf(fmt, args...)  OsalDebugInfo(fmt, ##args)
#else
	#define OsalMemDebugPrintf(...)
#endif //OSAL_MSG_DEBUG_EN

typedef osal_mem_head_t osal_msg_t;

extern osal_tcb_t g_osal_tcb_list[OSAL_TASK_TOTAL];
extern osal_task_id_t g_cur_task_total;
//==================================================================================================
void * OsalMsgQueueAlloc(size_t size)
{
    OsalMemDebugPrintf("%s: 0x%08lx\n", __func__, size);
    return OsalMemAlloc(size);
}

void OsalMsgQueueFree(void *p)
{
    OsalMemFree(p);
}

OSAL_ERR_T OsalMsgQueuePost(osal_task_id_t task_id, void *pMsg)
{
    osal_msg_t *pqueue;
    osal_msg_t *pmsg = (osal_msg_t*)pMsg;
    if(task_id >= g_cur_task_total || NULL == pMsg) return OSAL_ERR_INVALID_HANDLE;

    OsalEnterCritical();
    pmsg --;
    pmsg->mem.next = NULL;
    OsalMemDebugPrintf("%s: 0x%08x 0x%08x 0x%08x\n", __func__, (uint32_t)(g_osal_tcb_list[task_id].queue), (uint32_t)(pmsg), (uint32_t)(pmsg + 1));
    if(NULL == g_osal_tcb_list[task_id].queue)
    {
        g_osal_tcb_list[task_id].queue = pmsg;
        g_osal_tcb_list[task_id].events |= OSAL_EVENT_MSG;
        OsalExitCritical();
        return OSAL_ERR_SUCC;
    }

    pqueue = g_osal_tcb_list[task_id].queue;
    if(pqueue->mem.next)
    {
        do{
            pqueue = (osal_msg_t*)(pqueue->mem.next);
        }while(pqueue->mem.next);
    }
    pqueue->mem.next = (void*)pmsg;
    g_osal_tcb_list[task_id].events |= OSAL_EVENT_MSG;
    OsalExitCritical();
    return OSAL_ERR_SUCC;
}

void* OsalMsgQueuePend(osal_task_id_t task_id)
{
    osal_msg_t *pmsg;
    if(task_id >= g_cur_task_total || NULL == g_osal_tcb_list[task_id].queue) return NULL;

    OsalEnterCritical();
    pmsg = g_osal_tcb_list[task_id].queue;
    g_osal_tcb_list[task_id].queue = (osal_msg_t*)(pmsg->mem.next);
    pmsg->mem.next = NULL;
    OsalExitCritical();

    OsalMemDebugPrintf("%s: 0x%08x 0x%08x 0x%08x\n", __func__, (uint32_t)(g_osal_tcb_list[task_id].queue), (uint32_t)(pmsg), (uint32_t)(pmsg + 1));
    return (pmsg + 1);
}

void OsalMsgQueueinit(osal_task_id_t task_id)
{
#if (0)
    if(task_id >= OSAL_TASK_TOTAL) return;
    g_osal_tcb_list[task_id].queue = NULL;
#endif
}
//==================================================================================================
//end files
