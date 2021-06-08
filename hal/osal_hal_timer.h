


#ifndef __OSAL_HAL_TIMER_H__
#define __OSAL_HAL_TIMER_H__

#include "../core/osal_typedef.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define OSAL_MS_PER_TICK    10   //每个TICK有多少MS, 10ms的心跳
//==================================================================================================
/**
 * @brief 获取当前的tick值
 */
osal_system_tick_t OsalHalGetCurSystemTick(void);

/**
 * @brief 硬件定时器初始化，设定系统时钟
 */
void OsalHalTimerTickInit(void);

/**
 * @brief 开启硬件定时器，OSAL会根据程序中软件定时器的实际使用动态开启和关闭，为空则一直开启
 */
void OsalHalTimerTickStart(void);

/**
 * @brief 关闭硬件定时器，为空则一直不关闭
 */
void OsalHalTimerTickStop(void);
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_HAL_TIMER_H__
//==================================================================================================
//end files
