#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../core/osal.h"


#define AppDebugPrintf(...) printf(__VA_ARGS__)
//==================================================================================================
#define MAIN_EVENT_START    OSAL_BIT0
#define MAIN_EVENT_TIMER0   OSAL_BIT1
#define MAIN_EVENT_TIMER1   OSAL_BIT2
#define MAIN_EVENT_TIMER2   OSAL_BIT3
#define MAIN_EVENT_TIMER3   OSAL_BIT4

static osal_task_id_t g_main_task_id;
static osal_timer_t *pTimer0, *pTimer1;
static uint32_t timer_cnt[4];
static uint32_t g_main_task_index = 0;

typedef struct 
{
    uint32_t timer;
    uint32_t index;
    uint8_t buf[37];
}main_task_msg_t;

typedef struct 
{
    osal_task_id_t task_id;
}timer_cb_param_t;

static void Timer0_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    AppDebugPrintf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    main_task_msg_t *pmsg = OsalMsgQueueAlloc(sizeof(main_task_msg_t));
    if(pmsg)
    {
        pmsg->timer = 0;
        pmsg->index = g_main_task_index ++;
        if(OSAL_ERR_SUCC != OsalMsgQueuePost(p->task_id, pmsg))
        {
            AppDebugPrintf("%s timer0 send msg failed\n", __func__);
        }
    }
    else
    {
        AppDebugPrintf("%s timer0 msg alloc failed\n", __func__);
    }
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER0);
}

static void Timer1_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    AppDebugPrintf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    main_task_msg_t *pmsg = OsalMsgQueueAlloc(sizeof(main_task_msg_t));
    AppDebugPrintf("%s: msg alloc=%08x\n", __func__, (uint32_t)pmsg);
    if(pmsg)
    {
        pmsg->timer = 1;
        pmsg->index = g_main_task_index ++;
        if(OSAL_ERR_SUCC != OsalMsgQueuePost(p->task_id, pmsg))
        {
            AppDebugPrintf("%s timer1 send msg failed\n", __func__);
        }
    }
    else
    {
        AppDebugPrintf("%s timer1 msg alloc failed\n", __func__);
    }
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER1);
}

static void Timer2_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    AppDebugPrintf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    
    main_task_msg_t *pmsg = OsalMsgQueueAlloc(sizeof(main_task_msg_t));
    if(pmsg)
    {
        pmsg->timer = 2;
        pmsg->index = g_main_task_index ++;
        if(OSAL_ERR_SUCC != OsalMsgQueuePost(p->task_id, pmsg))
        {
            AppDebugPrintf("%s timer2 send msg failed\n", __func__);
        }
    }
    else
    {
        AppDebugPrintf("%s timer2 msg alloc failed\n", __func__);
    }
    OsalMemFree(param);
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER2);
}

static void Timer3_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    AppDebugPrintf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    
    main_task_msg_t *pmsg = OsalMsgQueueAlloc(sizeof(main_task_msg_t));
    if(pmsg)
    {
        pmsg->timer = 3;
        pmsg->index = g_main_task_index ++;
        if(OSAL_ERR_SUCC != OsalMsgQueuePost(p->task_id, pmsg))
        {
            AppDebugPrintf("%s timer3 send msg failed\n", __func__);
        }
    }
    else
    {
        AppDebugPrintf("%s timer3 msg alloc failed\n", __func__);
    }
    OsalMemFree(param);
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER3);
}

