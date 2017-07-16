#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <inttypes.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h> // etwas zu viel header aber umso besser 

char* buff;
char webseite[] = 
"HTTP/1.0 200 OK\r\n" // für den <status>
"Content-Type: text/html\r\n" //<MIME>
"Connection: close\r\n"
"Content-Length: 15000\r\n\r\n"; // <LEN> Die Zahl wussten wir nicht richtig anzugeben...

char error[] =
	"HTTP/1.0 404 Not Found\r\n"
	"Content-Type: text/html\r\n"
	"Connection: close\r\n"
	"Content-Length: 200\r\n\r\n" // welche zahl muss man hier genau eintragen ?
	"<html><body><h1>Ungültige Anweisung</h1></body></html>\r\n\r\n";
	
int main(void){
	struct sockaddr_in server_addr, client_addr;
	socklen_t sin_len = sizeof(client_addr);
	int fd_server, fd_client;
	char buf[2048];
	char buff[100];
	int fding;
	int token = 1;
	
	fd_server = socket(AF_INET, SOCK_STREAM, 0);
	if(fd_server < 0){
		perror("auf den socket achten.\n");
		exit(1);
	}
	
	setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, &token, sizeof(int));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080);
	
	if(bind(fd_server, (struct sockaddr *)&server_addr, sizeof(server_addr))== -1){ // den server mit der adresse verknüpfen
		perror("auf bind achten.\n"); 
		close(fd_server);
		exit(1);
	}
	
	if(listen(fd_server, 10)){ // wartet auf die Verbindung hier
		perror("auf listen achten!\n");
		exit(1);
	}
	while(1){
		fd_client = accept(fd_server, (struct sockaddr*)&client_addr, &sin_len);
		if(fd_client == -1)
		{
			perror("Client konnte sich nicht verbinden!\n");
			break;
		}
		printf("Client verbunden!\n");
	
		if(!fork()){
			close(fd_server); // der Kindprozess 
			memset(buf, 0, 2048);
			memset(buff, 0, 100);
			
			read(fd_client, buf, 2047);
			
			int space = 0;
			printf("%s\n", buf);
			
			while(buf[space+5] != ' '){
					buff[space] = buf[space+5]; // name wird in den neuen buff gesteckt
					space++;
			}
			
			buff[space] = '\0';	// markieren hiermit das ende		
			printf("%s\n", buff);
			
			FILE* test = fopen(buff, "r");
			if(test == NULL){
				write(fd_client, error, sizeof(error)-1);
				fclose(test);
				close(fd_client);
				exit(0);
			}		
			
			fseek(test, 0, SEEK_END);
			int size = ftell(test); // andernfalls die information über die größe nehmen
			rewind(test);
			fclose(test);
			write(fd_client, webseite, sizeof(webseite)-1);
			fding = open(buff, O_RDONLY);
			sendfile(fd_client, fding, NULL, size);
			
			close(fding); // schließen die File-Diskriptoren
				
			
			close(fd_client); // schließt den client
			exit(0);
		}
		close(fd_client);
	}
	return 0;
}

// Wir wussten nicht wirklich die Länge des zurückgelieferten Contents anzugeben leider 
// Unsere Quellen waren: deine echo server / https://www.youtube.com/watch?v=Q1bHO4VbUck ein tutorial zur veranschaulichung und dein Tutorium