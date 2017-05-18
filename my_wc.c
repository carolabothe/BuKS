#include <stdio.h>

int main(int argc, char *argv[]){
	if(argc != 2 && argc != 4){
		fprintf(stderr, "%s", "Wrong parameter count!\n");
		return 1;
	}
	int linecount = 0;
	int charactercount = 0;
	long bytecount = 0;
	int character;
	FILE *countit = fopen(argv[1], "r");
	while(1){
		character = fgetc(countit);
		if(character == '\n'){
			linecount++;
			charactercount--;
		}
		if(feof(countit)){
			bytecount = ftell(countit);
			break;
		}
		charactercount++;
	}
	fprintf(stdout, "%d %d %ld\n", linecount, charactercount, bytecount);
	fclose(countit);
	return 0;
}
