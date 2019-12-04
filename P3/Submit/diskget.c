#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

int getBlocksize(char* addr) {
	int blksize = 0;
	memcpy(&blksize, addr+8, 2);
	blksize=htons(blksize);
	return blksize;
}

int Rootdirstrt(char* addr) {
	int rds = 0;
	memcpy(&rds, addr+22, 4);
	rds=htonl(rds);
	return rds;
}

int Rootdirblk(char* addr) {
	int rdb = 0;
	memcpy(&rdb, addr+26, 4);
	rdb=htonl(rdb);
	return rdb;
}

int checkfilesize(char* addr) {
	int fsize = 0;
	memcpy(&fsize, addr+9, 4);
	fsize = ntohl(fsize);
	return fsize;
}

int findfilestart(char* addr, int bsize) {
	int start = 0;
	memcpy(&start, addr+1, 4);
	start = ntohl(start);
	start = start*bsize;
	return start;
}

int FATstrt(char* addr) {
	int start = 0;
	memcpy(&start, addr+14, 4);
	start=htonl(start);
	return start;
}

int findfileblksize(char* addr) {
	int blksize = 0;
	memcpy(&blksize, addr+5, 4);
	blksize=htonl(blksize);
	return blksize;
}

void copyfile(char* addr, char* name, char* output, int rsb, int rbs, int numblks, int blocksize, int FATstart) {
	//int endfile = 0xffffffff;

	for(int i=0; i<numblks; i++){
		int fileinfo = 0;
		char* addrstrt = addr + rsb + (rbs*i);
		memcpy(&fileinfo, addrstrt, 1);

		int filesize = checkfilesize(addrstrt);
		char fname[31];
		memcpy(&fname, addrstrt+27, 31);

		char nameoffile[31];
		strcpy(nameoffile, name);

		if(fileinfo == 3){
			if(strcmp(nameoffile, fname) == 0) {
				//int size = checkfilesize(addrstrt);
				int filestart = findfilestart(addrstrt, blocksize);
				int fatindex = filestart/blocksize;
				int numfileblocks = findfileblksize(addrstrt);
				int nextblock = 0;
				int count=0;

				FILE *fout;
				fout = fopen(output, "w");
				fwrite(addr+filestart, 1, blocksize, fout);
				
				for(int j = 1; j < numfileblocks; j++) {
					memcpy(&nextblock, addr+FATstart+(fatindex*4)+(count*4), 4);
					nextblock=htonl(nextblock);
					count++;
					int next = nextblock*blocksize;
					fwrite(addr+next, 1, blocksize, fout);
					//exit(0);
				}
				exit(0);
			}

		}
		
	}
	printf("File Not Found\n");
	exit(1);
}

int main(int argc, char* argv[]) {

	if (argc != 4) {
		printf("ERROR: Missing parameters. Please try again");
		exit(1);
	}

	int file = open(argv[1], O_RDWR);
	struct stat buffer;
    int status = fstat(file, &buffer);

    //assuming no sub directory accessing
    char* fname = strtok(argv[2], "/");
    char* outputname = argv[3];

    char* address=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    int fstrt = FATstrt(address);
    int blocksize = getBlocksize(address);
	int rdirstrt = Rootdirstrt(address);    
	int rdirblk = Rootdirblk(address);
	int FATstart = blocksize * fstrt;

	int rtstrtblk = rdirstrt * blocksize;
	int rtblksize = blocksize / rdirblk;

   	copyfile(address, fname, outputname, rtstrtblk, rtblksize, rdirblk, blocksize, FATstart);


	munmap(address,buffer.st_size);
 	close(file); 
}