#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "logger.h"

#define ENCODED_FILE "encoded.txt"
#define DECODED_FILE "decoded.txt"

void encoder(char*);
void decoder(char*);
char* decimalToBinary(char*);
char* base64ToBinary(char*);
char* reverseString(char*);
float progress;
void handleSignal(int);

const char base64[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                            'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                            'w', 'x', 'y', 'z', '0', '1', '2', '3',
                            '4', '5', '6', '7', '8', '9', '+', '/'};


int main(int argc, char** argv){
    if(signal(SIGINT, handleSignal) == SIG_ERR);
	if(signal(SIGUSR1, handleSignal) == SIG_ERR);
    if (argc < 3) {
        infof("USAGE: ./base64 --encode \"name  of file\" || ./base64 --decode \"name  of file\"\n");
        return -1;
    }

    int fd;
    if ((fd = open(argv[2], O_RDONLY, 0)) == -1) {
        infof("cp: can't open file %s, mode %03o\n", argv[2], O_RDONLY);
	    return -1;
    }
    off_t fsize;
    fsize = lseek(fd, 0, SEEK_END);
    close(fd);
    
    char* data = malloc(fsize);
    char buf[1];
    int n;
    if ((fd = open(argv[2], O_RDONLY, 0)) == -1) {
        infof("cp: can't open file %s, mode %03o\n", argv[2], O_RDONLY);
	    return -1;
    }
    int contadorProgreso = 0;
    infof("reading file...\n");
    while ((n = read(fd, buf, 1)) > 0) {
        contadorProgreso++;
        if(*buf != '\n')
            strcat(data, buf);
        progress = (float) (((float)contadorProgreso/(float)fsize)*100);
    }
    close(fd);

    if (strcmp(argv[1], "--encode") == 0) {
        infof("file to  encode: %s\n", argv[2]);
        encoder(data);
    } else if (strcmp(argv[1], "--decode") == 0) {
        infof("file to  decode: %s\n", argv[2]);
        decoder(data);
    }
    return 0;
}

void handleSignal(int sig) {
	infof("Progress: %f%%\n", progress);
}


char* reverseString(char* string) {
    char* newString = malloc(8);
    int len = strlen(string) - 1;
    for(int i = 0 ; i < strlen(string); i++) {
        char c = string[len--];
        newString[i] = c;
    }
    return newString;
}

char* decimalToBinary(char* string){
    infof("converting to binary...\n");
    char* firstNumber = malloc(8);
    char* listBinary = malloc(8 * strlen(string));
    int completed = strlen(string);
    for(int i = 0; i < strlen(string); i++){
        progress = (float) (((float)i/(float)strlen(string))*100);
        char str[50];
        int number = string[i];
        while(number > 0) {
            int modulus = number%2;
            sprintf(str, "%d", modulus);
            strcat(firstNumber, str);
            number /= 2;
        }

        int len = strlen(firstNumber);
        for(int i = strlen(firstNumber); i < 8; i++) {
            firstNumber[len] = '0';
            firstNumber[len+1] = '\0';
            len = strlen(firstNumber);
        }
        firstNumber = reverseString(firstNumber);
        strcat(listBinary, firstNumber);
        firstNumber[0] = '\0';
    }
    return listBinary;
}

char* base64ToBinary(char* string){
    infof("converting to binary...\n");
    char* firstNumber = malloc(8);
    char* listBinary = malloc(8 * strlen(string));
    for(int i = 0; i < strlen(string); i++){
        progress = (float) (((float)i/(float)strlen(string))*100);
        char str[50];
        int number;
        for (int j = 0; j < 64; j++) {
            if(base64[j] == string[i]){
                number  = j;
                break;
            }
        }
        while(number > 0) {
            int modulus = number%2;
            sprintf(str, "%d", modulus);
            strcat(firstNumber, str);
            number /= 2;
        }

        int len = strlen(firstNumber);
        for(int i = strlen(firstNumber); i < 6; i++) {
            firstNumber[len] = '0';
            firstNumber[len+1] = '\0';
            len = strlen(firstNumber);
        }
        firstNumber = reverseString(firstNumber);
        strcat(listBinary, firstNumber);
        firstNumber[0] = '\0';
    }
    return listBinary;
}

