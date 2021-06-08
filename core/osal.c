/**
 * @file osal.c
 * @brief OSAL message loop
 * @date 2021-06-08
 * @author sqqdfny
 */

#include "osal.h"
#include "osal_config.h"
#include "osal_timer.h"
#include "osal_mem.h"
#include "../hal/osal_hal.h"

typedef struct 
{
    osal_event_t (*pProcess)(osal_task_id_t task_id, osal_event_t events);
    osal_event_t events;
}osal_tcb_t;

static osal_tcb_t g_osal_tcb_list[OSAL_TASK_TOTAL];
static osal_task_id_t g_cur_task_total;
//==================================================================================================
OSAL_ERR_T OsalEventSet(osal_task_id_t task_id, osal_event_t events)
{
    if(task_id >= g_cur_task_total)
    {
        return OSAL_ERR_INVALID_HANDLE;
    }

    OsalEnterCritical();
    g_osal_tcb_list[task_id].events |= events;
    OsalExitCritical();
    return OSAL_ERR_SUCC;
}

OSAL_ERR_T OsalEventClear(osal_task_id_t task_id, osal_event_t events)
{
    if(task_id >= g_cur_task_total)
    {
        return OSAL_ERR_INVALID_HANDLE;
    }

    OsalEnterCritical();
    g_osal_tcb_list[task_id].events &= ~events;
    OsalExitCritical();
    return OSAL_ERR_SUCC;
}
//==================================================================================================
OSAL_ERR_T OsalAddTask(osal_task_id_t *pTaskId, osal_event_t (*pProcess)(osal_task_id_t task_id, osal_event_t events))
{
    if(g_cur_task_total >= OSAL_TASK_TOTAL) 
    {
        return OSAL_ERR_NO_TASK;
    }

    OsalEnterCritical();
    g_osal_tcb_list[g_cur_task_total].events = 0;
    g_osal_tcb_list[g_cur_task_total].pProcess = pProcess;
    if(pTaskId)
    {
        *pTaskId = g_cur_task_total;
    }
    g_cur_task_total ++;
    OsalExitCritical();
    return OSAL_ERR_SUCC;
}

void OsalStartSystem(void)
{
    osal_task_id_t i;
    osal_event_t events;
    do{
        OsalUpdateTimers();
        i = 0;
        do
        {
            OsalEnterCritical();
            events = g_osal_tcb_list[i].events;
            g_osal_tcb_list[i].events = 0;
            OsalExitCritical();

            if(0 != events)
            {
                if(NULL != g_osal_tcb_list[i].pProcess)
                {
                    events = g_osal_tcb_list[i].pProcess(i, events);

                    OsalEnterCritical();
                    g_osal_tcb_list[i].events = events;
                    OsalExitCritical();
                }
                else 
                {
                    events  = 0;
                }
                if(events) continue;
            }
            i ++;
        }while(i < g_cur_task_total);

        OsalHalt();
    }while(1);
}

/**
 * @brief OSAL系统初始化,
 *        传入的参数为内存管理的内存块地址和SIZE
 *        传入的addr要注意对齐的问题
 *        如果不使用 msg_queue, 同时用户也不调用内存管理相关的函数, 可以传入NULL
 */
void OsalInitSystem(uint32_t * addr, size_t size_bytes)
{
    int i;
    OsalEnterCritical();
    g_cur_task_total = 0;
    for(i = 0; i < OSAL_TASK_TOTAL; i ++)
    {
        g_osal_tcb_list[i].pProcess = NULL;
        g_osal_tcb_list[i].events = 0;
    }

    OsalMemInit(addr, size_bytes);
    OsalTimerInit();
    OsalExitCritical();
}
//==================================================================================================
//end files
