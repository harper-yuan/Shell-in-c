#ifndef _VAR_H_
#define _VAR_H_

#include<bits/stdc++.h>
using namespace std;

/*宏定义*/
#define MAX_CMD 10    // 最大命令数量
#define BUFFSIZE 255   // 输入最大命令字符数
#define MAX_CMD_LEN 80   // 每条命令的最大长度
#define MAX_HISTORY 256 //maximum number of saved commands



/*struct*/
typedef struct
{
    char queue[MAX_HISTORY][BUFFSIZE] = {'\0'}, front = 0, rear = 0;
}circular_queue;

/* 全局变量 */
extern int argc;      // 命令行的有效参数个数
extern char* argv[MAX_CMD];      // 参数指针数组
extern std::string command[MAX_CMD]; // string数组
extern char buf[BUFFSIZE] ;    // 接受键盘输入的参数数组
extern char backupBuf[BUFFSIZE] ; // buf数组的备份.
extern int i ;
extern int j ;
extern pid_t pid;
extern circular_queue q; //= (circular_queue *)malloc(sizeof(circular_queue));

#endif