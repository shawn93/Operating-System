#include "types.h"
#include "stat.h"
#include "user.h"

int x = 0;


int thread_func(void *arg)
{
  int id = (int) arg;
  printf(1, "Hello from thread %d\n", id);
  x = 99;
  exit();
}

int main(int argc, char **argv)
{
  //void *m;
  void *stack;
  //int rv;

  stack = malloc(4096);

  printf(1, "x = %d\n", x);
  printf(1, "&thread_func = %x\n", thread_func);
  clone(thread_func, stack, (void *) 0, (void *) 0);
  sleep(3);
  printf(1, "x = %d\n", x);
  wait();
  exit();
}
