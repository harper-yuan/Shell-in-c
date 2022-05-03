#include"var.h"
int argc;      // 命令行的有效参数个数
char* argv[MAX_CMD];      // 参数指针数组
string  command[MAX_CMD]; // 参数二维数组
char buf[BUFFSIZE];    // 接受键盘输入的参数数组
char backupBuf[BUFFSIZE]; // buf数组的备份.
int i ;
int j ;
pid_t pid;
circular_queue q; //= (circular_queue *)malloc(sizeof(circular_queue));