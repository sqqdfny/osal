

#ifndef __OSAL_TIMER_H__
#define __OSAL_TIMER_H__

#include "osal.h"

#ifdef __cplusplus
extern "C"
{
#endif
//==================================================================================================
typedef enum
{
    osal_timer_type_period = 0,
    osal_timer_type_one_shot,
}osal_timer_type_t;

typedef struct OSAL_TIMER_T
{
    struct OSAL_TIMER_T *next;
    uint32_t timeout;               //ms
    uint32_t reload_timeout;        //0表示是单次定时器
    void *param;
    void (*callback)(void *param);
    bool is_running;
}osal_timer_t;


OSAL_ERR_T OsalCreateTimer(osal_timer_t **ppTimer, osal_timer_type_t type, uint32_t timeout, void (*callback)(void *param), void *param);
OSAL_ERR_T OsalDeleteTimer(osal_timer_t *pTimer);
OSAL_ERR_T OsalTimerStart(osal_timer_t *pTimer);
OSAL_ERR_T OsalTimerStop(osal_timer_t *pTimer);
OSAL_ERR_T OsalTimerReset(osal_timer_t *pTimer, osal_timer_type_t type, uint32_t timeout);

void OsalUpdateTimers(void);
void OsalTimerInit(void);
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_TIMER_H__
//==================================================================================================
//end files