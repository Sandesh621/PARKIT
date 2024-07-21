#define BLYNK_TEMPLATE_ID "TMPL67NJUYpj8"
#define BLYNK_TEMPLATE_NAME "Smart Parking System"
#define BLYNK_AUTH_TOKEN "gEWdQryTi7NiFYxewb233TYt2Xe1GE6z"

#include <ESP32Servo.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "pramoshan_fctwn_2.4g";
char pass[] = "nepal12345";

LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myservo;

#define ir_enter 12
#define ir_back  14
#define ir_car1 15
#define ir_car2 2
#define ir_car3 4
#define led_1 27
#define led_2 26
#define led_3 25

int S1 = 0, S2 = 0, S3 = 0;
int flag1 = 0, flag2 = 0; 
int slot = 3, nslot = 0;  
int count = 0;

void setup() {
  Serial.begin(9600);

  pinMode(ir_car1, INPUT);
  pinMode(ir_car2, INPUT);
  pinMode(ir_car3, INPUT);
  pinMode(ir_enter, INPUT);
  pinMode(ir_back, INPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);  
  
  myservo.attach(5);
  myservo.write(180);

  Wire.begin();
  lcd.init();  
  lcd.backlight();  
  lcd.setCursor(0, 0);
  lcd.print("     Project    ");
  lcd.setCursor(0, 1);
  lcd.print("     PARKIT     ");
  delay(6000);  
  lcd.clear();  
  
  Read_Sensor();
  int total = S1 + S2 + S3;
  slot = slot - total; 

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Blynk.run();
}

void loop() {  
  
  Read_Sensor(); 
  
  if (S1 == 1) { digitalWrite(led_1, LOW);  Blynk.virtualWrite(V15, HIGH);}
  if (S1 != 1) { digitalWrite(led_1, HIGH); Blynk.virtualWrite(V15, LOW);}
  if (S2 == 1) { digitalWrite(led_2, LOW);  Blynk.virtualWrite(V2, HIGH);}
  if (S2 != 1) { digitalWrite(led_2, HIGH); Blynk.virtualWrite(V2, LOW);}
  if (S3 == 1) { digitalWrite(led_3, LOW);  Blynk.virtualWrite(V4, HIGH);}
  if (S3 != 1) { digitalWrite(led_3, HIGH); Blynk.virtualWrite(V4, LOW);}

  if (slot > 0) {
    if (digitalRead(ir_enter) == LOW && flag1 == 0) {
      flag1 = 1;
      if (flag2 == 0) { myservo.write(90); count = count + 1;}
    }
  }

  if (digitalRead(ir_enter) == LOW && flag1 == 3) {
    flag1 = 1;
  }

  if (digitalRead(ir_back) == LOW && flag2 == 0) {
    flag2 = 1;
    if (flag1 == 0) { myservo.write(90); }
    if (slot == 0) { flag1 = 3; }
  }

  Blynk.virtualWrite(V3, count);

  if (S1 == 0 && S2 == 0 && S3 == 0) {
    slot = 3; 
    nslot = 0;
  } else if (S1 == 1 && S2 == 0 && S3 == 0) {
    slot = 2; 
    nslot = 1;
  } else if (S1 == 0 && S2 == 1 && S3 == 0) {
    slot = 2; 
    nslot = 1;
  } else if (S1 == 0 && S2 == 0 && S3 == 1) {
    slot = 2; 
    nslot = 1;
  } else if (S1 == 1 && S2 == 1 && S3 == 0) {
    slot = 1; 
    nslot = 2;
  } else if (S1 == 1 && S2 == 0 && S3 == 1) {
    slot = 1; 
    nslot = 2;
  } else if (S1 == 0 && S2 == 1 && S3 == 1) {
    slot = 1; 
    nslot = 2;
  } else {
    slot = 0; 
    nslot = 3;
  }

  if (slot > 0) {
    lcd.setCursor(0, 0);
    lcd.print(" Usable slot: "); 
    lcd.print(slot);
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print(" Used slot: "); 
    lcd.print(nslot);
    lcd.print("   ");
    Blynk.virtualWrite(V1, slot);

  } else {
    lcd.setCursor(0, 0);
    lcd.print(" Sorry Parking  ");
    lcd.setCursor(0, 1);
    lcd.print("      Full      ");
    Blynk.virtualWrite(V1, "Sorry Parking Full");
  }

  if (flag1 == 1 && flag2 == 1) {
    delay(2000);
    myservo.write(180);
    flag1 = 0;
    flag2 = 0;
  }
}

void Read_Sensor() {
  S1 = 0; 
  S2 = 0; 
  S3 = 0;
  if (digitalRead(ir_car1) == 0) { S1 = 1; }
  if (digitalRead(ir_car2) == 0) { S2 = 1; }
  if (digitalRead(ir_car3) == 0) { S3 = 1; }
}
