
 //Høyttaler
unsigned long forrigeTid = 0;
unsigned long tid = 0;
int intervall = 2000;

 //Boolse sjekkpunkter
 boolean hodestotteOk = false;
 boolean mobilholderOk = false;
 boolean setebelteOk = false;
 boolean harLyst = false;

//Hoytaler
int frekvens = 250;

//Maaling av bilsikkerhet 
const int SETEBELTE = 12;
const int MOBILHOLDER = A0;
const int HODESTOTTE = 13;
int hodeTeller = 0;

//Tykkplate til telefon
int kraft;

//LEDs og lyd
const int GRONNLED1 = 9;
const int GRONNLED2 = 10;
const int GRONNLED3 = 11;
const int setebelteLED = 2;
const int mobilholderLED = 3;
const int hodeputeLED = 4;
const int statusLED = 6;
const int HOYTTALER = 8;

//Brukes til aa maale om initiell test er utfoert
int teller = 0;

void setup() {
  pinMode(SETEBELTE, INPUT_PULLUP);
  pinMode(HODESTOTTE, INPUT);
  pinMode(MOBILHOLDER, INPUT);
  pinMode(GRONNLED1, OUTPUT);
  pinMode(GRONNLED2, OUTPUT);
  pinMode(GRONNLED3, OUTPUT);
  pinMode(setebelteLED, OUTPUT);
  pinMode(hodeputeLED, OUTPUT);
  pinMode(mobilholderLED, OUTPUT);
  pinMode(statusLED, OUTPUT);
  digitalWrite(setebelteLED, HIGH);
  digitalWrite(hodeputeLED, HIGH);
  digitalWrite(mobilholderLED, HIGH);
  digitalWrite(statusLED, HIGH);
}

void loop() {

  //Tar forste sjekk av alle sensorer. 
if (teller < 1) {
  hovedsjekk();
}

//Sjekker alle sensorer, med unntak av nakkestoette. 
if (teller >= 1) {
  sjekk();
  pulserEnGang();
  }
}

void hovedsjekk(){

 sjekkSetebelte();
 sjekkMobilholder();
 sjekkHodestotte();
 
 if(hodestotteOk && mobilholderOk && setebelteOk) {
  teller += 1;
  pulserendeLys();
  harLyst = true;
  }
}

//Sjekker ved kjoring om det groenne pulserende lyset allerede har lyst naar forrige feil ble rettet. Hvis ikke, kjoerer den pulserendeLys()
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

void sjekk() {
   sjekkSetebelte();
   sjekkMobilholder();

   
   if (mobilholderOk == false || setebelteOk == false) {
    harLyst = false;
    tid = millis();
    if (forrigeTid + intervall < tid) { 
      lyd();
      forrigeTid = millis();
    }
  } 
} 

//Spiller av et lydsignal med forhaandsdefinert varighet
void lyd() {
  tone(HOYTTALER, frekvens, 150);
  
}

//Sjekker setebelte
void sjekkSetebelte(){
   if (digitalRead(SETEBELTE)) {
      digitalWrite(setebelteLED, HIGH);
      setebelteOk = false;
    }else {
    digitalWrite(setebelteLED, LOW);
    setebelteOk = true;
   }
 }

//Sjekker mobilholder
void sjekkMobilholder(){
  kraft = analogRead(MOBILHOLDER);
  if (kraft > 500) {
      digitalWrite(mobilholderLED, LOW);
      mobilholderOk = true; 
      
  }else {
    digitalWrite(mobilholderLED, HIGH);
    mobilholderOk = false;
   }
 
}

//Sjekker hodestoette.
void sjekkHodestotte(){
   if (digitalRead(HODESTOTTE)) {
    hodeTeller ++;
    digitalWrite(hodeputeLED, LOW);
    hodestotteOk = true;
   } 
   else if (hodeTeller > 0){
    digitalWrite(hodeputeLED, LOW);
    
   }
   else{
    digitalWrite(hodeputeLED, HIGH);
    
   }
}

//Brukes som delay
void millisDelay(int ms) {
 unsigned long now = millis();
 int ventetid = ms;
 
 while (millis() < now + ventetid) {
 }
  
  }

void pulserendeLys() {       
           for (int lysStyrke = 0 ; lysStyrke <= 200; lysStyrke += 5) {
    // sets the value (range from 0 to 255):
            analogWrite(GRONNLED1, lysStyrke);
            millisDelay(30);
          } 

           
          for (int lysStyrke = 200 ; lysStyrke >= 0; lysStyrke -= 5) {
    // sets the value (range from 0 to 255):
            analogWrite(GRONNLED1, lysStyrke);
            millisDelay(30);
          }
    
     return;
 }  
