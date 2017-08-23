#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, void *vsrc, int n)
{
  char *dst, *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

int 
thread_create(struct thread *t, int (*func)(void *), void *stack, void *arg)
{
  t->func = func;
  t->stack = stack;
  t->arg = arg;
  t->alive = 1;
  t->exit_value = 0;
  t->pid = clone(func, stack, (void *) arg, (void *) t);
  return 0;
}

int 
thread_exit(int exit_value) 
{
  struct thread *t;
  t = (struct thread *) getuthread();
  t->exit_value = exit_value;
  t->alive = 0;
  exit();
}

int 
thread_join(struct thread *t)
{
  join(t->pid);
  return t->exit_value;
}

int
thread_lock_init(struct lock *l)
{
  l->value = 0;
  return 0;
}

int 
thread_lock_acquire(struct lock *l)
{
  while(xchg(&l->value, 1) != 0)
    ;
  return 0;
}

int
thread_lock_release(struct lock *l)
{
  //asm volatile("movl $0, %0" : "+m" (l->value) : );
  xchg(&l->value, 0);
  return 0;
}

void *
sharedmem(void) {
  void *m;
  m = (void *)sharedalloc((void *) 0x7FFFF000);
  return m;
}
