#include <stdio.h>
#include <unistd.h>

int main(){

	char * argv[] = {"ls", "-l", "-h", "-a", NULL};
	execvp(arv[0], argv);
	return 0;
}
