#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>	//für zuletzt modifiziert
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>

int filter (const struct dirent* entry){
	if(entry->d_name[0] == '.'){return 0;}
	else{return 1;}
}

int main(int argc, char* argv[]){

struct dirent **namelist;

int64_t optl = 0;	//sortiert und letzte modifikation
int64_t opta = 0;	//versteckte Dateien
char path[100]=".";	//default ist CWD
int64_t n;
if(argc==1){
	n = scandir(".",&namelist, filter, NULL);	//öffnen momentanes Verzeichnis
}
if(argc>1){
	if(argc==3){
		strcpy(path,argv[2]); //gehen davon aus das erst optionen angegeben werden
	}
	
	if(argv[1][0]=='-'){
		switch(argv[1][1]){
			case 'a': opta = 1;break;
			case 'l': optl = 1;break;
			default: fprintf(stderr, "Keine gültige Option.\n");return 2;
		}
		switch(argv[1][2]){
			case '\0': break;
			case 'a': opta = 1;break;
			case 'l': optl = 1;break;
			default: fprintf(stderr, "Keine gültige Option.\n"); return 2;
		}
		if(opta == 1){	//versteckte datein auflisten
			if(optl == 1){//und sortieren
				n = scandir(path,&namelist, NULL, alphasort);}
			else{n = scandir(path,&namelist, NULL, NULL);}
		}
		else if(optl==1){ n = scandir(path,&namelist, filter, alphasort);}//nur sortieren
	}
	
	else{
		strcpy(path,argv[1]); //Ordner angegeben
		n = scandir(path,&namelist, filter, NULL); //nur ausgeben (ohne Option)
	}
}
if(n<0){
	fprintf(stderr,"Fehler beim Lesen des Ordners.\n");
	return 1;
}
if(n==0){printf("Der angegebene Ordner ist leer.\n");}

struct stat *buf;
buf = malloc(sizeof(struct stat));
for(int64_t i = 0;i<n;i++){
	if(optl == 1){
		stat(namelist[i]->d_name,buf);
		time_t t = buf->st_mtime;	//rufen letzte modifikation ab
		printf("%s\t%s",namelist[i]->d_name,asctime(localtime(&t)));//umwandeln in Kalenderzeit
	}	
	else{printf("%s\n", namelist[i]->d_name);}
	free(namelist[i]);

}
free(buf);

return 0;
}

