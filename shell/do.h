#include"var.h"
#ifndef _DO_H_
#define _DO_H_

int call_cd();

void push_queue(circular_queue *q , char *buf);
void print_history(circular_queue *q);
int find(char command[MAX_CMD][MAX_CMD_LEN] , char word[5]);

static void sig_child(int signo);
void command_in_background(char *buf);

int command_with_pipe(char buf[BUFFSIZE]);

int included(char command[MAX_CMD][MAX_CMD_LEN] , char word[5]);
void do_cmd(int argc , char *argv[]);


#endif