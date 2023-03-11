

#ifndef __OSAL_TIMER_H__
#define __OSAL_TIMER_H__

#include "osal.h"
#include "osal_list.h"

#ifdef __cplusplus
extern "C"
{
#endif
//==================================================================================================
#define OSAL_TIEMR_ONE_SHOT   0
#define OSAL_TIMER_PERIOD     1

struct osal_timer
{
    struct list_head list;          //used for timer manager
    uint32_t timeout;               //unit:ms 
    uint32_t reload_timeout;        //0 means one-shot
    void *param;
    void (*callback)(void *param);
    bool is_running;
};

#define INIT_OSAL_TIMER(timer, type, cb) \
        do{ \
            INIT_LIST_HEAD(&((timer)->list)); \
            (timer)->timeout = 0; \
            (timer)->reload_timeout = type; \
            (timer)->param = NULL; \
            (timer)->callback = cb; \
            (timer)->is_running = false; \
        } while(0)

void OsalCreateTimer(struct osal_timer *pTimer);

void OsalDeleteTimer(struct osal_timer *pTimer);

bool OsalTimerStart(struct osal_timer *pTimer, uint32_t timeout, void *param);
				
void * OsalTimerReStart(struct osal_timer *pTimer, uint32_t timeout, void *param);

void * OsalTimerStop(struct osal_timer *pTimer);
				
				

void OsalUpdateTimers(void);
void OsalTimerInit(void);
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_TIMER_H__
//==================================================================================================
//end files

