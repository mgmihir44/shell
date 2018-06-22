/*
 * Program to test signal function
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_handler(int signo);

int main(){
	
	signal(SIGINT, sig_handler);
	while(1);
	return 0;
}

void sig_handler(int signo){

	printf("Captured Signal\n");

}
