#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <X11/Xlib.h>

#include "display.h"


//Display-related variables
Display *display;
Window   win;
GC       gc;
char 	 orientation;
int  	 numCharsOnDisplay;


// Initialize and open the simulator window with size ENV_WIDTH x ENV_HEIGHT.
void initializeWindow(char horizontalOrVertical, int numChars) {
	orientation = horizontalOrVertical;
	numCharsOnDisplay = numChars;
	
  // Open connection to X server
  display = XOpenDisplay(NULL);

  // Create a simple window, set the title and get the graphics context then
  // make is visible and get ready to draw
  if (horizontalOrVertical == HORIZONTAL)
  	win = XCreateSimpleWindow(display,  RootWindow(display, 0), 0, 0,
			    (numCharsOnDisplay*HORIZONTAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(BORDER_SIZE_IN_PIXELS*2), 
			    (VERTICAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(BORDER_SIZE_IN_PIXELS*2)+(CELL_SIZE_IN_PIXELS*2), 
			    0, 0x000000, 0xFFFFFF);
  else
  	win = XCreateSimpleWindow(display,  RootWindow(display, 0), 0, 0,
			    (HORIZONTAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(BORDER_SIZE_IN_PIXELS*2)+(CELL_SIZE_IN_PIXELS), 
			    (numCharsOnDisplay*VERTICAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(BORDER_SIZE_IN_PIXELS*2), 
			    0, 0x000000, 0xFFFFFF);
  XStoreName(display, win, "Scrolling Sign");
  gc = XCreateGC(display, win, 0, NULL);
  XMapWindow(display, win);
  XFlush(display);
  usleep(20000);  // sleep for 20 milliseconds.
}



// Close the display window
void closeWindow() {
  XFreeGC(display, gc);
  XUnmapWindow(display, win);
  XDestroyWindow(display, win);
  XCloseDisplay(display);
}



// Redraw all the obstacles and the edges and the vertices
void displaySign(unsigned char bytes[], int numBytes) {
 	unsigned char buffer[numCharsOnDisplay*HORIZONTAL_CELLS_PER_CHAR][VERTICAL_CELLS_PER_CHAR];

	// Draw the sign frame and inner border
  	if (orientation == HORIZONTAL) {
		XSetForeground(display, gc, SIGN_FRAME_COLOR);
   	 	XFillRectangle(display, win, gc, 0, 0,
    			   	  	(numCharsOnDisplay*HORIZONTAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(BORDER_SIZE_IN_PIXELS*2), 
    			   	  	(VERTICAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(BORDER_SIZE_IN_PIXELS*2)+(CELL_SIZE_IN_PIXELS*2));
		XSetForeground(display, gc, SIGN_BACKGROUND_COLOR);
    	XFillRectangle(display, win, gc, BORDER_SIZE_IN_PIXELS, BORDER_SIZE_IN_PIXELS,
    			   		(numCharsOnDisplay*HORIZONTAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS), 
    			   		(VERTICAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(CELL_SIZE_IN_PIXELS*2));
    }
	else {
		XSetForeground(display, gc, SIGN_FRAME_COLOR);
   	 	XFillRectangle(display, win, gc, 0, 0,
    			   	  	(HORIZONTAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(BORDER_SIZE_IN_PIXELS*2)+(CELL_SIZE_IN_PIXELS),
    			   	  	(numCharsOnDisplay*VERTICAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(BORDER_SIZE_IN_PIXELS*2));
		XSetForeground(display, gc, SIGN_BACKGROUND_COLOR);
    	XFillRectangle(display, win, gc, BORDER_SIZE_IN_PIXELS, BORDER_SIZE_IN_PIXELS,
    			   		(HORIZONTAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS)+(CELL_SIZE_IN_PIXELS),
   			   			(numCharsOnDisplay*VERTICAL_CELLS_PER_CHAR*CELL_SIZE_IN_PIXELS));
 	}
 	
 	// Initialize the buffer
 	for (int c=0; c<numCharsOnDisplay*HORIZONTAL_CELLS_PER_CHAR; c++) {
 		for (int r=0; r<VERTICAL_CELLS_PER_CHAR; r++) {
 			buffer[c][r] = 0;
 		}
 	}
 	
 	// Now draw the buffer repeatedly until the message has completed
 	for (int i=0; i<numBytes; i++) {
 		// Copy the bytes 1 column to the left
 		for (int c=0; c<numCharsOnDisplay*HORIZONTAL_CELLS_PER_CHAR-1; c++) {
 			for (int r=0; r<VERTICAL_CELLS_PER_CHAR; r++) {
 				buffer[c][r] = buffer[c+1][r];
 			}
 		}
 		// Add the new column on the right from the message
 		char mask = 1;
 		for (int r=0; r<8; r++) {
 			if (orientation == HORIZONTAL) 
 				buffer[numCharsOnDisplay*HORIZONTAL_CELLS_PER_CHAR-1][r] = (bytes[i] & mask) >> r;
 			else
 				buffer[numCharsOnDisplay*HORIZONTAL_CELLS_PER_CHAR-1][VERTICAL_CELLS_PER_CHAR-r] = (bytes[i] & mask) >> r; 				
    		mask*=2;
    	}
    	
    	// Display the whole buffer
    	for (int c=0; c<numCharsOnDisplay*HORIZONTAL_CELLS_PER_CHAR; c++) {
    		int x, y;
    		
    		if (orientation == HORIZONTAL) {
    			x = BORDER_SIZE_IN_PIXELS + (c*CELL_SIZE_IN_PIXELS);
   				y = BORDER_SIZE_IN_PIXELS + CELL_SIZE_IN_PIXELS;
   			}
   			else {
   				x = BORDER_SIZE_IN_PIXELS;
   				y = BORDER_SIZE_IN_PIXELS + (c*CELL_SIZE_IN_PIXELS);
   			}
 			for (int r=0; r<VERTICAL_CELLS_PER_CHAR; r++) {
 				if (buffer[c][r] == 1) {
    				XSetForeground(display, gc, TEXT_COLOR);
					XFillRectangle(display, win, gc, x, y, CELL_SIZE_IN_PIXELS, CELL_SIZE_IN_PIXELS);    			
    			}
    			else {
   					XSetForeground(display, gc, SIGN_BACKGROUND_COLOR);
					XFillRectangle(display, win, gc, x, y, CELL_SIZE_IN_PIXELS, CELL_SIZE_IN_PIXELS);   			
    			}
    			if (orientation == HORIZONTAL)
    				y += CELL_SIZE_IN_PIXELS;
    			else
    				x += CELL_SIZE_IN_PIXELS;
 			}
 		}
  		// Add a delay a bit
 		XFlush(display);
    	usleep(20000);
	}

	// Wait to close
    getchar();
}


