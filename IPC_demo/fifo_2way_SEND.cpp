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
        // EEXIST表示fifo已经存在，无视它
        printf("[ERROR]  make fifo falied! \n");
        exit(-1);
    }

    int fd = open(fileName, open_mode);
    if (fd == -1)
    {
        printf("[ERROR]  open fifo falied! \n");
        exit(-1);
    }

    return fd;
}

int main()
{
    char buf[100] = {0};

    int fd1 = create_open_fifo(FIFO1, O_WRONLY);
    int fd2 = create_open_fifo(FIFO2, O_RDONLY);

    
    // 父进程：读管道 | 子进程：写管道
    //  小细节：父子进程的buf 是不一样的空间【私有的写时复制】
    int ret = fork();
    if (ret == 0)
    {
        int cnt = 5;
        while (cnt--)
        {
            bzero(buf, sizeof(buf));
            scanf("%s", buf);

            printf("[SEND] --send \n");
            write(fd2, buf, sizeof(buf));
        }
    }
    else if (ret > 0)
    {
        int cnt = 5;
        while (cnt--)
        {
            bzero(buf, sizeof(buf));
            read(fd1, buf, sizeof(buf));

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