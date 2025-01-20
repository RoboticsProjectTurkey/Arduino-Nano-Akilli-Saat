#include <virtuabotixRTC.h> 

virtuabotixRTC myRTC(6, 7, 8); // SCK , I/O , RST bağlantıları

void setup() {
 Serial.begin(9600);


 // saniye, dakika, saat, haftanın kaçıncı günü olduğu, ayın kaçıncı günü olduğu, ay, yıl
 myRTC.setDS1302Time(15, 22, 21, 7, 14, 1, 2018); 
}

void loop() {
myRTC.updateTime();

 Serial.print("Current Date / Time: ");
 Serial.print(myRTC.dayofmonth); 
 Serial.print("/");
 Serial.print(myRTC.month);
 Serial.print("/");
 Serial.print(myRTC.year);
 Serial.print(" ");
 Serial.print(myRTC.hours);
 Serial.print(":");
 Serial.print(myRTC.minutes);
 Serial.print(":");
 Serial.println(myRTC.seconds);
 delay(1000);
}
