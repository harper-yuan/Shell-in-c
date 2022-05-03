#include<bits/stdc++.h>
#include"var.h"
#include"input.h"
#include"do.h"
using namespace std;

int main()
{
    char path[100];
    while(1)
    {
        getcwd(path,255);
        cout<<path<<">";
        get_input(buf);
        parse(buf);
        push_queue(&q,buf);
        do_cmd(argc,argv);
    }
    return 0;
}
