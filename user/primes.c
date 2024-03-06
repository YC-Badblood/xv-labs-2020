// primes_ans.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define ARRSIZE 35
#define sizeof(i) __SIZEOF_INT__

void sleves(int pleft[2]);

int main(int argc, char **argv)
{
    // 创建管道
    int p[2];
    pipe(p);

    // 创建第一个子进程，并传入管道内数据
    if (fork() == 0)
    {
        close(p[1]); // 关闭写通道
        sleves(p);
        exit(0);
    }
    else
    {
        // 关闭读通道
        close(p[0]);
        int i;
        for (i=2; i <= ARRSIZE; i++)
        {
            write(p[1], &i, sizeof(i));
        }
        // 添加哨兵
        i = -1;
        write(p[1], &i, sizeof(i));
    }
    wait(0);
    exit(0);
}

void sleves(int pleft[2])
{
    int cmp;
    read(pleft[0], &cmp, sizeof(cmp)); // 读出第一个数
    // 若第一个数是-1，结束进程
    if (cmp == -1)
    {
        exit(0);
    }
    printf("prime %d\n", cmp);
    // 创建传出管道
    int pright[2];
    pipe(pright);
    // 第一个数必然是素数

    // 创建子进程，并将管道中内容传出
    if (fork() == 0)
    {
        close(pleft[0]);  // 封闭传入管道的读端
        close(pright[1]); // 封闭传出管道的写端
        sleves(pright);
    }
    else
    {                     // 当前进程，将筛选过的准素数传入管道
        close(pright[0]); // 封闭传出管道的读端
        int buf;
        while (read(pleft[0], &buf, sizeof(buf)) && buf != -1)
        {
            if (buf % cmp != 0)
            {
                write(pright[1], &buf, sizeof(buf));
            }                  
        }
        // -1被筛掉了，需要补写
        buf = -1;
            write(pright[1], &buf, sizeof(buf));
            wait(0); // 父进程都要添加wait
            exit(0); // 进程都要添加exit
    }
}