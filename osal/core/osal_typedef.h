

#ifndef __OSAL_TYPEDEF_H__
#define __OSAL_TYPEDEF_H__

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    OSAL_ERR_SUCC = 0,
    OSAL_ERR_NO_TASK,
    OSAL_ERR_NO_MEM,
    OSAL_ERR_NO_TIMER,
    OSAL_ERR_INVALID_HANDLE,
}OSAL_ERR_T;

#define OSAL_BIT0     (0x1U)
#define OSAL_BIT1     (0x2U)
#define OSAL_BIT2     (0x4U)
#define OSAL_BIT3     (0x8U)
#define OSAL_BIT4     (0x10U)
#define OSAL_BIT5     (0x20U)
#define OSAL_BIT6     (0x40U)
#define OSAL_BIT7     (0x80U)
#define OSAL_BIT8     (0x100U)
#define OSAL_BIT9     (0x200U)
#define OSAL_BIT10    (0x400U)
#define OSAL_BIT11    (0x800U)
#define OSAL_BIT12    (0x1000U)
#define OSAL_BIT13    (0x2000U)
#define OSAL_BIT14    (0x4000U)
#define OSAL_BIT15    (0x8000U)
#define OSAL_BIT16    (0x10000U)
#define OSAL_BIT17    (0x20000U)
#define OSAL_BIT18    (0x40000U)
#define OSAL_BIT19    (0x80000U)
#define OSAL_BIT20    (0x100000U)
#define OSAL_BIT21    (0x200000U)
#define OSAL_BIT22    (0x400000U)
#define OSAL_BIT23    (0x800000U)
#define OSAL_BIT24    (0x1000000U)
#define OSAL_BIT25    (0x2000000U)
#define OSAL_BIT26    (0x4000000U)
#define OSAL_BIT27    (0x8000000U)
#define OSAL_BIT28    (0x10000000U)
#define OSAL_BIT29    (0x20000000U)
#define OSAL_BIT30    (0x40000000U)
#define OSAL_BIT31    (0x80000000U)

#define OSAL_EVENT_MSG     OSAL_BIT31


#ifndef NULL
	#define NULL 	0
#endif
//==================================================================================================
typedef uint32_t osal_system_tick_t;
typedef uint32_t osal_msg_cmd_t;


//==================================================================================================
#define OsalEnterCritical()   OSAL_HAL_ENTER_CRITICAL()
#define OsalExitCritical()    OSAL_HAL_EXIT_CRITICAL()


#define OsLogInfo(fmt, args...) printf("osal info:" fmt, ##args)
#define OsLogErr(fmt, args...)  printf("osal err:" fmt, ##args)
//==================================================================================================
#ifdef __cplusplus
}
#endif
#endif //__OSAL_TYPEDEF_H__
//==================================================================================================
//end files
