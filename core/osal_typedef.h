

#ifndef __OSAL_TYPEDEF_H__
#define __OSAL_TYPEDEF_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    OSAL_ERR_SUCC = 0,
    OSAL_ERR_NO_MEM,
    OSAL_ERR_NO_TIMER,
    OSAL_ERR_INVALID_HANDLE,
}OSAL_ERR_T;

//==================================================================================================
typedef uint32_t osal_ucpu_t;   //与处理器位宽相同的类型
typedef uint32_t osal_task_id_t;
typedef uint32_t osal_system_tick_t;



//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_TYPEDEF_H__
//==================================================================================================
//end files
