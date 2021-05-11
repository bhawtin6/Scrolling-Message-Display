Generates a scrolling-message display, like you might see in the windows of shops. Works vertically and horizontally of desired sizes.

FILES INCLUDED: 
  signCoder.c
  display.c
  display.h

REQUIREMENTS
	C compiler
	libX11

COMPILATION AND RUNNING INSTRUCTIONS
download all files and place them in a known directory together
in terminal, navigate to the directory and use following commands to compile the program
  gcc -c signCoder.c
  gcc -o signCoder signCoder.c display.c -lX11
launch using command "./signCoder"

USING THE PROGRAM
You will be given 3 prompts asking you for the  
  1. Orientation of the sign
  2. Size of the sign (the number of letters that can be displayed at once.)
  3. The display message
After answering the prompts, a scrolling sign will be produced using the entered criteria.
  

