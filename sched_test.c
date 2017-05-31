/*
 * Autor: Thomas Tegethoff
 * Idee & Vorlage: Heiko Will
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "scheduler.h"

#define NUM_PROCESSES	4
#define ALGO_NUM		4

#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define MAGENTA	"\033[35m"
#define CYAN	"\033[36m"
#define NORMAL	"\033[39m"

//LIste von Prozessen mit arrival und service time:
uint64_t processes[NUM_PROCESSES][2] = {{0, 3}, {2, 7}, {4, 1}, {6, 5}};

void (*sched_func[ALGO_NUM])(Process*) = {
	rr,
	fcfs,
	spn,
	hrrn
};

char* algo_names[ALGO_NUM] = {
	"Round Robin",
	"First Come First Serve",
	"Shortest Process Next",
	"Highest Response Ratio Next"
};

int main(void) {

	printf(CYAN "ID (done, todo, waited)" NORMAL "\n\n");

	for(size_t ai=0; ai<ALGO_NUM; ai++) {
		Process* head = malloc(sizeof(Process));
		head->pID = 0;
		head->cycles_done = head->cycles_todo = head->cycles_waited = 0;
		head->next = head->prev = head;
		head->state = DEAD;
		
		printf(CYAN "%s: " NORMAL "\n", algo_names[ai]);
		
		size_t tick = 0;
		size_t ticks_todo = 0;
		do {
			// evtl neue Prozess einfuegen
			for(size_t pi=0; pi<NUM_PROCESSES; pi++) {
				if(processes[pi][0] == tick) {
					Process* new_process = malloc(sizeof(Process));
					new_process->pID = pi; //weisen neuem prozess Prozessnummer als pID zu
					
					new_process->cycles_done = new_process->cycles_waited = 0;
					new_process->cycles_todo = processes[pi][1];
					new_process->state = READY;
					
					new_process->prev = head->prev;
					new_process->next = head; //letzter prozess zeigt immer auf anfang
					head->prev->next = new_process; //next vom vorgänger von head zeigt auf neuen prozess??
					head->prev = new_process;
					
					ticks_todo += processes[pi][1];
				}
			}
			
			sched_func[ai](head);
			
			printf(CYAN "Tick %zu:%s" NORMAL, tick, (tick<10)?"  ":" ");
			
			Process* current = head->next;
			while(current != head) {
				if(current->state == RUNNING) {
					current->cycles_done++;
					current->cycles_todo--;
				} else if(current->state == READY) {
					current->cycles_waited++;
				}
			
				printf(
					MAGENTA "%"PRIu64" (%"PRIu64", %"PRIu64", %"PRIu64"): "	
					NORMAL,
					current->pID,
					current->cycles_done,
					current->cycles_todo,
					current->cycles_waited
				);//PRIu64 für PRInt uint64_t
	
				if(current->state == DEAD) {
					printf(RED "DEAD\t" NORMAL);
				} else if(current->state == RUNNING) {
					printf(BLUE "RUNNING\t" NORMAL);
				} else if(current->state == READY) {
					printf(GREEN "READY\t" NORMAL);
				}
				
				current = current->next;
			}
			printf("\n");
			
			tick++;
			ticks_todo--;
		} while(ticks_todo);
		
		if(ai != ALGO_NUM-1) {
			printf("\n" CYAN "Press any key for next algorithm." NORMAL "\n\n");
			getchar();
		}
	}

}

