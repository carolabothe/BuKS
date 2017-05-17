// Ueb03 zu BKS
//Etienne Jentzsch und Carola Bothe

#include <stdio.h>

//gibt Inhalt der Datei aus
int my_cat(char *fname){
	if (argc > 2){
		fprintf(stderr, "You entered too many arguments, please enter one file to read only.");
		return 1;
	}
	if (argc < 2){
		fprintf(stderr, "You entered too few arguments, please enter a file to read.");
		return 1;
	}	
	
	data=fopen(fname, "r");
	if (datei == NULL){
     	printf("Cannot open the file.");
      	return 1;
    }
    fprintf(data); 
	fclose(data);
	return 0;
}

// gibt Anzahl der Zeilen, Wörter und Bytes aus
int my_wc(char *fname){

	return 0;
}