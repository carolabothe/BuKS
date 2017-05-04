/*ueb01 
Tuğçe Çetinkaya and Carola Bothe
*/


#include <stdio.h>
#include <inttypes.h>

int main(int argc, char* argv[]){
	if (size(argc)==2){
		fprintf(stderr,"Please enter at least 2 arguments."); //Programm braucht mind. 2 Argumente um Summe zu bilden
	}	
	char* ptr, ptr2;
	int sum = 0;
    for (int i = 1; i < argc-1; ++i){	
    	sum += (int) strtol(argv[i], &ptr, 10);	//atoi alternative:  kein string part, basis ist 10 (dezimal)
    	

    }
    char c;
    if (sum == (int) strtol(argv[argc-1], &ptr2, 10)){
    	c = '1';
    }
    else{
    	c = '0';
    }
    if (size(ptr) != 0) || (size(ptr2) != 0){
    	fprintf(stderr,"Please enter integers only."); //strtol speichert alles nicht konvertierbare in einem string, der für unser programm also leer sein sollte
	}
    printf("%c\n",c);
}
