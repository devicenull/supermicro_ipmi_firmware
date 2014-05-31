#ifndef _Q_ENGINE_H_
#define _Q_ENGINE_H_

#define CONNECT_KEY 120
#define REV 2
#define SND 1
#define MAX_SIZE 2000
#define ARG_NUMB 6

#define R_FLAG 0

typedef struct msgbuff_t
{
  long mtype;
  int errno;
  int cmd;
  int arg1;
  int arg2;
  int arg3;
  int arg4;
  char data[MAX_SIZE];          
} msgbuff_t;

void handler(void);
int q_init(int *);
int q_engine(int , msgbuff_t *, int);
int q_free(int);

#endif




