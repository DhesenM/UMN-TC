// banlet_funcs.c: support functions for the banlet_main program.

#include "banlet.h"

// PROBLEM 1: Replace instances of character 'old' with 'new' in the
// string 'str'.  May use the strlen() library function to determine
// string length or directly look for a '\0' null termination
// character to end the string.
// 
// EXAMPLES:
// char *s1="A A B B A"; string_replace_char(s1, 'A', 'X'); // s1 is "X X B B X"
// char *s2="A A B B A"; string_replace_char(s2, 'B', 'Y'); // s2 is "A A Y Y A"
// char *s3="A A B B A"; string_replace_char(s3, ' ', '-'); // s3 is "A-A-B-B-A"
void string_replace_char(char *str, char old, char new){
	for(int i=0;i<strlen(str);i++){  // use strlen to find the length of string 'str'
		if(old == str[i]){           
			str[i] = new;            // replace old with new if the characters are identical
		}
	}
}

// PROBLEM 1: Counts the number of newline characters '\n' in the
// string 'msg'; return this number +1 as the end of lines will always
// be a line break. May use the strlen() library function to determine
// string length or directly look for a '\0' null termination
// character to end the string.
// 
// EXAMPLES:
// count_linebreaks("Hi")        ->  1
// count_linebreaks("Hi There")  ->  1
// count_linebreaks("Hi\nThere") ->  2
// count_linebreaks("O\nM\nG")   ->  3
int count_linebreaks(char *msg){
	int linebreaks = 1;              // initialize linebreaks (1)
	for(int i=0;i<strlen(msg);i++){  // use strlen to find the length of string 'msg'
		if(msg[i] == '\n'){          // find if there is \n in string 'msg' 
			linebreaks ++;           // add 1 to linebreak if there is \n
		}
	}
	return linebreaks;
}

// PROBLEM 1: Counts the linebreaks (newline '\n' chars and end of
// string) and returns an array of integers with the position for each
// linebreak in string 'msg'.  The 'nbreaks' parameter is an OUTPUT
// integer that should be set to the number of breaks in 'msg' using
// the C dereference operator (*).
//
// EXAMPLES:
// int nbreaks = -1;
// int *breaks = find_linebreaks("Hello\nWorld", &nbreaks);
// //            index in string: 012345 67890
// // nbreaks is now 2
// // breask is now [5, 11]
int *find_linebreaks(char *msg, int *nbreaks){
	int counter = 0;
	*nbreaks = count_linebreaks(msg);
    int *breaks = (int*)malloc(sizeof(int)* *nbreaks);
	for(int i=0;i<strlen(msg);i++){  // use strlen to find the length of string 'msg'
		if(msg[i] == '\n'){          // find if there is \n in string 'msg'
			breaks[counter]=i;       // add the number to the array breaks
			counter++;               // add 1 to the counter
		}
	}
	breaks[counter]=strlen(msg);     // add the last break into the array breaks
	return breaks;
}

// PROBLEM 1: Prints string 'msg' using 'font'. Only prints characters
// 0 to 'length-1'.  Iterates over each row in font->height and then
// scans across the charactes in 'msg' printing each "row" of the
// character. On reaching index 'length', prints a newline and then
// scans across 'msg' again printing characters from the next row.
// Each msg[i] character is used to as the index into fonts->glyphs[]
// to access the "glyph" that will be printed.
//
// NOTE: This function does NOT handle embedded newline '\n'
// characters. It is intended to be called repeatedly on each line in
// multiline text with '\n' characters found using the
// 'find_linebreaks()' function.
//
// EXAMPLE:
//
// print_fontified("Hello!", 6, &font_standard);
// // Prints the following on standard output:
//  _   _        _  _         _ 
// | | | |  ___ | || |  ___  | |
// | |_| | / _ \| || | / _ \ | |
// |  _  ||  __/| || || (_) ||_|
// |_| |_| \___||_||_| \___/ (_)
void print_fontified(char *msg, int length, font_t *font){
	for(int i=0; i<font->height; i++){         // use the first loop to switch the rows
		for(int j=0; j<length; j++){           // use the second loop to print the characters
			int codepoint = msg[j];            // change codepoint to each character
			printf("%s",font->glyphs[codepoint].data[i]);
			                                   // print one row of each character
		}
		printf("\n");                          // switch to the next line
	}
}

