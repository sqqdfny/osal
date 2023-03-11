/**
 * @file osal.c
 * @brief OSAL message loop
 * @date 2021-06-08
 * @author sqqdfny
 *         sqqdfny@163.com
 */

#include "osal.h"
#include "osal_config.h"
#include "osal_timer.h"
#include "osal_mem.h"
#include "../hal/osal_hal.h"


static LIST_HEAD(sg_osal_tcb_list);
//==================================================================================================
OSAL_ERR_T OsalPostMsg(struct osal_tcb *pTcb, osal_msg_cmd_t cmd, void *param)
{
		if(NULL == param) { return OSAL_ERR_INVALID_HANDLE; }
		
    struct osal_msg_head * msg_head;
    OsalDisableAllInterrupt();
		msg_head = (struct osal_msg_head *)param;
    msg_head->cmd = cmd;
    list_add_tail(&(msg_head->list), &(pTcb->msgQueue));
    OsalResumeAllInterrupt();
    return OSAL_ERR_SUCC;
}

OSAL_ERR_T OsalPostMsgIsr(struct osal_tcb *pTcb, osal_msg_cmd_t cmd, void *param)
{
    struct osal_msg_head * msg_head;
    OsalDisableAllInterrupt();
		msg_head = (struct osal_msg_head *)param;
    msg_head->cmd = cmd;
    list_add_tail(&(msg_head->list), &(pTcb->msgQueue));
    OsalResumeAllInterrupt();
    return OSAL_ERR_SUCC;
}

void OsalAddTask(struct osal_tcb *pTcb)
{
    INIT_LIST_HEAD(&(pTcb->msgQueue));
    list_add_tail(&(pTcb->list), &sg_osal_tcb_list);
		osal_msg_head_t * pMsg = OsalMemAlloc(sizeof(osal_msg_head_t));
    OsalPostMsg(pTcb, OSAL_MSG_TASK_CREATED, pMsg);
}

void OsalStartSystem(void)
{
    struct list_head *pTaskListHead, *pMsgHead, *pMsgTmp;
    struct osal_tcb *pTcb;
    void *param;
    osal_msg_cmd_t cmd;

    OsalHalTimerTickStart();
    do{
        OsalUpdateTimers();
        if(!list_empty(&sg_osal_tcb_list))
        {
            list_for_each(pTaskListHead, &sg_osal_tcb_list)
            {
                pTcb = list_entry(pTaskListHead, struct osal_tcb, list);
                if(!list_empty(&(pTcb->msgQueue)))
                {
                    OsalDisableAllInterrupt();
                    list_for_each_safe(pMsgHead, pMsgTmp, &(pTcb->msgQueue))
                    {
                        list_del_init(pMsgHead);
                        if(NULL != pTcb->pFun)
                        {
                            cmd = list_entry(pMsgHead, struct osal_msg_head, list)->cmd;
                            param = pMsgHead;
                            OsalResumeAllInterrupt();
							pTcb->pFun(cmd, (OSAL_MSG_TASK_CREATED == cmd) ? NULL : param);
                            OsalDisableAllInterrupt();
							if(OSAL_MSG_TASK_CREATED == cmd)
							{
								OsalMemFree(param);
                        }
                    }
                    }
                    OsalResumeAllInterrupt();
                }
            }
        }

        OsalHalt();
    }while(1);
}

/**
 * @brief OSAL系统初始化,
 *        传入的参数为内存管理的内存块地址和SIZE
 *        传入的addr要注意对齐的问题
 *        根据任务数量,size_bytes 最小为64
 *        发送无参消息时需要申请一个消息块
 */
void OsalInitSystem(uint32_t * addr, size_t size_bytes)
{
    OsalMemInit(addr, size_bytes);
    OsalTimerInit();
}
//==================================================================================================
//end files
