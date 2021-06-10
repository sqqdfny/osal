


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
#ifdef __cplusplus
}
#endif
#endif //__OSAL_TASK_H__
//==================================================================================================
//end files
