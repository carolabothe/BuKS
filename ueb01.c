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


#include <stdio.h>
#include <inttypes.h>

int main(int argc, char* argv[]){
	int sum = 0;
    for (int i = 1; i < argc-1; ++i){	//not i=0 because that would be the program itself
    	sum += (int) strtol(argv[i], (char **)NULL, 10);

    }
    char c;
    if (sum == (int) strtol(argv[argc-1], (char **)NULL, 10)){
    	c = '1';
    }
    else{
    	c = '0';
    }
    printf("%c\n",c);
}
