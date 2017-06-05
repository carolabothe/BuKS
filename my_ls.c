#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char* argv[]){
//Auflisten des aktuellen Ordnerinhalts, keine explizite Sortierung gefordert
//hilfreiche Seite: http://pubs.opengroup.org/onlinepubs/007908775/xsh/readdir.html
dirp = opendir(".");	//öffnen momentanes Verzeichnis
while(dirp){
	readdir(dirp);
}        
closedir(dirp);

//struct dirent *readdir(DIR *dirp);
//int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);





// my_ls -l Auflisten des aktuellen Ordnerinhalts, keine explizite Sortierung gefordert
// my_ls -a Versteckte Dateien und Ordner mit auflisten 
// my_ls beliebiger/ordner bzw. my_ls beliebige/datei = Auslesen des angegebenen Ordners bzw. der angegebenen Datei



return 0;
}