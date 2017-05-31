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

#include "scheduler.h"


//round robin:
void rr(struct Process* head) {

}

//first come first serve: prozesse durchgehen und gucken welcher gerade läuft, wenn einer noch auf running ist dann weiterlaufen lassen sonst nach process mit längster wartezeit suchen und den auf running setzen
void fcfs(struct Process* head) {

}

//shortest process next
void spn(struct Process* head) {

}

//highest response ratio next
void hrrn(struct Process* head) {

}

