/*
 * Autor: Thomas Tegethoff
 * Idee & Vorlage: Heiko Will
 * Hinweise:
 *  - die Prozessliste ist doppelt verkettet, d.h. jedes Element kennt seinen
 *    Vorgänger und Nachfolger
 *  - das erste Element / der Kopf (head) der Prozessliste ist ein Dummy, d.h. es
 *    enthält keine Daten
 *  - die Prozessliste ist im Kreis verkettet, d.h. das erste Element (head)
 *    zeigt auf das zweite usw, und das letzte Element zeigt wieder auf das
 *    erste (head)
 *  - wenn kein Prozess der bereit ist gefunden werden kann, soll nichts gemacht werden
 *  - die Funktionen sollen den state des Prozesses, der als nächstes laufen
 *    soll, zu RUNNING ändern
 *  - der state des Prozesses der bisher gelaufen ist soll (abhängig von
 *    Scheduling-Verfahren) auch angepasst werden, entweder zu READY wenn 
 *    cycles_todo ungleich 0 ist bzw. zu DEAD wenn cycles_todo gleich 0 ist
 *  - Sie sollen keine anderen Werte der Prozesse verändern!
 */

#include <stdlib.h>
#include <stdio.h>
#include "scheduler.h"

//Bearbeiter: Etienne Jentzsch, Carola Bothe

//round robin:
void rr(struct Process* head) {
	uint64_t readycounter = 0;
	uint64_t runcounter = 0;
	Process* readyP = head;
	Process* runningP = head;
	//bestimmen wie viele programme ready sind und welche und welches läuft, wenn eins läuft
	for(Process* p = head->prev;p != head;p=p->prev){	//rückwärts damit erster READY Prozess aus Schlange genommen wird
		if(p->state ==READY){
			readycounter ++;
			readyP = p; 	
		}
		if(p->state == RUNNING){
			runcounter ++;
			runningP = p;
		}
	}
	//setzen Status
	if(runcounter>1){fprintf(stderr,"Error: Mehrere Programme laufen gleichzeitig!");}
	if(readycounter == 1){ //nur ein READY Prozess in Schlange
		if(runcounter==0){
			readyP->state = RUNNING;
		}
		else{
			if(runningP->cycles_todo == 0){
				runningP->state = DEAD;
			}
			else{
				runningP->state=READY;
				readyP->state = RUNNING;	
			}
			readyP->state = RUNNING;
		}
	}
	else if(readycounter == 0){	//Programm soll weiter laufen, wenn es noch Zyklen übrig hat
		if(runningP->cycles_todo == 0){
			runningP->state = DEAD;
		}
		else {
			runningP->state=RUNNING; //brauchen wir eig nicht da schon der Fall...		
		}
	}
	else{ //mehrere wartende READY Programme		
		//setzen status vom vorher laufenden Programm
		if(runningP->cycles_todo == 0){
			runningP->state = DEAD;
		}
		else {
			runningP->state=READY;	
		}
		//finden nächstes READY Programm
		for(Process* p = runningP->next; p != runningP; p=p->next){
			if (p->state==READY){
				p->state = RUNNING;
				break;
			}
		}
	}
}

//first come first serve: prozesse durchgehen und gucken welcher gerade läuft, wenn einer noch auf running ist dann weiterlaufen lassen sonst nach process mit längster wartezeit suchen und den auf running setzen
void fcfs(struct Process* head){
	Process* current = head;
	while(current->next != head){
		if(current->next->cycles_todo != 0){
			current->next->state = RUNNING;
			current = head->prev;
		}
		else{
		current->next->state = DEAD;
		current = current->next;	
		}
	}
}

