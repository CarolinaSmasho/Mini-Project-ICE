#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
bool start = false;
unsigned long lt; // ใช้ unsigned long สำหรับเวลา 
int bitpin[] = {2, 3, 4, 5, 6, 7, 8, 9};
int time1[] = {B00111111,B00000110,B01011011,B01001111,B01100110,B01101101,B01111101,B00000111,B01111111,B01101111};
int current_time = 0; //ตัวแปรเก็บเวลาปัจจุบันที่แสดงบน 7-segment

void setup() {
  Serial.begin(4800);
  mySerial.begin(4800);
  for (int i = 0; i < 7; i++) {
    pinMode(bitpin[i], OUTPUT);
  }
  for(int i = 2; i <= 9; i++){
    digitalWrite(i, 1); // ปิด LED ทั้งหมด
  }
}

void loop() {
  if(mySerial.available()) {
    String a = mySerial.readStringUntil('\n');
    a.trim();  // ลบช่องว่างและอักขระพิเศษออก
    Serial.println(a);  // ตรวจสอบค่าที่ได้รับจาก mySerial
    
    if(a == "Duration: A"){  // เปรียบเทียบแบบตรง
//      Serial.println("Duration A Detected");
      lt = millis();  // เริ่มจับเวลา
      current_time = 8; // ตั้งค่าเริ่มต้นให้ 7-segment เริ่มที่ 9 วินาที
    }
    else if(a == "Duration: B"){
//      Serial.println("Duration B Detected");
      lt = millis();
      current_time = 6;
    }
    else if(a == "Duration: C"){
//      Serial.println("Duration C Detected");
      lt = millis();
      current_time = 4;
    }
    else if(a == "Game Over!!"){
      lt = millis();
      current_time = 0;
    }
  }

  // ตรวจสอบเวลานับถอยหลังทุกๆ 1 วินาที
  if (millis() - lt >= 1000 && current_time >= 0) {
    lt = millis(); // อัปเดตเวลา
    // แสดงผลบน 7-segment
    for(int j = 0; j < 8; j++){
      digitalWrite(bitpin[j], !bitRead(time1[current_time], j));
    }
//    Serial.print("Remaining time: ");
//    Serial.println(current_time);
    current_time--; // ลดเวลาลง 1 วินาที
  }
}
