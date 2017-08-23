#include <dirent.h> 
#include <stdio.h> 
#include <string.h>

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (argc == 1) {
        if (d != NULL) {
            while ((dir = readdir(d)) != NULL) {
                if (dir->d_name[0] == '.') {
                    continue;
                }
                printf("%s\n", dir->d_name);
            }

                closedir(d);
            }
        }

    if (argc == 2 && strcmp(argv[1],"-a") == 0) {
        if(d != NULL) {
            while ((dir = readdir(d)) != NULL) {
                printf("%s\n", dir->d_name);
            }

            closedir(d);
        }
    }
    return 0;
}