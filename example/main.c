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
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER0);
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
}

static void Timer1_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    AppDebugPrintf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER1);
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
}

static void Timer2_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    AppDebugPrintf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER2);
    
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
}

static void Timer3_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    AppDebugPrintf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER3);
    
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
}

static osal_event_t MainTask(osal_task_id_t task_id, osal_event_t events)
{
    OSAL_ERR_T ret;
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
        if(NULL == p)
        {
            AppDebugPrintf("%s timer0_0 mem failed\n", __func__);
        }
        p->task_id = task_id;
        ret = OsalCreateTimer(&pTimer0, osal_timer_type_period, 1000, Timer0_cb, (void*)p);
        if(OSAL_ERR_SUCC != ret)
        {
            AppDebugPrintf("%s MAIN_EVENT_START0 OsalCreateTimer err=%d\n", __func__, ret);
        }

        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL == p)
        {
            AppDebugPrintf("%s timer1_0 mem failed\n", __func__);
        }
        p->task_id = task_id;
        ret = OsalCreateTimer(&pTimer1, osal_timer_type_period, 2000, Timer1_cb, (void*)p);
        if(OSAL_ERR_SUCC != ret)
        {
            AppDebugPrintf("%s MAIN_EVENT_START1 OsalCreateTimer err=%d\n", __func__, ret);
        }

        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL == p)
        {
            AppDebugPrintf("%s timer2_0 mem failed\n", __func__);
        }
        p->task_id = task_id;
        ret = OsalCreateTimer(NULL, osal_timer_type_one_shot, 1000, Timer2_cb, (void*)p);
        if(OSAL_ERR_SUCC != ret)
        {
            AppDebugPrintf("%s MAIN_EVENT_START2 OsalCreateTimer err=%d\n", __func__, ret);
        }

        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL == p)
        {
            AppDebugPrintf("%s timer3_0 mem failed\n", __func__);
        }
        p->task_id = task_id;
        ret = OsalCreateTimer(NULL, osal_timer_type_one_shot, 2000, Timer3_cb, (void*)p);
        if(OSAL_ERR_SUCC != ret)
        {
            AppDebugPrintf("%s MAIN_EVENT_START3 OsalCreateTimer err=%d\n", __func__, ret);
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
            OsalTimerStop(pTimer0);
            OsalMemFree(OsalTimerGetParam(pTimer0));
            ret = OsalDeleteTimer(pTimer0);
            if(OSAL_ERR_SUCC != ret)
            {
                AppDebugPrintf("%s MAIN_EVENT_TIMER0 OsalDeleteTimer err=%d\n", __func__, ret);
            }
        }
    }

    if(events & MAIN_EVENT_TIMER1)
    {
        timer_cnt[1] ++;
        AppDebugPrintf("%s MAIN_EVENT_TIMER1: %u\n", __func__, timer_cnt[1]);
        if(timer_cnt[1] >= 5)
        {
            AppDebugPrintf("%s MAIN_EVENT_TIMER1: %u end\n", __func__, timer_cnt[1]);
            OsalTimerStop(pTimer1);
            OsalMemFree(OsalTimerGetParam(pTimer1));
            ret = OsalDeleteTimer(pTimer1);
            if(OSAL_ERR_SUCC != ret)
            {
                AppDebugPrintf("%s MAIN_EVENT_TIMER1 OsalDeleteTimer err=%d\n", __func__, ret);
            }

            AppDebugPrintf("%s MAIN_EVENT_TIMER1 end malloc: 0x%08lx\n", __func__, (size_t)(OsalMemAlloc(1024)));
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
            ret = OsalCreateTimer(NULL, osal_timer_type_one_shot, 1000, Timer2_cb, (void*)p);
            if(OSAL_ERR_SUCC != ret)
            {
                AppDebugPrintf("%s MAIN_EVENT_TIMER2 OsalCreateTimer err=%d\n", __func__, ret);
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
            ret = OsalCreateTimer(NULL, osal_timer_type_one_shot, 2000, Timer3_cb, (void*)p);
            if(OSAL_ERR_SUCC != ret)
            {
                AppDebugPrintf("%s MAIN_EVENT_TIMER3 OsalCreateTimer err=%d\n", __func__, ret);
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

    MainTaskInit();

    OsalStartSystem();
    return 0;
}
//==================================================================================================
//end files
