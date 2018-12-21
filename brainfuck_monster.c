
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
	unsigned char tape[30000] = {0}; // tape of 30 unsigned chars
	int cell = 0;                    // target given cell
	int insp = 0;                    // where in fstring is given char ("instruction pointer")
	FILE * f = fopen(argv[1], "rb");
	if (f == NULL) {
		printf("Could not open file.\n");
		printf("Was a file specified?\n");
		printf("Does the specified file exist?\n");
		printf("Usage: brainfuck_monster myfile.b");
		return 0;
	}
	fseek(f, 0, SEEK_END); // end of file
	long fsize = ftell(f); // file size
	fseek(f, 0, SEEK_SET); // reset to beginning of file
	char * fstring = malloc(fsize + 1); // allocate
	if (fstring == NULL) {
		printf("Alocation failed");
		return 0;
	}
	fread(fstring, fsize, 1, f);     // get file data as string
	fclose(f); 
	fstring[fsize] = 0;              // null terminate
	
	// get if matching brackets
	int startbrack = 0;
	int nbrackets = 0;
	for (insp = 0; insp < fsize; insp++) {
		if (fstring[insp] == ']') {
			if (startbrack >= 0) {
				startbrack--;
			}
		}
		else if (fstring[insp] == '[') {
			nbrackets++;
			startbrack++;
		}
	}
	if (startbrack != 0) {
		printf("Brackets don't match");
		return 0;
	}

	int * go2 = malloc(fsize * sizeof(int));
	int * skip2 = malloc(fsize * sizeof(int));
	if (go2 == NULL || skip2 == NULL) {
		printf("allocation failed");
		return 0;
	}
	int x;
	int y;
	for (x = 0; x < fsize; x++) {
		if (fstring[x] == '[') {
			startbrack = 1;
			for (y = x+1; y < fsize; y++) {
				if (fstring[y] == '[') {
					startbrack++;
				}
				else if (fstring[y] == ']') {
					startbrack--;
				}
				if (startbrack == 0) {
					skip2[x] = y;
					break;
				}
			}
		}
	}
	startbrack = 0;
	for (x = fsize-1; x >= 0; x--) {
		if (fstring[x] == ']') {
			startbrack = 1;
			for (y = x-1; y >= 0; y--) {
				if (fstring[y] == ']') {
					startbrack++;
				}
				else if (fstring[y] == '[') {
					startbrack--;
				}
				if (startbrack == 0) {
					go2[x] = y;
					break;
				}
			}
		}
	}


	insp = 0;
	int charsin = 0; // for debuggin
	while (insp < fsize) {
		// debuging
		// printf("\nCell:%d Val: %d\n", cell, tape[cell]);
		// printf("%s\n", fstring);
		// for (charsin = 0; charsin < insp; charsin++) {
		// 	putchar(' ');
		// }
		// putchar('|');
		// getchar();

		if (fstring[insp] == '+') {
			tape[cell]++;
		}
		else if (fstring[insp] == '-') {
			tape[cell]--;
		}
		else if (fstring[insp] == '<') {
			if (cell - 1 < 0) {
				cell = 29999;
			}
			else {
				cell--;
			}
		}
		else if (fstring[insp] == '>') {
			if (cell + 1 >= 30000) {
				cell = 0;
			}
			else
			{
				cell++;
			}
		}
		else if (fstring[insp] == '.') {
			putchar(tape[cell]);
		}
		else if (fstring[insp] == ',') {
			tape[cell] = getchar();
		}
		else if (fstring[insp] == '[') {
			if (tape[cell] == 0) {
				insp = skip2[insp];
			}
			
		}
		else if (fstring[insp] == ']') {
			if (tape[cell] != 0) {
				insp = go2[insp];
			}
		}
		insp++;
	}
	free(fstring);
	free(go2);
	free(skip2);
	return 0;
}
