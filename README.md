Author - Mihir Garude

Overview:
This is a simple implementation of the Linux shell.
I am implementing this as a hobby. I will add reference as I build the shell.

The shell_v1.c is the first initial version of a working shell. It will be improved in future versions.

Description: 
This is a complete implementation of Unix/Linux like shell. The shell is implemented using C and 
inbuilt functions available in Linux. 

To compile:
We have to link the readline library
gcc -o shell shell.c -lreadline

Features: 
- Changing directories.
- SIGINT signal handling using sigaction.
- Handling all shell commands.
- Soft reset on receiving SIGINT signal.

Limitations:
- Redirection not implemented.
- Pipes not implemented.
