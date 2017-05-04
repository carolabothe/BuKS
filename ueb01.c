/*ueb01 
Tuğçe Çetinkaya and Carola Bothe
*/


#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
if (argc<=2){
	fprintf(stderr,"Please enter at least 2 arguments.\n"); //Programm braucht mind. 2 Argumente um Summe zu bilden
	}
else{	
	char *ptr;
	char *ptr2;
	int sum = 0;
    	for (int i = 1; i < argc-1; ++i){	
    		sum += strtol(argv[i], &ptr, 10);	//atoi alternative:  kein string part, basis ist 10 (dezimal)
    		if (*ptr != 0){	
    			fprintf(stderr,"Please enter integers only.\n"); 
    		}
    	}
    	char c;
    	if (sum == strtol(argv[argc-1], &ptr2, 10)){
    		c = '1';
		if (*ptr2 != 0){	
    			c = '0';
			fprintf(stderr,"Last argument is not an integer.\n");
    		}
		
    	}
    	else{
    		c = '0';
    	}
    	
    printf("%c\n",c);
}
}
