//              | 信号量 semaphore |
//       1. 进程对共享数据操作，用来资源保护(A写很长一段数据，时间片突然timeout，切换的时候，B再写就寄了)
//       2. 线程里的锁也是同理
//       3. 操作原理： API_set ——> 写操作 ——> API_set
//          - 通过设置一个变量，当成开关一样，根据 开/关 来执行相应的 睡眠/执行 操作
//       4. 信号量功能： 互斥/同步
//          - 互斥：即使时间片timeout，切换后发现semaphore是开着的，立马睡眠
//          - 同步：同步只是保证了进程操作的 [顺序]
//       5. 本质：就是上锁
//          - 内核创建的变量，进程操作时，首先检查这个值

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>

int main() 
{


    return 0;
}