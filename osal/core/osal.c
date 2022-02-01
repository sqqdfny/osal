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


typedef struct osal_msg_head
{
    struct list_head list;
    osal_msg_cmd_t cmd;
    void *param;
}osal_msg_head_t;

static LIST_HEAD(sg_osal_tcb_list);
static LIST_HEAD(sg_osal_msg_head_list);
static struct osal_msg_head sg_msg_head_pool[OSAL_MSG_POOL_SIZE];
//==================================================================================================
static void OsalMsgPoolInit(void)
{
    int i;
    for(i = 0; i < OSAL_MSG_POOL_SIZE; i ++)
    {
        list_add_tail(&(sg_msg_head_pool[i].list), &sg_osal_msg_head_list);
    }
}
//==================================================================================================
OSAL_ERR_T OsalPostMsg(struct osal_tcb *pTcb, osal_msg_cmd_t cmd, void *param)
{
    struct osal_msg_head * msg_head;
    OsalEnterCritical();
    if(list_empty(&sg_osal_msg_head_list))
    {
        OsalExitCritical();
        OsLogErr("%s/%s[%d]: no mem!!!", __FILE__, __FUNCTION__, __LINE__);
        return OSAL_ERR_NO_MEM;
    }

    msg_head = list_first_entry(&sg_osal_msg_head_list, struct osal_msg_head, list);
    list_del(&(msg_head->list));
    msg_head->cmd = cmd;
    msg_head->param = param;
    list_add_tail(&(msg_head->list), &(pTcb->msgQueue));
    OsalExitCritical();
    return OSAL_ERR_SUCC;
}

void OsalAddTask(struct osal_tcb *pTcb)
{
    INIT_LIST_HEAD(&(pTcb->msgQueue));
    list_add_tail(&(pTcb->list), &sg_osal_tcb_list);
    OsalPostMsg(pTcb, OSAL_MSG_TASK_CREATED, NULL);
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
                    OsalEnterCritical();
                    list_for_each_safe(pMsgHead, pMsgTmp, &(pTcb->msgQueue))
                    {
                        list_del(pMsgHead);
                        if(NULL != pTcb->pFun)
                        {
                            cmd = list_entry(pMsgHead, struct osal_msg_head, list)->cmd;
                            param = list_entry(pMsgHead, struct osal_msg_head, list)->param;
                            list_add(pMsgHead, &sg_osal_msg_head_list);
                            OsalExitCritical();
                            pTcb->pFun(cmd, param);
                            OsalEnterCritical();
                        }
                    }
                    OsalExitCritical();
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
 *        如果不调用内存管理相关的函数, 可以传入NULL
 */
void OsalInitSystem(uint32_t * addr, size_t size_bytes)
{
    OsalMsgPoolInit();
    OsalMemInit(addr, size_bytes);
    OsalTimerInit();
}
//==================================================================================================
//end files
