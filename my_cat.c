// Ueb03 zu BKS
//Etienne Jentzsch und Carola Bothe

#include <stdio.h>

//gibt Inhalt der Datei aus
int main(int argc, char *argv[]){
	if (argc != 2){
		printf("Please enter exactly one argument: the file name.\n");
		return 1;
	}
	char *fname = argv[1];
	FILE *data=fopen(fname, "r");
	if (data == NULL){
     	printf("Cannot open the file.\n");
      	return 1;
    }
    	printf("%s\n",data); //finde das richtig %irgendwas nicht 
	fclose(data);
	return 0;
}