static osal_event_t MainTask(osal_task_id_t task_id, osal_event_t events)
{
    timer_cb_param_t *p;
    main_task_msg_t *pmsg;
    if(events & OSAL_EVENT_MSG)
    {
        pmsg = OsalMsgQueuePend(task_id);
        if(pmsg)
        {
            AppDebugPrintf("%s:msg timer=%u index=%u\n", __func__, pmsg->timer, pmsg->index);
            OsalMsgQueueFree(pmsg);
        }

        if(timer_cnt[0] >= 5 && timer_cnt[1] >= 5 && timer_cnt[2] >= 5 &&timer_cnt[3] >= 5)
        {
            AppDebugPrintf("%s all timer end malloc: 0x%08lx\n", __func__, (size_t)(OsalMemAlloc(1024)));
        }
    }

    if(events & MAIN_EVENT_START)
    {
        timer_cnt[0] = 0;
        timer_cnt[1] = 0;
        timer_cnt[2] = 0;
        timer_cnt[3] = 0;
        AppDebugPrintf("%s MAIN_EVENT_START: %u\n", __func__, task_id);

        //OsalMemAlloc(sizeof(timer_cb_param_t));
        //OsalMemAlloc(sizeof(timer_cb_param_t));
        #if (1)
        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL != p)
        {
            p->task_id = task_id;
            pTimer0 = OsalCreateTimer(osal_timer_type_period, 0, Timer0_cb);
            if(pTimer0)
            {
                if(!OsalTimerStart(pTimer0, 1000, (void*)p))
                {
                    OsalMemFree((void*)p);
                    AppDebugPrintf("%s MAIN_EVENT_START0 OsalTimerStart Failed\n", __func__);
                }
            }
            else
            {
                AppDebugPrintf("%s MAIN_EVENT_START0 OsalCreateTimer\n", __func__);
            }
        }
        else
        {
            AppDebugPrintf("%s timer0_0 mem failed\n", __func__);
        }
        

        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL != p)
        {
            p->task_id = task_id;
            pTimer1 = OsalCreateTimer(osal_timer_type_period, 2000, Timer1_cb);
            if(pTimer1)
            {
                if(!OsalTimerStart(pTimer1, 2000, (void*)p))
                {
                    OsalMemFree((void*)p);
                    AppDebugPrintf("%s MAIN_EVENT_START1 OsalTimerStart Failed\n", __func__);
                }
            }
            else
            {
                AppDebugPrintf("%s MAIN_EVENT_START1 OsalCreateTimer\n", __func__);
            }
        }
        else
        {
            AppDebugPrintf("%s timer0_0 mem failed\n", __func__);
        }

        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL != p)
        {
            p->task_id = task_id;
            osal_timer_t * ptimer = OsalCreateTimer(osal_timer_type_one_shot, 1000, Timer2_cb);
            if(ptimer)
            {
                if(!OsalTimerStart(ptimer, 1000, (void*)p))
                {
                    OsalMemFree((void*)p);
                    AppDebugPrintf("%s MAIN_EVENT_START2 OsalTimerStart Failed\n", __func__);
                }
            }
            else
            {
                AppDebugPrintf("%s MAIN_EVENT_START2 OsalCreateTimer\n", __func__);
            }
        }
        else
        {
            AppDebugPrintf("%s timer2_0 mem failed\n", __func__);
        }
        

        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL != p)
        {
            p->task_id = task_id;
            osal_timer_t * ptimer = OsalCreateTimer(osal_timer_type_one_shot, 2000, Timer3_cb);
            if(ptimer)
            {
                if(!OsalTimerStart(ptimer, 2000, (void*)p))
                {
                    OsalMemFree((void*)p);
                    AppDebugPrintf("%s MAIN_EVENT_START3 OsalTimerStart Failed\n", __func__);
                }
            }
            else
            {
                AppDebugPrintf("%s MAIN_EVENT_START3 OsalCreateTimer\n", __func__);
            }
        }
        else
        {
            AppDebugPrintf("%s timer3_0 mem failed\n", __func__);
        }
        #endif 
    }

    if(events & MAIN_EVENT_TIMER0)
    {
        timer_cnt[0] ++;
        AppDebugPrintf("%s MAIN_EVENT_TIMER0: %u\n", __func__, timer_cnt[0]);
        if(timer_cnt[0] >= 5)
        {
            AppDebugPrintf("%s MAIN_EVENT_TIMER0: %u end\n", __func__, timer_cnt[1]);
            ;
            OsalMemFree(OsalTimerStop(pTimer0));
            OsalDeleteTimer(pTimer0);
        }
    }

    if(events & MAIN_EVENT_TIMER1)
    {
        timer_cnt[1] ++;
        AppDebugPrintf("%s MAIN_EVENT_TIMER1: %u\n", __func__, timer_cnt[1]);
        if(timer_cnt[1] >= 5)
        {
            AppDebugPrintf("%s MAIN_EVENT_TIMER1: %u end\n", __func__, timer_cnt[1]);
            OsalMemFree(OsalTimerStop(pTimer1));
            OsalDeleteTimer(pTimer1);
        }
    }

    if(events & MAIN_EVENT_TIMER2)
    {
        timer_cnt[2] ++;
        if(timer_cnt[2] < 5)
        {
            p = OsalMemAlloc(sizeof(timer_cb_param_t));
            if(NULL == p)
            {
                AppDebugPrintf("%s timer2 mem failed\n", __func__);
            }
            p->task_id = task_id;
            AppDebugPrintf("%s MAIN_EVENT_TIMER2: %u\n", __func__, timer_cnt[2]);
            osal_timer_t * ptimer = OsalCreateTimer(osal_timer_type_one_shot, 1000, Timer2_cb);
            if(ptimer)
            {
                if(!OsalTimerStart(ptimer, 1000, (void*)p))
                {
                    OsalMemFree((void*)p);
                    AppDebugPrintf("%s MAIN_EVENT_START2 OsalTimerStart Failed\n", __func__);
                }
            }
            else
            {
                AppDebugPrintf("%s MAIN_EVENT_START2 OsalCreateTimer\n", __func__);
            }
        }
        else
        {
            AppDebugPrintf("%s MAIN_EVENT_TIMER2 %u end\n", __func__, timer_cnt[2]);
        }
    }

    if(events & MAIN_EVENT_TIMER3)
    {
        timer_cnt[3] ++;
        if(timer_cnt[3] < 5)
        {
            p = OsalMemAlloc(sizeof(timer_cb_param_t));
            if(NULL == p)
            {
                AppDebugPrintf("%s timer3 mem failed\n", __func__);
            }
            p->task_id = task_id;
            AppDebugPrintf("%s MAIN_EVENT_TIMER3: %u\n", __func__, timer_cnt[3]);
            osal_timer_t * ptimer = OsalCreateTimer(osal_timer_type_one_shot, 2000, Timer3_cb);
            if(ptimer)
            {
                if(!OsalTimerStart(ptimer, 2000, (void*)p))
                {
                    OsalMemFree((void*)p);
                    AppDebugPrintf("%s MAIN_EVENT_START3 OsalTimerStart Failed\n", __func__);
                }
            }
            else
            {
                AppDebugPrintf("%s MAIN_EVENT_START3 OsalCreateTimer\n", __func__);
            }
        }
        else
        {
            AppDebugPrintf("%s MAIN_EVENT_TIMER3 %u end\n", __func__, timer_cnt[3]);
        }
    }
    return 0;
}

void MainTaskInit(void)
{
    OSAL_ERR_T ret = OsalAddTask(&g_main_task_id, MainTask);
    if(OSAL_ERR_SUCC != ret)
    {
        AppDebugPrintf("OsalAddTask err=%d\n", ret);
        exit(1);
    }

    OsalEventSet(g_main_task_id, MAIN_EVENT_START);
}
//==================================================================================================



//==================================================================================================
#define MEM_BUF_SIZE    (0x400 * 16)
static uint32_t g_mem_buf[MEM_BUF_SIZE / 4];

int main()
{
    
    OsalInitSystem(g_mem_buf, MEM_BUF_SIZE);
    printf("OsalVersion:%s\n", OsalGetVersionStr());
    MainTaskInit();

    OsalStartSystem();
    return 0;
}
//==================================================================================================
//end files
