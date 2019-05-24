
//Høyttaler
unsigned long forrigeTid = 0;
unsigned long tid = 0;
int intervall = 2000;

//Boolse sjekkpunkter
boolean hodestotteOk = false;
boolean mobilholderOk = false;
boolean setebelteOk = false;
boolean harLyst = false;

//Maaling av bilsikkerhet 
const int setebelte = 12;
const int mobilholder = A0;
const int hodestotte = 13;
int hodeTeller = 0;

//Tykkplate til telefon
int kraft;

//LEDs og lyd
const int gronnLED = 9;
const int setebelteLED = 2;
const int mobilholderLED = 3;
const int hodeputeLED = 4;
const int statusLED = 6;
const int HOYTTALER = 8;

//Brukes til aa maale om initiell test er utfoert
int teller = 0;

void setup() {
 //De ulike portene settes til riktig modus
 pinMode(setebelte, INPUT_PULLUP);
 pinMode(hodestotte, INPUT);
 pinMode(mobilholder, INPUT);
 pinMode(gronnLED, OUTPUT);
 pinMode(setebelteLED, OUTPUT);
 pinMode(hodeputeLED, OUTPUT);
 pinMode(mobilholderLED, OUTPUT);
 pinMode(statusLED, OUTPUT);
 
 //Ved oppstart settes alle de roede lysene paa, i tillegg til statusLED som lyser naar enheten er paa
 digitalWrite(setebelteLED, HIGH);
 digitalWrite(hodeputeLED, HIGH);
 digitalWrite(mobilholderLED, HIGH);
 digitalWrite(statusLED, HIGH);
}

void loop() {
  //Tar forste sjekk av alle sensorer, dersom teller er mindre enn 1. 
if (teller < 1) {
  hovedsjekk();
}

//Sjekker alle sensorer, med unntak av nakkestoette, dersom teller er storre eller lik 1. 
//Dersom en feil oppstaar og rettes, kjores pulserEnGang().
if (teller >= 1) {
  sjekk();
  pulserEnGang();
  }
}

//Forestegangssjekk av alle sensorer. 
void hovedsjekk(){

 sjekkSetebelte();
 sjekkMobilholder();
 sjekkHodestotte();
 
 //Hvis alle sensorene er OK, settes teller til 1 og hovedsjekk vil ikke utfoeres paa nytt.
 if(hodestotteOk && mobilholderOk && setebelteOk) {
  teller += 1;
  pulserendeLys();
  harLyst = true;
  }
}


//Sjekker setebelte og mobilholder. Gir lys- og lydsignal dersom en av sensorkretsene brytes
void sjekk() {
   sjekkSetebelte();
   sjekkMobilholder();

  //Dersom mobilholder eller setebeltesensor ikke er OK, setter harLyst til false og et lydsignal spilles av.
  if (mobilholderOk == false || setebelteOk == false) {
    harLyst = false;
    tid = millis();
    if (forrigeTid + intervall < tid) { 
      lyd(300, 200);
      forrigeTid = millis();
    }
  } 
} 

//Sjekker ved kjoring om det groenne pulserende lyset allerede har lyst naar siste feil ble rettet. Hvis ikke, kjoerer den pulserendeLys()
//Lokken startes kun dersom harLyst == false, som kun er tilfellet dersom en sensor har gitt feilsignal.
//Dersom alle sensorene igjen er OK og harLyst fortsatt == false, kjores pulserendeLys(), for saa at harLyst settes lik true.
void pulserEnGang() {
  if(!harLyst) {
    sjekkSetebelte();
    sjekkMobilholder();
     if (mobilholderOk == true && setebelteOk == true && harLyst == false) {
       pulserendeLys();
       harLyst = true;
   }
  }
}

//Spiller av et lydsignal med forhaandsdefinert varighet
void lyd(int frekvens, int ms) {
  tone(hoyttaler, frekvens, ms);
}

//Sjekker setebelte og styrer LED som indikerer om setebeltet er tilkoblet eller ikke
void sjekkSetebelte(){
   if (digitalRead(setebelte)) {
      digitalWrite(setebelteLED, HIGH);
      setebelteOk = false;
    }else {
    digitalWrite(setebelteLED, LOW);
    setebelteOk = true;
   }
 }

//Sjekker mobilholder og styrer LED som indikerer om mobilen er satt riktig paa plass eller ikke
void sjekkMobilholder(){
  kraft = analogRead(mobilholder);
  if (kraft > 500) {
      digitalWrite(mobilholderLED, LOW);
      mobilholderOk = true; 
      
  } else {
    digitalWrite(mobilholderLED, HIGH);
    mobilholderOk = false;
   }
 
}

//Sjekker mobilholder og slaar av LED for hodeplassering dersom sensoren trigges ved riktig sittestilling.
//Sittestillingen sjekkes kun en gang etter oppstart.
void sjekkHodestotte(){
   if (digitalRead(hodestotte)) {
    hodeTeller ++;
    digitalWrite(hodeputeLED, LOW);
    hodestotteOk = true;
    
   } else if (hodeTeller > 0){
    digitalWrite(hodeputeLED, LOW);
    
   } else {
    digitalWrite(hodeputeLED, HIGH);
    
   }
}

//millis-basert metode for aa utsette handling under kjoering av kode
void millisDelay(int ms) {
 unsigned long now = millis();
 int ventetid = ms;
 
 while (millis() < now + ventetid) { }
  }

//Kjorer en lys-sekvens hvor alle RGB-diodene fader opp og ned gronnfarge.
//Lysstyrken okes med 5 og skrives til alle de analog-styrte diodene hver 30. ms
//Paa tilsvarende maate senkes lysstyrken igjen naar lyset har naadd maks lysstyrke paa 200. 
void pulserendeLys() {       
           for (int lysStyrke = 0 ; lysStyrke <= 200; lysStyrke += 5) {
    // sets the value (range from 0 to 255):
            analogWrite(gronnLED, lysStyrke);
            millisDelay(30);
          } 

           
          for (int lysStyrke = 200 ; lysStyrke >= 0; lysStyrke -= 5) {
    // sets the value (range from 0 to 255):
            analogWrite(gronnLED, lysStyrke);
            millisDelay(30);
          }
    
     return;
 }  
