/*
 * Simple working implementation of shell
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
 * Seperate the command and the flags entered by the user
 */
char ** delimit(char * input){

	char ** command = (char **)malloc(8 * sizeof(char *));
	if(command == NULL){
		perror("command malloc error");
		exit(1);
	}

	char * delim = " ";
	char * parsed;
	int index = 0;

	parsed = strtok(input, delim);
	while(parsed != NULL){
		command[index] = parsed;
		index++;
		parsed = strtok(NULL, delim);
	}

	command[index] = NULL;

	return command;
}

/*
 * Function to change directory ==> execute "cd"
 * cd is not a system program
 */
int cd(char * path){
	return chdir(path);
}

int main(){

	char ** command;
	char * input;
	pid_t proc_id;
	int status;

	while(1){
		input = readline("sh>");
		command = delimit(input);

		/* If the string is empty command */
		if(!command[0]){
			free(command);
			free(input);
			continue;
		}

		if(strcmp(command[0],"cd") == 0){
			if(cd(command[1]) < 0){
				perror("cd error:");
			}
			continue;
		}

		proc_id = fork();
		if(proc_id < 0){
			perror("Fork error:");
			exit(1);
		}else if(proc_id == 0){
			if(execvp(command[0], command) < 0){
				perror("execvp error:");
				exit(1);
			}
		}else{
			waitpid(proc_id, &status, WUNTRACED);
		}
	}

	free(input);
	free(command);

	return 0;
}

