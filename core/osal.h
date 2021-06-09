


#ifndef __OSAL_TASK_H__
#define __OSAL_TASK_H__

#include "osal_typedef.h"
#include "osal_timer.h"
#include "osal_mem.h"
#include "osal_msg_queue.h"

#ifdef __cplusplus
extern "C"
{
#endif

//==================================================================================================
typedef struct 
{
    osal_event_t (*pProcess)(osal_task_id_t task_id, osal_event_t events);
    void *queue;
    osal_event_t events;
}osal_tcb_t;
//==================================================================================================
typedef uint32_t (*OsalTaskFun_p)(osal_task_id_t task_id);

OSAL_ERR_T OsalEventSet(osal_task_id_t task_id, osal_event_t events);
OSAL_ERR_T OsalEventClear(osal_task_id_t task_id, osal_event_t events);

OSAL_ERR_T OsalAddTask(osal_task_id_t *pTaskId, osal_event_t (*pProcess)(osal_task_id_t task_id, osal_event_t events));
void OsalStartSystem(void);


/**
 * @brief OSAL系统初始化,
 *        传入的参数为内存管理的内存块地址和SIZE
 *        传入的addr要注意对齐的问题
 *        如果不使用 msg_queue, 同时用户也不调用内存管理相关的函数, 可以传入NULL
 */
void OsalInitSystem(uint32_t * addr, size_t size_bytes);
//==================================================================================================


//==================================================================================================
/**
 * 以下为对外提供的接口清单
 * void * OsalMemAlloc(size_t size_req);
 * void OsalMemFree(void* p);
 * 
 * void * OsalMsgQueueAlloc(size_t size);
 * void OsalMsgQueueFree(void *p);
 * OSAL_ERR_T OsalMsgQueuePost(osal_task_id_t task_id, void *pMsg);
 * void* OsalMsgQueuePend(osal_task_id_t task_id);
 * 
 * OSAL_ERR_T OsalCreateTimer(osal_timer_t **ppTimer, osal_timer_type_t type, uint32_t timeout, void (*callback)(void *param), void *param);
 * void * OsalTimerGetParam(osal_timer_t *pTimer);
 * OSAL_ERR_T OsalDeleteTimer(osal_timer_t *pTimer);
 * OSAL_ERR_T OsalTimerStart(osal_timer_t *pTimer);
 * OSAL_ERR_T OsalTimerStop(osal_timer_t *pTimer);
 * OSAL_ERR_T OsalTimerReset(osal_timer_t *pTimer, osal_timer_type_t type, uint32_t timeout);
 * 
 * OSAL_ERR_T OsalEventSet(osal_task_id_t task_id, osal_event_t events);
 * OSAL_ERR_T OsalEventClear(osal_task_id_t task_id, osal_event_t events);
 * OSAL_ERR_T OsalAddTask(osal_task_id_t *pTaskId, osal_event_t (*pProcess)(osal_task_id_t task_id, osal_event_t events));
 * void OsalStartSystem(void);
 * void OsalInitSystem(uint32_t * addr, size_t size_bytes);
 */
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_TASK_H__
//==================================================================================================
//end files
