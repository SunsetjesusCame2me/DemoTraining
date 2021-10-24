//          | message queue |
//      1. 内核创建存放消息的链表
//      2. 一个结点 就是一个消息
//          - kernel里的结构：[编号｜消息正文]
//      3. 进程封装 [消息包]
//          struct msgbuf
//            {
//                  long m_type;  消息编号
//                  char m_text[mgsz]; 消息正文
//            };
//      4. 不同的进程，通过 [m_type] 在队列里面取就OK
//      5. 实现网状交叉通信方便
//      6. 消息队列可以有 [阻塞方式] && [非阻塞方式] send/recv

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
#define FILE_PATH "../path"
#define MSG_SIZE 1000

// 定义消息结点: 只能有long和char的两个成员
struct msg_buf
{
    long m_type;
    char m_content[MSG_SIZE];    
};


int create_or_get_msgQueue()
{
    int msgid = -1;
    key_t key = -1;
    int fd = 0;

    fd = open(FILE_PATH, O_RDWR|O_CREAT, 0664);
    if (fd == -1) exit(-1);

    key = ftok(FILE_PATH, 'b');
    if (key == -1) exit(-1);

    // 获取or创建
    msgid = msgget(key, 0664|IPC_CREAT);
    if (msgid == -1) exit(-1);

    return msgid;
}

int main()
{
    int msgid = create_or_get_msgQueue();

    int ret = fork();
    if (ret == -1) exit(-1);

    // 子进程发送消息
    if (ret == 0)
    {
        msg_buf buff;
        
        int cnt = 5;
        while (cnt--)
        {
            // 封装消息包
            buff.m_type = 2251930;
            scanf("%s", buff.m_content);
            
            // 0——阻塞方式发送消息, 把结构体地址传进来
            msgsnd(msgid, &buff, MSG_SIZE, 0);
        }

        // 弄完干掉消息队列 --> 类似于 智能指针 的引用计数
        msgctl(msgid, IPC_RMID, NULL);

        printf("son dead\n");
    }
    // 父进程接受消息
    else 
    {
        msg_buf buff;
        
        int cnt = 5;
        while (cnt--)
        {
            // 0——阻塞方式读
            msgrcv(msgid, &buff, MSG_SIZE, 2251930, 0);

            printf("recv id: %d\n", buff.m_type);
            printf("recv content: %s\n", buff.m_content);
        }
        msgctl(msgid, IPC_RMID, NULL);

        printf("father dead\n");
    }


    return 0;
}