//gibt Inhalt der Datei aus
// Ueb03 zu BKS
//Etienne Jentzsch und Carola Bothe

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>

//gibt Inhalt der Datei aus
int readout(char *datei){
	FILE *toread = fopen(datei, "r");
	int lesezeichen;
	while(1){
		lesezeichen = fgetc(toread);
		if(feof(toread)){
			break;
		}
		fprintf(stdout, "%c", lesezeichen);
	}
	fclose(toread);
	return 0;
}



int main(int argc, char *argv[]){

if(argc < 2){
	printf("Please enter at least one argument to read from: a file name or directly text.\n");
}

char *fname = argv[1];
FILE *data=fopen(fname, "r");
if (argc==2){

	if (data == NULL){
		printf("Cannot open the file. Reading from stdin:\n");
		printf("%s\n",argv[1]);
	}
	fscanf(data,"r");
	printf("%s\n",data);
} 

    	
else{	//argc>2
	//to implement: printf("A few of your arguments are files, a few arent. Please decide whether to enter files or text directly. For the latter use quotation marks if it exceeds one word."\n);
	for(int16_t j = 1; j < argc; j++){
		pid_t pid = fork();
		readout(argv[j]);
		while(data != NULL){
			if(pid == 0){
				waitpid(pid, NULL, 0);
				break;
			}
		}
	}			
	fclose(data);
}


/*Nein, forken ist hier nicht sinnvoll, da die Reihenfolge der Ausgabe  eh der der Eingabe enstpricht und alle Prozesse die gleiche Ausgabe verwenden. Also selbst wenn der letzte Prozess vor dem ersten fertig ist, muss er trotzdem noch auf die Ausgabe der vorigen warten. 
else{printf("Please enter at least one additional argument: A file to read from or text to read!\n");return 1;}*/	
	
	return 0;
}





