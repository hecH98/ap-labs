#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define REPORT_FILE "packages_report.txt"
#define BUFSIZE 1

struct packages {
    char* name;
    char* installDate;
    char* lastUpdate;
    int totalUpdates;
    char* removed;
};

void analizeLog(char *logFile, char *report);
void printStruct(struct packages);

int main(int argc, char **argv) {

    if (argc < 2) {
	printf("Usage:./pacman-analizer.o pacman.log\n");
	return 1;
    }

    analizeLog(argv[1], REPORT_FILE);

    return 0;
}

void analizeLog(char *logFile, char *report) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int fd;
    if ((fd = open(logFile, O_RDONLY, 0)) == -1) {
        printf("cp: can't open file %s, mode %03o\n", logFile, O_RDONLY);
	return;
    }
    char buf[BUFSIZE];
    int n;
    int i = 0, countPack = 0, line = 0;
    char* string = malloc(sizeof(char)*120);
    char* type = malloc(sizeof(char)*20);
    char* date = malloc(sizeof(char)*20);
    char* name = malloc(sizeof(char)*50);
    int removed = 0, upgraded = 0;
    struct packages packs[600];// = (struct packages*) malloc(sizeof(struct packages));

    while ((n = read(fd, buf, BUFSIZE)) > 0) {
        if (*buf == '\n') {
	    printf("line: %d, ",line++);
	    //analizeString(string, packs, &countPack);
	    //string[0]='\0';
            printf("------------------------------\n");
            printf("%s\n\n",string);
            int j = 0;
            int corchete = 1;
            char caracter;
            while (string[j] != '\0') {
                if (string[j] == '[' && corchete) {
                    j++;
                    while (string[j] != ']') {
                        caracter = string[j];
                        strcat(date, &caracter);
                        //printf("%c",string[j]);
                        j++;
                    }
                    j++;
                    corchete = 0;
                    //printf("\n");
                    printf("date: %s\n", date);
                } else if (string[j] == '[' && !corchete) {
                    while (string[j] != ']') j++;
                    j++;
                } else if (string[j] != ' ') {
                    //type[0] = '\0';
                    //printf("type1: %s\n", type);
                    while (string[j] != ' ') {
                        caracter = string[j];
                        strcat(type, &caracter);
                        //printf("%c",caracter);
                        j++;
                    }
                    //printf("\n");
                    printf("type: %s\n", type);
                    j++;
                    if (strcmp(type,"installed") == 0) {
                        printf("type is installed!\n");
                        while (string[j] != ' ') {
                            caracter = string[j];
                            strcat(name, &caracter);
                        //printf("%c",caracter);
                            j++;
                        }
                        printf("name: %s\n", name);
                    } else if (strcmp(type,"upgraded") == 0) {
                        printf("type is upgraded!\n");
                        upgraded++;
                        while (string[j] != ' ') {
                            caracter = string[j];
                            strcat(name, &caracter);
                        //printf("%c",caracter);
                            j++;
                        }
                    } else if (strcmp(type,"removed") == 0) {
                        removed++;
                        printf("type is removed!\n");
                        while (string[j] != ' ') {
                            caracter = string[j];
                            strcat(name, &caracter);
                        //printf("%c",caracter);
                            j++;
                        }
                    }
                    break;
                    j++;
                }
                j++;
            }
printf("\n");

            if (strcmp(type, "installed") == 0) {
                //i = countPack;
                packs[countPack].name = name;
                packs[countPack].installDate = date;
                packs[countPack].lastUpdate = date;
                packs[countPack].totalUpdates = 0;
                packs[countPack].removed = "-";
                countPack++;
            } else if (strcmp(type, "upgraded") == 0) {
                int position = -1;
                for (i = 0; i < countPack; i++) {
                    if (strcmp(packs[i].name,name) == 0) {
                        //printf("----------- found upgraded %s at pos: %d ------------------\n", name, i);
                        position = i;
                    }
                    //printf("struct #%d: name: %s\n", i, packs[i].name);
                }
                if (position != -1) {
                    packs[position].lastUpdate = date;
                    packs[position].totalUpdates += 1;
                }
            } else if (strcmp(type, "removed") == 0) {
                int position = -1;
                for (i = 0; i < countPack; i++) {
                    if (strcmp(packs[i].name,name) == 0) {
                        //printf("----------- found upgraded %s at pos: %d ------------------\n", name, i);
                        position = i;
                    }
                    //printf("struct #%d: name: %s\n", i, packs[i].name);
                }
                if (position != -1) {
                    packs[position].removed = date;
                    //packs[position].totalUpdates += 1;
                }
            }
            //i = countPack;
            //packs[i].name = name;
            //packs[i].installDate = date;
            //packs[i].lastUpdate = "now";
            //packs[i].totalUpdates = i;
            //packs[i].removed = "-";
            //countPack += 1;
            type[0] = '\0';// = malloc(sizeof(char)*20);
            date = malloc(sizeof(char)*20);
            name = malloc(sizeof(char)*50);

            string[0] = '\0';
        }
	else {
	    strcat(string, buf);
	}
    }
