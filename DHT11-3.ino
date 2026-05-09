#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 2
#define DHTTYPE DHT11

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DHT dht(DHTPIN, DHTTYPE);

char mode = 'b';
unsigned long lastDisplay = 0;
const unsigned long displayInterval = 2000;

void updateLCD(float tempF, float humidity) {
  lcd.clear();

  if (mode == 't') {
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.setCursor(0, 1);
    lcd.print(tempF, 1);
    lcd.print((char)223);
    lcd.print("F");
  }
  else if (mode == 'h') {
    lcd.setCursor(0, 0);
    lcd.print("Humidity:");
    lcd.setCursor(0, 1);
    lcd.print(humidity, 1);
    lcd.print("%");
  }
  else { // both
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(tempF, 1);
    lcd.print((char)223);
    lcd.print("F");

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(humidity, 1);
    lcd.print("%");
  }
}

void sendSerialReading(float tempF, float humidity) {
  if (mode == 't') {
    Serial.print("TEMP_F,");
    Serial.println(tempF, 1);
  }
  else if (mode == 'h') {
    Serial.print("HUMIDITY,");
    Serial.println(humidity, 1);
  }
  else {
    Serial.print("BOTH,");
    Serial.print(tempF, 1);
    Serial.print(",");
    Serial.println(humidity, 1);
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  dht.begin();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DHT11 Ready");
  lcd.setCursor(0, 1);
  lcd.print("Mode: BOTH");
}

void loop() {
  while (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '\n' || cmd == '\r') {
      continue;
    }

    cmd = tolower(cmd);

    if (cmd == 't' || cmd == 'h' || cmd == 'b') {
      mode = cmd;

      float humidity = dht.readHumidity();
      float tempF = dht.readTemperature(true);

      if (isnan(humidity) || isnan(tempF)) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sensor Error");
        Serial.println("ERROR");
        return;
      }

      updateLCD(tempF, humidity);
      sendSerialReading(tempF, humidity);
    }
  }

  if (millis() - lastDisplay >= displayInterval) {
    lastDisplay = millis();

    float humidity = dht.readHumidity();
    float tempF = dht.readTemperature(true);

    if (isnan(humidity) || isnan(tempF)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sensor Error");
      return;
    }

    updateLCD(tempF, humidity);
  }
}
