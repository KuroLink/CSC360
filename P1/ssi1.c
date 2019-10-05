#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>

/*int compare(char* arg0, char* arg1){
	if(strcmp(arg0, "cd") == 0){
		printf("true cd alone");
		return 1;
	}
	else return 0;
}*/

int main()
{
	char *name; 
       	name=(char *)malloc(20*sizeof(char));
	cuserid(name);
	char hostname[20];
	int sethost = gethostname(hostname, sizeof(hostname));
	char cwd[100]; //Array to store current working directory
	getcwd(cwd, sizeof(cwd));
	char str[100]; //Array to store user input string
	while(1){
		printf("SSI: %s@%s %s > ", name, hostname, cwd);
		fgets(str, 50, stdin);
		char* args[50];	
		args[0] = strtok(str, " \n");
		int i=0;
		while(args[i]!=NULL){
			args[i+1]=strtok(NULL, " \n");
			i++;
		}

		pid_t pid;
		int status;
		if((pid = fork()) <0) {
			printf("ERROR\n");
			exit(1);
		}
		else if (pid == 0) {			
				//printf("My process ID : %d\n", getpid());
			//compare(args[0], args[1]);
			if(strcmp(args[0], "cd") ==0){
				if((args[1] == 0) || (strcmp(args[1], "~")==0)){
					chdir(getenv("HOME"));
					getcwd(cwd, sizeof(cwd));
				}
				else if(strcmp(args[1], "..") ==0){
					chdir("..");
					getcwd(cwd, sizeof(cwd));
				}
				else {
					if(chdir(args[1]) != 0){
						perror("Error: ");
						exit(1);
					}
					getcwd(cwd, sizeof(cwd));
				}
			}	
			else if(execvp(args[0], args) < 0) {
				printf("ERROR exe\n");
				exit(1);
				}
			}
		else {
			while(wait(&status) != pid);
			}
	}		
}
