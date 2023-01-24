#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/wait.h>

void saveToHistory(char input[]) {
  char * homePath = getenv("HOME");
  if (homePath == NULL) {
    puts("Unable to get home directory");
  } else {
    char historyPath[1024];
    snprintf(historyPath, sizeof(historyPath), "%s/.history.txt", homePath);
    FILE * history = fopen(historyPath, "a+");
    if (history == NULL) {
      puts("Unable to open history text file");
    } else {
      fprintf(history, "%s", input);
      fclose(history);
    }
  }
}

void history() {
  char * homePath = getenv("HOME");
  if (homePath == NULL) {
    puts("Unable to get home directory");
  } else {
    char historyPath[1024];
    snprintf(historyPath, sizeof(historyPath), "%s/history.txt", homePath);
    FILE * history = fopen(historyPath, "r");
    if (history == NULL) {
      puts("Unable to open history text file");
    } else {
      char line = fgetc(history);
      while (line != EOF) {
        printf("%c", line);
        line = fgetc(history);
      }
      fclose(history);
    }
  }
}

int command(char * input, char * parameters[]) {
  if (fgets(input, 1024, stdin) != NULL) {
    saveToHistory(input);
    int length = strlen(input) - 1;
    input[length] = 0;
    int wordCounter = 0;
    char * parameter = strtok(input, " ");
    for (int i = 0; parameter != NULL; i++) {
      parameters[i] = parameter;
      parameter = strtok(NULL, " ");
      wordCounter++;
    }
    parameters[wordCounter] = NULL;
    return wordCounter;
  }
  return 0;
}

void help() {
  printf("Microshell\nCreated by Wajs Bartosz\nMicroshell it's a small project for Operating systems");
  printf("Own implementation of commands:\ncd, help, exit, ls and touch\n");
  printf("Other commands are implemented using fork and execvp functions.");
}

void cd(int size, char * parameters[]) {
  if (parameters[1] == NULL || strcmp("~", parameters[1]) == 0) {
    chdir(getenv("HOME"));
  } else if (size > 2) {
    puts("-bash: cd: too many arguments");
  } else if (chdir(parameters[1]) != 0) {
    char error[1024];
    sprintf(error, "-bash: cd: %s", parameters[1]);
    perror(error);
  }
}

void ls(int size, char * parameters[]) {
  DIR * openDirectory;
  struct dirent * entry;
  char path[1024] = "";
  char isASet = 0;
  int IndexA = 1;
  for (int i = 1; i < size; i++) {
    if (strcmp("-a", parameters[i]) == 0) {
      isASet = 1;
      break;
    } else {
      IndexA++;
    }
  }
  if (size == 1 || (size == 2 && isASet == 1)) {
    getcwd(path, sizeof(path));
  } else if (size == 2 && isASet == 0) {
    strcpy(path, parameters[1]);
  } else if (size == 3 && isASet == 1) {
    strcpy(path, parameters[3 - IndexA]);
  } else {
    puts("Too many arguments");
    return;
  }
  openDirectory = opendir(path);
  if (openDirectory == NULL) {
    puts("Unable to find directory");
  } else {
    while ((entry = readdir(openDirectory)) != NULL) {
      if (isASet == 1) {
        printf("%s   ", entry -> d_name);
      } else {
        if (entry -> d_name[0] != '.') {
          printf("%s   ", entry -> d_name);
        }
      }
    }
    printf("\n");
    closedir(openDirectory);
  }
}

void touch(int size, char * parameters[]) {
  FILE * fPtr;
  for (int i = 1; i < size; i++) {
    char name[1024];
    strcpy(name, parameters[i]);
    fPtr = fopen(name, "w");
    if (fPtr == NULL) {
      printf("Unable to create file");
    } else {
      fclose(fPtr);
    }
  }
}

int main(int argc, char * argv[]) {
  char path[1024];
  char input[1024];
  char * parameters[1024];
  int size;
  while (1) {
    getcwd(path, sizeof(path));
    printf("\033[1;31m");
    printf("%s:%s$ ", getenv("USER"), path);
    printf("\033[0m");
    size = command(input, parameters);
    if (size > 0) {
      if (strcmp("exit", parameters[0]) == 0) {
        exit(0);
      } else if (strcmp("help", parameters[0]) == 0) {
        help();
      } else if (strcmp("cd", parameters[0]) == 0) {
        cd(size, parameters);
      } else if (strcmp("ls", parameters[0]) == 0) {
        ls(size, parameters);
      } else if (strcmp("touch", parameters[0]) == 0) {
        touch(size, parameters);
      } else if (strcmp("history", parameters[0]) == 0) {
        history();
      } else {
        int pid = fork();
        if (pid == 0) {
          if (execvp(parameters[0], parameters) == -1) {
            puts("Error: Unknown command");
          } else {
            exit(0);
          }
        } else {
          wait(NULL);
        }
      }
    }
  }
  return 0;
}