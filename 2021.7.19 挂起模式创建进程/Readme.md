# 挂起模式启动进程



使用`CREATE_SUSPENDED`挂起进程，再使用`ResumeThread`恢复执行



![process9](process9.gif)



## 获取ImageBase + OEP



使用到线程挂起时的`CONTEXT`结构，`ImageBase + OEP`存放在CONTEXT结构中的`Eax`寄存器里



![image-20210719154220072](image-20210719154220072.png)



## 获取ImageBase



读取其他进程用到`ReadProcessMemory`，`ImageBase`存放的地址为CONTEXT结构中的`Ebx + 8`



![image-20210719160046827](F:/笔记/二进制/image-20210719160046827.png)