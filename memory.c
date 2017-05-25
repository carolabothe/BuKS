#include "memory.h"
#include <stdio.h>
#include <inttypes.h>
#include <stddef.h> //für ptrdiff_t
#include <sys/types.h> //für size_t

//Fragen:
//1. Woher kennt man größe der metadaten?
//2. Wie speichert man die inforationen nur in memory? Bzw. wenn nicht, wie übertragt man die infos auf die anderen Funktionen?

char memory[MEM_SIZE];	//definiert in header
int32_t bcount; //anzahl der blöcke

struct MemBlock {
	struct MemBlock* next; //pointer auf nächsten Datenblock
	ptrdiff_t size; 	//Größe des Blocks
	ptrdiff_t ds; //größe von Daten die wir reinschreiben (data size)
};

                                                                                                                        
void memory_init(){
	struct MemBlock* m = (struct MemBlock*)memory;
	m->next=NULL;
	m->ds=0;
	m->size=m->ds;	
	bcount = 1;	
	memory = (char [MEM_SIZE])m;
}


void* memory_allocate(size_t byte_count){ //suche nach freiem Speicher
	struct MemBlock *n;
	for(int32_t i=0;i<MEM_SIZE;i++){	//gehe Speicher durch
		if((memory[i]-memory[MEM_SIZE-1])>=byte_count){	
			struct MemBlock* pre = (struct MemBlock*)memory; //Vorgänger. Woher weiß ich worauf der zeigt?
			n = (struct MemBlock*)memory[i]; //neuer Block
			n->next = pre->next; //neuer Block übernimmt next
			pre->next = n;	//alter block zeigt jetzt auf neuen
			n->ds = byte_count; //angeforderte Größe
		}
	}
	
	printf("Es gibt freien Speicherplatz an der Stelle %s des Speichers.\n",n);
	bcount ++;
	return n;
}


void memory_free(void* pointer){}


void memory_print(){
	printf("Der Zustand des Speichers ist wie folgt:\n\t\tGröße\tFrei\tZeigt auf\n");
	/*for (int16_t i=0; i<bcount; i++){
		printf("Block %i:\t%i\t%i\t&h",i,(&memory[i]-&next),(&next-&b),next); 
	}*/
	printf("Insgesamt: \t%i\n", MEM_SIZE);
	for (int32_t j=0; j<MEM_SIZE; j++){printf("%c",memory[j]);}
} 