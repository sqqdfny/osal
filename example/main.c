
#include "../core/osal_api.h"


//==================================================================================================
static uint32_t MainTask(uint8_t task_id)
{

    return 0;
}
//==================================================================================================
static const OsalTaskFun_p g_pTaskList[] = 
{
    MainTask,
};


int main()
{
    return 0;
}
