#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void swap(char*);

int main() {
    char c;
    char *res = (char *)malloc(100*sizeof(char));

    int length;

    while ((c = getchar()) != EOF) {
		if(c == '\n'){
		int i ;
		for(i =0; i<strlen(res);i++){
}
            swap(res);
        }
        else {
            //printf("Concat \"%c\" with \"%c\"\n", *res, c);
            strcat(res, &c);
        }
	}

    free(res);
    return 0;
}

void swap(char *string) {

    int i;
    char temp;
    int length= (strlen(string))/2;
    int lengthtotal= strlen(string);
    // printf("length: %d\n", length);
    // printf("length of string total: %lu\n", strlen(string));
    for (i = 0; i < length; i++){
        // printf("Changing \"%c\" with \"%c\"\n", string[i], string[lengthtotal-i-1]);
        temp = string[i];
        string[i] = string[lengthtotal-i-1];
        string[lengthtotal-i-1] = temp;
        // printf("Changing \"%c\" with \"%c\"\n", temp, string[length-i]);
    }
    for (i = 0; i < lengthtotal; i++){
        printf("%c",string[i]);
    }
    printf("\n");
    string[0] = '\0';
}
