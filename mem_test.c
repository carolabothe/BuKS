#include <stdio.h>
#include "memory.c"

int main (int argc, char* argv[]){
	memory_init();
	if (argc < 2){
			fprintf(stderr,"Bitte Argumente wie folgt eingeben: -p für print oder (2 Argumente) -a byte_count für allocate oder -f pointer für free.\n");
			return 1;
	}
	//argv[1] = {-d,-r,-w}
	else{switch(argv[1][1]){
		case 'a':
			memory_allocate((int32_t)argv[2]);
			break;	
		case 'f':
			memory_free((void*)argv[2]);
			break;
		case 'p':
			memory_print();
			break;
		default:
			fprintf(stderr,"Bitte Argumente wie folgt eingeben: -p für print oder (2 Argumente) -a byte_count für allocate oder -f pointer für free.\n");
			return 1;
	}}
	return 0;
}