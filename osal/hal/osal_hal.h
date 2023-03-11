


#ifndef __OSAL_HAL_TIMER_H__
#define __OSAL_HAL_TIMER_H__

#include "../core/osal_typedef.h"


#ifdef __cplusplus
extern "C"
{
#endif

#ifdef USED_PLATFORM_LINUX_SIMULATOR
	#define OSAL_MS_PER_TICK    10   //每个TICK有多少MS, 10ms的心跳
#endif 

#ifdef USED_PLATFORM_MICROBLAZE
	#define OSAL_MS_PER_TICK    10   //每个TICK有多少MS, 10ms的心跳
#endif

#ifdef USED_PLATFORM_COTEX_A9
	#define OSAL_MS_PER_TICK    1   //每个TICK有多少MS, 1ms的心跳
#endif


/**
 * @brief 临界状态的进入与退出
 *        如果不在中断中调用OSAL的相关函数，可以为空定义
 */
void OSAL_HAL_DISABLE_ALL_INTERRUPT(void);
void OSAL_HAL_ENABLE_ALL_INTERRUPT(void);
	
#define OsalDisableAllInterrupt()   OSAL_HAL_DISABLE_ALL_INTERRUPT()
#define OsalResumeAllInterrupt()    OSAL_HAL_ENABLE_ALL_INTERRUPT()
	
//如果在中断中调用内存管理相关的接口, 则下面的宏必须为有效的接口
#define OsalMemEnterCritical()      OsalDisableAllInterrupt()		
#define OsalMemExitCritical()	    OsalResumeAllInterrupt()	

//如果在中断中调用定时器相关的接口, 则下面的宏必须为有效的接口
#define OsalTimerEnterCritical()    OsalDisableAllInterrupt()  
#define OsalTimerExitCritical()     OsalResumeAllInterrupt()

#define OsalReboot()                //NVIC_SystemReset()
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
 * @brief 开启硬件定时器，为空则一直开启
 */
void OsalHalTimerTickStart(void);

/**
 * @brief 关闭硬件定时器，为空则一直不关闭
 */
void OsalHalTimerTickStop(void);

/**
 * @brief 没有任务执行时CPU调用的函数
 */
void OsalHalt(void);
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_HAL_TIMER_H__
//==================================================================================================
//end files
