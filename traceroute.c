#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT		4242 //beides willkürlich
#define BUFF_SIZE	1024

//Das Programm tut ohne sudo leider gar nichts und hängt sich mit sudo auf(keine Ahnung 
//wieso, es ist ja nirgendwo ne Endlosschleife) ohne nach einem pw zu fragen.
//Da es leider erst Donnerstag Abend fertig geworden ist, wussten wir auch nicht, 
//wie wir es im zedat netz testen sollen. Vielleicht klappt es ja trotzdem,
//kompilieren tut es zumindest ohne warnings :)

int main(int argc, char* argv[]) {

if(argc !=2){
	fprintf(stderr, "Bitte genau ein Argument eingeben, das Ziel in Form einer IP Adresse.\n");
	return 1;
}
// Sender (UDP) Socket initialisieren
int sendsockfd = socket(PF_INET, SOCK_DGRAM, 0);
// Empfänger (RAW) socket
int recvsockfd = socket(PF_INET,SOCK_RAW,IPPROTO_ICMP);

//Adresse des Ziels
struct sockaddr_in target_addr;
target_addr.sin_family = AF_INET; // IPv4
target_addr.sin_addr.s_addr = inet_addr(argv[1]); //gegeben beim Aufruf
target_addr.sin_port = htons((uint16_t)PORT); //gewählter Port, siehe oben


//Schleife
struct sockaddr_in *buf = malloc(BUFF_SIZE);
char *message;
message = "Nachricht für traceroute";
ssize_t res;
char *output = malloc(20);
for(uint16_t hop = 1; hop <=30; hop++){ //höchstens 30 hops (hop = TTL)
	setsockopt(sendsockfd, IPPROTO_IP, IP_TTL, &hop, sizeof(hop));
	sendto(sendsockfd, message, strlen(message), 0,
                      (struct sockaddr*)&target_addr,
				sizeof(&target_addr));
	//
	res = recvfrom(recvsockfd, buf, BUFF_SIZE-1, 0,NULL,NULL); //egal woher die Nachricht kommt; buf ist struct sockaddr_in *
	if(res < 0) {
		shutdown(recvsockfd, SHUT_RDWR);
		return 1;
	}
	unsigned short *givenport = &buf->sin_port;
	if(*givenport == PORT){ //paket an gleichem Port also für uns
		inet_ntop(AF_INET,&buf,output,20); //umwandeln in string
		printf("%i:\t%s\n",hop,output);
		free(output);
		free(givenport);	
	}
	if(inet_ntoa(buf->sin_addr) == argv[1]){ //gleiche IP heißt, haben Ziel erreicht
		printf("Ziel erreicht nach %i hops.\n",hop);		
		break;
	}
}

free(message);
free(buf);
fflush(stdout);
shutdown(sendsockfd, SHUT_RDWR);	
shutdown(recvsockfd, SHUT_RDWR);
return 0;
}





