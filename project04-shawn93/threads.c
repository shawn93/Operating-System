#include "types.h"
#include "stat.h"
#include "user.h"

int x = 0;


int thread_func(void *arg)
{
  int id = (int) arg;
  printf("Hello from thread %d\n", arg);
  x = 99;
  thread_exit(0);
}

int main(int argc, char **argv)
{
  void *m;
  void *stack;
  struct thread t;
  int rv;

  stack = malloc(4096);

  printf("x = %d\n", x);
  thread_create(&t, thread_func, stack, (void *) 0);
  rv = thread_join(&t);
  printf("x = %d\n", x); 
}
