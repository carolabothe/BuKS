#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>

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

int main(int argc, char *argv[1]){
	int i = 1;
	if(argc < 1){
		fprintf(stderr, "%s", "Falsche Eingabe!\n");
	}
	for(int j = 0; j <= argc-1; j++){
	pid_t pid = fork();
	readout(argv[i]);
	while(1){
		if(pid == 0){
	waitpid(pid, NULL, 0);
	break;
		}
	}
	return 0;
}
}
