#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

void errorMsg(char*);
char** parserLine(char*, int*);
void runCmd(int, char**, int);
int checkType(char* cmd);

void main()
{
    char cmd[256], ** argv;
    int argc, i;
    while (1)
    {
        printf("AdvShell> ");
        fgets(cmd, 256, stdin);             // getting user command from stdin 
        if (cmd[strlen(cmd) - 1] == '\n')   //date\n -> date
            cmd[strlen(cmd) - 1] = '\0';

        // checking exit status
        if (strcmp(cmd, "exit") == 0)
        {
            printf("Goodbye\n");
            exit(1);
        }
        // split the string to vector of arguments
        argv = parserLine(cmd, &argc);
        if (argc == -1) continue; // no command has been detected -> skip
        if (argc > 4)
            errorMsg("Arguements amount is too big. \nClosing program");

        // if restaurant command => pass 1, else 0
        runCmd(argc, argv, checkType(argv[0]) == 1 ? 1 : 0);
    }

    printf("Goodbye\n");
    exit(1);

}

char** parserLine(char* cmd, int* argc)
{
    // this function breaks the string 'cmd' to discrete arguments, and count them.
    // the function return a vector, and sets (by address) the 'argc'.
    char* ptr, * temp[256], ** argv;
    int i, argm = 0;
    //if(cmd == NULL)
     //   return (strcmp)
    // split each word by 'space' and insert into temp array
    ptr = strtok(cmd, " ");
    while (ptr != NULL)
    {
        temp[argm++] = ptr;
        ptr = strtok(NULL, " ");
    }
    if (argm == 0) { *argc = -1;  return (argv = NULL); }  // empty line
    argv = (char**)malloc(sizeof(char*) * (argm + 1));     // allocate memory based on number of arguments
    if (argv == NULL) errorMsg("allocation failed.");

    for (i = 0; i < argm; i++)                                 // insert the arguments one by another to the new array
        argv[i] = temp[i];

    argv[argm] = NULL;

    *argc = argm;                                       // set pointer value to real number of arguments inserted
    return argv;
}

void runCmd(int argc, char** argv, int flag) {
    // the function gets an argument counter, vector and a flag.
    // based on a flag, the function execute the relevant program
    // flag == 0: unix command
    // flag == 1: restaurant menu

    int child_status;
    char path[256];
    pid_t pid;

    strcpy(path, flag == 0 ? "/bin/" : "");     // set the address
    if (flag == 0 && argc > 4)                   // unix command -> check if more than 4 arg were given
    {
        printf("Arguements amount is too big, only three parameters allowed.\n");
        return;
    }
    strcat(path, argv[0]);                      // anyway - copy the command name

    pid = fork();                               // create new proccess

    if (pid == -1) 		                        // father failed forking
        errorMsg("fork() failed: ");

    if (pid == 0)                               // enter children 
    {
        execv(path, argv);                      // execute 
        printf("Not Supported. \n");            // the execv has failed
        exit(1);
    }

    else
        waitpid(pid, &child_status, 0);         // waits for child to end

    return;
}

void errorMsg(char* msg) {
    // this function prints an error msg and exit the program
    printf("Error: %s\n", msg);
    exit(1);
}


int checkType(char* cmd)
{   // this functiion check if given command is a restaurant command or not
    char* exec[] = { "CreateMenu","getMenu","MakeOrder","getOrderNum","getPrice" };
    int i = 0;
    for (i = 0; i <= 4; i++) {
        if (strcmp(cmd, exec[i]) == 0)
            return 1;
    }
    return 0;
}