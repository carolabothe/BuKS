// Ueb03 zu BKS
//Etienne Jentzsch und Carola Bothe

#include <stdio.h>

//gibt Inhalt der Datei aus
int main(int argc, char *argv[]){
	if (argc < 2){
		printf("Please enter at least one argument to read from: a file name or the text directly.\n");
		return 1;
	}
	char *fname = argv[1];
	FILE *data=fopen(fname, "r");
	if (data == NULL){
     		printf("Cannot open the file. Trying to read from stdin.\n");
		printf("%s\n",fname);    
}
    	
	else{
		fscanf(data, fname);
		printf(data);
		printf("\n");} 
	fclose(data);
	


	
	
	return 0;
}
