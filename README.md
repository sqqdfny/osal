# osal

这是仿照TI的OSAL重写的一个框架，主要用于适配资源比较紧张的MCU。

//==================================================================================================
移植说明
1.core/osal_typedef.c
  提供数据类型的定义，如下
  typedef uint32_t osal_task_id_t;       
  typedef uint32_t osal_system_tick_t;
  typedef uint32_t osal_event_t;

  进入和退出临界状态,如果应用不在中断中调用OSAL的相关接口,这两个宏可以为空
  #define OsalEnterCritical()   OSAL_HAL_ENTER_CRITICAL()
  #define OsalExitCritical()    OSAL_HAL_EXIT_CRITICAL()

  调试打印信息宏,实际应用中这两个宏建议为空
  #define OsalDebugInfo(fmt, args...) printf("osal info:" fmt, ##args)
  #define OsalDebugErr(fmt, args...)  printf("osal err:" fmt, ##args)


2.core/osal_config.h
  系统最大支持的任务个数,最小值为1,此宏应该与实际的任务数一致
  #define OSAL_TASK_NUM       2    

  系统最大支持的定时器个数,最小值为2,此宏根据实际应用需求设置
  #define OSAL_TIMERS_NUM     8    


3.core/osal_mem.h
  内存管理的对齐设置,实际的对齐字节有可能比设置的大
  #define HEAP_ALIGNMENT				16


4.hal/osal_hal.h
  定义每个系统TICK有多少MS,建议为1或者10
  #define OSAL_MS_PER_TICK    10   

4.hal/osal_hal.c
  硬件相关的一些接口实现

  获取当前的tick值,供OSAL系统调用,用来驱动系统定时器
  osal_system_tick_t OsalHalGetCurSystemTick(void);

  硬件定时器初始化,为系统提供TICK,初始化之后定时器为开启状态
  void OsalHalTimerTickInit(void);
  
  以下两个函数用于开启和关闭系统TICK定时器,如果应用没有低功耗的要求,建议为空
  OSAL目前没有提供低功耗与休眠管理,这两个函数是提供给应用层自己实现功耗管理时调用
  void OsalHalTimerTickStart(void);
  void OsalHalTimerTickStop(void);

//==================================================================================================
以下为对外提供的接口清单

1.内存分配与回收,回收内存时,会合并相邻的内存块
  void * OsalMemAlloc(size_t size_req);
  void OsalMemFree(void* p);
 

2.消息队列管理,每个任务有独立的消息队列
  下面两个函数为消息数据块的申请与回收,
  向消息队列发送数据的内存块必须调用OsalMsgQueueAlloc()函数申请
  在任务中处理完消息后,也必须调用OsalMsgQueueFree()函数回收
  void * OsalMsgQueueAlloc(size_t size);
  void OsalMsgQueueFree(void *p);

  向指定任务发送消息和从指定的任务消息队列取出消息
  OsalMsgQueuePost()函数返回不为OSAL_ERR_SUCC时,必须要主动回收相关的消息内存块
  OSAL_ERR_T OsalMsgQueuePost(osal_task_id_t task_id, void *pMsg);
  void* OsalMsgQueuePend(osal_task_id_t task_id);
 

3.定时器
  创建定时器,成功返回定时器句柄指针,创建成功之后为停止状态
  osal_timer_t* OsalCreateTimer(osal_timer_type_t type, uint32_t timeout, void (*callback)(void *param));
  
  删除定时器
  void OsalDeleteTimer(osal_timer_t *pTimer);

  启动定时器,param为传递给回调的参数
  如果传递给回调的参数为内存块,用户需要自己管理内存块的回收
  bool OsalTimerStart(osal_timer_t *pTimer, void *param);

  停止定时器,返回OsalTimerStart()函数传递给定时器的参数
  此函数在定时器删除前可多次调用
  void * OsalTimerStop(osal_timer_t *pTimer);
  

4.事件管理
  OSAL_ERR_T OsalEventSet(osal_task_id_t task_id, osal_event_t events);
  OSAL_ERR_T OsalEventClear(osal_task_id_t task_id, osal_event_t events);
 

5.任务管理
  创建任务,创建成功后,通过pTaskId返回任务ID,任务只能在初始化时创建,不能在运行中动态创建
  此函数在调用OsalInitSystem()之后,调用OsalStartSystem()函数前调用
  OSAL_ERR_T OsalAddTask(osal_task_id_t *pTaskId, osal_event_t (*pProcess)(osal_task_id_t task_id, osal_event_t events));


6.启动系统
  启动系统,此函数不会返回
  void OsalStartSystem(void);

7.初始化系统
  addr:用于内存管理的内存基址
  size_bytes:内存块的大小,单位为字节
  如果不使用 msg_queue, 同时用户也不调用内存管理相关的函数, 可以传入NULL
  void OsalInitSystem(uint32_t * addr, size_t size_bytes);
 
//==================================================================================================