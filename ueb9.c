#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<inttypes.h>

char cxor (char a, char b);	//xor für chars
char* crc(char* bits); //bekommt payload als string und gibt prüfsumme als string zurück
int main(int argc , char *argv[]){
	if (argc == 1){
		printf("Prüfsumme: %s\n",crc(argv[1]));
	}
	else if(argc == 2){
		FILE * file;
		file = fopen( argv[1] , "r+");	//open given file; man: "a+" Open for reading and writing.  The file is created if it does not exist.  The stream is positioned at the end of the file.
		char* idc = malloc(1000);		//idc because i dont care about the name
		char* payload = fgets(idc,1024,file); //1024 ist einfach mal das limit der stringlänge
		//printf("Prüfsumme: %s\n",crc(payload));
		fprintf(file, "%s",crc(payload)); //klappt nicht aber mit 'a+' oben gibt es segmentation fault
	
		free(idc);
	}
	else{
		fprintf(stderr,"Falsche Anzahl an Argumenten.\n");
		return 1;
	}
	return 0;
}

char* crc(char* payload){
	char* genpol = "11000000000000101"; //generatorpolynom von CRC16
	char* bits = strcat(payload,"0000000000000000"); //16 0en anhängen
	while(strlen(genpol)<=strlen(bits)){
		for(uint64_t l = 0;l<strlen(genpol);l++){	//XOR von genpol und bits
			char c = cxor(bits[l],genpol[l]);
			bits[l]=c;	//neue Stelle der payload ist ergebnis von xor
		}
		for(int64_t i = 0; i<strlen(bits); i++){
			if(bits[i]=='0'){
				bits++;//wenn neue stelle 0 ist, wird sie rausgeschmissen
			}
			else if(bits[i]=='1'){
				break;
			}
			else{
				fprintf(stderr,"Fehler beim anfangs0en Löschen.\n");
				break;
			}
		}
	}
	return bits;	//gibt Rest zurück
}

char cxor (char a, char b){ //funktioniert, habe ich getestet
	if(a==b){
		return '0';
	}
	else{
		return '1';
	}
}