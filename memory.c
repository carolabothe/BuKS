#include "memory.h"
#include <stdio.h>
#include <inttypes.h>
#include <stddef.h> //für ptrdiff_t
#include <sys/types.h> //für size_t

//Leider haben wir es nicht geschafft, dass die Funktionen tun was sie sollen :(
char memory[MEM_SIZE];	//definiert in header

struct MemBlock {
	struct MemBlock *next; //pointer auf nächsten Datenblock
	ptrdiff_t size; 	//Größe des Blocks
	ptrdiff_t ds; //größe von Daten die wir reinschreiben (data size)
};

                                                                                                                        
void memory_init(){
	struct MemBlock* first = (struct MemBlock*)memory;
	first->next=NULL;	
	first->ds=0;
	first->size=first->ds;	
}


void* memory_allocate(size_t byte_count){ //suche nach freiem Speicher
	struct MemBlock *first;
	struct MemBlock *n;
	memory_init();
	struct MemBlock *i = first;
	do{
		if((i->next - i)>=byte_count){
			n->next = i->next; //neuer Block übernimmt next
			i->next = n;	//alter block zeigt jetzt auf neuen
			n->ds = byte_count; //angeforderte Größe
			break;
		}
		i=i->next;
	}while(i!=NULL);
	
	printf("Es gibt freien Speicherplatz an der Stelle %p des Speichers.\n",n);
	return n;
}


void memory_free(void* pointer){
	struct MemBlock *first;
	struct MemBlock *p = pointer;
	memory_init();
	struct MemBlock *i = first;
	do{
		if(i->next==p){
			i->next=p->next;
			break;
		}
		i=i->next;
	}while(i!=NULL);
	
	
	
	printf("Der gewünschte Speicherplatz an %p wurde frei gegeben.\n",pointer);

}


void memory_print(){
	struct MemBlock *first;
	memory_init();
	printf("Der Zustand des Speichers ist wie folgt:\n\t\tGröße\tFrei\tZeigt auf\n");
	int16_t counter = 1;
	for(struct MemBlock *i = first; i!=NULL;i=i->next){
		printf("Block %i:\t%ti\t%li\t%p\n",counter,i->size,i->size - i->ds,i->next);	
		counter ++;
	}
	printf("Insgesamt: \t%i\n", MEM_SIZE);
	for (int32_t j=0; j<MEM_SIZE; j++){printf("%c",memory[j]);}
} 