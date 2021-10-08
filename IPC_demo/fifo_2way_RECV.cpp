#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO1 "/Users/dmm/Desktop/fifo1"
#define FIFO2 "/Users/dmm/Desktop/fifo2"

int create_open_fifo(const char *fileName, int open_mode)
{
    int ret = mkfifo(fileName, 0664);
    if (ret == -1 && errno != EEXIST)
    {
        printf("[ERROR]  make fifo error \n");
        exit(-1);
    }

    int fd = open(fileName, open_mode);
    if (fd == -1)
    {
        printf("[ERROR]  open fifo error \n");
        exit(-1);
    }

    return fd;
}

int main()
{
    char buf[100] = {0};

    // fd1:  接收端 --write--> fifo1
    int fd1 = create_open_fifo(FIFO1, O_WRONLY);
    // fd2:  接收端 <--read-- fifo2
    int fd2 = create_open_fifo(FIFO2, O_RDONLY);

    
    // 父进程：读管道 | 子进程：写管道 
    // 由于读会阻塞，为了让读写分离，使用多进程【用线程会更好】
    int ret = fork();
    if (ret == 0)
    {
        int cnt = 5;
        while (cnt--)
        {
            bzero(buf, sizeof(buf));
            scanf("%s", buf);

            printf("[SEND] --send \n");
            write(fd1, buf, sizeof(buf));
        }
    }
    else if (ret > 0)
    {
        int cnt = 5;
        while (cnt--)
        {
            bzero(buf, sizeof(buf));
            read(fd2, buf, sizeof(buf));

            printf("[SEND] recv: %s\n", buf);
        }
    }
    else
    {
        printf("== fork failed ==\n");
        exit(-1);
    }

    return 0;
}