/*
 * Testing the fork system call
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(){

	pid_t proc_id = fork();

	/* fork() returns 0 to the child and PID of the child to the parent */
	if(proc_id == 0){
		printf("\n*** Child Process ***\nCurrent PID: %d and Child PID: %d\n", getpid(), proc_id);
	}else{
		sleep(1);		/* Sleep for 1 second */
		printf("\n*** Parent Process ***\nCurrent PID: %d and Child PID: %d\n", getpid(), proc_id);
	}

	return 0;
}
