#include<stdio.h> 
#include<string.h>    
#include<sys/socket.h>    
#include<arpa/inet.h> 
#include <netinet/in.h>
#include <netdb.h>
#include<sys/mman.h>
#include<fcntl.h>
 
int main(int argc , char *argv[]){
    int sock;
    struct sockaddr_in server;
    char message[2017] , server_reply[2828]; //man kann eine Zahl zwischen 2000 und 6500 wählen ca.
     
    sock = socket(AF_INET , SOCK_STREAM , 0);//Socket erstellen
    if (sock == -1){
        printf("Konnte Socket nicht erstellen");
    }
    puts("Socket wurde erstellt");
     
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); //Standardadresse
    server.sin_family = AF_INET; //muss das immer af_inet sein ??
    server.sin_port = htons( 8888 ); // man kann beliebigen Port wählen
	
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0){
        perror("connect hat nicht funktioniert");
        return 1;
    }
     
    puts("Connected\n");
    while(1){//kommunizieren mit dem server
        printf("Eingabe für den Server : ");
        scanf("%s" , message);
        if( send(sock , message , strlen(message) , 0) < 0){ //senden..
            puts("senden fehlgeschlagen");
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
     
    close(sock); //schließt den socket
    return 0;
}