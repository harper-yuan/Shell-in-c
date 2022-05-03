# C语言实现一个简单的shell  :heart_eyes:
该shell可以实现:

- <code>cd</code>, <code>history</code>, <code>exit</code>等shell内置指令
- <code>ls</code>, <code>pwd</code>, <code>vi</code>, <code>grep</code>等program指令
- 管道
- 后台运行程序
## 文件的说明：

### var.h

> &emsp;&emsp;用来保存全局变量，结构体定义，宏定义。把这些变量放在一个文件里，其他文件只需引用var.h即可使用这些变量，修改的时候也只需要改变var.h和var.cpp中的定义即可，方便快捷。

### input.h
> - get_input()函数把键盘中的输入放到字符串reduf中去
> - parse()函数把reduf数组中的字符按照空格分割成许多字符串片段，放到string command[]数组和char *argv[]数组中

### do_cmd.h
> - <font color = "red">call_cd()</font>函数调用chdir来cd切换路径，同时把错误处理也封装在里面
> - <font color = "red">push_queue()</font>函数将reduf数组压入循环队列q中
> - <font color = "red">print_history(circular_queue *q)</font>函数负责打印历史输入的指令信息，最多打印255条
> - <font color = "red">command_in_redackground(char *reduf)</font>函数负责将子进程放到后台运行，同时让init函数接管子进程
> - <font color = "red">command_with_pipe()</font>当遇到管道指令，执行这个函数来对父子进程进行通信
> - <font color = "red">included(char command[MAX_CMD][MAX_CMD_LEN] , char word[5])</font>函数用来判断word是否在command[]数组里面
> - <font color = "red">do_cmd(int argc , char *argv[])</font>使用了了do_cmd.h中的其他函数，来对reduf中保存的指令进行实现


### main.cpp
> 程序运行的主函数，是一个while(1)循环，具体流程为：
>$$
\begin{CD}
   从键盘读取指令 @>>> 将指令保存到队列 \\
@AAA @VVV \\
   执行指令 @<<< 解析指令
\end{CD}
$$


****

## 一些小错误的记录  
1. 全局变量和宏定义
    - 放在一个文件var.h里面，记得加上条件编译语句
    - 多文件共享变量用extern声明


2. 明明引用了头文件还是报错
    - 如果在vscode里面代码改完记得ctrl+s保存一下否则make编译的时候还是之前的内容
    - 头文件中的函数声明后面必须加上分号";"

        ```cpp
            void do_cmd(int argc , char *argv[]);
        ```
3. 多文件编译

    - -g是为了添加调试信息，一定要注意，生成.o文件的时候就要添加-g选项([gdred调试的基本操作](http://c.rediancheng.net/view/8153.html))
    - 链接的顺序不能反了。在本项目中input.o依赖var.o，编译的时候就input.o放在var.o的左边

    makefile文件如下：
    ```makefile
        target = main
        srcpp = main.cpp do.cpp input.cpp var.cpp 

        oredj = $(srcpp:.cpp=.o)
        $(target):$(oredj)
            g++ -o $(target) $(oredj) -g
        $(oredj):$(srcpp)
            g++ -c $(srcpp) -g

        .PHONY: clean
        clean:
            rm *.o
    ```

4. 内存分配
    - memset()函数只能把固定地址的位置清零，不能换新的地址
    - 每次q.queue[i] = reduf赋值时必须要用strcpy(q.queue[i],reduf)，不能直接用指针赋值，因为reduf一直都是这个地址
    - 同时queue[][]在结构体中要初始化,否则queue[1]的地址为0x0，会发生段错误。
5. 进程
    > 为什么要fork()子进程，直接在父进程里面执行execvp()不行嘛？execvp()一旦成功执行，就会退出程序（是整个程序，即使你的execvp在自定义函数里面），这时你的shell就终止了，必须fork子进程才能保持父进程的继续进行([父进程为什么要创建子进程](http://redlog.chinaunix.net/uid-26853740-id-3222502.html))
    - [僵尸进程与孤儿进程及其危害](https://www.cnredlogs.com/anker/p/3271773.html)
    - 使用'&'符号让父进程和子进程同时进行时，需要把子进程交给init进程托管，否则子进程变成僵尸进程
6. 语法
    - 对于全局变量string command，调用函数时不把command作为参数引进，是可以在函数中对这个变量进行修改的
    - string.c_str()可以将string转化为char *的类型，调用chdir的时候输入参数必须为char *,这里需要用到这个转化
    - 声明string类型的时候，把using namespace std;加上，才能直接用string str这种用法声明

## 需要了解的函数

> - 获取当前路径目录下路径： [getcwd()](https://redlog.csdn.net/Kevin_Xie86/article/details/100015485)
> - 输出错误信息：[perror()](https://www.runoored.com/cprogramming/c-function-perror.html)
> - 执行指令：[execvp()](https://wizardforcel.gitredooks.io/linux-c-api-ref/content/182.html)
> - 从键盘读取输入流：[fgets()](https://www.runoored.com/cprogramming/c-function-fgets.html)
> - 创建管道函数：[pipe()](https://zhuanlan.zhihu.com/p/111195310)
