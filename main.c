#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int command(char *input, char *parameters[]){
	if(fgets(input, 1024, stdin) != NULL){
		int length = strlen(input) - 1;
		input[length] = 0;
		int wordCounter = 0;
		char* parameter = strtok(input, " ");
		for(int i = 0; parameter != NULL; i++){
			parameters[i] = parameter;
			parameter = strtok(NULL, " ");
			wordCounter++;
		}
		parameters[wordCounter] = NULL;
		return wordCounter;
	}
	return 0;
}


void help(){
	printf("Microshell\nCreated by Wajs Bartosz\n");
	printf("Available commands:\ncd, help, exit\n");
}

void cd(char* parameter){
	if(parameter[0] == '~'){
		chdir(getenv("HOME"));
	}else if(chdir(parameter) != 0){
		char error[1024];
		sprintf(error, "-bash: cd: %s", parameter);
		perror(error);
	}
}

int main(int argc, char *argv[]) {
    char path[1024];
    char input[1024];
    char* parameters[1024];
	int size;
    while(1){
	getcwd(path, sizeof(path));
	printf("\033[1;31m");
	printf("%s:%s$ ", getenv("USER"), path);
	printf("\033[0m");
	size = command(input, parameters); 
	if(size > 0){
	if(strcmp("exit", parameters[0]) == 0){
		exit(0);
	}else if(strcmp("help", parameters[0]) == 0){
		help();
	}else if(strcmp("cd", parameters[0]) == 0){
		if(parameters[1] == NULL){
			chdir(getenv("HOME"));
		}else if(size > 2){
			puts("-bash: cd: too many arguments");
		}else{
			cd(parameters[1]);
		}
	}else{
		int pid = fork();
		if(pid == 0){
			if(execvp(parameters[0], parameters) == -1){
				perror("Error");
			}else{
				exit(0);
			}
		}else{
			wait(NULL);
		}
	}
	}
	}
    return 0;
}
