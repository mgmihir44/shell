#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

	char * args[] = {"ls", "-l", "-a", "-h", NULL};
	execvp(args[0], args);

	return 0;
}
