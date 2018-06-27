Author - Mihir Garude

Overview:
This is an implementation of the Linux shell.

Description: 
This is a complete implementation of Unix/Linux like shell. The shell is implemented using C and 
inbuilt functions available in Linux. 

To compile:
We have to link the readline library:
gcc -o shell shell.c -lreadline

Features: 
- Changing directories.
- SIGINT signal handling using sigaction.
- Handling all shell commands.
- Soft reset on receiving SIGINT signal.

Limitations:
- Redirection not implemented.
- Pipes not implemented.
