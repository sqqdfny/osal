/**
 * @file osal_hal.c
 * @brief 硬件操作层的实现，包括定时器和中断控制相关，移植时需要修改该文件
 *        本文件为 xilinx microblaze 的实现
 * @date 2021-06-08
 * @author sqqdfny
 *         sqqdfny@163.com
 */

#ifdef USED_PLATFORM_MICROBLAZE
//==================================================================================================
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../include/osal/hal/osal_hal.h"
#include <xil_io.h>
#include <xintc_l.h>
#include "../../../include/platform/intc/intc.h"

static volatile uint32_t g_enter_critical_count = 0;
static volatile osal_system_tick_t g_system_tick;

//==================================================================================================
void OSAL_HAL_DISABLE_ALL_INTERRUPT(void)  
{  
	Xil_Out32(((INTC*)GetCurIntcInstance())->BaseAddress + XIN_MER_OFFSET, 0);
	g_enter_critical_count ++;
}

void OSAL_HAL_ENABLE_ALL_INTERRUPT(void)
{
	g_enter_critical_count ? (g_enter_critical_count --) : 0;
	if(0 == g_enter_critical_count)
	{
		Xil_Out32(((INTC*)GetCurIntcInstance())->BaseAddress + XIN_MER_OFFSET, XIN_INT_MASTER_ENABLE_MASK);
	}
}
//==================================================================================================
//此处添加硬件定时器中断溢出函数，中断周期1～10ms
static void SysTick_Handler(void *InstancePtr)
{
	g_system_tick ++;
}
/**
 * @brief 获取当前的tick值
 */
osal_system_tick_t OsalHalGetCurSystemTick(void)
{
    return g_system_tick;
}

/**
 * @brief 硬件定时器初始化，设定系统时钟
 */
void OsalHalTimerTickInit(void)
{
	int Status;
	g_system_tick = 0;
	
	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	Status = XIntc_Connect(GetCurIntcInstance(), XPAR_MICROBLAZE_0_AXI_INTC_SYSTEM_IN7_0_INTR,
				   (XInterruptHandler) SysTick_Handler,
				   NULL);
	if (Status != XST_SUCCESS) {
		while(1)
		{}
	}

	/*
	 * Enable the interrupts for the IIC device.
	 */
	XIntc_Enable(GetCurIntcInstance(), XPAR_MICROBLAZE_0_AXI_INTC_SYSTEM_IN7_0_INTR);
}

/**
 * @brief 开启硬件定时器，为空则一直开启
 */
void OsalHalTimerTickStart(void)
{
	
}

/**
 * @brief 关闭硬件定时器，为空则一直不关闭
 */
void OsalHalTimerTickStop(void)
{

}

/**
 * @brief 没有任务执行时CPU调用的函数
 * 空闲时尽量让core进入休眠
 */
void OsalHalt(void)
{

}
//==================================================================================================
#endif //USED_PLATFORM_MICROBLAZE
//end files

