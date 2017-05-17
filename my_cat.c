// Ueb03 zu BKS
//Etienne Jentzsch und Carola Bothe

#include <stdio.h>

//gibt Inhalt der Datei aus
int main(char *fname){
	
	FILE *data=fopen(fname, "r");
	if (data == NULL){
     	printf("Cannot open the file.");
      	return 1;
    }
    	fprintf("%s",data); 
	fclose(data);
	return 0;
}
