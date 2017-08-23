#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_mygetpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

/* gdbp4 */
int
sys_halt(void)
{
  const char s[] = "Shutdown";
  const char *p;

  outw( 0xB004, 0x0 | 0x2000 );

  for (p = s; *p != '\0'; p++)
    outb (0x8900, *p);

  return 0;
}

/* gdbp4 */
int
sys_dumpmap(void)
{
  return dumpmap(proc->pgdir);
}

/* gdbp4 */
int
sys_allocpage(void)
{
  int n;
  void *addr;
  void *rv;
  
  if(argint(0, &n) < 0)
    return -1;
  
  addr = (void *) n;
  rv = allocpage(proc->pgdir, addr);
  if (rv != 0) {
    proc->allocpage = rv;
  }
  return (int) rv;
}

int
sys_clone(void)
{
  int v;
  int id;
  void *f;
  void *s;
  void *a;
  void *t;

  if(argint(0, &v) < 0)
    return -1;

  f = (void *) v;

  if(argint(1, &v) < 0)
    return -1;

  s = (void *) v;

  if(argint(2, &v) < 0)
    return -1;

  a = (void *) v;

  if(argint(3, &v) < 0)
    return -1;

  t = (void *) v;

  cprintf("f = %x\n", f);
  cprintf("s = %x\n", s);
  cprintf("a = %x\n", a);
  cprintf("t = %x\n", t);

  id = clone(f, s, a, t);

  return id;
}

int
sys_getuthread(void)
{
  return getuthread();
}

int
sys_sharedalloc(void){
  int n;
  void *addr;
  void *rv;

  if(argint(0,&n) < 0)
    return -1;

  if(proc->sharedm == 0) {
    proc->sharedm = (struct sharedmem *) kalloc();
    proc->sharedm->addr = 0;
  }

  addr = (void *) n;
  rv = allocpage(proc->pgdir, addr);
  if (rv != 0) {
    proc->sharedm->addr = rv;
  }

  return (int) rv;
}

int
sys_join(void) {
  //void *t;

  //if(argptr(1, (void*)&t, sizeof(t)) < 0)
  //  return -1;
  int tpid;
  if(argint(0, &tpid) < 0)
    return -1;
  
  return Thread_join(tpid);
}
