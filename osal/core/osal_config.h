/**
 * @file osal_config.h
 * @brief 系统配置,定时器数量,队列数量等
 * @date 2022-1-27
 * @author sqqdfny
 */
#define OSAL_MSG_POOL_SIZE      32  //用于向任务发送消息的消息池SIZE
#if (OSAL_MSG_POOL_SIZE < 8)
    #error "OSAL_MSG_POOL_SIZE must equal or more than 8!!!"
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
