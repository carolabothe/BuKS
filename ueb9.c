#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<inttypes.h>
#include<stdbool.h>

uint16_t crc(uint8_t message[], int nBytes);

int main(int argc , char *argv[]){
/*uint16_t test;
printf("%d\n",test = crc((uint8_t*)"?~", 2));
return 0;*/

	if(argc!=2){
		fprintf(stderr,"Falschen Anzahl an Argumenten.\n");
		return 1;
	}
	FILE * file,*newfp; //file ist immer die normale datei und newfp die mit .crc also mit checksumme
	if(strcmp(&argv[1][strlen(argv[1])-4],".crc")==0){ //checksumme prüfen
		newfp = fopen(argv[1] , "r+");	//open given file; 
		fseek(newfp, 0L, SEEK_END); //bis zum ende gehen
		long sz = ftell(newfp);//größe der datei durch jetzige position
		rewind(newfp); //zurück zum start des files
		//nachricht lesen
		uint8_t message[sz+1];
		uint16_t i,c= 0;
		file = fopen(&argv[1][strlen(argv[1])-4],"w+");		
		do{
    	    if(ftell(newfp)==fseek(newfp, sz-4, SEEK_SET)){ //sind bei checksumme angekommen
    	     	break;
    	  	}
    	    c = fgetc(file);
    	  	fputc(c,newfp);//kopieren inhalt in neue datei
			message[i] = c;
			i++;   
		}while(1);
		uint16_t checksum = crc(message,sizeof(message));
		//checksumme rauslesen
		i = 0;
		char givencsstr [4]; //string checksumme
		do{
    	    if(feof(file)!=0){
    	     	break;
    	  	}
    	  	givencsstr[i] = (char)fgetc(file); //erstelle string der checksumme
    	  	i++;
		}while(1);	
		
		fclose(file);
		fclose(newfp);
		uint16_t givencs = (int)strtol(givencsstr, NULL, 16);       // number base 16
		if(checksum != givencs){
			fprintf(stderr,"Die gegebene Datei (%s) hatte eine falsche Checksumme. Die richtige lautet: %"PRIx16"\n",argv[1],checksum);
			remove(&argv[1][strlen(argv[1])-4]);
		}
		
	}
	else{ //checksumme berechnen
		file = fopen(argv[1] , "r+");	//open given file; 
		fseek(file, 0L, SEEK_END); //bis zum ende gehen
		long sz = ftell(file);//größe der datei durch jetzige position
		rewind(file); //zurück zum start des files
		uint8_t message[sz];
		uint16_t i,c= 0;
		newfp = fopen(strcat(argv[1],".crc"),"w+");
		do{
    	    c = fgetc(file);
    	    if(feof(file)!=0){
    	     	break;
    	  	}
    	  	fputc(c,newfp);//kopieren inhalt in neue datei
			message[i] = c;
			i++;   
		}while(1);
		
		/*alternative:
		char *restrict messtring = malloc(1000000);
		fread(messtring, 10000000, 1, file);
		uint16_t message[strlen(messtring)];
		for(uint16_t i; i<strlen(messtring); i++){
			//wandeln string in array aus ints um 
			message[i] = (uint16_t)(messtring[i]);
		}*/
		fprintf(newfp, "%-4.4"PRIx16"", crc(message,sizeof(message))); //-4.4 legen mindestens&höchstens 4 chars fest für hexzahl
		fclose(file);
		fclose(newfp);
		//free(messtring);
	}
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