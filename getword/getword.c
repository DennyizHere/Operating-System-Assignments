/*
 * Name: Denny Cao
 * Class account: cssc0012
 * Professor: John Carroll
 * Class: CS570
 * Due date: 02/09/18
 */

#include "getword.h"
#include <stdio.h>

int getword(char *w) {
	int size = 0;
	int iochar;
	int slashBool = 0;

	//reads in char stream until EOF is reached
	while ((iochar = getchar()) != EOF) {

		if (size == (STORAGE - 1)) {
			ungetc(iochar,stdin);
			*w = '\0';
			return size;
		}

		//gets rid of any leading spaces
		if (size == 0 && iochar == 32) {
			while (iochar == 32) { 
				iochar = getchar();
				*w = '\0';
			}
		}

		//anything that is not a space, new line, or # is processed
		if (iochar != 32 && iochar != 35 && iochar != 10 && iochar != 47 && iochar != 60 && iochar != 62 && iochar != 38 && iochar != 92 && iochar != 124) {
			size++;
			*w++ = iochar;
			if ((iochar = getchar()) == 60 || iochar == 62 || iochar == 38) {
				*w = '\0';
				ungetc(iochar,stdin);
				return size;
			}
			ungetc(iochar,stdin);
		}

		//finds a space, returns the word
		else if (iochar == 32) {
			if (*(w - 1) != 32) {
				*w = '\0';
				return size;
			}
		}

		//deals with a slash
		else if (iochar == 92) {
			if ((iochar = getchar()) == 60 || iochar == 62 || iochar == 38 || iochar == 92) {
				size++;
				*w++ = iochar;
			}
			if (iochar == 35 && size == 0) {
				size++;
				*w++ = iochar;
			}
			else {
				ungetc(iochar, stdin);
			}
			slashBool = 1;
		}

		//deals with the '|' or '|&'
		else if (iochar == 124) {
			size--;
			*w++ = iochar;
			if ((iochar = getchar()) == 38) {
				size--;
				*w++ = iochar;
				*w = '\0';
				return size;
			}
			else {
				*w = '\0';
				ungetc(iochar,stdin);
				return size;
			}
		}

		//if there's a meta character
		else if ((iochar == 60 || iochar == 62 || iochar == 38 || iochar == 35)) {
			if (size == 0) {
				*w++ = iochar;
				*w = '\0';
				return -1;
			} 
			else if (slashBool == 1) {
				*w++ = iochar;
				size++;
			}
			else if (iochar == 35 && *(w - 1) != '\0') {
				size++;
				*w++ = iochar;
			}
			else {
				*w = '\0';
				return size;
			}
		}
/*
		//process when a '#' is found	
		else if (iochar == 35) {
			//if there is # right at the beginning of a new word, then return a '-1'
			if (*(w - 1) == '\0') {
				*w++ = 35;
				*w = '\0';
				return -1;	
			}
			//processes if found in the middle of a word
			else {
				size++;
				*w++ = 35;
			}
		}
*/

		//process a new line
		else if (iochar == 10) {
			//in case a new line character is right after a word instead of a space, then return the processed word, and puts back a new line
			if (*(w - 1) != '\0') {
				*w = '\0';
				ungetc(10,stdin); //turns out this is how you force put in things into a stream
				return size;
			}
			//if the stream just has a new line, then return '-10'
			if (size == 0) {
				*w = '\0';
				return -10;
			}
		}

		//process if there is a possiblity of a premature EOF
		if ((iochar = getchar()) == EOF) {
			*w = '\0';
			return size;
		}
		//undo the getchar in the check
		ungetc(iochar,stdin);
	}
	*w = '\0';
	return size;
}
