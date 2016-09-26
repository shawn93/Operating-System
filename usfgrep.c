/* usfgrep.c - grep lines(contain a specitif string) in one or more files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXLINELEN 511
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
			printf(ANSI_COLOR_MAGENTA "A line length is greater than 511." ANSI_COLOR_RESET "\n");
			exit(-1);
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
			printf(ANSI_COLOR_RED "%s" ANSI_COLOR_RESET ANSI_COLOR_BLUE "[%d]: " ANSI_COLOR_RESET  ANSI_COLOR_GREEN"%s\n" ANSI_COLOR_RESET, filename, line, tmp_line);
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
	int line = 0;

	if (argc < 3) {
		printf(ANSI_COLOR_MAGENTA "Insufficient arguments" ANSI_COLOR_RESET "\n" );
		printf(ANSI_COLOR_MAGENTA "Usage: usfgrep <string> <file1> [<file2> ...]" ANSI_COLOR_RESET "\n");	
		exit(-1);	
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
			printf(ANSI_COLOR_MAGENTA "file: %s does not exist." ANSI_COLOR_RESET "\n", fileList[i]);
			exit(-1);
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
	return(0);
}
