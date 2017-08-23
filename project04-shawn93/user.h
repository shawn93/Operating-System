struct stat;
struct rtcdate;

struct thread {
  int (*func)(void *);
  void *stack;
  void *arg;
  int pid;
  int alive;
  int exit_value;
};

struct lock {
	uint value;
};

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(char*, int);
int mknod(char*, short, short);
int unlink(char*);
int fstat(int fd, struct stat*);
int link(char*, char*);
int mkdir(char*);
int chdir(char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
/* gdbp4 */
int halt(void);
int mygetpid(void);
int dumpmap(void);
void *allocpage(void *);
int clone(int (*func)(void *), void *stack, void *arg, void *thread);
struct thread *getuthread(void);
int sharedalloc(void *);
int join(int);

// ulib.c
int stat(char*, struct stat*);
char* strcpy(char*, char*);
void *memmove(void*, void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, char*, ...);
char* gets(char*, int max);
uint strlen(char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);

int thread_create(struct thread *t, int (*func)(void *), void *stack, void *arg);
int thread_exit(int exit_value);
int thread_join(struct thread *t);
int thread_lock_init(struct lock *l);
int thread_lock_acquire(struct lock *l);
int thread_lock_release(struct lock *l);
void* sharedmem(void);

/*
int thread_cond_init(struct cond *c);
int thread_cond_wait(struct cond *c, struct lock *l);
int thread_cond_signal(struct cond *c);
*/


