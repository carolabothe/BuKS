// BuKS Übung 03
//Carola Bothe


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <dirent.h>

//TODO: Fehlerbehandlung nicht vergessen

int main(int argc, char* argv[]){
	int64_t fdtrash = open(".ti3_trashcan", O_CREAT, S_IRWXU); //ändern sodass ordner und nicht datei erstellt wird?!
	if (argv[1] == "-d"){ //datei in trash verschieben
		if (argc != 3){ write(2, "Please enter a file name");}
		char *filename = argv[2];
		char *targetn = ".ti3_trashcan/" + argv[2];
		copy(filename, targetn);
		//hier fehlt noch was..		
	}		
	if (argv[1] == "-l"){ //alle dateien im trash auflisten
		dirp = opendir(".ti3_trashcan");
		while(dp=readdir(dirp) != NULL){
			//dateien zählen
		}
		closedir(dirp);
	}		
	if (argv[1] == "-r"){}
	if (argv[1] == "-f"){}		
	else{}
	
}
//kopiert Inhalt aus Datei in Zieldatei, nur wenn diese noch nicht existiert
int copy(char *sourcename, char *targetname){
	//inhalt lesen und in data speichern
	int64_t fdsource = open(sourcename, O_RDONLY);
	char data[256];
	read(fdsource, data, 256);
	if (read(fdsource, data, 256)<0){	//leere Datei
		write(2,"The entered file is empty.",256);
		return -1;
	}
	//neue Datei erstellen	

	int64_t fdtarget = open(targetname, O_WRONLY | O_CREAT, S_IRWXU);
	write(fdtarget, data, 256);
	//wenn write nicht klappt:
	if (write(fdtarget, data, 256)<0) {
       		write(2, "There was an error writing on the given file.", 50);
       		return -1;
   		}

return 0;
}
