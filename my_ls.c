#define POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
//auswählen der Option



//Auflisten des aktuellen Ordnerinhalts, keine explizite Sortierung gefordert
//hilfreiche Seite: http://pubs.opengroup.org/onlinepubs/007908775/xsh/readdir.html
/* aus scandir manual, für -l:
struct dirent **namelist;
int n;
n = scandir(".",&namelist, NULL, alphasort);
if(n<0){
	fprintf(stderr,"Fehler beim Lesen des Ornders");
	return 1;
}

for(int64_t i = 0;i<n;i++){
	printf("%s\n", namelist[i]->d_name);
	free(namelist[i]);
}*/

DIR* dirp;
if((argc==2)&(argv[2][1]!='-')){
dirp = opendir(argv[2]);
}
else if(argc==3){
dirp = opendir(argv[3]);//gehen davon aus das erst optionen angegeben werden
}
else{
dirp = opendir(".");	//öffnen momentanes Verzeichnis
}
if(dirp == NULL){printf("Fehler beim Öffnen.");}
struct dirent* entry = readdir(dirp);
while((entry = readdir(dirp))){ 
	printf("%s\n",entry->d_name);
}
closedir(dirp);
//printf("\n");





// my_ls -l Auflisten des aktuellen Ordnerinhalts, keine explizite Sortierung gefordert –>alphasort?
// my_ls -a Versteckte Dateien und Ordner mit auflisten 
// my_ls beliebiger/ordner bzw. my_ls beliebige/datei = Auslesen des angegebenen Ordners bzw. der angegebenen Datei



return 0;
}
