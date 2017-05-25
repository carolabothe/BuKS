//Exercise tutorial: pointers and different int sizes
#include <stdio.h>
#include <inttypes.h>

int main(int argc, char *argv[]){
	int64_t x = 1;
	int64_t* px = &x;	//& vor x, da wir dem pointer die Adresse und nicht den Wert zuweisen wollen
	int32_t y = 2;
	int32_t* py = &y;	
	int16_t z = 3;
	int16_t* pz = &z;

	printf("Wert der Var, Adresse der Var (Wert Pointer), Größe der Var, Größe des Pointers:\n");
	printf("%lli, %p, %lu, %lu\n",x,px,sizeof(x),sizeof(px)); //lli for long long int, depends on OS
	printf("%i, %p, %lu, %lu\n",y,py,sizeof(y),sizeof(py));	//lu = long unsigned
	printf("%hi, %p, %lu, %lu\n",z,pz,sizeof(z),sizeof(pz));	//h for hexa
	//size is given in Bytes so 8 for 64bit system
}
