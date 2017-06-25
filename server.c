#include<stdio.h> 
#include<string.h>  
#include<stdlib.h>         
#include<sys/socket.h>    
#include<arpa/inet.h> 
#include<netinet/in.h>
#include<netdb.h>
#include<sys/mman.h>
#include<fcntl.h>
#include <dirent.h>
#include<sys/un.h>  
#include<unistd.h>
 
 //aufruf: $ ./server -u 8080	//oder -t
// ./server -U kingkong
int main(int argc , char *argv[]){
    int sock, socket_descriptor , client_sock , c , read_size;
	int64_t socktype;
    struct sockaddr_in server , client;
	char client_message[2017];
	
	if(argc!=3){
    	fprintf(stderr,"Falsche Anzahl an Argumenten.\n");
    	return 1;
    }
    //Socket erstellen je nach option wie beim client
    if(argv[1][0]=='-'){
		switch(argv[1][1]){
			case 'U': 
				sock = socket(PF_INET , SOCK_STREAM , 0);
				socktype = 0;
				break;
			case 'u': 
				sock = socket(PF_INET , SOCK_DGRAM , 0);
				socktype = 1;
				break;
			case 't': 
				sock = socket(PF_INET , SOCK_STREAM , 0);
				socktype = 2;
				break;

			default: 
				fprintf(stderr, "Keine gültige Option.\n");
				return 2;
		}
		if (sock == -1){
			fprintf(stderr, "Konnte Socket nicht erstellen\n");
			exit(1);
		}
    }
    printf("Socket wurde erstellt\n");
     
    //wichtig für die socket Adresse in struct
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr(argv[2]); geht nicht
    server.sin_port = htons( 5555 ); //legen wir fest
     
    //Adresse an den Server binden
    if( bind(sock,(struct sockaddr *)&server , sizeof(server)) < 0){
        //Schickt die error message an sdtrr
        perror("bind fehlgeschlagen");
        return 1;
    }
    puts("bind geklappt");

    //Listen
    listen(sock, 3);
    while(1){
    if((socktype==0)|(socktype==2)){ //tcp Fall
    	//Falls noch keine Eingabe da ist
    	puts("Warte auf Eingabe vom Client...");
    	socklen_t c = sizeof(struct sockaddr_in);
     
    	//accept die Eingabe vom client
    	client_sock = accept(sock, (struct sockaddr *)&client, (socklen_t*)&c);
    	if (client_sock < 0){
    	    perror("Eingabe hat nicht geklappt\n");
    	    return 1;
    	}
    	puts("Eingabe hat geklappt");
     
    	while( (read_size = recv(client_sock , client_message , 2017 , 0)) > 0 ){		
			FILE * file;
			file = fopen(client_message , "r+"); //Öffnen Dateipfad, den uns client schickt
			char* idc = malloc(1000);		
			char* content = fgets(idc,1024,file); //1024 ist einfach mal das limit der stringlänge	
			write(client_sock , content , strlen(content));
			free(idc);
     	}
    	if(read_size == 0){
    	    puts("Client rausgeflogen"); //der Client disconnected
    	    fflush(stdout);
    	}
    	else if(read_size == -1){
    	    fprintf(stderr, "übergabe der Nachricht ist fehlgeschlagen\n");
    	}
    }
    else if(socktype==1){ //udp
    	puts("Warte auf Eingabe vom Client...");
    	socklen_t c = sizeof(struct sockaddr_in);
		
		char addr_str[INET_ADDRSTRLEN] = "";
		ssize_t read_size = 0;
		while((read_size = recvfrom(sock, client_message,1000,0,(struct sockaddr*)&client,&c))>0){						 
			FILE * file;
			file = fopen(client_message , "r+"); //Öffnen Dateipfad, den uns client schickt
			char* idc = malloc(1000);		
			char* content = fgets(idc,1024,file); //1024 ist einfach mal das limit der stringlänge	
			sendto(sock, content, read_size, 0,(struct sockaddr*)&client,client_sock);
			free(idc);
		}
	    if(read_size == 0){
    	    puts("Client rausgeflogen"); //der Client disconnected
    	    fflush(stdout);
    	}
    	else if(read_size == -1){
    	    fprintf(stderr, "übergabe der Nachricht ist fehlgeschlagen\n");
    	}
	}
	}	
    
    shutdown(sock,2); //schließt den socket
    shutdown(client_sock,2); //schließt den socket
    return 0;

}