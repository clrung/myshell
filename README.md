A custom implementation of a shell that I wrote for CSCI 315: Operating Systems Design, during my Junior year at Bucknell.
The shell is able to read and execute multiple commands that are separated by a semicolon, and also supports piping.

Use the provided makefile to compile the source.
```Shell
myshell user$ make
gcc -std=gnu99 -Wall -g  myshell.c -o myshell
```

To view debug output, uncomment the `—DDEBUG` flag in the makefile, run `make clean` to delete the compiled files, and then run `make` again.
`make clean` removes compiled files. 