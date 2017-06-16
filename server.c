#include<stdio.h>
#include<string.h>    
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>    
 
int main(int argc , char *argv[]){
    int socket_descriptor , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2017];
     
    //Socket erstellen
    socket_descriptor = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_descriptor == -1){
        printf("Konnte kein socket erstellt werden");
    }
    puts("Socket erstellt");
     
    //wichtig für die socket Adresse in struc
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
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
    while( (read_size = recv(client_sock , client_message , 2017 , 0)) > 0 ){
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