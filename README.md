
这是仿照TI的OSAL重写的一个基于事件驱动的框架，主要用于适配资源比较紧张的MCU。

example/linux_simulator 目录为一个基于linux编译和运行的示例程序，简单的演示了创建任务和定时器的使用，以及向任务发送消息。
linux_simulator 编译步骤：
1. cd example\linux_simulator  // 进行DEMO主目录
2. mkdir build                 // 创建编译目录
3. cd build                    // 进行编译目录
4. cmake ..                    // 运行 Cmake
5. make                        // 运行 make
6. ./linux_simulator           // 运行编译生成的可执行文件 