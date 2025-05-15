#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 16

int main(int argc, char* argv[])
{
    int ifd, ofd;
    char* buff;
    ssize_t nreads, nwrites;

    if(argc < 2) {
        fprintf(stdout, "No arguments\n");
        return 0;
    } else if(argc == 2) {
        // Output to stdout
        ifd = open(argv[1], O_RDONLY);
        if (ifd == -1) {
            perror("Error opening input file");
            return 1;
        }

        // Allocate buffer
        if(!(buff = (char*) malloc(sizeof(char) * BUFF_SIZE))) {
            fprintf(stdout, "Failed to allocate memory\n");
            close(ifd);
            exit(-1);
        }

        // Read from file and write to stdout
        while ((nreads = read(ifd, buff, BUFF_SIZE)) > 0) {
            if (write(STDOUT_FILENO, buff, nreads) != nreads) {
                perror("Error writing to stdout");
                free(buff);
                close(ifd);
                return 1;
            }
        }

        free(buff);
        close(ifd);

    } else if(argc == 4 && !strcmp(argv[2], "into")) {
        // Output to redirection file
        ifd = open(argv[1], O_RDONLY);
        if (ifd == -1) {
            perror("Error opening input file");
            return 1;
        }

        ofd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (ofd == -1) {
            perror("Error opening output file");
            close(ifd);
            return 1;
        }

        // Allocate buffer
        if(!(buff = (char*) malloc(sizeof(char) * BUFF_SIZE))) {
            fprintf(stdout, "Failed to allocate memory\n");
            close(ifd);
            close(ofd);
            exit(-1);
        }

        // Copy contents from input to output file
        while ((nreads = read(ifd, buff, BUFF_SIZE)) > 0) {
            nwrites = write(ofd, buff, nreads);
            if (nwrites != nreads) {
                perror("Error writing to output file");
                free(buff);
                close(ifd);
                close(ofd);
                return 1;
            }
        }

        free(buff);
        close(ifd);
        close(ofd);

    } else {
        fprintf(stdout, "Wrong arguments\n");
        return 0;
    }

    return 0;
}