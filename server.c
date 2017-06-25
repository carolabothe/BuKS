#include<stdio.h> 
#include<string.h>    
#include<sys/socket.h>    
#include<arpa/inet.h> 
#include<netinet/in.h>
#include<netdb.h>
#include<sys/mman.h>
#include<fcntl.h>
#include <dirent.h>
#include<sys/un.h>  
 
int main(int argc , char *argv[]){
    int sock, socket_descriptor , client_sock , c , read_size;
	int64_t socktype;
    struct sockaddr_in server , client;
	char client_message[2017];
	
	if(argc!=3){
    	fprintf(stderr,"Falsche Anzahl an Argumenten.");
    	return 1;
    }
    //Socket erstellen je nach option wie beim client
    if(argv[1][0]=='-'){
		switch(argv[1][1]){
			case 'U': 
				sock = socket(AF_UNIX , SOCK_STREAM , 0);
				server.sin_family = AF_LOCAL;
				socktype = 0;
				break;
			case 'u': 
				sock = socket(AF_INET , SOCK_DGRAM , 0);
				server.sin_family = AF_LOCAL;
				socktype = 1;
				break;
			case 't': 
				sock = socket(AF_INET , SOCK_STREAM , 0);
				server.sin_family = AF_LOCAL;
				socktype = 2;
				break;

			default: 
				fprintf(stderr, "Keine gültige Option.\n");
				return 2;
		}
		if (sock == -1 || sock >0){
			printf(stderr, "Konnte Socket nicht erstellen");
			exit(1);
    }
    printf("Socket wurde erstellt");
     
    //wichtig für die socket Adresse in struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[2]);
    server.sin_port = htons( 5555 );
     
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
    puts("Warte auf Eingabe vom Client...");
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
        write(client_sock , client_message , strlen(client_message));
    }
     
    if(read_size == 0){
        puts("Client rausgeflogen"); //der Client disconnected
        fflush(stdout);
    }
    else if(read_size == -1){
        fprintf(stderr, "übergabe der Nachricht ist fehlgeschlagen");
    }
     
    return 0;
    }
}