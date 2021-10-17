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
    void* shmaddr = nullptr;

    // 1. 创建pp，还没映射
    create_or_get_shm();

    // 2. 这个recv端也把send搞的共享内存映射过来
    shmaddr = shmat(shmid, NULL, 0);
    if (shmaddr == (void*)-1) exit(-1);

    // 映射的空间指针类型是 void* ，所以可以为所欲为，存什么类型就转什么类型
    int cnt = 10;
    int i=0;
    while (cnt--)
    {
        printf("shm_recv: %d   |   ", *(((int*)shmaddr)+i));
        printf("read at: %p\n", ((int*)shmaddr)+i);
        i++;
    }

    return 0;
}