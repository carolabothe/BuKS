/*ueb01 
Tugçke Çetinkaya and Carola Bothe
*/


#include <stdio.h>
#include <inttypes.h>

int main(int argc, char* argv[]){
	int sum = 0;
    for (int i = 1; i < argc-1; ++i){	//not i=0 because that would be the program itself
    	sum += (int) strtol(argv[i], (char **)NULL, 10);	//atoi alternative (right one?)

    }
    char c;
    if (sum == (int) strtol(argv[argc-1], (char **)NULL, 10)){
    	c = '1';
    }
    else{
    	c = '0';
    }
    printf("%c\n",c);
}
