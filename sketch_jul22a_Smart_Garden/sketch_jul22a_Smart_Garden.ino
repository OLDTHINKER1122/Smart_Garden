#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <SoftwareSerial.h>

// 初始化元件 VCC -> 3.3, 5V都行 ; LCD ->5V
DHT dht(2, DHT11); //DH out -> 2
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD SCL -> A5("右"上), SDA -> A4(AREF上面) VCC -> "5V"
SoftwareSerial BT(10, 11); //藍芽TX -> arduino 10, RX -> arduino 11

int LED = 5;
int water = 6;
int fan = 7;
char LEDstatus;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);  // 設定藍牙通訊速率
  
  pinMode(LED, OUTPUT);  
  pinMode(water, OUTPUT);  
  pinMode(fan, OUTPUT); 
  
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  // 讀取溫濕度數據
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // 顯示溫度和濕度
  if (!isnan(h) && !isnan(t)) {
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(t, 1);
    lcd.print((char) 0xDF);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(h, 1);
    lcd.print("%");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error ");
    lcd.setCursor(0, 1);
    lcd.print("               ");  // 清空第二行
  }

  delay(500); // 更新頻率

  // 檢查藍牙指令
  if (BT.available()) {
    LEDstatus = BT.read();
    Serial.println(LEDstatus);

    // 根據藍牙指令控制設備
    switch (LEDstatus) {
      case 'a':
        digitalWrite(LED, HIGH);
        break;
      case 'b':
        digitalWrite(LED, LOW);
        break;
      case 'c':
        digitalWrite(water, HIGH);
        break;
      case 'd':
        digitalWrite(water, LOW);
        break;
      case 'e':
        digitalWrite(fan, HIGH);
        break;
      case 'f':
        digitalWrite(fan, LOW);
        break;
    }
  }
}
