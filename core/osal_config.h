/**
 * @file osal_config.h
 * @brief 系统配置,定时器数量,队列数量等
 * @date 2021-06-08
 * @author sqqdfny
 */
#define OSAL_TASK_TOTAL     2    //系统最大支持的任务个数,最小值为1

#define OSAL_TIMERS_MAX     8    //系统最大支持的定时器个数,最小值为2
#if (OSAL_TIMERS_MAX < 2)
    #error "OSAL_TIMERS_MAX must equal or more than 2!!!"
#endif


#ifndef __OSAL_CONFIG_H__
#define __OSAL_CONFIG_H__

#ifdef __cplusplus
extern "C"
{
#endif
//==================================================================================================



//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_CONFIG_H__
//end files