printf("salio!\n");
    free(string);
   printf("value of count: %d\n", countPack);
    for (i = 0; i < countPack; i++) {
        printf("struct #%d\n",i);
        printStruct(packs[i]);
    }


    printf("\nPacman Packages Report\n");
    printf("------------------------\n");
    printf("Installed Packages  : %d\n", countPack);
    printf("Removed Packages    : %d\n", removed);
    printf("Upgraded Packages   : %d\n", upgraded);
    printf("Current Installed   : %d\n", countPack-removed);
    struct packages prueba;
    prueba.name = "nombre";
    prueba.installDate = "today";
    prueba.lastUpdate = "now";
    prueba.totalUpdates = 3;

    close(fd);
    int f2;
    if ((f2 = open(report, O_WRONLY | O_CREAT, 0644)) == -1) {
        printf("cp: can't create %s, mode %03o\n", report, O_CREAT);
    }

    char* string22 = malloc(sizeof(char)*15);

    write(f2, "Pacman Packages Report\n", 24);
    write(f2, "----------------------\n", 24);
    write(f2, "Installed Packages  : ", 22);
    sprintf(string22, "%d", countPack);
    write(f2, string22, sizeof(string22));
    write(f2, "\n", 1);
    write(f2, "Removed Packages    : ", 22);
    sprintf(string22, "%d", removed);
    write(f2, string22, sizeof(string22));
    write(f2, "\n", 1);
    write(f2, "Upgraded Packages   : ", 22);
    sprintf(string22, "%d", upgraded);
    write(f2, string22, sizeof(string22));
    write(f2, "\n", 1);
    write(f2, "Current Installed   : ", 22);
    int currentInstalled = countPack - removed;
    sprintf(string22, "%d", currentInstalled);
    write(f2, string22, sizeof(string22));
    write(f2, "\n", 1);
    write(f2, "\nList of Packages\n", 18);
    write(f2, "----------------\n", 17);
    i = 0;

    while (i < countPack) {
	write(f2, "- Package Name            : ", 28);
        write(f2, packs[i].name, 50);
	write(f2, "\n", 1);
        write(f2, "  - Install date          : ", 28);
        write(f2, packs[i].installDate, 20);
        write(f2, "\n", 1);
        write(f2, "  - Last update date      : ", 28);
        write(f2, packs[i].lastUpdate, 20);
        write(f2, "\n", 1);
        write(f2, "  - How many updates      : ", 28);
        sprintf(string22, "%d", packs[i].totalUpdates);
        write(f2, string22, 1);
	string22[0] = '\0';
        write(f2, "\n", 1);
        write(f2, "  - Removal date          : ", 28);
        if (strcmp(packs[i].removed, "-") == 0) {
	    write(f2, packs[i].removed, 1);
	} else {
	    write(f2, packs[i].removed, 20);
	}
        write(f2, "\n\n", 2);
       i++;
    }
    close(f2);
    printf("Report is generated at: [%s]\n", report);
}

void printStruct(struct packages package) {
    printf("- Package Name            : %s\n", package.name);
    printf("  - Install date          : %s\n", package.installDate);
    printf("  - Last update date      : %s\n", package.lastUpdate);
    printf("  - How many updates      : %d\n", package.totalUpdates);
    printf("  - Removal date          : %s\n", package.removed);
}
