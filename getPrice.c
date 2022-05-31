#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define MAX 256
void errorMsg(char*);
int main(int argc, char* argv[])
{
    // this program search for a dish in the restaurant menu, and return its price
    if (argc < 3)
        errorMsg("Invalid arguements amount, Closing program.");

    int fd_from, rbytes, indx, i, j = 3, cnt = 0;
    char buffer[1024], path[MAX], * pos, * temp, price[MAX], dish[MAX];
    sprintf(path, "%s.txt", argv[1]); strcpy(dish, argv[2]);
    for (argc; argc > 3; argc--)
    {   // get price of multiple name for dish
        strcat(dish, " ");
        strcat(dish, argv[j]);
        j++;
    }

    if ((fd_from = open(path, O_RDONLY)) == -1) { printf("Restaurant Not Found!\n"); exit(1); }

    if ((rbytes = read(fd_from, buffer, 1024)) == -1)  errorMsg("Error reading file");
    pos = buffer;
    while (pos = strstr(++pos, dish))
    {
        cnt = 0;
        // get index of pointer
        indx = pos - buffer; i = indx;  // save indx loc in i for now.
        // --- Start checking exact match ---

        // count how many letters are BEFORE getting to first tab.
        while (buffer[--indx] != '\t') cnt++;
        if (cnt >= 1) { continue; }                  // continue to the next word

        cnt = 0; indx = i;  // set indx again to original place
        // count how many letters are AFTER getting to first dot.
        while (buffer[++indx] != '.')    cnt++;
        if (cnt > strlen(dish)) { continue; }         // continue to the next word

        // --- End checking exact match ---

        i = 0;
        while (!isdigit(buffer[indx]))
            indx++;
        // get new string of currect item price
        while (isdigit(buffer[indx]))
        {
            price[i] = buffer[indx];
            indx++; i++;
        }
        price[i] = '\0';
        printf("%s NIS\n", price);
        exit(1);
    }

    printf("Dish not Found!\n");

    return 0;  // end of program



}

void errorMsg(char* msg)
{
    printf("Error: %s\n", msg);
    exit(1);
}
