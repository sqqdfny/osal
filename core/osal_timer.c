/**
 * @file osal_timer.c
 * @brief OSAL对外提供的定时器接口
 * @date 2021-06-08
 * @author sqqdfny
 */

#include "osal_timer.h"
#include "osal_config.h"
#include "../hal/osal_hal.h"


static osal_timer_t g_osal_timer_list[OSAL_TIMERS_NUM];
static osal_timer_t *g_pUsedTimerList, *g_pFreeTimerList;
static osal_system_tick_t g_old_tick;
//==================================================================================================
static osal_timer_t * TimerAlloc(void)
{
    osal_timer_t * p;
    if(NULL != g_pFreeTimerList)
    {
        p = g_pFreeTimerList;
        g_pFreeTimerList = g_pFreeTimerList->next;
        return p;
    }
    return NULL;
}

static void AddTimerToUsedList(osal_timer_t * pTimer)
{
    pTimer->next = g_pUsedTimerList;
    g_pUsedTimerList = pTimer;
}

static void TimerFree(osal_timer_t * pTimer)
{
    osal_timer_t *pPrev;
    if(g_pUsedTimerList == pTimer)
    {
        g_pUsedTimerList = g_pUsedTimerList->next;
    }
    else
    {
        pPrev = g_pUsedTimerList;
        do{
            if((NULL == pPrev) || (pTimer == pPrev->next)) break;
            pPrev = pPrev->next;
        }while(1);
        if(NULL == pPrev)
        {
            OsalDebugErr("%s not find: %08x\n", __func__, (uint32_t)pTimer);
            return;
        }
        pPrev->next = pTimer->next;
    }
    pTimer->next = g_pFreeTimerList;
    g_pFreeTimerList = pTimer;
}

osal_timer_t* OsalCreateTimer(osal_timer_type_t type, uint32_t timeout, void (*callback)(void *param))
{
    OsalEnterCritical();
    osal_timer_t * pTimer = TimerAlloc();
    if(NULL == pTimer) 
    {
        OsalExitCritical();
        return NULL;
    }
    pTimer->timeout = timeout;
    pTimer->callback = callback;
    pTimer->param = NULL;
    pTimer->reload_timeout = (osal_timer_type_period == type) ? timeout : 0;
    pTimer->is_running = false;
    AddTimerToUsedList(pTimer);
    OsalExitCritical();
    return pTimer;
}

void OsalDeleteTimer(osal_timer_t *pTimer)
{
    if(NULL != pTimer) 
    {
        OsalEnterCritical();
        TimerFree(pTimer);
        OsalExitCritical();
    }
}

bool OsalTimerStart(osal_timer_t *pTimer, uint32_t timeout, void *param)
{
    bool result = false;
    if((NULL != pTimer) && (true != pTimer->is_running))
    {
        OsalEnterCritical();
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
        OsalExitCritical();
    }
    return result;
}

void * OsalTimerStop(osal_timer_t *pTimer)
{
    void *param = NULL;
    if(NULL != pTimer) 
    {
        OsalEnterCritical();
        pTimer->is_running = false;
        param = pTimer->param;
        OsalExitCritical();
    }
    return param;
}
//==================================================================================================
void OsalUpdateTimers(void)
{
    osal_timer_t * pTimer, *pTimerNext;
    osal_system_tick_t tick = OsalHalGetCurSystemTick();
    uint32_t ms = tick - g_old_tick;

    OsalEnterCritical();
    g_old_tick = tick;
    if((NULL != g_pUsedTimerList) && (ms > 0))
    {
        ms = ms * OSAL_MS_PER_TICK;
        pTimer = g_pUsedTimerList;
        do
        {
            pTimerNext = pTimer->next;
            if(pTimer->is_running)
            {
                pTimer->timeout = (pTimer->timeout > ms) ? (pTimer->timeout - ms) : 0;
                if(0 == pTimer->timeout)
                {
                    if(pTimer->callback)
                    {
                        OsalExitCritical();
                        pTimer->callback(pTimer->param);
                        OsalEnterCritical();
                    }
                    if(0 != pTimer->reload_timeout)
                    {//period
                        pTimer->timeout = pTimer->reload_timeout;
                    }
                    else
                    {//one-shot
                        TimerFree(pTimer);
                    }
                }
            }
            pTimer = pTimerNext;
        } while (NULL != pTimer);
    }
    OsalExitCritical();
}

void OsalTimerInit(void)
{
    int i;
    for(i = 0; i < (OSAL_TIMERS_NUM - 1); i ++)
    {
        g_osal_timer_list[i].next = &(g_osal_timer_list[i + 1]);
    }
    g_osal_timer_list[i].next = NULL;
    g_pFreeTimerList = &(g_osal_timer_list[0]);
    g_pUsedTimerList = NULL;
    
    OsalHalTimerTickInit();
    g_old_tick = OsalHalGetCurSystemTick();
}
//==================================================================================================
//end files
