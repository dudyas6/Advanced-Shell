#include <stdio.h>
#include <pthread.h>
#include<stdlib.h>
#include <unistd.h>
#include <time.h>

#define N 50
void* lookup(void* (num));
int arr[N];
int flag = 1;

int main() {
    srand(time(0));
    int i, argFunc[3], threadRet1, threadRet2, num_to_find;
    pthread_t thread1, thread2;

    for (i = 0; i < N; i++) 
        arr[i] = rand() % 101;
    
    printf("\nPlease enter a number between 0-100: ");
    scanf("%d", &num_to_find);

    argFunc[0] = 0; argFunc[1] = N / 2; argFunc[2] = num_to_find;
    threadRet1 = pthread_create(&thread1, NULL, lookup, (void*)argFunc); // create thread1

    argFunc[0] = N/2; argFunc[1] = N; argFunc[2] = num_to_find;
    threadRet2 = pthread_create(&thread2, NULL, lookup, (void*)argFunc); // create thread2

    if ((threadRet1 != 0) || (threadRet2 != 0)) {
        printf("Can't create thread\n");
        exit(0);
    }
    // wait for 2 threads to end
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf(flag == 0 ? "The num exist in the array\n" : "The num doesn't exist in the array\n");
 
}
void* lookup(void* argv) {
    int i;
    int* pointer = (int*)argv;
    int start = pointer[0], end = pointer[1], find_me = pointer[2];
    for (i = start; i < end && flag != 0; i++) {
        if (arr[i] == numsearch) 
            flag = 0; 
        if (flag == 0) 
            pthread_exit(NULL);
    }
    pthread_exit(NULL);

}