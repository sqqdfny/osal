


#ifndef __OSAL_TASK_H__
#define __OSAL_TASK_H__

#include "osal_typedef.h"
#include "osal_timer.h"
#include "osal_mem.h"
#include "osal_list.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define OSAL_VERSION    "V2.0"

#define OSAL_MSG_POOL_SIZE      32
 
#define OSAL_MSG_TASK_CREATED   0     
#define OSAL_MSG_USR_BASE       10
//==================================================================================================
typedef struct osal_tcb
{
    struct list_head list;          //used for task manager
    struct list_head msgQueue;      //used for task massge queue
    void (*pFun)(osal_msg_cmd_t cmd, void *param);
}osal_tcb_t;
//==================================================================================================
#define INIT_OSAL_TCB(tcb, fun)  do{ (tcb)->pFun = fun; }while(0)

OSAL_ERR_T OsalPostMsg(osal_tcb_t *pTcb, osal_msg_cmd_t cmd, void *param);
#define OsalPostMsgIsr(tcb, param)  OsalPostMsg(tcb, param)

void OsalAddTask(osal_tcb_t *pTcb);
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
