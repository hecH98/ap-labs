#include <stdlib.h>
int mystrlen(char *str){
    int i = 0;
    while (str[i] != '\0'){
        i++;
    }
    return i;
}

char *mystradd(char *origin, char *addition){
    char* res = malloc(sizeof(char));
    int i = 0, j = 0;
    while(origin[i] != '\0') {
        res[j++] = origin[i++];
    }
    i = 0;
    while(addition[i] != '\0') {
        res[j++] = addition[i++];
    }
    return res;
}

int mystrfind(char *origin, char *substr){
    int sub = 0;
    int orig = 0;
    int subLength = mystrlen(substr);
    while(origin[orig] != '\0'){
        if(origin[orig] == substr[sub]){
            sub++;
            if(sub == subLength) {
                break;
            }
            orig++;
        }
        else {
            orig++;
            sub=0;
        }
    }
    return sub ? orig-sub+1 : -1;
}
