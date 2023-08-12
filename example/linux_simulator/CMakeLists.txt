# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.5)

# 编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast")
#add_definitions(-Wall -g -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast)

# 添加宏
add_definitions(-DUSED_PLATFORM_LINUX_SIMULATOR)

# 获取当前目录名
STRING( REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

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

# 查找osal目录下的所有源文件
# 并保存到变量 SRC_LIST_DIR_OSAL
file(GLOB_RECURSE SRC_LIST_DIR_OSAL "../../osal/*.c")
list(APPEND SRC_LIST ${SRC_LIST_DIR_OSAL})   # 添加到 SRC_LIST

# 添加 include 目录
include_directories(${PROJECT_SOURCE_DIR}/../../osal) 

# 添加库
#add_subdirectory(/../../osal)

# 指定生成目标
message("SRC_LIST:" ${SRC_LIST})
message("PROJECT_BINARY_DIR:" ${PROJECT_BINARY_DIR})
message("PROJECT_SOURCE_DIR:" ${PROJECT_SOURCE_DIR})
add_executable(${PROJ_NAME} ${SRC_LIST})

target_link_libraries(${PROJ_NAME} PUBLIC  pthread)

target_include_directories(${PROJECT_NAME} PUBLIC
                           ${PROJECT_BINARY_DIR}
                           ${PROJECT_SOURCE_DIR}../../osal
                          )