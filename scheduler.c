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
		if(runningP->cycles_todo == 0){
			runningP->state = DEAD;
		}
		else {
			runningP->state=READY;	
		}
		readyP->state = RUNNING;	//readyP sollte automatisch erstes sein wegen rückläufiger schleife oben
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
	for(Process* p = head->prev;p != head;p=p->prev){	//rückwärts damit erster READY Prozess aus Schlange genommen wird
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
