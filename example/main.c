#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../osal/osal_export.h"


#define MSG_TIMER0  (OSAL_MSG_USR_BASE + 0)
#define MSG_TIMER1  (OSAL_MSG_USR_BASE + 1)
#define MSG_TIMER2  (OSAL_MSG_USR_BASE + 2)
#define MSG_TIMER3  (OSAL_MSG_USR_BASE + 3)
//==================================================================================================

static osal_tcb_t sg_tcb;
static osal_timer_t timer[4];
static uint32_t timer_cnt[4] = {0, 0, 0, 0};

typedef struct 
{
    uint32_t timer;
    uint32_t index;
    uint8_t buf[37];
}main_task_msg_t;

static void Timer0_cb(void * param)
{
    OsLogInfo("%s: %u\n", __FUNCTION__, *((uint32_t *)param));
    OsalPostMsg(&sg_tcb, MSG_TIMER0, param);
    (*((uint32_t *)param)) ++;
}

static void Timer1_cb(void * param)
{
    OsLogInfo("%s: %u\n", __FUNCTION__, *((uint32_t *)param));
    OsalPostMsg(&sg_tcb, MSG_TIMER1, param);
    (*((uint32_t *)param)) ++;
}

static void Timer2_cb(void * param)
{
    OsLogInfo("%s: %u\n", __FUNCTION__, *((uint32_t *)param));
    OsalPostMsg(&sg_tcb, MSG_TIMER2, param);
    (*((uint32_t *)param)) ++;
}

static void Timer3_cb(void * param)
{
    OsLogInfo("%s: %u\n", __FUNCTION__, *((uint32_t *)param));
    OsalPostMsg(&sg_tcb, MSG_TIMER3, param);
    (*((uint32_t *)param)) ++;
}

static void MainTask(osal_msg_cmd_t cmd, void *param)
{
    switch(cmd)
    {
        case OSAL_MSG_TASK_CREATED: 
            OsLogInfo("%s recv:OSAL_MSG_TASK_CREATED\n", __FUNCTION__);
            break;
        case MSG_TIMER0:
            OsLogInfo("%s recv:MSG_TIME%u %u\n", __FUNCTION__, cmd - OSAL_MSG_USR_BASE, *((uint32_t *)param));
            break;
        case MSG_TIMER1:
            OsLogInfo("%s recv:MSG_TIME%u %u\n", __FUNCTION__, cmd - OSAL_MSG_USR_BASE, *((uint32_t *)param));
            break;
        case MSG_TIMER2:
            OsLogInfo("%s recv:MSG_TIME%u %u\n", __FUNCTION__, cmd - OSAL_MSG_USR_BASE, *((uint32_t *)param));
            break;
        case MSG_TIMER3:
            OsLogInfo("%s recv:MSG_TIME%u %u\n", __FUNCTION__, cmd - OSAL_MSG_USR_BASE, *((uint32_t *)param));
            break;
        default:;
    }
}

void MainTaskInit(void)
{
    INIT_OSAL_TCB(&sg_tcb, MainTask);
    OsalAddTask(&sg_tcb);
    INIT_OSAL_TIMER(&timer[0], OSAL_TIMER_PERIOD, Timer0_cb);
    INIT_OSAL_TIMER(&timer[1], OSAL_TIMER_PERIOD, Timer1_cb);
    INIT_OSAL_TIMER(&timer[2], OSAL_TIMER_PERIOD, Timer2_cb);
    INIT_OSAL_TIMER(&timer[3], OSAL_TIMER_PERIOD, Timer3_cb);
    OsalCreateTimer(&timer[0]);
    OsalCreateTimer(&timer[1]);
    OsalCreateTimer(&timer[2]);
    OsalCreateTimer(&timer[3]);

    OsalTimerStart(&timer[0], 1000, &timer_cnt[0]);
    OsalTimerStart(&timer[1], 2000, &timer_cnt[1]);
    OsalTimerStart(&timer[2], 3000, &timer_cnt[2]);
    OsalTimerStart(&timer[3], 4000, &timer_cnt[3]);
}
//==================================================================================================



//==================================================================================================
#define MEM_BUF_SIZE    (0x400 * 16)
static uint32_t g_mem_buf[MEM_BUF_SIZE / 4];

int main()
{
    OsalInitSystem(g_mem_buf, MEM_BUF_SIZE);
    OsLogInfo("OsalVersion:%s\n", OSAL_VERSION);
    MainTaskInit();

    OsalStartSystem();
    return 0;
}
//==================================================================================================
//end files
