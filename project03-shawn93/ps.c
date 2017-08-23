#include "types.h"
#include "stat.h"
#include "user.h"

enum procstate { UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE };
struct proc {
  enum procstate state;        
  int pid;          
  uint sz;
  char name[16];              
};

#define MAX_PROC 10

int
main(int argc, char *argv[]){  
  struct proc ptable[MAX_PROC];
  struct proc *p;
  int err;

  err = getptable(10*sizeof(struct proc),&ptable);
  if(err !=0)
    printf(1,"Error getting ptable");
  
  p = &ptable[0];
  printf(1, "PID  STATE  SIZE  CMD \n");
  while(p != &ptable[MAX_PROC-1] && p->state != UNUSED){
  	printf(1,"%d ",p->pid);
  	switch(p->state){
  	case UNUSED:
  		printf(1," %s ", "UNUSED");
  		break;
  	case EMBRYO:
  		printf(1," %s ", "EMBRYO");
  		break;
  	case SLEEPING:
  		printf(1," %s ", "SLEEPING");
  		break;
  	case RUNNABLE:
  		printf(1," %s ", "RUNNABLE");
  		break;
  	case RUNNING:
  		printf(1," %s ", "RUNNING");
  		break;
  	case ZOMBIE:
  		printf(1," %s ", "ZOMBIE");
  		break;
  	}
        printf(1, " %d ", p->sz);
  	printf(1," %s \n", p->name);
  	p++;
  }
  	  
  exit();
}
