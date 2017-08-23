/* usfgrep.c - grep lines(contain a specitif string) in one or more files */
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MAXLINELEN 511
#define NULL ((void *) 0)

int isContained = 0;

int get_line_length(int fd, char *string, char *tmp_line) {

    int n;
    int i;
    int len = 0;
    char c;

    while ((n = read (fd, &c, 1) > 0)) {

        if ( c == '\n') {
            break;
        }

        len += 1;

        if (len > MAXLINELEN) {
            printf(1, "A line length is greater than 511.\n");
            exit();
        }

        //add c into tmp_line
        for (i=0; i<MAXLINELEN; i++) {
            if (tmp_line[i] == '\0') {
                tmp_line[i] = c;
                tmp_line[i+1] = '\0';
                break;
            }
        }
    }

    char *strstr(const char *s1, const char *s2)
    {
      int n;
      if (*s2)
      {
        while (*s1)
        {
          for (n = 0; *(s1 + n) == *(s2 + n); n ++)
          {
            if (!*(s2 + n + 1))
              return (char *)s1;
          }
          s1++;
        }
        return NULL;
      }
      else
        return (char *)s1;
    }  
    
    if (strstr(tmp_line, string)) {
        isContained = 1;
    }

    //at the end of file
    if (n ==0 && len ==0) {
        len = -1;   
    }

    return len;
}

void get_output(int fd, char *string, char *filename) {

    int line = 0;
    int len;
    char tmp_line[MAXLINELEN];

    while ((len = get_line_length(fd,string, tmp_line)) >= 0) {

        line += 1;

        if (isContained) {
            printf(1, "%s[%d]: %s\n", filename, line, tmp_line);
            //reset the isContained variable to do next
            isContained = 0;
        }
        //reinitialize the tmp_line for the next line
        tmp_line[0] = '\0';
    }
}

int main(int argc, char* argv[]) {

    int i;
    int fd;

    if (argc < 3) {
        printf(1, "Insufficient arguments\n" );
        printf(1, "Usage: usfgrep <string> <file1> [<file2> ...]\n");   
        exit();   
    }

    int filenum = argc - 2;
    char* fileList[filenum];
    char* string = argv[1];

    for (i = 2; i < argc; i++) {
        fileList[i - 2] = malloc(strlen(argv[i]) + 1);
        strcpy(fileList[i-2],argv[i]);
    }

    for (i=0; i<filenum; i++) {

        fd = open(fileList[i], O_RDONLY);

        if (fd < 0){
            printf(1, "file: %s does not exist.\n", fileList[i]);
            exit();
        } 
        else {
            get_output(fd, string, fileList[i]);
        }
    }  

    //free fileList
    for (i=0; i<filenum; i++) {
        free(fileList[i]);
    }
    close(fd);
    exit();
}
