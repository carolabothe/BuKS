#include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>    
 
//aufruf: $ ./server -u 8080	//oder -t
// ./server -U kingkong

int main(int argc , char *argv[]){
    int socket_descriptor , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2017];
     
    //Socket erstellen je nach option
    if(argv[1][0]=='-'){
		switch(argv[1][1]){
			case 'U': 
				socket_descriptor = socket(AF_UNIX , SOCK_STREAM , 0);
				server.sin_family = AF_UNIX; //in quelle steht dass das immer AF:_INET sein soll?
			case 'u': 
				socket_descriptor = socket(AF_INET , SOCK_DGRAM , 0);
				server.sin_family = AF_INET;

			case 't': 
				socket_descriptor = socket(AF_INET , SOCK_STREAM , 0);
				server.sin_family = AF_INET;

			default: 
				fprintf(stderr, "Keine gültige Option.\n");
				return 2;
		}
    if (socket_descriptor == -1){
        printf("Konnte kein socket erstellt werden");
    }
    puts("Socket erstellt");
     
    //wichtig für die socket Adresse in struc
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 ); //8888 is port number
     
    //Adresse an den Server binden
    if( bind(socket_descriptor,(struct sockaddr *)&server , sizeof(server)) < 0){
        //Schickt die error message an sdtrr
        perror("bind fehlgeschlagen");
        return 1;
    }
    puts("bind geklappt");
     
    //Listen
    listen(socket_descriptor , 3);
     
    //Falls noch keine Eingabe da ist
    puts("Warte auf Eingabe...");
    c = sizeof(struct sockaddr_in);
     
    //accept die Eingabe vom client
    client_sock = accept(socket_descriptor, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0){
        perror("Eingabe hat nicht geklappt");
        return 1;
    }
    puts("Eingabe hat geklappt");
     
    //vom Client die nachricht bekommen:
    // statt read ans write sendto and receivefrom ?
    while( (read_size = recv(client_sock , client_message , 2017 , 0)) > 0 ){		//müssen unterscheiden ob wir link zur datei bekommen oder firekt text von stdin
        //Client die Message zurückschicken (nur ein test für nächste woche)
        write(client_sock , client_message , strlen(client_message));
    }
     
    if(read_size == 0){
        puts("Client rausgeflogen"); //der Client disconnected
        fflush(stdout);
    }
    else if(read_size == -1){
        perror("übergabe der Nachricht ist fehlgeschlagen");
    }
     
    return 0;
}


// Wir hatten Probleme den client wirklich mit dem Server zu verbinden bzw die richtige Adresse zu finden.
// Quelle für die bearbeitung dieser Aufgabe ist http://www.linuxhowtos.org/C_C++/socket.htm