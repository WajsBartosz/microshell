#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void command(char *input, char *parameters[]){
	if(fgets(input, 1024, stdin) != NULL){
		int length = strlen(input) - 1;
		input[length] = 0;
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

void cd(char* parameter){
	if(parameter[0] == '~'){
		chdir(getenv("HOME"));
	}else if(chdir(parameter) != 0){
		printf("No such file or directory\n");
	}
}

int main(int argc, char *argv[]) {
    char path[1024];
    char input[1024];
    char* parameters[1024];
    while(1){
	getcwd(path, sizeof(path));
	printf("%s@%s$ ", getenv("USER"), path);
	command(input, parameters);
	if(strcmp("exit", parameters[0]) == 0){
		exit(0);
	}else if(strcmp("help", parameters[0]) == 0){
		help();
	}else if(strcmp("cd", parameters[0]) == 0){
		if(parameters[1] == NULL){
			chdir(getenv("HOME"));
		}else{
			cd(parameters[1]);
		}
	}else{
		printf("Unknown command\n");
	}


		
		
    }
    return 0;
}
