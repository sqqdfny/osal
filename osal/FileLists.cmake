
# 列举源文件, OSAL_ROOT_DIR在APP中设置
set(osal_core_SRCS
    ${OSAL_ROOT_DIR}/core/osal_list.c
    ${OSAL_ROOT_DIR}/core/osal_mem.c
    ${OSAL_ROOT_DIR}/core/osal_timer.c
    ${OSAL_ROOT_DIR}/core/osal.c
    ${OSAL_ROOT_DIR}/hal/osal_hal_linux.c
)

# osal libraries 生成 lib(非必须)
add_library(osalcore EXCLUDE_FROM_ALL ${osal_core_SRCS})