//shortest process next
void spn(struct Process* head) {
	uint64_t readycounter = 0;
	uint64_t runcounter = 0;
	Process* runningP = head;
	uint64_t sertime = 10000; //brauchen großen Wert...
	struct Process* shortestP = head;
	//bestimmen wie viele programme ready sind und welches läuft, wenn eins läuft
	for(Process* p = head->prev;p != head;p=p->prev){	//rückwärts damit erster READY Prozess aus Schlange genommen wird
		if(p->state ==READY){
			readycounter ++;
			if(p->cycles_todo <= sertime){
				sertime = p->cycles_todo;
				shortestP = p;
			}	
		}
		if(p->state == RUNNING){
			runcounter ++;
			runningP = p;
		}
	}
	if(runcounter==0){
		if(readycounter!=0){//wenigstens ein READY Prozess
			shortestP->state = RUNNING;
		}
	}
	else if (runcounter == 1){
		if(runningP->cycles_todo == 0){
			runningP->state = DEAD;
			if(readycounter!=0){
				shortestP->state = RUNNING;
			}
		}
		else{
			runningP->state=RUNNING; //nur für Klarheit, ist ja schon RUNNING eigentlich
		}
	}
	else{
		fprintf(stderr,"Fehler: Zwei Prozesse laufen gleichzeitig."); //sollte nicht passieren...
	}

}


//highest response ratio next
void hrrn(struct Process* head){
	uint64_t readycounter = 0;
	uint64_t runcounter = 0;
	Process* runningP = head;
	uint64_t hrrn = 0;
	struct Process* hrrnP = head;
	//bestimmen wie viele programme ready sind und welches läuft, wenn eins läuft
	for(Process* p = head->next;p != head;p=p->next){	
		if(p->state ==READY){
			readycounter ++;
			uint64_t currenthrrn = (p->cycles_waited + p->cycles_todo) / p->cycles_todo;
			if(currenthrrn > hrrn){
				hrrn = currenthrrn;
				hrrnP = p;
			}	
		}
		if(p->state == RUNNING){
			runcounter ++;
			runningP = p;
		}
	}
	if(runcounter==0){
		if(readycounter!=0){//wenigstens ein READY Prozess
			hrrnP->state = RUNNING;
		}
	}
	else if (runcounter == 1){
		if(runningP->cycles_todo == 0){
			runningP->state = DEAD;
			if(readycounter!=0){
				hrrnP->state = RUNNING;
			}
		}
		else{
			runningP->state=RUNNING; //nur für Klarheit, ist ja schon RUNNING eigentlich
		}
	}
	else{
		fprintf(stderr,"Fehler: Zwei Prozesse laufen gleichzeitig."); //sollte nicht passieren...
	}
}

/* b) Testfälle
{{0, 2}, {0, 2}, {0, 2}, {0, 2}}
Illustriert schön, wie rr einmal jeden Prozess laufen lässt und dann zum nächsten geht. 
fcfs, spn und hrrn machen alle das selbe und zwar nacheinander alle Prozesse zu ende laufen lassen, 
da die Prozesse alle gleich lang sind und alle zum gleichen Zeitpunkt kommen und daher auch alle gleich lange warten.
{{0, 3}, {2, 6}, {4, 4}, {6, 5}, {8, 2}}
zeigt den Unterschied zwischen spn und hrrn. Bei Tick 9 wird bei spn zuerst der 5. Prozess ausgewählt da er nur
2 Zyklen lang ist und nicht 4 wie Prozess 3 (ID 2). Hingegen wählt hrrn Prozess 3, da sein hrrn = 6+4/4 = 2.5 und 
damit größer ist als hrrn(p5) = 1+2/2 = 1.5 .Dies zeigt, dass längere Prozesse bei hrrn weniger benachteiligt werden.
fcfs geht wieder Stumpf alle Prozesse der Reihe nach durch, wodurch der 5. kurze Prozesse 10 Ticks warten muss.
{{0, 8}, {1, 1}, {2, 4}, {6, 2}}
Zeigt nochmal schön die Unterschiede:
Algo	pID		kommt bei 	Fertig bei 	#Wartezyklen
rr		0		0			15			7
		1		1			2			0
		2		2			10			4
		3		6			11			3
								gesamt:	14
								
fcfs	0		0			8			0
		1		1			9			7
		2		2			13			7
		3		6			15			7
								gesamt:	21

spn		0		0			8			0
		1		1			9			7
		2		2			15			9
		3		6			11			3
								gesamt:	19

hrrn	0		0			8			0
		1		1			9			7
		2		2			13			7
		3		6			15			7
								gesamt: 21
*/
