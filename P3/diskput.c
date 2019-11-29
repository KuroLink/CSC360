#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

int main(int argc, char* argv[]) {

	/*if (argc != 4) {
		printf("ERROR: Missing parameters. Please try again");
		exit(1);
	}*/

	int file = open(argv[1], O_RDWR);
	struct stat buffer;
    int status = fstat(file, &buffer);

    int filemove = open(argv[2], O_RDWR);
    struct stat buffer2;
    int statuscheck = fstat(filemove, &buffer2);

    printf("hour is %s\n", ctime(&buffer2.st_mtime));

    //const char *hourstring = ctime(&buffer2.st_mtime); 

    //int hour;

    //sscanf(hourstring, "%d", &hour);

    //printf("Hour is converted to int %d\n", hour);

    /*int min;
    int sec;
    int year;
    int mth;
    int day;*/

    //assuming no sub directory accessing
    char* input = argv[2];
    char* dirfile = strtok(argv[3], "/");

    char* address=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    /*int blocksize = getBlocksize(address);
	int rdirstrt = Rootdirstrt(address);    
	int rdirblk = Rootdirblk(address);

	int rtstrtblk = rdirstrt * blocksize;
	int rtblksize = blocksize / rdirblk;

   	transferfile(address, input, dirfile, rtstrtblk, rtblksize, rdirblk, blocksize);*/


	munmap(address,buffer.st_size);
 	close(file);
 	close(filemove);

}