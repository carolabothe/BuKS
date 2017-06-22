#include<stdio.h> 
#include<string.h>    
#include<sys/socket.h>    
#include<arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h>
#include<sys/mman.h>
#include<fcntl.h>

//aufruf: $ ./client -u 13.42.39.28:8080 /tolle/datei
// $ ./client -U kingkong /tolle/datei (kingkong ist servername)
int main(int argc , char *argv[]){
    int sock;
    struct sockaddr_in server;
    char message[2017] , server_reply[2828]; //man kann eine Zahl zwischen 2000 und 6500 wählen ca.
    
    if(argc!=3){
    	fprintf(stderr,"Falsche Anzahl an Argumenten.");
    	return 1;
    }
    //Socket erstellen je nach option
    if(argv[1][0]=='-'){
		switch(argv[1][1]){
			case 'U': 
				sock = socket(AF_UNIX , SOCK_STREAM , 0);
				server.sin_family = AF_UNIX;
				int64_t socktype = 0;
			case 'u': 
				sock = socket(AF_INET , SOCK_DGRAM , 0);
				server.sin_family = AF_INET;
				int64_t socktype = 1;
			case 't': 
				sock = socket(AF_INET , SOCK_STREAM , 0);
				server.sin_family = AF_INET;
				int64_t socktype = 2;

			default: 
				fprintf(stderr, "Keine gültige Option.\n");
				return 2;
		}
    if (sock == -1){
        printf("Konnte Socket nicht erstellen");
    }
    puts("Socket wurde erstellt");
     
    server.sin_addr.s_addr = INADDR_ANY; //Standardadresse argv[2]
    server.sin_port = htons( 8888 ); // man kann beliebigen Port wählen
	if((socktype == 0) | (socktype == 2)){ //TCP also stream sockets
    	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){	
    			fprintf(stderr, "Connect hat nicht funktioniert.");
    	    return 1;
    	}
    	puts("Connected\n");
    		while(1){//kommunizieren mit dem server
        		printf("Eingabe für den Server : ");
        		scanf("%s" , message);
        		if( send(sock , message , strlen(message) , 0) < 0){ //senden..
    				fprintf(stderr, "Senden fehlgeschlagen.");
        		    return 1;
        		}
        	 
        		//antwort vom server bekommen
        		if( recv(sock , server_reply , 2017 , 0) < 0){
        		    puts("keine antwort");
        		    break;
        		}
         
        		puts("Server reply :");
        		puts(server_reply);
    		}
    	}
    	else if(argc == 3){
    		if(send(sock, argv[3], strlen(argv[3]), 0)<0){
    			fprintf(stderr, "Senden fehlgeschlagen.");
    		}
    	else{
    		fprintf(stderr, "Falsche Anzahl an Argumenten);
    	}
    }
    else if (socktype == 1){ //datagram socket
    	while(1){//kommunizieren mit dem server
        	printf("Eingabe für den Server : ");
			sendto        	   
        	 
        	//antwort vom server bekommen
        	if( recv(sock , server_reply , 2017 , 0) < 0){
    			fprintf(stderr, "Keine Antwort.");
        	    break;
        	}
         
        	puts("Server reply :");
        	puts(server_reply);
    	}	
    }
    close(sock); //schließt den socket
    return 0;
}