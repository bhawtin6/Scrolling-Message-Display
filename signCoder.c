#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "display.h"

#define MAX_MESSAGE_SIZE  100

int main() {
    
    //array of characters in bit representation
    unsigned char letter[28][8] = {
        { 16,  40,  68,  68, 124,  68,  68, 238},    //A0
        {248,  68,  72, 124,  66,  66,  66, 252},    //B1
        { 58,  70, 130, 128, 128, 130,  70,  58},    //C2
        {248,  68,  66,  66,  66,  66,  68, 248},    //D3
        {254,  66,  64, 120,  64,  64,  66, 254},    //E4
        {254,  66,  66,  72, 120,  72,  64, 224},    //F5
        { 62,  66, 128, 158, 146, 130,  66,  60},    //G6
        {238,  68,  68, 124,  68,  68,  68, 238},    //H7
        {254,  16,  16,  16,  16,  16,  16, 254},    //I8
        { 62,   8,   8,   8, 200, 136, 136, 112},    //J9
        {238,  68,  72, 112,  72,  68,  68, 238},    //K10
        {224,  64,  64,  64,  64,  64,  66, 254},    //L11
        {198, 108,  84,  84,  68,  68,  68, 238},    //M12
        {206, 100,  84,  84,  76,  68,  68, 238},    //N13
        { 56,  68, 130, 130, 130, 130,  68,  56},    //O14
        {252,  66,  66, 124,  64,  64,  64, 224},    //P15
        {124, 130, 130, 130, 130, 146, 124,  16},    //Q16
        {252,  66,  66, 124,  80,  72,  68, 238},    //R17
        {122, 134, 130, 120,   4, 130, 194, 188},    //S18
        {254, 146,  16,  16,  16,  16,  16,  56},    //T19
        {238,  68,  68,  68,  68,  68,  68,  56},    //U20
        {238,  68,  68,  68,  68,  68,  40,  16},    //V21
        {238,  68,  68,  68,  84,  84,  84,  40},    //W22
        {238,  68,  40,  16,  16,  40,  68, 238},    //X23
        {238,  68,  68,  40,  16,  16,  16,  56},    //Y24
        {254, 130,   4,  24,  32,  64, 130, 254},    //Z25
        {  0,   0,   0,   0,   0,   0, 192, 192},    //Dot26
        {  0,   0,   0,   0,   0,   0,   0,   0}     //Space27
    };

    //variables for setting sign settings
    unsigned char orientation = HORIZONTAL;
    unsigned char ansOrient;
    int signSize = MAX_CHARACTERS;
    int ansSize;

    //get and set the sign orientation
    printf("Would you like the sign to be horizontal or vertical?\nType 'H' or 'V' : ");
    ansOrient = getchar();
    if (ansOrient == 'V' || ansOrient == 'v') orientation = VERTICAL;

    //get and set the sign size
    while ((getchar()) != '\n'); //clear the input buffer
    printf("How many characters should the sign be able to show at once? ");
    scanf("%d", &ansSize);
    if (ansSize < 1) signSize = 1;
    else if (ansSize > MAX_CHARACTERS) signSize = MAX_CHARACTERS;
    else signSize = ansSize;

    //get and set the sign text
    unsigned char text[100+signSize];
    unsigned char ch;
    int textLength = 0;
    while ((getchar()) != '\n'); //clear the input buffer
    printf("Input the sign text (max 100 characters): ");
    while ((ch = getchar()) != '\n' && textLength < 100){
        if ((ch > 64 && ch < 91) || ch == 46 ){text[textLength] = ch;}
        else if (ch > 96 && ch < 123){text[textLength] = ch - 32;}
        else {text[textLength] = 32;}
        textLength++;
    }
    for (int i = 0; i < signSize ; i++){
        text[textLength] = ' ';
        textLength++;
    }
    
    
    
    unsigned char inputBytes[8*textLength];
    int inputBytesLength = 0;
	
    //determine inputBytes if the orientation is vertical	
    if (orientation == VERTICAL){
        for (int i = 0 ; i < textLength ; i++){ //for each letter

            //get letterID based on text[i]
            int letterID;
            if (text[i] == 46){
                letterID = 26;
            }
            else if (text[i] == 32){
                letterID = 27;
            }
            else{
                letterID = text[i] - 65;
            }

            //in order, add the appropriate byte to inputBytes
            for (int j = 0 ; j < VERTICAL_CELLS_PER_CHAR; j++){ //for each byte in the letter
                inputBytes[inputBytesLength] = letter[letterID][j];
                inputBytesLength++;
            }
            
            inputBytes[inputBytesLength] = 0;
            inputBytesLength++;
        }
    }
    
    //determine inputBytes if the orientation is horizontal
    else if (orientation == HORIZONTAL){
        for (int i = 0 ; i < textLength ; i++){ //for each letter
            
            //get letterID based on text[i]
            int letterID;
            if (text[i] == 46){
                letterID = 26;
            }
            else if (text[i] == 32){
                letterID = 27;
            }
            else{
                letterID = text[i] - 65;
            }   
	    
	    
            //since the orientation is horizontal, we must 
            //flip the byte arrays so that the 'left' edge byte (flipped[0])
            //goes first and the right edge (flipped[7]) goes last
            unsigned char flipped[8]; //this array will hold the data for the 'flipped' letter
		
            for (int o = 0; o <8 ;o++){ //initialize the flipped array
            	flipped[o] = 0;
            }
            
            for (int j = 0 ; j < HORIZONTAL_CELLS_PER_CHAR; j++){ //for each byte in the letter        
                for (int k = 0 ; k < 8 ; k++){ //for each bit in each byte
                    flipped[k] += (((letter[letterID][j] >> (7-k)) & 0b00000001) << j);
                }
            }

            //in order, add the appropriate byte to inputBytes
            for (int j = 0 ; j < HORIZONTAL_CELLS_PER_CHAR; j++){
                inputBytes[inputBytesLength] = flipped[j];
                inputBytesLength++;
            }          
        }
    }
    else {
        printf("An error has occurred. Orientation = %c", orientation);
    }

    initializeWindow(orientation, signSize);
    displaySign(inputBytes, inputBytesLength); 
      
    
    
    
    



    


		
}

