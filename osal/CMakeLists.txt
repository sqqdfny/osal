# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.5)

# 获取当前目录名
STRING( REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

# 项目信息
set(PROJ_NAME ${CURRENT_FOLDER})
project(${PROJ_NAME} LANGUAGES C)

# 添加当前目录下的所有源文件
# 并保存到 SRC_LIST
# 此处必须手动添加每一个源文件, 不能使用 file 函数搜索, 否则会因为递归搜索 build 目录导致报 main() redefintion 错误
set(SRC_LIST "")

# 查找core目录下的所有源文件
# 并保存到变量 SRC_LIST_DIR_CORE
file(GLOB_RECURSE SRC_LIST_DIR_CORE "core/*.c")
list(APPEND SRC_LIST ${SRC_LIST_DIR_CORE})   # 添加到 SRC_LIST

# 查找hal目录下的所有源文件
# 并保存到变量 SRC_LIST_DIR_HAL
file(GLOB_RECURSE SRC_LIST_DIR_CORE "hal/*.c")
list(APPEND SRC_LIST ${SRC_LIST_DIR_HAL})   # 添加到 SRC_LIST

# 指定生成目标
message("OSAL_SRC_LIST:" ${SRC_LIST})
add_library(${PROJ_NAME} ${SRC_LIST})
