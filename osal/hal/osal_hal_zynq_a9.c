/**
 * @file osal_hal.c
 * @brief 硬件操作层的实现，包括定时器和中断控制相关，移植时需要修改该文件
 *        本文件为 xilinx ZYNQ A9 的实现
 * @date 2023-03-11
 * @author sqqdfny
 *         sqqdfny@163.com
 */

#ifdef USED_PLATFORM_ZYNQ_A9
//==================================================================================================
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../include/osal/hal/osal_hal.h"
#include "../../../include/platform/intc/intc.h"
#include "xil_exception.h"
#include "xscutimer.h"
#include "xtime_l.h"

#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define TIMER_IRPT_INTR		XPAR_SCUTIMER_INTR


static volatile uint32_t g_enter_critical_count = 0;
static volatile osal_system_tick_t g_system_tick;
static XScuTimer sgTimerInstance;	/* Cortex A9 Scu Private Timer Instance */
//==================================================================================================
void OSAL_HAL_DISABLE_ALL_INTERRUPT(void)  
{  
	Xil_ExceptionDisable();
	g_enter_critical_count ++;
}

void OSAL_HAL_ENABLE_ALL_INTERRUPT(void)
{
	g_enter_critical_count ? (g_enter_critical_count --) : 0;
	if(0 == g_enter_critical_count)
	{
		Xil_ExceptionEnable();
	}
}
//==================================================================================================
//此处添加硬件定时器中断溢出函数，中断周期1～10ms
static void SysTick_Handler(void *InstancePtr)
{
	XScuTimer_ClearInterruptStatus((XScuTimer *) InstancePtr);
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
	
	XScuTimer_Config *ConfigPtr;

	/*
	 * Initialize the Scu Private Timer driver.
	 */
	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);

	/*
	 * This is where the virtual address would be used, this example
	 * uses physical address.
	 */
	Status = XScuTimer_CfgInitialize(&sgTimerInstance, ConfigPtr,
					ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return ;
	}

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	Status = XScuGic_Connect(GetCurIntcInstance(), TIMER_IRPT_INTR,
				(Xil_ExceptionHandler)SysTick_Handler,
				(void *)&sgTimerInstance);
	if (Status != XST_SUCCESS) {
		return ;
	}

	/*
	 * Enable the interrupt for the device.
	 */
	XScuGic_Enable(GetCurIntcInstance(), TIMER_IRPT_INTR);

	/*
	 * Enable the timer interrupts for timer mode.
	 */
	XScuTimer_EnableInterrupt(&sgTimerInstance);

	/*
	 * Enable Auto reload mode.
	 */
	XScuTimer_EnableAutoReload(&sgTimerInstance);

	/*
	 * Load the timer counter register.
	 */
	//XScuTimer_LoadTimer(&sgTimerInstance, 0xFFFF);//
	XScuTimer_LoadTimer(&sgTimerInstance, COUNTS_PER_SECOND / (1000 / OSAL_MS_PER_TICK));   //1ms

	/*
	 * Start the timer counter and then wait for it
	 * to timeout a number of times.
	 */
	XScuTimer_Start(&sgTimerInstance);
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
#endif  //USED_PLATFORM_ZYNQ_A9
//end files
