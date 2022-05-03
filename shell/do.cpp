#include<bits/stdc++.h>
#include"input.h"
#include"var.h"
#include"do.h"
#include <sys/wait.h>
using namespace std;


void push_queue(circular_queue *q , char *buf)
{
    if((q->front+1)%MAX_HISTORY == q->rear) //queue is full
    {
        q->front = (q->front+1)%MAX_HISTORY;
        q->rear = (q->rear+1)%MAX_HISTORY;//overwrite the orginal content
        strcpy(q->queue[q->front],buf);
    }
    else
    {
        q->front = (q->front+1)%MAX_HISTORY;
        strcpy(q->queue[q->front],buf);
    }
}

void print_history(circular_queue *q)
{
    int end = q->rear , i = 1;
    while(end!=q->front)
    {
        cout<<i<<' '<<q->queue[end+1]<<endl;
        i++;
        end = (end+1)%MAX_HISTORY;
    }
}

int included(string command[MAX_CMD] , string word)
{
    for(i = 0;i<MAX_CMD;i++)
    {
        if(command[i] == word)
        return 1;
    }
    return 0;
}

static void sig_child(int signo)
{
    pid_t        pid; 
    int        stat;
     //处理僵尸进程
     while ((pid = waitpid(-1, &stat, WNOHANG)) >0)
            printf("child process %d terminated.\n", pid);
}

void command_in_background(char *buf)
{
    char back_ground_buf[strlen(buf)];
    memset(back_ground_buf,'\0',strlen(buf));
    for(i = 0;i<BUFFSIZE;i++) //clear symbol "&"
    {
        if(back_ground_buf[i] == '&') {back_ground_buf[i-1]='\0' ; break;}
    }
    pid_t pid;
    signal(SIGCHLD,sig_child);
    pid = fork();
    if(pid<0)
    {
        perror("fork()");
    }
    else if(pid == 0)
    {
        parse(back_ground_buf);
        signal(SIGCHLD,SIG_IGN);//init process takes over child process
        execvp(argv[0],argv);
        printf("%s: 命令输入错误\n", argv[0]);
        // exit函数终止当前进程, 括号内参数为1时, 会像操作系统报告该进程因异常而终止
        exit(1);
    }
    else return;
}

int call_cd()
{
    int result = chdir(command[1].c_str());//The input of chdir funcation must be char*
    if(result+1) return 1;
    else 
    {
        cout<<"Path dose not exist,my baby!"<<endl<<"Are you kidding me?"<<endl;
        return 0;
    }
}


int command_with_pipe(char buf[BUFFSIZE]) {
    // 获取管道符号的位置索引
    for(j = 0; buf[j] != '\0'; j++) {
        if (buf[j] == '|')
            break;
    }

    // 分离指令, 将管道符号前后的指令存放在两个数组中
    // outputBuf存放管道前的命令, inputBuf存放管道后的命令
    char outputBuf[j];
    memset(outputBuf, 0x00, j);
    char inputBuf[strlen(buf) - j];
    memset(inputBuf, 0x00, strlen(buf) - j);
    for (i = 0; i < j - 1; i++) {
        outputBuf[i] = buf[i];
    }
    for (i = 0; i < strlen(buf) - j - 1; i++) {
        inputBuf[i] = buf[j + 2 + i];
    }


    int pd[2];
    pid_t pid;
    if (pipe(pd) < 0) {
        perror("pipe()");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(1);
    }


    if (pid == 0) {                     // 子进程写管道
        close(pd[0]);                   // 关闭子进程的读端
        dup2(pd[1], STDOUT_FILENO);     // 将子进程的写端作为标准输出
        parse(outputBuf);
        execvp(argv[0], argv);
        if (pd[1] != STDOUT_FILENO) {
            close(pd[1]);
        }
    }else {                              // 父进程读管道
        /** 关键代码
         *  子进程写管道完毕后再执行父进程读管道, 
         *  所以需要用wait函数等待子进程返回后再操作
         */
        int status;
        waitpid(pid, &status, 0);       // 等待子进程返回
        int err = WEXITSTATUS(status);  // 读取子进程的返回码
        if (err) { 
            printf("Error: %s\n", strerror(err));
        }
        pid = fork();
        if (pid < 0) 
        {
            perror("fork()");
            exit(1);
        }
        else if(pid == 0)
        {
            close(pd[1]);                    // 关闭父进程管道的写端
            dup2(pd[0], STDIN_FILENO);       // 管道读端读到的重定向为标准输入
            parse(inputBuf);
            execvp(argv[0], argv);
            if (pd[0] != STDIN_FILENO) close(pd[0]);
        }
        else return 0;
    }
    return 1;
}

void do_cmd(int argc , char *argv[])
{
    if(included(command , "|"))
        {
            strcpy(buf,backupBuf);
            command_with_pipe(buf);
            return;
        }
    if(included(command , "&"))
        {
            strcpy(buf,backupBuf);
            command_in_background(buf);
            return;
        }
    if(included(command,"cd"))
        call_cd();
    else if(included(command,"history"))
        print_history(&q);
    else if(included(command,"exit"))
        exit(0);
    else
    {
        pid = fork();
        if(pid<0)
        {
            cout<<"Fail to creat child process!";
            return;
        }
        else if(pid == 0)
        {
            execvp(argv[0], argv);
            printf("%s: 命令输入错误\n", argv[0]);
            // exit函数终止当前进程, 括号内参数为1时, 会像操作系统报告该进程因异常而终止
            exit(1);
        }
        else
        {
            int status;
            waitpid(pid,&status,0);//parent process waits for child process to return
        }
    }
}