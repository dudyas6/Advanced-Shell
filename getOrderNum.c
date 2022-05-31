#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#define MAX 256
void errorMsg(char*);


int main(int argc, char* argv[])
{
    // This program get the number of orders for a restaurant

    if (argc != 2)
        errorMsg("Invalid arguements amount.\nClosing program.");

    char path[MAX];
    int count = 0;
    DIR* d;
    struct dirent* dir;

    sprintf(path, "%s_Order", argv[1]);
    if ((d = opendir(path)) == NULL)
        errorMsg("Error opening dir");

    // counting dirs
    while ((dir = readdir(d)) != NULL)
        count++;

    printf("number of orders at %s is: %d \n", argv[1], count - 2);
    closedir(d);
    return 0;
}

void errorMsg(char* msg)
{
    printf("Error: %s\n", msg);
    exit(1);
}
