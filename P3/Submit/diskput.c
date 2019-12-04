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
	int blksize=0;
	memcpy(&blksize, addr+8, 2);
	blksize=htons(blksize);
	return blksize;
}

int Rootdirstrt(char* addr) {
	int rds=0;
	memcpy(&rds, addr+22, 4);
	rds=htonl(rds);
	return rds;
}

int Rootdirblk(char* addr) {
	int rdb=0;
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

int FATstrt(char* addr) {
	int start=0;
	memcpy(&start, addr+14, 4);
	start=htonl(start);
	return start;
}

int FATblck(char* addr) {
	int fblock=0;
	memcpy(&fblock, addr+18, 4);
	fblock=htonl(fblock);
	return fblock;
}

int findfileblksize(char* addr) {
	int blksize = 0;
	memcpy(&blksize, addr+5, 4);
	blksize=htonl(blksize);
	return blksize;
}

int convertmth(char* m) {
	int month = 0;
	char mth[4];
	strcpy(mth, m);

	char jan[4] = "Jan";
	char feb[4] = "Feb";
	char mar[4] = "Mar";
	char apr[4] = "Apr";
	char may[4] = "May";
	char jun[4] = "Jun";
	char jul[4] = "Jul";
	char aug[4] = "Aug";
	char sep[4] = "Sep";
	char oct[4] = "Oct";
	char nov[4] = "Nov";
	char dec[4] = "Dec";

	if (strcmp(jan, mth) == 0) {
		month = 1;
	}
	else if (strcmp(feb, mth) == 0) {
		month = 2;
	}
	else if (strcmp(mar, mth) == 0) {
		month = 3;
	}
	else if (strcmp(apr, mth) == 0) {
		month = 4;
	}
	else if (strcmp(may, mth) == 0) {
		month = 5;
	}
	else if (strcmp(jun, mth) == 0) {
		month = 6;
	}
	else if (strcmp(jul, mth) == 0) {
		month = 7;
	}
	else if (strcmp(aug, mth) == 0) {
		month = 8;
	}
	else if (strcmp(sep, mth) == 0) {
		month = 9;
	}
	else if (strcmp(oct, mth) == 0) {
		month = 10;
	}
	else if (strcmp(nov, mth) == 0) {
		month = 11;
	}
	else if (strcmp(dec, mth) == 0) {
		month = 12;
	}
	return month;
}

/*void addfileentry(char* addr, int rsb, int rbs, int rdb, char* argv1, int year, int mnth, int day, int hour, int min, int sec){




}*/

/*void transferfile(char* addr, int rsb, int rbs, int numblks, char* writefile) {
	for(int i=0; i<numblks; i++){
		int fileinfo = 0;
		char* addrstrt = addr + rsb + (rbs*i);
		memcpy(&fileinfo, addrstrt, 1);

		int filesize = checkfilesize(addrstrt);
		char fname[31];
		memcpy(&fname, addrstrt+27, 31);

		char inname[31];
		strcpy(inname, writefile);
		if(strcmp(fname, inname) == 0) {
			printf("file found and name is %s\n");
		}
		else {
			printf("not right file\n");
		}

	}
}*/

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

    //printf("hour is %s\n", ctime(&buffer2.st_mtime));
	int count = 0;
    char str[25]; 
    strcpy(str, ctime(&buffer2.st_mtime));

    const char s[2] = " ";
    char* token;
    token = strtok(str, s);

    int hour = 0;
    int min = 0;
    int sec = 0;
    int year = 0;
    int mnth = 0;
    int day = 0;

    //tokenize the string about date and time of file
    while( token != NULL ) {
    	if(count == 1) {
    		mnth = convertmth(token);
    	}
    	if(count == 2) {
    		day = atoi(token);
    	}
    	if(count == 3) {
    		char* temphr = malloc(2);
    		strncpy(temphr, token, 2);
    		hour = atoi(temphr);

    		char* tempmin = malloc(2);
    		strncpy(tempmin, token+3, 2);
    		min = atoi(tempmin);

    		char* tempsec = malloc(2);
    		strncpy(tempsec, token+6, 2);
    		sec = atoi(tempsec);
    	}
    	if(count == 4) {
    		year = atoi(token);
    	}
    	count++;
   		token = strtok(NULL, s);
   }
   printf("%d/%d/%d %d:%d:%d\n", year, mnth, day, hour, min, sec);
    //assuming no sub directory accessing
    char* input = argv[2]; 
    char* dirfile = strtok(argv[3], "/");

    char* address=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    int blocksize = getBlocksize(address);
	int rdirstrt = Rootdirstrt(address);    
	int rdirblk = Rootdirblk(address);

	int rtstrtblk = rdirstrt * blocksize;
	int rtblksize = blocksize / rdirblk;

    int fstrt = FATstrt(address);
    int fblk = FATblck(address);	
	int FATstart = blocksize * fstrt;
    int FATend = blocksize * fblk; 
    int FATblockcount = FATstart + FATend;

    int inputsize = buffer2.st_size;
	//printf("size is %d\n", inputsize);

    for(int i=FATstart; i<FATblockcount; i+=4){
    	int FATentry = 0;
    	memcpy(&FATentry, address+i, 4);
    	FATentry = htonl(FATentry);
    	if (FATentry == 0) {
    		
    	}
    }

	for (int i=0; i<rdirblk; i++) {
		int fileinfo = 0;
		char* addrstrt = address + rtstrtblk + (rtblksize*i);
		memcpy(&fileinfo, addrstrt, 1);

		if (fileinfo == 0) {
			
		}
	}

   	//transferfile(address, input, dirfile, rtstrtblk, rtblksize, rdirblk, blocksize);
	//transferfile(address, rtstrtblk, rtblksize, rdirblk, dirfile);

	munmap(address,buffer.st_size);
 	close(file);
 	close(filemove);

}