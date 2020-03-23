#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFSIZE 50

/* filecopy:  copy file ifp to file ofp */
void filecopy(int ifp, int ofp)
{
    int c[BUFSIZE];
    int number;
    while ( (number = read(ifp, c, BUFSIZE)) > 0) {
	write(ofp, c, BUFSIZE);
    }

}

/* cat:  concatenate files, version 2 */
int main(int argc, char *argv[])
{
    int fp;
    void filecopy(int, int);
    char *prog = argv[0];   /* program name for errors */

    if (argc == 1)  /* no args; copy standard input */
        filecopy(1, 0);
    else
        while (--argc > 0)
            if ((fp = open(*++argv, O_RDONLY, 0)) == -1) {
                fprintf(stderr, "%s: can′t open %s\n", prog, *argv);
                return 1;
            } else {
                filecopy(fp, 0);
                close(fp);
            }

    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        return 2;
    }

    return 0;
}
