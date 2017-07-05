/*
 * author: Thomas Tegethoff
 */

/*
 * Der Server muss wissen auf welchem Port er auf eingehende Verbindungen
 * warten soll, seine IP-Adresse ist die, des Geräts auf dem der Server läuft.
 *
 * Für die Festlegung dieses Ports gibt es zwei Möglichkeiten, entweder
 * impliziet durch den Server selber (Http-Webserver laufen z.B. immer auf Port
 * 80 oder SSH-Server standardmäßig auf Port 22) oder impliziet durch den User.
 * 
 * Dieses Bsp. geht von einer Festlegung durch den User aus.
 * Das UDP Bsp. arbeitet mit einem statischen Port
 */

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

#define PORT			8888
#define BUFF_SIZE_NUM	1024

const size_t BUFF_SIZE = BUFF_SIZE_NUM;

int ss = 0;

void sigHandler(int signal) {
	(void)signal;
	printf("\nterminating\n");
	shutdown(ss, SHUT_RDWR);
	exit(EXIT_SUCCESS);
}

/*
 * @description: wenn res kleiner 0 ist, wird eine Fehlermeldung ausgegeben und
 * das Programm beendet
 * @param{int64_t res}: Rückgabewert der Funktion der überprüft wird
 * @param{char* func}: Name der Funktion deren Rückgabewert überprüft wird
 */
static inline void funcErrorHandler(int64_t res, char* func) {
	if(res < 0) {
		fprintf(stderr, "Error in \"%s\": \"%s\"!\n", func, strerror(errno));
		if(ss) {
			shutdown(ss, SHUT_RDWR);
		}
		exit(EXIT_FAILURE);
	}
}

/*
 * ./server
 */
int main(void) {
	// Socket initialisieren
	ss = socket(AF_INET, SOCK_DGRAM, 0);
	funcErrorHandler(ss, "socket");
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; // IPv4
	// mit dem Macro INADDR_ANY sagen wir dem server er soll an alle Endpunkten
	// die das Gerät hat auf eingehende Verbindungen warten 
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// die funktion htons sortiert die Bits der variable port so um, dass sie
	// für den Transportlayer korrekt sind
	server_addr.sin_port = htons((uint16_t)PORT);

	// das server socket wird auf die Adresse des Geräts und den Port vom User
	// gebunden
	int itest = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	funcErrorHandler(itest, "bind");
	
	while(true) {
		struct sockaddr_in client_addr;
		socklen_t client_addr_size = sizeof(struct sockaddr_in);
		
		char addr_str[INET_ADDRSTRLEN] = "";
		char buffer[BUFF_SIZE];
		ssize_t res = 0;
		do {
			res = recvfrom(
				ss,
				buffer,
				BUFF_SIZE,
				0,
				(struct sockaddr*)&client_addr,
				&client_addr_size
			);
			funcErrorHandler(res, "recv");
			
			if(res == 0) { // 0 bedeutet die Verbindung wurde beendet
				break;
			}
			
			// die Bits des Ports werden von Netzwerk-Reihenfolge in
			// Computer-Reihenfolge gebracht 
    		uint16_t client_port = ntohs(client_addr.sin_port); //ntohs network to host short
    		// IP aus Client-Infos ausgelese, in String gewandelt und ausgegeben
    		const char* ctest = inet_ntop(
    			AF_INET,
    			&client_addr.sin_addr,
    			addr_str,
    			INET_ADDRSTRLEN //wichtiger parameter, da sonst buffer overflows produziert werden können
    		);
    		// immer schön auf Fehler testen
    		funcErrorHandler(((int64_t)ctest)-1, "inet_ntop");
			printf("Msg from %s:%u.\n", addr_str, client_port);
			
			// zurück senden (und schön auf Fehler testen)
			res = sendto(
				ss,
				buffer,
				res,
				0,
				(struct sockaddr*)&client_addr,
				client_addr_size		
			);
			funcErrorHandler(res, "send");
		} while((size_t)res == BUFF_SIZE);
		
	}
	
	// unreacheble
	shutdown(ss, SHUT_RDWR);
	return EXIT_SUCCESS;
}