// PROBLEM 2: Uses print_fontified() with find_linebreaks() to
// correctly print 'msg' with 'font' even if there are linebreaks in
// 'msg'.  Uses find_linebreaks() to find the end of each line in
// 'msg' and then iterates over lines printing each.  Uses pointer
// arithmetic to pass the starting position of each line into calls of
// print_fontified(). Frees memory allocated before returning.
//
// EXAMPLE:
// print_fontified_linebreaks("apple\nB@N@N@\nCarr0^", &font_standard);
// // Shows the following_ on standard output:
//   __ _  _ __   _ __  | |  ___ 
//  / _` || '_ \ | '_ \ | | / _ \
// | (_| || |_) || |_) || ||  __/
//  \__,_|| .__/ | .__/ |_| \___|
//        |_|    |_|                    
//                       
//  ____     ____   _   _    ____   _   _    ____  
// | __ )   / __ \ | \ | |  / __ \ | \ | |  / __ \ 
// |  _ \  / / _` ||  \| | / / _` ||  \| | / / _` |
// | |_) || | (_| || |\  || | (_| || |\  || | (_| |
// |____/  \ \__,_||_| \_| \ \__,_||_| \_| \ \__,_|
//          \____/          \____/          \____/ 
//   ____                      ___   /\ 
//  / ___|  __ _  _ __  _ __  / _ \ |/\|
// | |     / _` || '__|| '__|| | | |    
// | |___ | (_| || |   | |   | |_| |    
//  \____| \__,_||_|   |_|    \___/     
void print_fontified_linebreaks(char *msg, font_t *font){
	int nbreaks = -1;                                // initialize nbreaks
	int *breaks = find_linebreaks(msg,&nbreaks);     // initialize *breaks with the linebreaks of msg
	                                                 // also allocates the memory of nbreaks
	for(int k=0;k<nbreaks;k++){                      // use for loop to switch the rows 
		if(k!=0){                                     
			print_fontified(msg+breaks[k-1]+1,breaks[k]-breaks[k-1]-1,font);
	// print all the rows except for the first row
	//     msg+breaks[k-1]+1 decides the start postion.
	//     Plus 1 because msg+breaks[k-1] is the postion where '\n' located
    //     For example, for "Hello\nworld". If k=1
	//		            then msg+breaks[k-1]+1 = msg+breaks[0]+1 = msg+5+1 = msg+6, which starts at 'w'

	//     breaks[k]-breaks[k-1]-1 decides the length of the specific row of characters
	//     Minus 1 because breaks[k]-breaks[k-1] is the length of the characters between two '\n'
	//     For example, for "Hello\nworld" breaks is provided above as [5,11]. If k=1,
	//                  then breaks[k]-breaks[k-1]-1 = 11-5-1=5 (same as the length of 'world')
		}
		else{
			print_fontified(msg,breaks[0],font);     // print the first row of characters
			                                         // breaks[0] is the length
		}
	}
	free(breaks);	                                 // free the allocated memory
}

// PROVIDED: Initializes a glyph to mostly X's except for its
// codepoint on the first line.
void glyph_init(glyph_t *glyph, int codepoint){
  glyph->codepoint = codepoint;
  glyph->width = 6;
  for(int i=0; i<MAX_HEIGHT; i++){
    for(int j=0; j<MAX_WIDTH; j++){
      if(j == glyph->width){
        glyph->data[i][j] = '\0'; // null terminate
      }
      else{
        glyph->data[i][j] = 'X';
      }
    }
  }
  int len = sprintf((char *)glyph->data, "%d",codepoint); // add codepoint # to glyph
  glyph->data[0][len] = 'X';                              // remove null termination char
}        

