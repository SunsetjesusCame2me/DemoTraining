//                  |shared memory|
//          创建pp的shm --> 将它映射到一个起始vp --> 弄完之后记得删除shm          
//              tips：void* 是不允许加减的指针类型，只有转换了之后才能操作下
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define SHM_FILE_PATH "../path"
#define SHM_SIZE 4096

// 往shm写结构体类型
struct Node
{
    int a;
    char b;
};

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
    
    int ret = fork();
    if (ret==-1) exit(-1);

    // 子进程：来接收shm里面的东西
    if (ret == 0)
    {
        create_or_get_shm();

        shmaddr = shmat(shmid, NULL, 0);
        if (shmaddr == (void*)-1) exit(-1);

        int cnt=5, idx=0;
        while (cnt--)
        {
            printf("Node.a = %d  |",(*(((Node*)shmaddr)+idx)).a);
            printf("Node.b = %c\n", (*(((Node*)shmaddr)+idx)).b);
            idx++;
            
            sleep(1);
        }
    }
    // 父进程：写shm内容
    else 
    {
        create_or_get_shm();
        
        // 0-可写可读；NULL-系统自定一个初始地址
        shmaddr = shmat(shmid, NULL, 0);
        if (shmaddr == (void*)-1) exit(-1);

        int cnt=5, idx=0;
        while (cnt--)
        {
            (*(((Node*)shmaddr)+idx)).a = cnt;
            (*(((Node*)shmaddr)+idx)).b = 'F';
            
            // void不允许指针算术运算，转换type后，+1就是想要移动的bias
            printf("write at: %p\n", (((Node*)shmaddr)+idx));
            idx++;
            
            sleep(1);
        }
    }
    
    return 0;
}