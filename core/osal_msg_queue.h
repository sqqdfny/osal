
#ifndef __OSAL_MSG_QUEUE_H__
#define __OSAL_MSG_QUEUE_H__

#include "osal_typedef.h"


#ifdef __cplusplus
extern "C"
{
#endif
//==================================================================================================
void * OsalMsgQueueAlloc(size_t size);
void OsalMsgQueueFree(void *p);
OSAL_ERR_T OsalMsgQueuePost(osal_task_id_t task_id, void *pMsg);
void* OsalMsgQueuePend(osal_task_id_t task_id);
void OsalMsgQueueinit(osal_task_id_t task_id);
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_MSG_QUEUE_H__
//==================================================================================================
//end files