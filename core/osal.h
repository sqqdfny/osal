


#ifndef __OSAL_TASK_H__
#define __OSAL_TASK_H__

#include "osal_typedef.h"
#include "osal_timer.h"

#ifdef __cplusplus
extern "C"
{
#endif
//==================================================================================================

typedef uint32_t (*OsalTaskFun_p)(osal_task_id_t task_id);

OSAL_ERR_T OsalEventSet(osal_task_id_t task_id, osal_event_t events);
OSAL_ERR_T OsalEventClear(osal_task_id_t task_id, osal_event_t events);

OSAL_ERR_T OsalAddTask(osal_task_id_t *pTaskId, osal_event_t (*pProcess)(osal_task_id_t task_id, osal_event_t events));
void OsalStartSystem(void);
void OsalInitSystem(void);
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_TASK_H__
//==================================================================================================
//end files