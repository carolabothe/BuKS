#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>           
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/sendfile.h>
#include <string.h>

// (a)

int delete(char* sourcename,char* targetname){  // delete kriegt 2 Dateinamen bzw. Pfade

  int startfd;                              // Int für den Fildiscriptor
  startfd = open(sourcename,O_RDWR,00700);  //weisen startfd das Ergebnis von open zu , wobei open  die Rechte Schreiben/Lesen durch O_RDWR hat , und 00700 benutzer hat auch schreibe/lese Rechte
  if(startfd <0){                           // vergleichen ob startfd <0 -> irgendwas hat nicht geklappt Datei z.b. exisitert nicht
                                           // Fehlerbehandlung man sollte den entstehenden error genauer abfangen indem man errno auf die Art des Fehlers prüft
    char* msg = "Datei existiert nicht\n"; // kreieren einen String für write um leichter strlen zu benutzen
    write(STDOUT_FILENO,msg,strlen(msg));  // write braucht den FD STDOUT_FILENO um in die Console zu schreiben, 2 Argument ist der String der ausgegeben werden soll ,3 Argument Länge des String (kriegen wir mit strlen heraus)
    return 0;     // damit wir raus gehen aus dem Code und nicht weiter lesen
    }

  chdir(".ti3_trashcan");   // wechseln ins .ti3 (Mülleimer) Verzeichnis
  int endfd;                // int für FD(Filedistriptor)
  endfd = open(targetname,O_CREAT | O_RDWR| O_EXCL,00700);  // ordnen endfd den Wert von open zu, O_CREAT Flag heißt falls Ordner mit namen targetname nicht vorhanden wird er kreiert aber ****nur**** dann (aufgrund vom O_EXCL Flag)
  if (endfd == -1){                                         // falls Fehler geben wir eine Meldung aus
    char* msg1= "Datei existiert schon im Mülleimer erstmal löschen bitte\n";   // Fehlerbehandlung errno näher analysieren bzw überhaupt
    write(STDOUT_FILENO,msg1,strlen(msg1));
    close(endfd);                                           // schließen unseren geöffneten Ordner um Fehler zu vermeiden
    return 0;
    }
                                                           // bis jetzt leere Datei kopiert in den Mülleimer // es fehlt noch ,Inhalt kopieren und die Datei im Verzeichnis zu löschen
  sendfile(endfd,startfd,NULL,100);                        // sendfile read,write Funktion in einem schreiben von der Datei im Verzeichnis in die kopierte im Mülleimer , NUll (OFFSET) , 100 := lesen und schreiben 100 Zeichen
  chdir("..");                                             // springen vom Mülleimer wieder ein Verzeichnis zurück
  unlink(sourcename);                                       //löschen die alte Datei
  char* info = "Die Datei wurde in den Papierkorb geschoben\n";
  write(STDOUT_FILENO,info,strlen(info));                  // info

  close(endfd);                                            // schließen die geöffneten Ordner
  close(startfd);
  return 0;
}





int create_trash(){
  int dir;                                  // erstellen den Papierkorb
  dir = mkdir(".ti3_trashcan",00700);
  //if (dir == -1){
  //  if(errno == EEXIST){                                      // Fehlernachricht falls der Papierkorb schon existiert haben wir aufgehoben weil sie genervt hat
      //char* msg2 = "Der Papierkorb existiert schon \n";
  //    write(STDOUT_FILENO,msg2,strlen(msg2));
  //  }
  //}
   return 0;
}





