/*ueb01 
Erstellen Sie ein Programm, das <n> Kommandozeilenparameter bekommt, 
die ersten <n-1> Komman- dozeilenparameter addiert und prüft, ob diese Summe 
dem <n>ten Kommandozeilenparameter entspricht. Ist dies der Fall gibt es den 
char-Wert ’1’ aus, sonst den char-Wert ’0’. Führen Sie eine Fehlerbehandlung durch 
und erläutern Sie im Quellcode (als Kommentar) warum Sie sich für genau 
diese Fehlerbehandlung entschieden haben. Beachten Sie den Typ der übergebenen 
Parameter und nutzen Sie die inttypes.h-Bibliothek.
Beispielausgabe:
$ . / program 1 2 3 1 1 8
1
$ . / program 1 2 3 1 1 9
0
Ihr Programm muss mit folgenden Compiler-Flags ohne Warnungen und / oder Fehler kompilieren:
$ gcc −std=c11 −o ueb01.out −Wall −Wextra −pedantic ueb01.c
*/
//TODO: Fehlerbehandlung

#include <stdio.h>
#include <inttypes.h>

int main(int argc, char *argv[]){
	/*int sum = 0;
    for (int i = 0; i < argc-1; ++i){
    	sum += argv[i];
    }
    char c = '';
    if (sum == argv[argc-1]){
    	char c = '1';
    }
    else{
    	char c = '0';
    }
    printf("%c", c);
}
*/
//what we did in the tutorial:
	for(int i=0;i<argc;i++){
		printf("%s\n",argv[i]);
	}
}