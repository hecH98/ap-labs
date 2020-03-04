#include <stdio.h>

int mystrlen(char *);
char *mystradd(char *, char *);
int mystrfind(char *, char *);

int main(int argc, char** argv) {
    if(argc <= 3) {
        printf("Usage:\n");
        printf("./nameOfProgram originalString addition add\n");
        return 0;
    }

    int strLength = mystrlen(argv[1]);
    printf("Initial length:     : %d\n", strLength);
    char* strConcat = mystradd(argv[1], argv[2]);
    printf("New String          : %s\n",strConcat);
    int pos = mystrfind(strConcat, argv[3]);
    printf("SubString was found : ");
    if(pos != -1) {
        printf("yes, in position: %d\n", pos);
    }
    else {
        printf("no\n");
    }

    return 0;
}
