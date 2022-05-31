#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define MAX_LINE 256
#define MAX_DISH 40

void errorMsg(char*);
void buildDish(char*, char**);

int main(int argc, char* argv[])
{
    // this program create a menu for a restaurant
    if (argc != 3)
        errorMsg("please enter 2 parameters");

    int fd_rest, wbytes;
    char rest[MAX_LINE], subdish[MAX_LINE], dir[MAX_LINE], dish[MAX_LINE] = "a. ", * priceName[2];
    int i, j = 0, k = 1, len;
    // set filename: 
    strcpy(rest, argv[1]); strcat(rest, ".txt\0");

    //creating restname.txt file as the Menu.
    if ((fd_rest = open(rest, O_WRONLY | O_CREAT, 0664)) == -1)
        errorMsg("Failed creating file");

    // write the name of the restarunt into the file
    strcpy(rest, "\n\t\t\t"); strcat(rest, argv[1]); strcat(rest, " Menu\n\n");
    if ((wbytes = write(fd_rest, rest, strlen(rest)) == -1))
        errorMsg("Failed writing file");


    for (i = atoi(argv[2]); i > 0; i--, k = 1)
    {
        // insert <argv[2]> dishes types
        printf("Insert Type Dish %c:\n", 97 + j);
        fgets(subdish, MAX_LINE, stdin);
        dish[0] += j++;  // counting the letters, starting from 'a'
        strcat(dish, subdish);


        wbytes = write(fd_rest, dish, strlen(dish));  // write the line to the file
        if (wbytes == -1) errorMsg("Writing Error.");


        while (1)
        {
            // insert sub-dish until "Stop" typed in
            strcpy(dish, "\t");                     // default space required
            printf("Insert dish name %d:\n", k++);
            fgets(subdish, MAX_LINE, stdin);
            if (strcmp(subdish, "Stop\n") == 0)
                break;
            buildDish(subdish, priceName);
            len = MAX_DISH - strlen(priceName[0]) - strlen(priceName[1]);
            strcat(dish, priceName[0]);
            for (len; len > 0; len--)
                strcat(dish, ".");               // adding dots as long as need 
            strcat(priceName[1], "NIS\n");       // after the price, add the unit
            strcat(dish, priceName[1]);          // stick all together
            wbytes = write(fd_rest, dish, strlen(dish));    // write to file
            if (wbytes == -1)
            {
                perror("write");
                return(-1);
            }
            if (wbytes != strlen(dish))
            {
                perror("Write was corrupted\n");
                return -1;
            }

            // free allocations
            free(priceName[0]);
            free(priceName[1]);
        }
        // set the counter to 'a' again. will be changed at the start of the loop
        strcpy(dish, "a. ");
    }

    // write the end of the menu
    if (write(fd_rest, "\n\n\t\t\tBon Appetit\n", 17) == -1)
    {
        perror("writing to file\n"); exit(-1);
    }

    // set new directory name and create it //
    strcpy(dir, argv[1]); strcat(dir, "_Order");
    mkdir(dir, 0777);
    printf("Successfully created \n");
    return 0;
}


void buildDish(char* str, char** priceName)
{
    // the function gets a string and break it into 2 parts:
    // priceName[0] will contain a several words
    // priceName[1] will contain a number, using a string type

    char tempname[256], tempprice[256];
    int i = 0, j = 0;

    while (!isdigit(str[i]))
    {
        tempname[i] = str[i]; i++;
    }
    tempname[i] = '\0';                         // make this a string
    // tempname stored the name of the dish (can be multiple names with spaces between
    priceName[0] = (char*)(malloc(i * sizeof(char*) + 1));
    if (priceName[0] == NULL) errorMsg("allocation failed");
    strcpy(priceName[0], tempname);             // copy the dish name into the returned array

    while (str[i] != '\0' && str[i] != '\n')
        tempprice[j++] = str[i++];
    tempprice[j] = '\0';                        // make this a string
    priceName[1] = (char*)(malloc(j * sizeof(char*) + 1));
    if (priceName[1] == NULL) errorMsg("allocation failed");

    strcpy(priceName[1], tempprice);            // copy the dish price into the returned array

}

void errorMsg(char* msg)
{
    printf("Error: %s\n", msg);
    exit(1);
}
