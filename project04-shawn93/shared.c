#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv)
{
  void *m;
  char *s;
  int id;
  
  dumpmap();
  m = sharedmem();
  s = (char *) m;
  dumpmap();
  //check parent
  //strcpy(s, "Hello from child\n");
  //printf(1,"s1 = %s\n", s);
  id = fork();

  if (id == 0) {
    printf(1,"dumpmap from child\n");
    dumpmap();
    strcpy(s, "Hello from child\n");
    //check fork()
    //printf(1,"s...(fork) = %s\n", s);
    exit();
  }
/*
  id = fork();
  if (id == 0) {
    printf(1,"dumpmap from child2\n");
    dumpmap();
    strcpy(s, "Hello from child2\n");
    exit();
  }
*/
  wait();
  printf(1,"s = %s\n", s);
  exit();  
}
