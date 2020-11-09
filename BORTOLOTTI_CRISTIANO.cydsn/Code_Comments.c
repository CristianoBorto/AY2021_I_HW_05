/* 
===================================================================================================================================================
                                                    Commenti sul codice
Il codice è suddiviso nei seguenti file:
- Defines.h: qui vengono definite tutte le macro per definire i vari indirizzi dei registri, le maschere e la dichiarazione delle 
struct utilizzate.
- MyFunction.h: dichiarazione della interrupt, delle varaibili globali e delle funzioni implementate nel corrispondente file c.
- MyFunction.c: Definizione delle varie funzioni (commentate e descritte direttamente nel file).
- main.c: sviluppo vero e proprio del codice che descriverò passo per passo.

Componenti utilizzati:
- EEPROM: utilizzato per salvare in memoria il valore attuale della freq di sampling;
- On-Board Button + Debouncer: input hardware che triggera l'interrupt per aggiornare la frequenza;
- I2C
- UART
- On-Board LED (ADDIZIONALE): ho aggiunto questo elemento per vare un feedback visivo dell'effettivo cambiamento della freq di 
sampling andando a modificare contemporaneamente la freq di blinking del LED. Nel TopDesign è riportata una tabella con la convenzi-
ne utilizzata.

Inizializzazione
- Start alle varie periferiche (in particolare viene aggiornata una prima volta la temperatura dell'EEPROM e viene intro-
dotto un piccolo delay per la corretta accensione dei vari componenti);
- Dichiarazione variabili e azzeramento flag:
    1) Button_flag: flag utilizzato per aggiornare la frequenza alla pressione del bottone potendo entrare nella corrispondente area
    nel while(1); se il tasto viene premuto durante una lettura dei dati, essa viene terminata e poi viene cambiata la freq.
    2) Error_flag: flag utilizzato per segnalare un errore durante la lettura dei registri dei dati provenienti dai 3 assi in modo
    tale che non avvenga l'elaborazione e l'invio dei dati ma che torni subito a controllare se sono pronti nuovi dati e a cominciare
    una nuova lettura.
    3) Setting_flag: flag utilizzato per segnalare errori durante il settaggio dei registri e blocca il dispositivo non facendogli
    compiere nessuna azione nel while(1); ho fatto questa scelta in un'ottica di "protezione" del dispositivo evitando che compia
    azioni "potenzialmente pericolose" con i registri non settati correttamente.
    4) count: viene utilizzata questa variabile contatore per tenere traccia dello "step" attuale della freq di sampling per settare
    correttamente il PWM e permettere il passaggio da f=200 Hz a f=1 Hz. Quando viene letta la freq dalla memoria viene anche inizia-
    lizzato correttamente anche il contatore allo step corrispondente.
- Set registro 4 (viene abilitata la High Resolution mode e il Block Data Update); 
- Set registro 1 (set della frequenza letta dalla memoria --> se il dato letto dalla memoria non corrisponde a nessuna delle maschere
per il settaggio delle varie frequenze (ipotesi: alla primissima accensione non so cosa ci sia scritto in quel punto della memoria)
impongo la frequenza a 1 e salvo il correspondente valore nella memoria);
- Start dell'interrupt (viene abilitata appena prima del while(1) affinchè la pressione del bottone non interrompa l'inizializzazione
e il settaggio dei registri).

Algoritmo
Se il settaggio dei registri è andato correttamente, si entra nel vero e proprio algoritmo; si evidenziano due blocchi principali:
1) Button_flag == FALSE: 
    a) viene letto lo status register --> se è pronto un nuovo dato su tutti e tre gli assi e non ci sono stati errori in questa 
    lettura allora si comincia la lettura dei registri dei dati. 
    b) In modo sequenziale, si legge prima il registro low e poi quello high per tutti e tre gli assi salvando in una struct di array 
    i valori letti. Se non ci sono stati errori si prosegue, altrimenti viene messo a 1 Error_flag e viene saltato il prossimo step
    tornando allo step a.
    c)Vengono fatte delle elaborazioni ai dati prodotti:
        - vengono assemblati i valori low e high dei 3 assi e visto che sono left-alligned, vengono allineati a destra
        - i dati assemblati vengono trasformati da digit a m/s^2 sfruttando la retta di calibrazione e vengono "salvate" le prime 3
        cifre decimali moltiplicando per mille 
        - cast dei dati da float a int16 per poter mandarli correttamente al Bridge Control Panel --> in questo modo vengono 
        eliminati tutti i decimali indesiderati --> andando poi a sistemare il gain nel BCP sarà possibile poi visualizzare di nuovo 
        le tre cifre decimali precedentemente "salvate".
    d) Vengono impacchettati i dati mettendo prima il byte low e poi quello high e vengono mandati al BCP.
2) Button_flag == TRUE: si identificano due situazioni:
    a) 0 < count < 6: 
        - viene cambiato il setting del PWM,
        - viene incrementata la frequenza,
        - viene aggiornato il valore contenuto in memoria,
        - viene aggiornato il control register 1.
    b) count > 6:
        - azzeramento count,
        - viene cambiato il setting del PWM,
        - viene scritto in memoria la maschera per settare f sampling = 1Hz;
        - viene aggiornato il control register 1.

===================================================================================================================================================
*/

