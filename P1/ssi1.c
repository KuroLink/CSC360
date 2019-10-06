#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>

int compare(char* args0, char* args1){ //check if command is to change directory
	if(strcmp(args0, "cd") == 0){
		if((args1 == 0) || (strcmp(args1, "~")==0)){
				chdir(getenv("HOME"));
			}
			else if(strcmp(args1, "..") ==0){
				chdir("..");
			}
			else {
				if(chdir(args1) != 0){
					perror("Error: ");
					exit(1);
				}
			}
			return 1;
	}
	else return 0;
}

void processinput(char *args[], char str[]){
	fgets(str, 50, stdin);
	args[0] = strtok(str, " \n");
	int i=0;
	while(args[i]!=NULL){
		args[i+1]=strtok(NULL, " \n");
		i++;
	}
}
	
int command(char* args[]){
	pid_t pid;
		int status;
		if((pid = fork()) <0) {
			printf("ERROR\n");
			exit(1);
		}
		else if (pid == 0) {
			int c = compare(args[0], args[1]);
			if(c != 0){
				return 1;
			}	
			else if(execvp(args[0], args) < 0) {
				printf("ERROR exe\n");
				exit(1);
				}
			}
		else {
			while(wait(&status) != pid);
			}
	return 0;
}	

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
	char* args[50]; //Array to store tokenized user input
	
	while(1){
		printf("SSI: %s@%s %s > ", name, hostname, cwd);
		processinput(args, str);
		if (command(args) == 1){
			getcwd(cwd, sizeof(cwd));
		}
	}	
}
