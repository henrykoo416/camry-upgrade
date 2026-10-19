#include <DHT.h>
#define DHTPIN 7
#define DHTTYPE DHT11

#include <LiquidCrystal.h>

DHT dht(DHTPIN, DHTTYPE);
float h;
float t;
float oldt;
float f;
float hif;
int heat;
unsigned long lastRead = 0;
const unsigned long interval = 5000;


bool unit = 0;
int unitC = 6;
int screen = 8;
int backlight = 9;
bool unitChange = 0;

bool buttonstate;
bool screenstate;
bool lightstate = 1;
//bool lastbuttonstate = 0;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  Serial.println(F("DHT11 test!"));
  dht.begin();
  pinMode(unitC, INPUT_PULLUP);
  delay(500);
  pinMode(screen, INPUT_PULLUP);
  pinMode(backlight, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:


  buttonstate = digitalRead(unitC);
  if (buttonstate == 1){
    unit = !unit; 
    unitChange = 1;
    delay(200);
    display();
  }

  screenstate = digitalRead(screen);
  if (screenstate == 1){
    lightstate = !lightstate;
    digitalWrite(backlight, lightstate);
    delay(200);
  }
  measure();
//  lastbuttonstate = buttonstate;
}

void logSerial(){
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("% Temperature:" ));
  Serial.print(f);
  Serial.print(F("°F Heat Index: "));
  Serial.print(hif);
  Serial.println(F("°F"));
}

void display(){
  if (oldt != t || unitChange == 1){
    unitChange = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    if (unit == 0){
      lcd.print(f);
      lcd.print((char)223);
      lcd.print("F");
    }
    else{
      lcd.print(t);
      lcd.print((char)223);
      lcd.print("C");
    }
  }
}

void measure(){
  if (millis() - lastRead >= interval){
    lastRead = millis();
    h = dht.readHumidity();
    oldt = t;
    t = dht.readTemperature();
    f = dht.readTemperature(true);

    if (isnan(h) || isnan(t) || isnan(f)){
      Serial.println(F("Failed to read from DHT sensor"));
      return;
  }

  hif = dht.computeHeatIndex(f,h);

  display();
  logSerial();
  }
}


/*  heat = round(hif);
  for (int i = 0; i < 500; i++) {
    heatOvernight[i] = heat;
    if (heatOvernight[499] > 0){
      for (int i = 0; i < 500; i++){
        Serial.print(heatOvernight[i]);
        Serial.print(",");*/
