#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h>
#include<inttypes.h>
#include<stdbool.h>

uint16_t crc(uint8_t message[], int nBytes);

int main(int argc , char *argv[]){

	if(argc!=2){
		fprintf(stderr,"Falschen Anzahl an Argumenten.\n");
		return 1;
	}
	FILE * file,*newfp; //file ist immer die normale datei und newfp die mit .crc also mit checksumme
	
	//Checksumme prüfen und datei davon befreien
	if(strcmp(&argv[1][strlen(argv[1])-4],".crc")==0){ 
		newfp = fopen(argv[1] , "r+");	//open given file 8(.crc)
		if(newfp == NULL){
			fprintf(stderr,"Error beim Öffnen der Datei.\n");
			return 1;
		}		
		int fd = fileno(newfp);
		long sz = lseek(fd, 0, SEEK_END); //größe der datei (anzahl zeichen)
		rewind(newfp); //zurück zum start des files
		//nachricht lesen
		uint8_t message[sz-4]; //letzte 4 sind crc
		uint16_t i,c= 0;
		char *filename = argv[1];
		filename[strlen(filename)-4] = '\0'; //letzte 4 Zeichen (.crc) wegschmeißen
		file = fopen(filename,"w+");	//
		char* givencsstr = malloc(5); //string checksumme
		while(i<sz-4){
   	    	c = fgetc(newfp);
   	  		fputc(c,file);//kopieren inhalt in neue datei (ohne .crc)
			message[i] = c;
			//printf("i=%u, message: %u\n",i,c);
			i++; 
		}
		fgets(givencsstr,5,newfp);
		uint16_t checksum = crc(message,sizeof(message));
		uint16_t givencs = (int)strtol(givencsstr, NULL, 16);       // number base 16
		if(checksum != givencs){
			fprintf(stderr,"Die gegebene Datei (%s) hatte eine falsche Checksumme (nämlich %"PRIx16"). Die richtige lautet: %"PRIx16"\n",argv[1],givencs,checksum);
			remove(filename);
		}
		free(givencsstr);
		fclose(file);
		fclose(newfp);		
	}
	
	//checksumme berechnen
	else{ 
		file = fopen(argv[1] , "r+");	//open given file; 
		int fd = fileno(file);
		long sz = lseek(fd, 0, SEEK_END); //größe der datei (anzahl zeichen)
		rewind(file); //zurück zum start des files
		uint8_t message[sz]; //nachricht ist array mit so vielen einträgen wie chars in file, jeder eintrag ist ein/e char/zahl
		uint16_t i,c= 0;
		newfp = fopen(strcat(argv[1],".crc"),"w+");
		if(newfp == NULL){
			fprintf(stderr,"Error beim Öffnen der Datei.\n");
			return 1;
		}		
		while(i<sz){
    	    c = fgetc(file);
    	  	fputc(c,newfp);//kopieren inhalt in neue datei
			message[i] = c;
			i++;   
		}
		fprintf(newfp, "%-4.4"PRIx16"", crc(message,sizeof(message))); //-4.4 legen mindestens&höchstens 4 chars fest für hexzahl
	}
	
	fclose(file);
	fclose(newfp);
	return 0;
}


#define WIDTH  (8 * sizeof(uint16_t))
#define TOPBIT (1 << (WIDTH - 1))

uint16_t crc(uint8_t message[], int nBytes){
    uint16_t  remainder = 0;	   
    // modulo-2 division, byte für byte
    for (int byte = 0; byte < nBytes; ++byte){
        // nächstes Byte in Rest
        remainder ^= (message[byte] << (WIDTH - 8));
    	// modulo-2 division, bit für bit
        for (uint8_t bit = 8; bit > 0; --bit){
            //Momentanes payloadbit teilen wenn möglich
            if (remainder & TOPBIT){
                remainder = (remainder << 1) ^ 0x18005; //unser generatorpolynom - mit oder ohne 1?
            }
            else{
                remainder = (remainder << 1);
            }
        }
    }
    return (remainder); //checksumme ist rest
}   