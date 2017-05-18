#include <inttypes.h>
#include <stdio.h>

int main (int argc, char *argv[]){
	uint64_t var= 123456789123456789;
	//uint16_t *ptr = &var;	//weisen 16bit pointer auf 64bit variable, compiler macht es aber wirft warning. Können wir umgehen mit casten:
	uint16_t *ptr2 = (uint16_t*)&var;
	printf("%lx\n", var);	//lx = long hexa, alternativ li für long integer
	for(uint16_t i=0;i<4;i++){
		printf("%x\n",i[ptr2]);	//x ist hexadez. Pointer sind arrays!
//i[ptr2] == ptr2[i] denn beides = ptr2 + i;
	}
	//nehmen für beides hexa zum Vergleich und hexa generell weil es ja 4 Pakete mit 16 sind und mit hexa die Stückelung feststeht
}
