#include "types.h"
#include "stat.h"
#include "user.h"

#define NTHREADS 10

int x = 0;

int thread_func(void *arg)
{
  int id = (int) arg;
  printf(1, "Hello from thread %d\n", id);
  x = 99;
  thread_exit(id);
  return 0;
}

int main(int argc, char **argv)
{
  void *stack;
  struct thread t[NTHREADS];
  int rv;
  int i;

  for (i = 0; i < NTHREADS; i++) {
    stack = malloc(4096);
    thread_create(&t[i], thread_func, stack, (void *) i);
  }

  for (i = 0; i < NTHREADS; i++) {
    rv = thread_join(&t[i]);
    printf(1, "thread_join(&t[%d]) = %d\n", i, rv);
  }

  exit();
}
