#include "kernel/types.h"
#include "user/user.h"
/*
 xv6可运行
 chapter01: ping pong练习程序
*/

int main()
{
    // pipe1(p1):读子端进程,写父端进程 父到子方向
    // pipe2(p2):读父端进程,写子端进程 子到父方向
    int p1[2], p2[2];

    // 来回传输的字符数组，长度为一个字节
    char buffer[] = {'X'};
    long length = sizeof(buffer);

    pipe(p1);
    pipe(p2);

    // 创建子进程并切换到子进程
    if (fork() == 0)
    {
        // 父读子写
        close(p1[1]); // 关父进程写
        close(p2[0]); // 关子进程读

        // 子进程从pipe1的读端，读取字符数组
        if (read(p1[0], buffer, length) != length)
        {
            printf("a->b read error!");
            exit(1);
        }

        printf("%d: received ping\n", getpid());

        // 子进程写
        if (write(p2[1], buffer, length) != length)
        {
            printf("b->a write error");
            exit(1);
        }
        //printf("%d: child sent pong\n", getpid());
        exit(0);
    }
    // 父进程，从父读，并塞进管道
    close(p1[0]); // 阻塞父-》子读出
    close(p2[1]); // 阻塞子-》父写入
    // 将buffer塞入管道中
    if (write(p1[1], buffer, length) != length)
    {
        printf("a->b write error!");
        exit(1);
    }
    //printf("%d: father sent ping\n", getpid());
    // 从管道中取出buffer
    if (read(p2[0], buffer, length) != length)
    {
        printf("b->a read error");
        exit(1);
    }
    printf("%d: received pong\n", getpid());
    // 等待子进程结束
    wait(0);
    exit(0);
}