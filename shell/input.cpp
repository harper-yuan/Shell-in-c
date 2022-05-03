#include<bits/stdc++.h>
#include"input.h"
#include"var.h"
using namespace std;

int get_input(char *buf)
{
    memset(buf,0,BUFFSIZE);
    memset(backupBuf,0,BUFFSIZE);

    fgets(buf,BUFFSIZE,stdin);//表示从输入流读取字符，即键盘输入的内容
    buf[strlen(buf) - 1] = '\0';
    return strlen(buf);
}

/*
举例：buf为ls -a
那么将ls放到command[0]
-a 放到command[1]中
*/
void parse(char *buf)
{
    //初始化command和argv数组
    for(i = 0;i<MAX_CMD;i++)
    {
        argv[i] = NULL;
        command[i].clear();
    }

    strcpy(backupBuf,buf);

    //将buf指令解析按照空格分成几个部分
    string str = buf ; 
    int index;
    for(i=0 ; (index = str.find(" ")) != string::npos ;i++)
    {
        command[i] = str.substr(0,index);
        str.erase(0,index+1);
    }
    command[i] = str;

    /** 构建argv数组
     *  即若输入buf为"ls -a"
     *  strcmp(argv[0], "ls") == 0 成立且
     *  strcmp(argv[1], "-a") == 0 成立*/
    argc = 0;
    int flag = 1;
    for(i=0;buf[i]!='\0';i++)
    {
        if(flag == 1 && buf[i]!=' ')
        {
            flag = 0;
            argv[argc++] = buf + i;
        }
        else if (flag == 0 && buf[i]==' ')
        {
            flag = 1;
            buf[i] = '\0';
        }
    }
}



