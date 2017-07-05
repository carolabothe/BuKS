/*
 * Author: Thomas Tegethoff
 */

/*
 * Damit sich ein Client an einen Server verbinden kann, muss er zwei Dinge
 * wissen:
 * 1. die IP-Adresse des Geräts auf dem der Server läuft (IPv4: 32bit Integer)
 * 2. den Port unter dem der Server auf dem Zielgerät läuft (16bit Integer)
 * Die IP-Adresse dient also dazu ein Gerät im Netzwerk zu identifizieren,
 * während der Port dazu eine Anwendung auf dem Gerät zu indentifizieren. Evtl.
 * will ja mehr als eine Anwendung auf dem Gerät Ding im Internet tun.
 *
 * Daraus folgt, dass die IP-Adresse immer durch den User angegeben werden muss!
 * (und sei es als eine URL die durch ein DNS-Request aufgelöst wird)
 * 
 * Für den Port gibt es zwei Moeglichkeiten:
 * Die erste ist, das der Port festgelegt ist. So ist z.B. der Port für SSH,
 * standardmäßig 22 und der für Http-Webserver 80
 * Die andere Möglichkeit ist, dass der User den Port angibt. Das erfolgt
 * meistens in der Form "addr:port", also z.B. "34.188.82.11:8080".
 * 
 * Dieses Bsp benutzt einen statischen Port, das TCP Bsp. erwartet das ein Port
 * vom User eingegeben wird
 */

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT			8888
#define BUFF_SIZE_NUM	1024

const size_t BUFF_SIZE = BUFF_SIZE_NUM;

/*
 * @description: wenn res kleiner 0 ist, wird eine Fehlermeldung ausgegeben und
 * das Programm beendet
 * @param{int64_t res}: Rückgabewert der Funktion der überprüft wird
 * @param{char* func}: Name der Funktion deren Rückgabewert überprüft wird
 */
static inline void funcErrorHandler(int64_t res, char* func) {
	if(res < 0) {
		fprintf(stderr, "Error in \"%s\": \"%s\"!\n", func, strerror(errno));
		exit(EXIT_FAILURE);
	}
}

/*
 * ./client <addr>
 */
int main(int argc, char* argv[]) {
	if(argc < 2) {
		fputs("Not enough Arguments!\n", stdout);
		printf("Usage: ./client <addr:port>\n");
		return EXIT_FAILURE;
	}

	// struct der die Addressinformationen über den Server enthält
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	
	// IP-Adresse des Servers von str zu int wandeln und in
	// Adress struct schreiben
	int itest = inet_pton(AF_INET, argv[1], &(server_addr.sin_addr));
	funcErrorHandler(itest, "inet_pton");

	// die funktion htons sortiert die Bits der variable port so um, dass sie
	// für den Transportlayer korrekt sind
	server_addr.sin_port = htons((uint16_t)PORT);
	
	// socket erstellen
	int ss = socket(AF_INET, SOCK_DGRAM, 0);
	funcErrorHandler(ss, "socket");

	printf("Socket ready.\n");
	
	char text_buff[BUFF_SIZE];
	ssize_t res;
	// Einlesen und versenden der Eingabe
	do {
		char* ctest = fgets(text_buff, BUFF_SIZE, stdin);
		// mal wieder ein Fehler test
		if(!ctest) {
			shutdown(ss, SHUT_RDWR);
		}
		funcErrorHandler(((int64_t)ctest)-1, "fgets");
		
		res = sendto(
			ss,
			text_buff,
			strlen(text_buff),
			0,
			(struct sockaddr*)&server_addr,
			sizeof(struct sockaddr_in)
		);
		// gerade bei Netzwerk-Funktionen sollte man immer auf Fehler testen
		if(res < 0) {
			shutdown(ss, SHUT_RDWR);
		}
		funcErrorHandler(res, "send");
	// -1 um den Nullterminator zu beachten
	} while(strlen(text_buff) == BUFF_SIZE-1);
	
	// im 5. und 6. Argument können Informationen über den Absender
	// gespeichert werden, den kennen wir in diesem Fall aber schon
	// (der server)
	do {
		res = recvfrom(ss, text_buff, BUFF_SIZE-1, 0, NULL, NULL);
		if(res < 0) {
			// wird nicht von funcErrorHandler gemacht
			shutdown(ss, SHUT_RDWR);
		}
		funcErrorHandler(res, "recv");
	
		text_buff[res] = 0;
		printf("%s", text_buff);
	// wenn die Länge der Nachricht exakt ein vielfaches BUFF_SIZE ist, bleibt
	// der Client hängen, weil es keine Möglichkeit git dann festzustellen ob
	// fertig gesendet wurde. UDP ist halt Mist.
	} while((size_t)res == BUFF_SIZE-1);
		
	// immer schön auf Fehler testen
	fflush(stdout); // fals kein Zeilenumbruch gesendet wurde
	printf("\nfinisched\n");
	
	shutdown(ss, SHUT_RDWR);
	return EXIT_SUCCESS;
}