void encoder(char *data) {
    char* binario = decimalToBinary(data);
    int iteraciones;
    char* res;
    if(strlen(binario)%6 != 0) {
        iteraciones = (strlen(binario)/6) + 1;
        res = malloc((strlen(binario)/6) + (4-(iteraciones%4)));
    } else {
        res = malloc(strlen(binario)/6);
        iteraciones = strlen(binario)/6;
    }
    int indiceRes = 0;
    infof("converting to base64...\n");
    int contador = 0;
    char* segment = malloc(6);
    for(int i = 0; i < iteraciones; i++) {
        for(int j = 0; j < 6; j++) {
            segment[j] = binario[contador++];
        }
        if(i == (iteraciones - 1)) {
            int len = strlen(segment);
            for(int i = strlen(segment); i < 6; i++) {
                segment[len] = '0';
                segment[len+1] = '\0';
                len = strlen(segment);
            }
        }
        int num = atoi(segment);
        int binary_val = num, decimal_val = 0, base = 1, rem;
        while (num > 0) {
            rem = num % 10;
            decimal_val = decimal_val + rem * base;
            num = num / 10 ;
            base = base * 2;
        }
        res[indiceRes++] = base64[decimal_val];
        segment[0] = '\0';
    }
    if(iteraciones%4 != 0){
        int len = strlen(res);
        int elementosFaltantes = 4-(iteraciones%4);
        for(int i = 0; i < elementosFaltantes; i++) {
            res[len] = '=';
            res[len+1] = '\0';
            len = strlen(res);
        }
    }
    int f2;
    fclose(fopen(ENCODED_FILE, "w"));
    if ((f2 = open(ENCODED_FILE, O_WRONLY | O_CREAT, 0777)) == -1) {
        infof("cp: can't create %s, mode %03o\n", ENCODED_FILE, O_CREAT);
    }
    int rev = strlen(res)/100;
    contador = 0;
    for(int i = 0; i < rev + 1; i++) {
        char* string = malloc(100);
        for(int j = 0; j < 100; j++) {
            string[j] = res[contador++];
        }

        write(f2, string, strlen(string));
        write(f2, "\n", 1);
    }
    close(f2);
    infof("done!\n");
}

void decoder(char* data){
    char* binario = base64ToBinary(data);
    int iteraciones;
    char* res;
    res = malloc(strlen(binario)/8);
    iteraciones = strlen(binario)/8;
    int indiceRes = 0;
    int contador = 0;
    char* segment = malloc(8);
    infof("converting to normal text...\n");
    for(int i = 0; i < iteraciones; i++) {
        for(int j = 0; j < 8; j++) {
            segment[j] = binario[contador++];
        }
        int num = atoi(segment);
        int binary_val = num, decimal_val = 0, base = 1, rem;
        while (num > 0) {
            rem = num % 10;
            decimal_val = decimal_val + rem * base;
            num = num / 10 ;
            base = base * 2;
        }
        res[indiceRes++] = decimal_val;
        segment[0] = '\0';
    }
    int f2;
    fclose(fopen(DECODED_FILE, "w"));
    if ((f2 = open(DECODED_FILE, O_WRONLY | O_CREAT, 0777)) == -1) {
        infof("cp: can't create %s, mode %03o\n", DECODED_FILE, O_CREAT);
    }
    int rev = strlen(res)/100;
    contador = 0;
    for(int i = 0; i < rev + 1; i++) {
        char* string = malloc(100);
        for(int j = 0; j < 100; j++) {
            string[j] = res[contador++];
        }

        write(f2, string, strlen(string));
        write(f2, "\n", 1);
    }
    close(f2);
    infof("done\n");
}
