#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char* argv[]){
//auswählen der Option



//Auflisten des aktuellen Ordnerinhalts, keine explizite Sortierung gefordert
//hilfreiche Seite: http://pubs.opengroup.org/onlinepubs/007908775/xsh/readdir.html
DIR* dirp = opendir(".");	//öffnen momentanes Verzeichnis
if(dirp == NULL){printf("Fehler beim Öffnen.");}
struct dirent* entry = readdir(dirp);
while((entry = readdir(dirp))){ 
	printf("%c\t",entry->d_name[FILENAME_MAX]); ////FILENAME_MAX ist in stdio definiert
}
closedir(dirp);
//printf("\n");





// my_ls -l Auflisten des aktuellen Ordnerinhalts, keine explizite Sortierung gefordert –>alphasort?
// my_ls -a Versteckte Dateien und Ordner mit auflisten 
// my_ls beliebiger/ordner bzw. my_ls beliebige/datei = Auslesen des angegebenen Ordners bzw. der angegebenen Datei



return 0;
}
