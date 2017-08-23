include "types.h"
#include "stat.h"
#include "user.h"

int main(void) {
  int count = 0;
  char buf[512];

  printf(1, "Testing pipe_count System Call.\n");
  printf(1,"The pipe is empty.\n");

  int fd[2];
  pipe(fd);

  count = pipe_count(fd[0]);
  printf(1,"Count: %d\n",count);

  printf(1,"Writing \"hello world\" to pipe\n");
  write(fd[1],"hello world\n",12);
  count = pipe_count(fd[0]);
  printf(1,"Count: %d\n", count);
  
  //clean pipe                                                                  
  read(fd[0], buf, 12);

  printf(1,"The pipe is full\n");
  int i = 0;
  while( i < 512){
    buf[i] = 'z';
    i++;
  }
  
  write(fd[1], buf, 512);
  
  count = pipe_count(fd[0]);
  printf(1,"Count: %d\n", count);

  printf(1,"Not a valid pipe\n");

  close(fd[0]);
  close(fd[1]);

  //Invalid pipe                                                               
  count = pipe_count(fd[0]);
  printf(1,"Count: %d\n", count);

  exit();
}