// PROBLEM 2: Loads a banner font from 'filename'.  The format of text
// file is documented more thoroughly in the project specification but
// is generally comprised of a first line that indicate the height of
// each glyph in the font followed by a sequence of each glyph
// starting with its codepoint (ASCII index) and a grid of characters
// in it. To make parsing easier, the @ character is used to represent
// blank spaces in glyph shapes.
//
// EXAMPLE:
// height: 4
// 42
// @@@
// \|/
// /|\
// @@@
// 43
// @@@
// _|_
// @|@
// @@@
//
// The two characters above are the codepoint 42 '*' and codepoint 43
// '+' with the @ symbols eventually being replaced by spaces during
// loading.
// 
// If 'filename' cannot be opened for reading, returns NULL.
//
// Memory allocation happens in two steps: (1) allocates memory for a
// font_t struct then (2) allocates memory for an array of glyph_t
// structs of length NUM_ASCII_GLYPHS (a constant defined in
// banlet.h). Sets the font->glyphs field to be the allocated array of
// glyphs.  Reads the font->height field from teh first line of the
// file.  Iterates over each element of the glyphs array and calls the
// glyph_init() function on it to populate it with default data.  Then
// proceeds to read glyphs from the file. Glyphs are read by reading
// the integer codepoint first which determins which element of the
// glyph array to read into.  Then a loop over the height of the font
// is used to read each row of the glyph into the
// glyph[codepoint]->data[row]; fscanf() with '%s' specifier is used
// for this.  Finally, the string_replace_char() function is used to
// replace all '@' characters with ' ' (space) characters in the glyph
// data. Sets the width of each glyph using the strlen() function on
// any balid row of the glyph data.
//
// Glyphs are read from 'filename' until an attempt to read a glyph's
// codepoint with fscanf() returns EOF (end of file). This causes the
// routine to return the allocated font_t data for use elsewhere.
font_t *font_load(char *filename){
	// allocate memory for a font_t and a glyph_t and set the font->glyphs field
	font_t *font = malloc(sizeof(font_t));   
	glyph_t *glyph = malloc(sizeof(glyph_t)*NUM_ASCII_GLYPHS);
	font->glyphs = glyph;
	// initialize the glyph with default data using glyph_init() function
	for(int i=0;i<NUM_ASCII_GLYPHS-1;i++){
		glyph_init(&glyph[i],i);
	}

	// initialize a FILE *file and an int height
	FILE *file;             
	int height;
	// open the txt file filename as file
	file = fopen(filename, "r");
	// if the filename provided is unvalid, free the font and return NULL
	if(file == NULL){
		font_free(font);       
		return NULL;
	}

	// Reads the font->height field from the first line of the file
	fscanf(file,"height: %d",&height);
	font->height = height;
	// initialize int codepoint and ret and continue to read the glyphs from the file
	int codepoint = 32;     
	int ret;                
	while(1){
        ret = fscanf(file,"%d",&codepoint);
		if(ret == EOF){   // break if no codepoint can be read
			break;
		}
		// use for loop over the height to read each row of the glyph into font
		for(int j=0;j<height;j++){    
			fscanf(file,"%s",font->glyphs[codepoint].data[j]);
			// replace '@' by ' '(space)
			string_replace_char(font->glyphs[codepoint].data[j],'@',' ');
			}
	}
	fclose(file);        // close the file
	return font;         // return font
}



// PROBLEM 2: Frees the memory associated with 'font'.  First frees
// the glyph array, then frees the font itself. Hint: this funciton
// should be 2 lines long.
void font_free(font_t *font){
	free(font->glyphs);
	free(font);
}
