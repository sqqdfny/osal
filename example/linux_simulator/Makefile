# mainWindow make file

DIR_CROSS =
C_CC = $(DIR_CROSS)gcc
C_FLAGS = $(DEBUG) -Wall -g -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast


DirRoot    = ../../
DirApp     = $(DirRoot)example/linux_simulator/
DirObjRoot = $(DirApp)obj/
DirLib     = 

#source file
#源文件，自动找所有.c，
SOURCES_C =  $(wildcard $(DirApp)*.c)
include $(DirRoot)osal/component.mk

#将目标定义为同名.o文件
OBJS =  $(patsubst $(DirRoot)%.c,$(DirObjRoot)%.o,${SOURCES_C})

#包含路径
C_FLAGS += -I$(DirRoot)

#宏定义
C_FLAGS += -DUSED_PLATFORM_LINUX_SIMULATOR

#target you can change main to what you want
TARGET = main

$(TARGET):$(OBJS)
	$(C_CC) $(C_FLAGS) -o $@ $^ -lpthread

$(DirObjRoot)%.o:$(DirRoot)%.c
	@test -d $(@D) || mkdir -p $(@D)
	$(C_CC) $(C_FLAGS) -c  $^ -o $@

.PHONY:clean
clean:
	-rm -rf main $(DirApp)obj


