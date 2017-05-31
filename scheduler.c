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
	Process* current = head->next;
	int64_t counter = 0;
	uint64_t sertime = head->cycles_todo;
	struct Process* jetztdran = head;
	while(current != head){
		if(current->state == RUNNING){
			current->cycles_todo --;
			current->cycles_done ++;
			counter++;
			break;
		}
		if(counter>1){
			fprintf(stderr,"Fehler: Zwei Prozesse laufen gleichzeitig."); //geht das überhaupt? 
		}
		if(current->cycles_todo < sertime){
			sertime = current->cycles_todo;
			jetztdran = current;
		}
		current = current->next;	
	}	
	if(counter==0){
		jetztdran->state = RUNNING;
	}
}


//highest response ratio next
void hrrn(struct Process* head){
	Process* current = head->next;
	int64_t counter = 0;
	uint64_t hrrn = 0; 
	struct Process* jetztdran = head;
	while(current != head){
		/*if(current->state == RUNNING){
			current->cycles_todo --;
			current->cycles_done ++;
			counter++;
			break;
		}
		if(counter>1){
			fprintf(stderr,"Fehler: Zwei Prozesse laufen gleichzeitig."); //geht das überhaupt? 
		}*/
		if(current->cycles_todo == 0){
			current = current->next;
			break;				
		}
		else{
			uint64_t currenthrrn = (current->cycles_waited + current->cycles_todo) / current->cycles_todo;
			if(currenthrrn > hrrn){
				hrrn = currenthrrn;
				jetztdran = current;
			}
		}	
		current = current->next;	
	}	
	if(counter==0){
		jetztdran->state = RUNNING;
	}
}
