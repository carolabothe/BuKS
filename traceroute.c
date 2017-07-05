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
const void *package;
//empfänger spcket öffnen
int recvsockfd = socket(PF_INET,SOCK_RAW,IPPROTO_ICMP);

struct sockaddr_in client_addr;
client_addr.sin_addr = htonl(argv[1]);
socklen_t client_addr_size = sizeof(struct sockaddr_in);
//Schleife
char buf[BUFF_SIZE];
for(uint16_t hop = 1; hop <=30; hop++){
	//paket senden mit richtiger ttl=hop
sendto(sendsockfd, package, sizeof(package), 0,
                      (struct sockaddr*)&client_addr,
				&client_addr_size);
	//richtige icmp von empfänger socket herausfinden und ausgeben
recvfrom(recvsockfd, buf, BUFF_SIZE, 0,
                      (struct sockaddr*)&client_addr,
				&client_addr_size);
char* output;
inet_ntop(AF_INET,client_addr.sin_addr,output,20); //umwandeln in string
printf("%i:\t%s\n",hop,output);

	
}
freeaddrinfo(servinfo);
shutdown(sendsockfd, SHUT_RDWR);	
shutdown(recvsockfd, SHUT_RDWR);
return 0;
}



