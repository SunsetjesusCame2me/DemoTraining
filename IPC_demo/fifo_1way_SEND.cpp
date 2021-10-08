/*
*  2021.10.08 22:40 
*       有名管道的单向通信: 发送端进程
*/
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

// 管道文件放到desktop上
#define FIFO_PATH "/Users/dmm/Desktop/fifo"

// @brief:  创建or打开fifo文件
// @param:  open_mode 打开权限
// @ret:    fifo的文件描述符
int create_open_fifo(int open_mode)
{
    // 创建管道文件, 0664保证读写权限, 注意：只有mkfifo才可以创建【管道file】
    int ret = mkfifo(FIFO_PATH, 0664);
    if (ret == -1 && errno != EEXIST)
    {
        // EEXIST表示fifo已经存在，无视它
        printf("[ERROR]  make fifo falied! \n");
        exit(-1);
    }

    // 以打开文件的方式来操作管道: O_WRONLY 只写权限
    int fd = open(FIFO_PATH, open_mode);
    if (fd == -1)
    {
        printf("[ERROR]  open fifo falied! \n");
        exit(-1);
    }

    return fd;
}

int main()
{
    int fd = create_open_fifo(O_WRONLY);
    int cnt = 10;
    char buf[100] = {0};

    while (cnt--)
    {
        bzero(buf, sizeof(buf));
        scanf("%s", buf);
        
        write(fd, buf, sizeof(buf));
    }

    return 0;
}