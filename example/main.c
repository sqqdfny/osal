#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../core/osal.h"

//==================================================================================================
#define MAIN_EVENT_START    OSAL_BIT0
#define MAIN_EVENT_TIMER0   OSAL_BIT1
#define MAIN_EVENT_TIMER1   OSAL_BIT2
#define MAIN_EVENT_TIMER2   OSAL_BIT3
#define MAIN_EVENT_TIMER3   OSAL_BIT4

static osal_task_id_t g_main_task_id;
static osal_timer_t *pTimer0, *pTimer1;
static uint32_t timer_cnt[4];

typedef struct 
{
    osal_task_id_t task_id;
}timer_cb_param_t;

static void Timer0_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    printf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER0);
}

static void Timer1_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    printf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER1);
}

static void Timer2_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    printf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER2);
    OsalMemFree(param);
}

static void Timer3_cb(void * param)
{
    timer_cb_param_t *p = (timer_cb_param_t *)param;
    printf("%s: task_id=%u param=%08x\n", __func__, p->task_id, (uint32_t)param);
    OsalEventSet(p->task_id, MAIN_EVENT_TIMER3);
    OsalMemFree(param);
}

static osal_event_t MainTask(osal_task_id_t task_id, osal_event_t events)
{
    OSAL_ERR_T ret;
    timer_cb_param_t *p;
    if(events & OSAL_EVENT_MSG)
    {
        
    }

    if(events & MAIN_EVENT_START)
    {
        timer_cnt[0] = 0;
        timer_cnt[1] = 0;
        timer_cnt[2] = 0;
        timer_cnt[3] = 0;
        printf("%s MAIN_EVENT_START: %u\n", __func__, task_id);

        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL == p)
        {
            printf("%s timer0_0 mem failed\n", __func__);
        }
        p->task_id = task_id;
        ret = OsalCreateTimer(&pTimer0, osal_timer_type_period, 1000, Timer0_cb, (void*)p);
        if(OSAL_ERR_SUCC != ret)
        {
            printf("%s MAIN_EVENT_START0 OsalCreateTimer err=%d\n", __func__, ret);
        }

        #if (1)
        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL == p)
        {
            printf("%s timer1_0 mem failed\n", __func__);
        }
        p->task_id = task_id;
        ret = OsalCreateTimer(&pTimer1, osal_timer_type_period, 2000, Timer1_cb, (void*)p);
        if(OSAL_ERR_SUCC != ret)
        {
            printf("%s MAIN_EVENT_START1 OsalCreateTimer err=%d\n", __func__, ret);
        }

        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL == p)
        {
            printf("%s timer2_0 mem failed\n", __func__);
        }
        p->task_id = task_id;
        ret = OsalCreateTimer(NULL, osal_timer_type_one_shot, 1000, Timer2_cb, (void*)p);
        if(OSAL_ERR_SUCC != ret)
        {
            printf("%s MAIN_EVENT_START2 OsalCreateTimer err=%d\n", __func__, ret);
        }

        p = OsalMemAlloc(sizeof(timer_cb_param_t));
        if(NULL == p)
        {
            printf("%s timer3_0 mem failed\n", __func__);
        }
        p->task_id = task_id;
        ret = OsalCreateTimer(NULL, osal_timer_type_one_shot, 2000, Timer3_cb, (void*)p);
        if(OSAL_ERR_SUCC != ret)
        {
            printf("%s MAIN_EVENT_START3 OsalCreateTimer err=%d\n", __func__, ret);
        }
        #endif 
    }

    if(events & MAIN_EVENT_TIMER0)
    {
        timer_cnt[0] ++;
        printf("%s MAIN_EVENT_TIMER0: %u\n", __func__, timer_cnt[0]);
        if(timer_cnt[0] >= 5)
        {
            printf("%s MAIN_EVENT_TIMER0: %u end\n", __func__, timer_cnt[1]);
            OsalTimerStop(pTimer0);
            OsalMemFree(OsalTimerGetParam(pTimer0));
            ret = OsalDeleteTimer(pTimer0);
            if(OSAL_ERR_SUCC != ret)
            {
                printf("%s MAIN_EVENT_TIMER0 OsalDeleteTimer err=%d\n", __func__, ret);
            }
        }
    }

    if(events & MAIN_EVENT_TIMER1)
    {
        timer_cnt[1] ++;
        printf("%s MAIN_EVENT_TIMER1: %u\n", __func__, timer_cnt[1]);
        if(timer_cnt[1] >= 5)
        {
            printf("%s MAIN_EVENT_TIMER1: %u end\n", __func__, timer_cnt[1]);
            OsalTimerStop(pTimer1);
            OsalMemFree(OsalTimerGetParam(pTimer1));
            ret = OsalDeleteTimer(pTimer1);
            if(OSAL_ERR_SUCC != ret)
            {
                printf("%s MAIN_EVENT_TIMER1 OsalDeleteTimer err=%d\n", __func__, ret);
            }
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
                printf("%s timer2 mem failed\n", __func__);
            }
            p->task_id = task_id;
            printf("%s MAIN_EVENT_TIMER2: %u\n", __func__, timer_cnt[2]);
            ret = OsalCreateTimer(NULL, osal_timer_type_one_shot, 1000, Timer2_cb, (void*)p);
            if(OSAL_ERR_SUCC != ret)
            {
                printf("%s MAIN_EVENT_TIMER2 OsalCreateTimer err=%d\n", __func__, ret);
            }
        }
        else
        {
            printf("%s MAIN_EVENT_TIMER2 %u end\n", __func__, timer_cnt[2]);
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
                printf("%s timer3 mem failed\n", __func__);
            }
            p->task_id = task_id;
            printf("%s MAIN_EVENT_TIMER3: %u\n", __func__, timer_cnt[3]);
            ret = OsalCreateTimer(NULL, osal_timer_type_one_shot, 2000, Timer3_cb, (void*)p);
            if(OSAL_ERR_SUCC != ret)
            {
                printf("%s MAIN_EVENT_TIMER3 OsalCreateTimer err=%d\n", __func__, ret);
            }
        }
        else
        {
            printf("%s MAIN_EVENT_TIMER3 %u end\n", __func__, timer_cnt[3]);
        }
    }
    return 0;
}

void MainTaskInit(void)
{
    OSAL_ERR_T ret = OsalAddTask(&g_main_task_id, MainTask);
    if(OSAL_ERR_SUCC != ret)
    {
        printf("OsalAddTask err=%d\n", ret);
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