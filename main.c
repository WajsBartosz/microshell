#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void command(char *input, char *parameters[]){
	if(fgets(input, 1024, stdin) != NULL){
		char* parameter = strtok(input, " ");
		for(int i = 0; parameter != NULL; i++){
			parameters[i] = parameter;
			parameter = strtok(NULL, " ");
		}
	}
}


void help(){
	printf("Microshell\nCreated by Wajs Bartosz\n");
	printf("Available commands:\ncd, help, exit\n");
}


int main(int argc, char *argv[]) {
    char path[1024];
    char input[1024];
    char* parameters[1024];
    getcwd(path, sizeof(path));
    while(1){
	printf("%s@%s$ ", getenv("USER"), path);
	command(input, parameters);
	if(strcmp("exit\n", parameters[0]) == 0){
		exit(0);
	}else if(strcmp("help\n", parameters[0]) == 0){
		help();
	}	

		
		
    }
    return 0;
}
