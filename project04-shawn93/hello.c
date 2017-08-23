/* gdbp4 */
#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv)
{
  void *m;
  char *s;

  // old test code
  // m = malloc(4096 * 10);
  //((char *)m)[0] = 'X';  
  //printf(1, "Hello, World!\n");
  //printf(1, "getpid() = %d\n", getpid());
  //printf(1, "mygetpid() = %d\n", mygetpid());

  dumpmap();

  printf(1, "---\n");
  m = allocpage((void *) 0x7FFFF000);
  s = (char *) m;
  strcpy(s, "hello\n");
  printf(1, "%s\n", s);
  printf(1, "---\n");

  dumpmap();

  exit();
}
