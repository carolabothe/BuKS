// BuKS Übung 03
//Carola Bothe

//TODO: Fehlerbehandlung nicht vergessen

//kopiert inhalt aus Datei in Zieldatei, nur wenn diese noch nicht existiert
int copy(char *sourcename, char *targetname){
	//inhalt lesen und in data speichern
	int filedes = open(sourcename);
	char data[256];
	if (read(filedes, data, 256)<0){	//leere Datei
		write(2,"The entered file is empty\n",256);
		return -1;
	}
	//neue Datei erstellen	
    struct stat fileStat;
	if (stat(open(targetname), &fileStat)<0){		//open(targetname) gibt fd, wenn stat failed
		//in neue datei schreiben
		write(open(targetname), data, 256);
		//wenn write nicht klappt:
		if (write(open(targetname), data, 256) {
        	write(2, "There was an error writing on the given file.\n", 44);
        	return -1;
    	}
	}
}