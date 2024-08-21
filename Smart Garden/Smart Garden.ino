#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SoftwareSerial.h>

DHT dht(2, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int LED = 5;
int water = 6;
int fan = 7;
char LEDstatus;
//定義Arduino PIN10及PIN11分別為RX及TX腳
SoftwareSerial BT(10,11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BT.begin(9600); // 設定和 HC-06 通訊的速度 (預設 9600)
  pinMode(LED, OUTPUT);  
  pinMode(water, OUTPUT);  
  pinMode(fan, OUTPUT); 
  
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Temp");
  lcd.setCursor(0,1);
  lcd.print("Humidity");
}

void loop() {
  // put your main code here, to run repeatedly:
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  //讀取藍芽模組訊息，並傳送到電腦
  if (BT.available()){
    LEDstatus=BT.read();
    Serial.println(LEDstatus);  
 }
    
  if(isnan(h) || isnan(t)){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed to read");
    lcd.setCursor(0, 1);
    lcd.print("from DHT sensor");
    return;
  }
    lcd.setCursor(9, 0);
    lcd.print(t, 2);
    lcd.print((char) 0xDF);
    lcd.print("C");

    lcd.setCursor(9, 1);
    lcd.print(h, 2);
    lcd.print("%");
    delay(2000);
    
    switch (LEDstatus){ 
    case 'a':
    digitalWrite(LED,HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("lighting");
    break;
    
    case 'b':
    digitalWrite(LED,LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("close L");
    break;
    
    case 'c':
    digitalWrite(water,HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("watering");
    break;
    
    case 'd':
    digitalWrite(water,LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("close w");
    break;
    
    case 'e':
    digitalWrite(fan,HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("open f");
    break;
    
    case 'f':
    digitalWrite(fan,LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("close f");
    break;
    }
}
