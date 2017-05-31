/*
 * Autor: Thomas Tegethoff
 * Idee & Vorlage: Heiko Will
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <inttypes.h>

enum ProcessState {
	DEAD = 0,		// Prozess fertig (tot)
	RUNNING = 1,	// zuletzt gelaufener Prozess
	READY = 2		// Prozess bereit
};
typedef enum ProcessState ProcessState; //sorgt dafür dass wir enum weglassen können: ProcessState ist ein Alias für enum ProcessState

struct Process {
	uint64_t pID;				// Prozess ID
	enum ProcessState state;	// aktueller Prozessstatus

	uint64_t cycles_done;	// wie viele Cyclen gelaufen
	uint64_t cycles_waited;	// wie viele Cyclen gewartet
	uint64_t cycles_todo;	// wie viele Cyclen noch zu laufen

	struct Process* next;	// nächster Prozess in der Liste
	struct Process* prev;	// vorheriger Prozess in der Liste
};
typedef struct Process Process;

void rr(struct Process* head);
void fcfs(struct Process* head);
void spn(struct Process* head);
void hrrn(struct Process* head);

#endif
