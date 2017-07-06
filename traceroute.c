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
//#include <ip.h>

#define PORT		4242 //beides willkürlich
#define BUFF_SIZE	1024

//TODO: package erstellen mit richtigem ip und udp header - include von struct ip_hdr?
// - chaos mit sender/empfänger adressen beseitigen

int main(int argc, char* argv[]) {

if(argc !=2){
	fprintf(stderr, "Bitte genau ein Argument, das Ziel, eingeben.\n");
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
struct sockaddr_in *buf[BUFF_SIZE];
char *message;
message = "Nachricht für traceroute";
ssize_t res;
char *output = malloc(20);
for(uint16_t hop = 1; hop <=30; hop++){
	setsockopt(sendsockfd, IPPROTO_IP, IP_TTL, &hop, sizeof(hop));
	sendto(sendsockfd, message, strlen(message), 0,
                      (struct sockaddr*)&target_addr,
				sizeof(&target_addr));
	//richtige icmp von empfänger socket herausfinden und ausgeben
	
	do {
		res = recvfrom(recvsockfd, buf, BUFF_SIZE-1, 0,NULL,NULL); //egal woher die Nachricht kommt
		if(res < 0) {
			shutdown(recvsockfd, SHUT_RDWR);
			return 1;
		}
		//text_buff[res] = 0; //brauchen wir nen nullterminator?
	} while((size_t)res == BUFF_SIZE-1);

	inet_ntop(AF_INET,&buf,output,20); //umwandeln in string (**)
	printf("%i:\t%s\n",hop,output);
	free(output);	
}
//freeaddrinfo(servinfo);
free(message);
shutdown(sendsockfd, SHUT_RDWR);	
shutdown(recvsockfd, SHUT_RDWR);
return 0;
}
/*
**
       const char *inet_ntop(int af, const void *src,
                             char *dst, socklen_t size);

DESCRIPTION
       This function converts the network address structure src in  the
       af address family into a character string.  The resulting string
       is copied to the buffer pointed to by dst, which must be a  non-
       null  pointer.   The caller specifies the number of bytes avail‐
       able in this buffer in the argument size.
*/


/* raw socket approach chaos:
//adress info des empfängers
const struct addrinfo *hints;
struct addrinfo *destinfo;
int getaddrinfo(argv[1],     // e.g. "www.example.com" or IP
                (char*)(PORT),  // noch htons((uint16_t)?, e.g. "http" or port number
		&hints,
                &targetinfo);
//raw socket zum senden öffnen
int sendsockfd = socket(PF_INET, SOCK_RAW, 0);
//paket machen und ausfüllen (instanz von struct IP-hdr)
struct ip_hdr packageinfo;	//in ip.h
const void *message = "Dies ist eine traceroute Testnachricht.";

struct sockaddr_in client_addr;
client_addr.sin_addr = htonl(argv[1]);
socklen_t client_addr_size = sizeof(struct sockaddr_in);
*/

