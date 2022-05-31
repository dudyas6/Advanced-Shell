#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#define MAX 256

void errorMsg(char*);
void buildDish(char*, char**);


int main(int argc, char* argv[])
{
    // this program gets an order from the user for a specific restaurant.
    // the user insert meal and quantity one by another, and the program will calculate the final sum, and write it to file.
    // the user can be happy

    if (argc != 3)
        errorMsg("Invalid arguement amount.");

    int flag = 0, fd_from, fd_to, wbytes, rbytes, totalPrice = 0, indx, i = 0, cnt = 0;
    char buffer[1024], dishname[MAX], writepath[MAX], readpath[MAX], header[MAX], * pos, curPrice[64] = "";
    char* priceName[2];

    // variables to get current time.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    //set write location, read location, and file header.
    strcpy(header, argv[1]); strcat(header, " Order\n\n");
    strcpy(writepath, argv[1]); strcat(writepath, "_Order/"); strcat(writepath, argv[2]); strcat(writepath, ".txt");
    strcpy(readpath, argv[1]); strcat(readpath, ".txt");

    if ((fd_from = open(readpath, O_RDONLY)) == -1)
    {
        printf("Restaurant Not Found!\n"); exit(1);
    }

    if ((fd_to = open(writepath, O_WRONLY | O_CREAT, 0664)) == -1)
        errorMsg("Error opening write file");

    if ((rbytes = read(fd_from, buffer, 1024)) == -1)
        errorMsg("Error reading file");

    wbytes = write(fd_to, header, strlen(header));

    if (wbytes == -1)
        errorMsg("Writing Error.");


    printf("Insert your order (Finish to finish):\n");
    while (1)
    {
        fgets(dishname, MAX, stdin);
        // reset current item string
        for (i = 0; i < 64; i++) curPrice[i] = '\0';
        i = 0; flag = 0;

        if (strcmp(dishname, "Finish\n") == 0)
            break;

        // split current item to name and count
        buildDish(dishname, priceName);
        pos = buffer;
        while (pos = strstr(++pos, priceName[0]))
        {
            cnt = 0;
            // get index of pointer
            indx = pos - buffer; i = indx;  // save indx loc in i for now.

            // count how many letters are BEFORE getting to first tab.
            while (buffer[--indx] != '\t') cnt++;
            if (cnt >= 1) { continue; }                  // continue to the next word

            cnt = 0; indx = i;  // set indx again to original place
            // count how many letters are AFTER getting to first dot.
            while (buffer[++indx] != '.')    cnt++;
            if (cnt > strlen(priceName[0])) { continue; }         // continue to the next word

            i = 0;
            // got here means there is a exact match
            while (!isdigit(buffer[indx]))
                indx++;
            // get new string of currect item price
            while (isdigit(buffer[indx]))
            {
                curPrice[i] = buffer[indx];
                indx++; i++;
            }

            // add the order price to total order price amount
            // curPrice = dish price, priceName[1] = amount of dishes.
            totalPrice += (atoi(curPrice) * atoi(priceName[1]));
            flag = 1;
            // write current item and amount to file
            wbytes = write(fd_to, dishname, strlen(dishname));
            if (wbytes == -1) errorMsg("write");
            if (wbytes != strlen(dishname)) errorMsg("Write was corrupted\n");
        }
        if (!flag) printf("Dish not found!\n");
    }

    // ask the costumer to accept total price
    printf("Total Price is: %d (Confirm to confim | else Cancel):\n", totalPrice);
    while (1)
    {
        fgets(buffer, MAX, stdin);
        if (strcmp(buffer, "Confirm\n") == 0)
            break;

        else if (strcmp(buffer, "Cancel\n") == 0) {
            remove(writepath);                  // cancel will delete the file
            errorMsg("Order Canceled\n");       // end the program
        }
        // if not one of the options, retry.
        else printf("Wrong input, Confirm or Cancel please: ");
    }

    // print to file total price and date if accepted order
    sprintf(curPrice, "Total Price: %dNIS\n\n%02d/%02d/%d", totalPrice, tm.tm_mday, (tm.tm_mon + 1), 2022);
    wbytes = write(fd_to, curPrice, strlen(curPrice));

    if (wbytes == -1)   errorMsg("write");

    if (wbytes != strlen(curPrice))  errorMsg("Write was corrupted\n");

    printf("Order Created!\n");
    chmod(writepath, 0444);     // changed permission to READ ONLY (if we need exe also, change to 0333)
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

    tempname[i - 1] = '\0';                         // make this a string
    // tempname stored the name of the dish (can be multiple names with spaces between
    priceName[0] = (char*)(malloc(i * sizeof(char*)));
    if (priceName[1] == NULL) errorMsg("allocation failed");
    strcpy(priceName[0], tempname);             // copy the dish name into the returned array

    while (str[i] != '\0' && str[i] != '\n')
        tempprice[j++] = str[i++];
    tempprice[j] = '\0';                        // make this a string
    priceName[1] = (char*)(malloc(j * sizeof(char*)));
    if (priceName[1] == NULL) errorMsg("allocation failed");

    strcpy(priceName[1], tempprice);            // copy the dish price into the returned array

}

void errorMsg(char* msg)
{
    printf("Error: %s\n", msg);
    exit(1);
}
