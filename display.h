#define BORDER_SIZE_IN_PIXELS		20 
#define CELL_SIZE_IN_PIXELS			10 
#define MAX_CHARACTERS	        	12
#define VERTICAL_CELLS_PER_CHAR	 	 8
#define HORIZONTAL_CELLS_PER_CHAR	 8
#define HORIZONTAL     				 0
#define VERTICAL     				 1

#define SIGN_FRAME_COLOR 			0xAAAAFF
#define SIGN_BACKGROUND_COLOR 		0x000000
#define TEXT_COLOR	 				0xAA0000


// These are the external functions being used by the test program
extern void initializeWindow(char, int);
extern void closeWindow();
extern void displaySign(unsigned char[], int);

