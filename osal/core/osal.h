


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

#define OSAL_VERSION    "V2.1"
 
#define OSAL_MSG_TASK_CREATED   0     //这个消息由系统在创建任务时发送一次
#define OSAL_MSG_USR_BASE       10    //用户消息ID从这个数开始,前面为保留消息
//==================================================================================================
struct osal_tcb
{
    struct list_head list;          //used for task manager
    struct list_head msgQueue;      //used for task massge queue
    void (*pFun)(osal_msg_cmd_t cmd, void *param);
};


/**
*这个结构体必须嵌入要发送给任务的消息，而且必须为第一个元素
*/
typedef struct osal_msg_head
{
    struct list_head list;
    osal_msg_cmd_t cmd;
}osal_msg_head_t;  

#define OsalGetCurSystemTick() OsalHalGetCurSystemTick()
#define OsalMsToTick(ms)       (ms / OSAL_MS_PER_TICK)
//==================================================================================================
#define INIT_OSAL_TCB(tcb, fun)  do{ (tcb)->pFun = fun;}while(0)

OSAL_ERR_T OsalPostMsg(struct osal_tcb *pTcb, osal_msg_cmd_t cmd, void *param);
OSAL_ERR_T OsalPostMsgIsr(struct osal_tcb *pTcb, osal_msg_cmd_t cmd, void *param);

void OsalAddTask(struct osal_tcb *pTcb);
void OsalStartSystem(void);


/**
 * @brief OSAL系统初始化,
 *        传入的参数为内存管理的内存块地址和SIZE
 *        传入的addr要注意对齐的问题
 *        根据任务数量,size_bytes 最小为64
 *        发送系统保留消息(eg:OSAL_MSG_TASK_CREATED)时需要申请一个消息块(sizeof(osal_msg_head_t))
 *        如果没有足够的消息块,会导致系统保留消息发送不成功,但创建的TASK会照常运行
 */
void OsalInitSystem(uint32_t * addr, size_t size_bytes);
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_TASK_H__
//==================================================================================================
//end files
