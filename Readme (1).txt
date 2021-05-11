Name: Bennett Hawtin

Student Number: 101211270

Source files submitted: signCoder.c, display.c, display.h

Compile and run instructions:
0. Ensure access to X11/Xlib
1. Open terminal/shell
2. Navigate to directory with the source files (listed above)
3. Enter the following commands into the shell:
gcc -c signCoder.c
gcc -o signCoder signCoder.c display.c -lX11
./signCoder

