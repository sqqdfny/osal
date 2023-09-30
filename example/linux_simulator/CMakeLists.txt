# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.5)

# 设置编译器
set (CMAKE_C_COMPILER "/usr/bin/gcc")
#set (CMAKE_CXX_COMPILER "/usr/bin/g++")

# 编译模式()
SET(CMAKE_BUILD_TYPE "Debug")      # "Debug" or "Release"
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_Release "${CMAKE_C_FLAGS_Release} -DNDBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_Release "${CMAKE_CXX_FLAGS_Release} -DNDBUG")


# 编译选项, 全局
add_definitions(-Wall)
# 编译选项, C or c++
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable " CACHE STRING "Set C Compiler Flags" FORCE)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# 添加宏
add_definitions(-DUSED_PLATFORM_LINUX_SIMULATOR)

# 获取当前目录名
STRING( REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

# 设置项目根目录
set(PROJ_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# 项目信息
set(PROJ_NAME ${CURRENT_FOLDER})
project(${PROJ_NAME} LANGUAGES C)

# 添加当前目录下的所有源文件
# 并保存到 SRC_LIST
# 此处必须手动添加每一个源文件, 不能使用 file 函数搜索, 否则会因为递归搜索 build 目录导致报 main() redefintion 错误
set(SRC_LIST "main.c")

# 查找src目录下的所有源文件
# 并保存到变量 SRC_LIST_DIR_SRC
file(GLOB_RECURSE SRC_LIST_DIR_SRC "src/*.c")
list(APPEND SRC_LIST ${SRC_LIST_DIR_SRC})   # 添加到 SRC_LIST

# 添加 osal 库和目录
# 设置OSAL的ROOT路径
set(OSAL_ROOT_DIR ${PROJ_ROOT}/osal)
message("OSAL_ROOT_DIR:" ${OSAL_ROOT_DIR})
include(${PROJECT_SOURCE_DIR}/../../osal/FileLists.cmake)
include_directories(${PROJECT_SOURCE_DIR}/../../osal) 
list(APPEND SRC_LIST ${osal_core_SRCS})   # 添加到 SRC_LIST

# 指定生成目标
message("C_FLAGS:" ${CMAKE_C_FLAGS})
message("CXX_FLAGS:" ${CMAKE_CXX_FLAGS})
message("SRC_LIST:" ${SRC_LIST})
message("PROJECT_BINARY_DIR:" ${PROJECT_BINARY_DIR})
message("PROJECT_SOURCE_DIR:" ${PROJECT_SOURCE_DIR})
add_executable(${PROJ_NAME} ${SRC_LIST})

#添加第三方库
target_link_libraries(${PROJ_NAME} PUBLIC  pthread)

target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                          )