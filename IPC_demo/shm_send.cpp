//                  |shared memory|
//   参数shmget: [key]: 
//                   1.自己指定长整型数(hex)
//                   2.ftok函数：通过filePath和8位int来生成（常用）
//                   
//              [size]：共享内存大小，4k的整数倍(1页4k B)，如果不是整数倍，会向上取整
//              [shmflag]: 文件权限 0664|IPC_CREAT
//
//          tips：void* 是不允许加减的指针类型，只有转换了之后才能操作下
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define SHM_FILE_PATH "../path"
#define SHM_SIZE 4096

int shmid = -1;

void create_or_get_shm()
{
    int fd = open(SHM_FILE_PATH, O_CREAT|O_RDWR, 0664);
    if (fd == -1) exit(-1);

    // 8位整型数 -> char一个就ok
    key_t key = ftok(SHM_FILE_PATH, 'b');
    if (key == -1) exit(-1);

    // 如果已经创建，就使用；没有就创建
    shmid = shmget(key, SHM_SIZE, 0664|IPC_CREAT);
    if (shmid == -1) exit(-1);
}

int main()
{
    void* shmaddr = NULL;

    // 1. 创建pp，还没映射
    create_or_get_shm();

    // 2. 把pp映射到vp，返回映射后的vp起始地址
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1) exit(-1);

    // 映射的空间指针类型是 void* ，所以可以为所欲为，存什么类型就转什么类型
    int cnt = 10;
    int i=0;
    while (cnt--)
    {
        *(((int*)shmaddr)+i) = cnt;

        printf("write at: %p\n", ((int*)shmaddr)+i);
        // 因为转换成了 int*，所以自加一下就是4字节，不要搞成+=4
        // i+=4;
        ++i;
    }

    return 0;
}