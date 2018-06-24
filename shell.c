/*
 * Author: Mihir Garude
 * Date: 09/15/2016
 *
 * Description: This is a Unix like shell implemented in C which handles basic
 *		features like executing shell commands, changing directory, etc.
 */

/* Includes section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

/* Globals section */
#define MAXLINE 1024
static sigjmp_buf params;				/* Store details of stack pointer and instruction pointer for non local jumps */
/* Variable to provide access to jump location only after 
 * the jump location has been initialized. Access to this
 * variable is atomic
 */
static volatile sig_atomic_t jump_initialized = 0;	

/* Function Definations */
char **split_line(char * line);				/* Divide input line in format of command */
int cd(char * dir);					/* Change the current working directory */
void init_signal(void);					/* Initialize sigaction parameters */
void sigint_handler(int signum);			/* Handler to handle SIGINT */

/* Main function
 * Code starts here
 */
int main(){

	char ** sh_command;							/* Store the array of command and flags */
	char * user_input;							/* Store the user input */
	pid_t proc_id;
	int status;

	init_signal();								/* Initialize signal parameters */

	while(1){
		/* Initialize jump parameters and wait for call from signal handler*/
		if(sigsetjmp(params, 1) == 42){
			fprintf(stdout, "\n");
			continue;
		}

		jump_initialized = 1;						/* Jump location is initialized */

		user_input = readline("sh>");					/* Get input from the user */
		sh_command = split_line(user_input);				/* split the input and store it in an array */

		/* Check if the user wants to change the current working directory */
		if(strcmp(sh_command[0],"cd") == 0){
			if(cd(sh_command[1]) == -1){
				perror("Cannot change directory:");
			}
			continue;
		}

		/* If user types "exit", exit the shell and return success */
		if(strcmp(sh_command[0],"exit") == 0){
			return 0;
		}	

		proc_id = fork();						/* create a new process */
		if(proc_id < 0){
			perror("Process Creation:");
			exit(1);
		}
		/* Spawning a new process to execute user command */
		else if(proc_id == 0){
			init_signal();						/* Initialize signal for new process */
			execvp(sh_command[0], sh_command);
			perror("Cannot replace current process");
			exit(1);
		}
		/* wait for the process to terminate */
		else{
			waitpid(proc_id, &status, WUNTRACED);
		}

	}	
		free(user_input);
		free(sh_command);

	return 0;
}

/*
 * Inputs: Line containing commands entered by the user
 *
 * Outputs: Command and flags for the commands
 *
 * Descritption: This function takes a string of characters which is the
 * 		 command with flags/arguments entered by the user and
 *		 stores them in an array of strings seperately, returning 
 *		 the array to the caller.
 */
char ** split_line(char * line){

	char ** com_fla = (char **)malloc(MAXLINE * sizeof(char *));		/* Array of strings */
	if(com_fla == NULL){
		perror("malloc failed");
		exit(1);
	}
	char * delim = " ";							/* Delimiter for strtok */
	char * parse;								/* Hold the current value of string from input line */
	int pos = 0;								/* Position of each string in arra com_flag */

	parse = strtok(line, delim);						/* Get the first string before " " */

	/* Loop this the end of line */
	while(parse != NULL){
		com_fla[pos] = parse;						/* Store the string in the array */
		pos++;
		parse = strtok(NULL, delim);					/* Get the next string */
	}

	com_fla[pos] = NULL;							/* Null terminate the string array */
	return com_fla;
}

/*
 * Input: Directory where we want to navigate - string of characters
 *
 * Output: 0 upon success and -1 upon failure
 *
 * Description: This function takes a string of characters which is the 
 * 		directory where user wants to navigate.
 */
int cd(char * dir){
	return chdir(dir);
}

/*
 * Input: None
 *
 * Output: None
 *
 * Description: This function will initialize the parameters required for 
 *		sigaction
 */
void init_signal(void){
	struct sigaction s;
	s.sa_handler = sigint_handler;
	sigemptyset(&s.sa_mask);						/* Initialize sa_mask with emtpy set */
	s.sa_flags = SA_RESTART;						/* Signal handler will be invoked while the system is in the middle of a system call*/
	sigaction(SIGINT, &s, NULL);
}

/*
 * Input: Signal number to catch
 *
 * Output: None
 *
 * Description: This function catches the SIGINT signal and calls the 
 * 		siglongjmp function which will jump to the location set by
 * 		segsetjmp. In short, we give a soft restart to the shell
 */
void sigint_handler(int signum){
	if(!jump_initialized){
		return;
	}

	siglongjmp(params, 42);	
}