int listet(){                       // soll die  Dateien im Papierkorb auflisten
  DIR* dirzeiger;                   // Pointer vom Verzeichnis
  struct dirent* dirEigenschaften;  // Verzeichnis struct der durch readdir weiterhilft mit bestimmten Eigenschaften
  dirzeiger = opendir (".");        // "." ist der aktuelle Ordner ,dirzeiger kriegt den aktuellen Zeigen vom Verzeichnis Typ DIR*
  if (dirzeiger){                   // falls kein Fehler gehen wir in die schleife
    while (0 != (dirEigenschaften = readdir(dirzeiger))){ // readdir macht aus dem Verzeichnis Zeiger ein Typ Struct bzw  liest das Verzeichnis womit man auf bestimmte Eigenschaften zugreifen kann,
      //solgane wir nicht am ende des verzeichnises sind führen wir die Schleife aus wobei jedes readdir den Zeiger um 1+ erhöt am ende wird 0 ausgeben dann brechen wir die scheife ab
                             // der Struct speichert den Namen von der Datei wo der Zeiger gerade ist dieser wird bei jedem Schleifendurchgang erhöht
         write(STDOUT_FILENO,dirEigenschaften->d_name,strlen(dirEigenschaften->d_name));
         write(STDOUT_FILENO,"\n",1);
    }

  }
}




 int recover(char* targetname){
   chdir(".ti3_trashcan");                                  // gleiches Prinzip wie bei delete nur andersrum  gehen zu erst in den Mülleimer checken ob die Datei da ist
   int trashdata;
   trashdata= open(targetname,O_RDWR,00700);
   if(trashdata < 0){
     char* msg4 = "Die Datei existiert nicht im Müll\n";    // falls nicht Meldung
     write(STDOUT_FILENO,msg4,strlen(msg4));
     return 0;
   }
   chdir("..");
   int recovered;
   recovered = open(targetname,O_CREAT | O_RDWR| O_EXCL,00700);
   if (recovered <0){
     char* msg5 = "Bitte erst die Datei im aktuellen Verzeichnis löschen\n";    // fall die Datei im akteullen Verzeichnis schon vorhanden ist überschreiben wir sie nicht
     write(STDOUT_FILENO,msg5,strlen(msg5));                                    // dies könnte man auch als Fehlerbehandlung sehen
     return 0;
   }
   sendfile(recovered,trashdata,NULL,100);                                      // nach dem kopieren der Datei müssen wir noch den Inhalt reinkopieren/schreiben
   chdir(".ti3_trashcan");                                                      // Fehlerbehandlung könnte sein das wir nur 100 Zeichen lesen und schreiben also begrenzt sind
   unlink(targetname);
   char* info1 = "Die Datei wurde wieder aus dem Papierkorb hergestellt\n";
   write(STDOUT_FILENO,info1,strlen(info1));


   close(trashdata);
   close(recovered);
   return 0;
 }

int superdelete(char* killeddata){
  chdir(".ti3_trashcan");
  int killd;                                            // sehr ähnlich wie delete nur leichter gehen in Mülleimer schauen ob Datei vorhanden falls ja löschen wir ise mit unlink
  killd = open(killeddata,O_RDWR,00700);
  if(killd < 0){
    char* msg6 = "Die Datei existiert nicht im Müll um sie entgültig zu löschen\n";
    write(STDOUT_FILENO,msg6,strlen(msg6));
    return 0;
  }
   unlink(killeddata);
   char* info2 = "Die Datei wurde unwiederruflich gelöscht\n";
   write(STDOUT_FILENO,info2,strlen(info2));
   close(killd);
   return 0;

}









int main (int argc, char* argv[] ){

  if(argc  <=1 || argc > 3){                        // machen ein Paramet test um hinzuweisen das was nicht stimmt
    char* msg0 = "Falsche Parameter\n";
    write(STDOUT_FILENO,msg0,strlen(msg0));
    return 0;
  }


  create_trash();      // kreieren den Mülleimer

  int deld;
  deld = strcmp(argv[1],"-d");                    // schauen ob "-d" eingegeben wurde strcmp vergleicht zwei String header nötig
  if(deld == 0){                                  // falls beide String gleich lang gibt strcmp eine 0 aus
    delete(argv[2],argv[2]);                      // falls erfüllt rufen wir delete auf
    return 0;
  }


  int list;
  list = strcmp(argv[1],"-l");                  // test ob -l eingeben wurde
  if(list == 0 && argc == 2){
    chdir(".ti3_trashcan");
    listet();
    return 0;
  }



  int recd;
  recd = strcmp(argv[1],"-r");                  // test auf -r
  if(recd ==0){
    recover(argv[2]);
    return 0;
  }



  int supd;
  supd = strcmp(argv[1],"-f");                  //test auf -f
  if(supd == 0){
    superdelete(argv[2]);
    return 0;
  }



  char* endmessage = "gültige Befehele:\n -l(zeigt die Dateien im Papierkorb an)\n -d <dName> (löschen einer Datei)\n -r <dName> (stellt eine Datei aus dem Papierkorb wieder her)\n -f <dNAme> (löscht eine Datei aus dem Papierkorb entgültig)\n";
  write(STDOUT_FILENO,endmessage,strlen(endmessage));

  return 0;



}
