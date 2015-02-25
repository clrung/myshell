## myshell
A custom implementation of a shell that I wrote for CSCI 315: Operating Systems Design, during my Junior year at Bucknell.
The shell is basic, but has some added functionality; it is able to read and execute multiple commands that are separated by a semicolon, and it also supports piping.

### Installation
Use the provided makefile to compile the source.
```Shell
$ make
gcc -std=gnu99 -Wall -g  myshell.c -o myshell
```

To view debug output, uncomment the `—DDEBUG` flag in the makefile, run `make clean` to delete the compiled files, and then run `make` again.

### Usage
After running myshell from the command line, you will see myshell’s prompt
```Shell
$ ./myshell
myshell>
```
Input any bash command into this shell and it will execute the command as you would expect in a regular shell.
```Shell
myshell>ls
LICENSE		makefile	myshell.c
README.md	myshell		myshell.dSYM
```
Separating commands with a semicolon executes both commands.
```Shell
myshell>mkdir test; ls
LICENSE		makefile	myshell.c	test
README.md	myshell		myshell.dSYM
```