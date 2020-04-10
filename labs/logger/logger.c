#include <stdio.h>
#include <signal.h>
#include <stdarg.h>
#define RESET		0
#define BRIGHT 		1
#define DIM		    2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8
#define BLACK 		0
#define RED		    1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

void colorText(int text, int background) {
    char command[13];
	sprintf(command, "%c[0;%d;%dm", 0x1B,text + 30, background + 1);
	printf("%s", command);
}

int infof(const char *format, ...) {
	colorText(WHITE, BLACK);
	va_list input;
	va_start(input, format);
    vprintf(format, input);
	va_end(input);
	return 0;
}

int warnf(const char *format, ...) {
	colorText(YELLOW, BLACK);
    va_list input;
    va_start(input, format);
    vprintf(format, input);
    va_end(input);
	colorText(WHITE, BLACK);
	return 0;
}

int errorf(const char *format, ...) {
	colorText(RED, BLACK);
    va_list input;
    va_start(input, format);
    vprintf(format, input);
    va_end(input);
    colorText(WHITE, BLACK);
    return 0;
}

int panicf(const char *format, ...) {
    colorText(MAGENTA, BLACK);
    va_list input;
    va_start(input, format);
    vprintf(format, input);
    va_end(input);
	raise(SIGABRT);
    return -1;
}
