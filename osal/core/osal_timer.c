/**
 * @file osal_timer.c
 * @brief OSAL对外提供的定时器接口
 * @date 2021-06-08
 * @author sqqdfny
 *         sqqdfny@163.com
 */

#include "osal_timer.h"
#include "osal_config.h"
#include "../hal/osal_hal.h"


#define OsalTimerEnterCritical()   //OsalEnterCritical()
#define OsalTimerExitCritical()    //OsalExitCritical()

static LIST_HEAD(sg_osal_timer_list);
static osal_system_tick_t g_old_tick;
//==================================================================================================
void OsalCreateTimer(osal_timer_t *pTimer)
{
    OsalTimerEnterCritical();
    list_add_tail(&(pTimer->list), &sg_osal_timer_list);
    OsalTimerExitCritical();
}

void OsalDeleteTimer(osal_timer_t *pTimer)
{
    OsalTimerEnterCritical();
    list_del(&(pTimer->list));
    OsalTimerExitCritical();
}

bool OsalTimerStart(osal_timer_t *pTimer, uint32_t timeout, void *param)
{
    bool result = false;
    if((NULL != pTimer) && (!pTimer->is_running))
    {
        OsalTimerEnterCritical();
        if(timeout > 0)
        {
            pTimer->timeout = timeout;
            if(pTimer->reload_timeout > 0)
            {
                pTimer->reload_timeout = timeout;
            }
        }
        pTimer->param = param;
        pTimer->is_running = true;
        result = true;
        OsalTimerExitCritical();
    }
    return result;
}

void * OsalTimerStop(osal_timer_t *pTimer)
{
    void *param = NULL;
    if(NULL != pTimer) 
    {
        OsalTimerEnterCritical();
        pTimer->is_running = false;
        param = pTimer->param;
        OsalTimerExitCritical();
    }
    return param;
}
//==================================================================================================
void OsalUpdateTimers(void)
{
    struct list_head *pListHead;
    osal_timer_t *pTimer;
    osal_system_tick_t tick = OsalHalGetCurSystemTick();
    uint32_t ms = tick - g_old_tick;

    OsalTimerEnterCritical();
    g_old_tick = tick;
    if((!list_empty(&sg_osal_timer_list)) && (ms > 0))
    {
        ms = ms * OSAL_MS_PER_TICK;
        list_for_each(pListHead, &sg_osal_timer_list)
        {
            pTimer = list_entry(pListHead, osal_timer_t, list);
            if(pTimer->is_running)
            {
                pTimer->timeout = (pTimer->timeout > ms) ? (pTimer->timeout - ms) : 0;
                if(0 == pTimer->timeout)
                {
                    if(pTimer->callback)
                    {
                        OsalTimerExitCritical();
                        pTimer->callback(pTimer->param);
                        OsalTimerEnterCritical();
                    }
                    if(0 != pTimer->reload_timeout)
                    {//period
                        pTimer->timeout = pTimer->reload_timeout;
                    }
                    else
                    {//one-shot
                        pTimer->is_running = false;
                    }
                }
            }
        }
    }
    OsalTimerExitCritical();
}

void OsalTimerInit(void)
{
    OsalHalTimerTickInit();
    g_old_tick = OsalHalGetCurSystemTick();
}
//==================================================================================================
//end files
