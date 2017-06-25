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

//aufruf: $ ./client -u 13.42.39.28:8080 /tolle/datei
// $ ./client -U kingkong /tolle/datei (kingkong ist servername)
// wenn der Server: open funktioniert nicht ->directory auch nicht -> file existiert nicht  
int main(int argc , char *argv[]){
    int sock;
	int64_t socktype;
    struct sockaddr_in server, client;
    char message[2017] , server_reply[2828]; //man kann eine Zahl zwischen 2000 und 6500 wählen ca.
    
    if(argc!=4){
    	fprintf(stderr,"Falsche Anzahl an Argumenten.\n");
    	return 1;
    }
    //Socket erstellen je nach option
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
    if (sock == -1){
        fprintf(stderr, "Konnte Socket nicht erstellen.\n");
    }
    printf("Socket wurde erstellt.\n");
     
    server.sin_addr.s_addr = inet_addr(argv[2]); //nimmt das argv[2]
    server.sin_port = htons( 5555 ); // man kann beliebigen Port wählen
	if(socktype == 0 | (socktype == 2)){ //UNIX 
    	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){	
    		fprintf(stderr, "Connect hat nicht funktioniert.\n");
    	    return 1;
    	}
    	puts("Connected\n");
    }
    else if (socktype == 1){ //datagram socket
			printf("%s: unbekannter Host '%s' \n"); 
			}//sendto 
			
        	//antwort vom server bekommen
        	if( recv(sock , server_reply , 2017 , 0) < 0){
    			fprintf(stderr, "Keine Antwort.");
        	}
			
        	puts("Server reply :");
        	puts(server_reply);	

    
    close(sock); //schließt den socket
    return 0;
	}
}