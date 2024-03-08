// xargs.c
#include "kernel/types.h"
#include "user/user.h"
#include "user/user.h"
#include "kernel/fs.h"

void run(char * program, char **args){
    if(fork() == 0){    // child exec
        exec(program, args);
        exit(0);
    }
    return;
}

int main(int argc, char *argv[]){
    char buf[512];  // 读入时使用的内存池
    char *p = buf, *last_p=buf; // 当前参数的结束、开始指针
    // 字符串指针数组
    char *argsbuf[128]; // 全部参数列表，包含argv传进来的参数和stdin读入的参数
    char **args = argsbuf;  // 指向argsbuf中第一个从stdin读入的参数
    for (int i=1;i<argc;i++){
        // 将argv中的参数复制到最终的参数列表中
        *args = argv[i];
        args++;
    }
    char **pa=args; // 指向各个参数的指针，是一个工作指针
    // 开始读标准输入
    while(read(0, p, 1) > 0){   // char占一个字节
        if(*p == ' ' || *p == '\n'){
            // 读入一个参数完成（以空格分离，如`echo hello world`,则
            // echo和hello各为一个参数）
            *p = '\0'; // 将空格替换为\0 分割开各个参数，这一可以直接使用内存池中的字符串作为参数字符串，而不用开辟额外空间
            /*
            下面一行为什么能这么写？
            last_p指向的是刚刚结束的这个参数的开始位置，所以last_p+1
            也就是存了该参数的地址，而不用将参数完整复制进一个新的位置
            pa也只是存各个参数的指针，所以也不用复制参数 
            */
            *(pa++) = last_p;  // 将参数指针加入参数列表，pa指向下一个参数
            last_p = p+1;   // 更新下一个参数的开始指针

            // 这个操作对应回车键，表示输入结束
            if(*p == '\n'){
                // 读入完成
                *pa = 0;    // 参数列表最后一个参数为0
                run(argsbuf[0], argsbuf);   // 执行命令 
                pa = args;  // 重置读入参数指针，准备读下一行
            }
        }
        p++;
    }
    if(pa != args){ // 如果最后一行没有换行符
        *p='\0';    // 将最后一个参数的结束符设置为0
        *(pa++) = last_p;  // 将参数指针加入参数列表，pa指向下一个参数
        // 读入完成
        *pa = 0;    // 参数列表最后一个参数为0
        run(argsbuf[0], argsbuf);   // 执行命令 
    }
    while(wait(0) != -1);    // 等待所有子进程结束
    exit(0);
}