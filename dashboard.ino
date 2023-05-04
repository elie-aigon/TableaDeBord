#include <DHT.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd = LiquidCrystal(8, 9, 10, 11, 12, 13);

#define B 3950
#define RESISTOR 200000
#define THERMISTOR 100000
#define NOMINAL 25
#define sensor A0
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int LDR_PIN = A0;
const int TRIG_PIN = 5;
const int ECHO_PIN = 4;
const int Lred = A2;
const int Lblue = A3;
const int Lgreen = A4;
const int Lyellow = A5;
const int btn = 6;
const int motor = 3;
const int buzzer = 7;
bool is_press = false;
bool live = false;
 
void setup() {
Serial.begin(9600);
dht.begin();
lcd.begin(16, 2);
pinMode(Lred, OUTPUT);
pinMode(Lblue, OUTPUT);
pinMode(Lgreen, OUTPUT);
pinMode(Lyellow, OUTPUT);
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);
pinMode(motor, OUTPUT);
pinMode(sensor, INPUT);
pinMode(btn, INPUT);
pinMode(buzzer, OUTPUT);
analogWrite(motor,0);
}
 
void loop() {
int state = digitalRead(btn);
if (state == 0){
  is_press = true;
}
if (state == 1 && is_press){
  is_press = false;
  if (live){
    live = false;
    off();
  } else{
    live = true;
  }
}
if (live){
  int t = analogRead(sensor);
  // Température Humiditée
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("Humidite= ");
  lcd.print(humidity); lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Temp= "); lcd.print(temperature); lcd.print(" C");
  // Luminosité
  int valeur_LDR = analogRead(LDR_PIN);
  lcd.setCursor(17, 0);
  lcd.print("Luminosite= ");
  lcd.print(valeur_LDR); lcd.print(" LUX");
  // Distance
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distance = duration * 0.034 / 2;
  lcd.setCursor(17, 1);
  lcd.print("Distance= "); lcd.print(distance); lcd.print(" cm");
  if (distance < 10){
    tone(buzzer, 15000);
    delay(500);
    noTone(buzzer);
  }
  if (humidity> 30){
    digitalWrite(Lgreen, 255);
    digitalWrite(Lyellow, 0);
  }else{
    digitalWrite(Lgreen, 0);
    digitalWrite(Lyellow, 255);
  }
  if (temperature >26){
    digitalWrite(Lred, 0);
    digitalWrite(Lblue, 0);
    analogWrite(motor, 180);
  }
  if (temperature >= 19 && temperature <= 25){
    digitalWrite(Lblue, 0);
    digitalWrite(Lred, 255);
    analogWrite(motor, 0);
  }
  if (temperature <= 19){
    digitalWrite(Lblue, 255);
    digitalWrite(Lred, 0);
    analogWrite(motor, 0);
    }
  }
  delay(500);
  lcd.scrollDisplayLeft();
}

void off(){
  digitalWrite(Lred, 0);
  digitalWrite(Lblue, 0);
  digitalWrite(Lyellow, 0);
  digitalWrite(Lgreen, 0);
  analogWrite(motor, 0);
}