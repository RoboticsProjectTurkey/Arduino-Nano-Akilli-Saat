
//Arduıno nano saat Kağan Arsal 
//kronometre , press game , sıcaklık ölçer , saat başı alarm açayıp kapayabilme , uyku modu , fener , ses açıp kapayabilme , marie cruıe resmi , invert mod , pil seviyesi ölçer 
// malzemeler: arduıno nano (atmega328p bootloader) , pil sarj modülü , ds1302 saat modülü , buuzzer , led , oled (ssd1306) ekran , push button , lm35 sıcaklık sensörü , (800mah) lityum telefon bataryası , delikli plaket , cr1220 pil

#include <virtuabotixRTC.h>   //saat kütüphanesi
#include <Adafruit_SSD1306.h>  //ekran kütüphanesi
#include <Adafruit_GFX.h>    //ekran kütüphanesi

virtuabotixRTC myRTC(6, 7, 8); // SCK , I/O , RST bağlantıları   // saat pinleri

Adafruit_SSD1306 display;

const unsigned char PROGMEM icon [] = {     //menüdeki saat resmi
  0x00, 0x1F, 0xF8, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x1F, 0xF8, 0x00,
  0x00, 0x1F, 0xF8, 0x00, 0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0xE0,
  0x00, 0x1F, 0xF8, 0xF0, 0x00, 0x7F, 0xFE, 0x70, 0x00, 0xFF, 0xFF, 0x20, 0x01, 0xF0, 0x0F, 0x80,
  0x03, 0xC1, 0x83, 0xC0, 0x03, 0x81, 0x81, 0xC0, 0x07, 0x01, 0x80, 0xE0, 0x07, 0x01, 0x80, 0xE0,
  0x0E, 0x01, 0x80, 0x70, 0x0E, 0x01, 0x80, 0x70, 0x0E, 0x03, 0xC0, 0x70, 0x0E, 0x03, 0xC0, 0x70,
  0x0E, 0x03, 0xC0, 0x70, 0x0E, 0x01, 0x80, 0x70, 0x0E, 0x00, 0x00, 0x70, 0x07, 0x00, 0x00, 0xE0,
  0x07, 0x00, 0x00, 0xE0, 0x07, 0x80, 0x01, 0xE0, 0x03, 0xC0, 0x03, 0xC0, 0x01, 0xE0, 0x07, 0x80,
  0x00, 0xF8, 0x1F, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x07, 0xE0, 0x00

};
int b = 0;
int i = 0;
int but = 0;       // press sayacı değişkeni

int led = 9;       // led pini
int buzzer = 10;   // buzzer pini

int upButton = 5;     //menü yukarı çıkma düğmesi
int downButton = 4;     //menü aşağı inme düğmesi
int selectButton = 3;   //menü seçme düğmesi
int menu = 1;        // menü değişkeni

float gerilimDeger = 0;     //sıcaklık sensörü için değişkenler
float sensorDeger = 0;     //sıcaklık sensörü için değişkenler
float sicaklikDeger = 0;   //sıcaklık sensörü için değişkenler

int poz = 1;      //kronometre değişkenleri
int broj = 1;     //kronometre değişkenleri
int kretanjeY = 30;  //kronometre değişkenleri

int sec1 = 0;  //kronometre değişkenleri
int min1 = 0;  //kronometre değişkenleri
unsigned long msec = 0; //kronometre değişkenleri
unsigned long mili = 0;  //kronometre değişkenleri
int pres = 0;  //kronometre değişkenleri
int fase = 0;  //kronometre değişkenleri
int start = 0;  //kronometre değişkenleri
unsigned long tim = 0;  //kronometre değişkenleri


int mindeger = 246;   //pil seviyesi için değişkenler
float yuzde  = 0;    //pil seviyesi için değişkenler
float deger = 0;    //pil seviyesi için değişkenler
int gerilim = 0;    //pil seviyesi için değişkenler

bool durum ;    //korometre açma değişkeni
bool saat ;      //saat ekranı için değişkeni
bool alarm ;    //alarm açma kapama değişkeni
bool game ;    //press sayacı açma değişkeni
bool uyku;    //ekran açma kapama değişkeni
bool ses ;   //ses açma kapama değişkeni
bool sabah ;
bool cakar ;
bool sos;
bool stil ;
bool stil2 ;
bool inver ;


void setup() {


  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);   //ekran boyutu
  Wire.begin();

  saat = true;    //açma kapama değişkenleri için ilk durumları
  durum = false;
  alarm = true;
  game = false;
  uyku = false;
  ses = true;
  sabah = true;
  cakar = false;
  sos = false;
  stil = true;
  stil2 = true;
  inver = true;
  //                 saniye,dakika,saat,gün,ay,yıl
   myRTC.setDS1302Time(30, 59, 22, 7, 14, 1, 2018); 

  pinMode(upButton, INPUT_PULLUP);     //düğmelerin tanımı
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);

  pinMode(led, OUTPUT);     //ledin tanımı
  pinMode(buzzer, OUTPUT);    //buzzerın tanımı

}

void loop() {

  myRTC.updateTime();   //saat yükleme
  display.display();    //ekranı çağırma

  if (uyku == true) { //uyku modunu alma
    display.clearDisplay();    //ekranı temizledik  (ekranı kapatmak anlamında)
    saat = false;   //saat modundan çıktık
    if (!digitalRead(selectButton)) {   //seçme düğmesine basarsa
      uyku = false;   //uyku modundan çık
      saat = true;     // saate geç
      while (!digitalRead(selectButton));
    }
  }

  if (alarm == true) { //alarm açıksa
    Alarm();   //alarmı çağır yani zamanı kontrol et
  } 
  if (myRTC.minutes % 60 == 0) {
    
  }
  else{
    alarm = true;
  }
   if (cakar == true) { 
      for (int c = 0 ; c <= 60 ; c++) {   //otuz kere
    digitalWrite(led, HIGH);   //fener açıyor
    delay(300);
     digitalWrite(led, LOW);
     delay(300);
     cakar = false;
}
  } 
   if (sos == true) { 
        digitalWrite(led, HIGH);   //fener açıyor
    delay(2000);
     digitalWrite(led, LOW);
      delay(1000);
      digitalWrite(led, HIGH);   //fener açıyor
    delay(1000);
     digitalWrite(led, LOW);
      digitalWrite(led, HIGH);   //fener açıyor
    delay(2000);
     digitalWrite(led, LOW);
     sos = false;
  } 
  if (sabah == true) { //alarm açıksa
    alarmb();   //alarmı çağır yani zamanı kontrol et
  }
  if (game == true) { //press oyunu için değişkeni ise oyuna geç

    if (!digitalRead(selectButton)) {  //seç düğmesine basarsa
     ses1();
      but = but + 1;  //değişkeni yani basma sayısını bir arttır
      delay(100);  //bekle 
      while (!digitalRead(selectButton));
    }
    display.clearDisplay();  //ekranı temizle
    still();   //çerçeveyi çiz
    display.setTextSize(1);  //yazı boyutu
    display.setCursor(32, 0); //yazının yazılacağı nokta
    display.print("Press Game");  //yazı
    display.setTextSize(2);  //yeni yazımızın boyutu
    display.setCursor(32, 10); //yazının yazılacağı nokta
    display.print("Bas...");//yazı
    display.setCursor(32, 30); //basma sayısının yazılacağı nokta
    display.print(but);//basma sayısı


  }
  if (saat == true) { //saat moduna geçme eğer true ise geç

    display.clearDisplay();  //ekranı temizler
    stilll();
    display.setTextSize(1);//yazı boyutu
    gerilim = analogRead(A2); //gerlimi okuduğu pin

    if (gerilim >= 300) { //pil seviyesi işlemleri
      gerilim = 300; //pil seviyesi işlemleri
    }
    if (gerilim <= 246) { //pil seviyesi işlemleri
      gerilim = 246; //pil seviyesi işlemleri
    }
    yuzde = gerilim - mindeger;  //pil seviyesi işlemleri
    deger  = (100.00 / 92.00) * yuzde; //pil seviyesi işlemleri

    if (sabah == true) { //alarm açık ise
      display.setCursor(0, 15); //ekrana on yazdır
      display.print("on");
    }

    if (sabah == false) { //alarm açık değise
      display.setCursor(0, 15); //ekrana off yazdır
      display.print("off");
    }
    if (ses == true) { //ses açık ise yani buzzerımız herzaman çalışçaksa
      display.setCursor(0, 25); //ekrana on yazdır
      display.print("on");
    }
    if (ses == false) { //sesm kapalı yani sessiz modda çalışmıcaksa
      display.setCursor(0, 25); //ekrana off yazdır
      display.print("off");
    }
    display.setCursor(0, 0);  //ekrana yazdıracaklarmız
    display.print("KaGan ARSAL");  //adımı yazdım
    display.setCursor(85, 0);
    display.print("%"); //pil seviyesi işlemleri
    display.setCursor(90, 0);
    display.print(deger);  //pil seviyesi işlemleri
    display.setCursor(35, 20);
    display.setTextSize(3);
    display.print(myRTC.hours);  //saat
    display.setCursor(65, 20);
    display.print(":");
    display.setCursor(75, 20);
    display.print(myRTC.minutes); //dakika
    display.setTextSize(1);
    display.setCursor(0, 55);
    display.print(myRTC.dayofmonth); //gün
    display.setCursor(15, 55);
    display.print("/");
    display.setCursor(25, 55);
    display.print(myRTC.month);  //ay
    display.setCursor(35, 55);
    display.print("/");
    display.setCursor(40, 55);
    display.print(myRTC.year); //yıl
    display.setCursor(100, 55);
    display.print("menu");
    if (uyku == false) { //uyku modunda değilse (üsste alacağımız mod ekran kapatma)
      if (!digitalRead(selectButton)) { //seç butonuna basılırsa
        uyku = true;  //uyku true olsun ki üstte ki if komutu gerçekleşsin
        saat = false;  //saat te gerçekleşmesin
        while (!digitalRead(selectButton));
      }
    }
    if (ses == false) { //eğerki ses kapalı ise
      if (!digitalRead(upButton)) { //yukarı butonuna basılırsa
        ses = true;//sesi aç (altlarda ses komutu var en altta)
        le();  //ledi aç kapat
        while (!digitalRead(upButton));
      }
    }
    if (ses == true) { //eğer ki ses açıksa
      if (!digitalRead(upButton)) { //üste gitme butonuna basılırsa
        ses = false;  //sesi kapat
        le();//ledi açma kapama çağır(en altta var)
        while (!digitalRead(upButton));
      }
    }

  }

  if (durum == true) {//kronometre açık ise kodları getir
    if (digitalRead(3) == 0)
    {
      if (pres == 0)
      {
        fase = fase + 1;
        pres = 1;
        if (fase > 2)
          fase = 0;
      }
    } else {
      pres = 0;
    }


    if (fase == 0)
    {
      display.clearDisplay();
      display.setFont();
      display.setCursor(10, 0);
      display.print("Bas...");
      display.drawBitmap(14, 12,  icon, 32, 32, 1);


      sec1 = 0;
      min1 = 0;
      tim = 0;
      mili = 0;
      msec = 0;
      start = 0;
    }

    if (fase == 1)
    {
      display.clearDisplay();
      display.setCursor(2, 0);
      display.print("Kronometre");
      display.drawBitmap(90, 30,  icon, 32, 32, 1);


      if (start == 0)
      {
        start = 1;
        tim = millis();
      }
      msec = (millis() - tim);



      min1 = msec / 60000;


      if ((msec / 1000) > 59)
      {
        sec1 = (msec / 1000) - (min1 * 60);
      } else {
        sec1 = msec / 1000;
      }

      mili = (msec % 1000) / 10;

      display.setCursor(0, 24);
      if (min1 <= 9)
      {
        display.print("0");
        display.print(min1);
      } else {
        display.print(min1);
      }

      display.print(":");
      if (sec1 <= 9)
      {
        display.print("0");
        display.print(sec1);
      } else {
        display.print(sec1);
      }
      display.setCursor(0, 47);

      if (mili <= 9)
      {
        display.print("0");
        display.print(mili);
      } else {
        display.print(mili);
      }





    }

    if (fase == 2)
    {
      display.clearDisplay();
      display.setFont();
      display.setCursor(0, 0);
      display.print("zaman:");
      display.drawBitmap(90, 30,  icon, 32, 32, 1);
      display.setCursor(0, 24);
      if (min1 <= 9)
      {
        display.print("0");
        display.print(min1);
      } else {
        display.print(min1);
      }

      display.print(":");

      if (sec1 <= 9)
      {
        display.print("0");
        display.print(sec1);
      } else {
        display.print(sec1);
      }
      display.setCursor(0, 47);

      if (mili <= 9)
      {
        display.print("0");
        display.print(mili);
      } else {
        display.print(mili);
      }


    }




  }//kronometre burada bitti



  if (inver == true ) {//yazı rengi ayarlama inver iki ise beyaz

    display.setTextColor(WHITE);
  }
  if (inver == false ) {//yazı rengi ayarlama inver iki ise siyah yazı beyaz arkaplan
    display.setTextColor(BLACK, WHITE);

  }

  sensorDeger = analogRead(A0);  //sıcaklık sensörü işlemleri
  gerilimDeger = (sensorDeger / 1023) * 5000; //sıcaklık sensörü işlemleri
  sicaklikDeger = gerilimDeger / 10.0; //sıcaklık sensörü işlemleri

  if (!digitalRead(downButton)) {//menü gezinme aşağı düğmesine tıklanırsa 
    ses1();  //ses çal veya çalma ayarı çağır (bu da en altta)
    durum = false;   //modlardan çık
    saat = false;    //modlardan çık
    game = false;     //modlardan çık
    menu++;   //menü değişkeni arttır
    updateMenu();  
    delay(300);  //bekle
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(upButton)) {   //menü gezinme yukarı düğmesine tıklanırsa 
    ses1();   //ses çal veya çalma ayarı çağır
    durum = false; //modlardan çık
    game = false;   //modlardan çık
    menu--; //  menü değişkeni azalt
    updateMenu();
    delay(300);
    digitalWrite(buzzer, LOW);
    digitalWrite(led, LOW);
    while (!digitalRead(upButton));
  }
  if (!digitalRead(selectButton)) { //menü gezinme seç düğmesine tıklanırsa
    executeAction();   //actionu gerçekleştir
    delay(300);
    while (!digitalRead(selectButton));
  }




}
void updateMenu() {  //menüler
  switch (menu) {
    case 0:
      menu = 0;
      break;
    case 1:
      saat = true ;
      break;
    case 2:
      display.clearDisplay();
      still();
      display.setCursor(32, 0);
      display.print("Ana Menu");  //menü başlığı
      display.setCursor(0, 20);
      display.print(">Kronometre");   //seçili uygulama
      display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      display.setCursor(0, 30);
      display.print(" Sicaklik");   //
      break;
    case 3:
      display.clearDisplay();
      still();
      display.setCursor(32, 0);
      display.print("Ana Menu");  //menü başlığı
      display.setCursor(0, 20);
      display.print(" Kronometre");
      display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      display.setCursor(0, 30);
      display.print(">Sicaklik");   //seçili uygulama
      break;
     case 4:
      display.clearDisplay();
      still();
      display.setCursor(32, 0);
      display.print("Ana Menu");  //menü başlığı
      display.setCursor(0, 20);
      display.print(">Press Game");
      display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      display.setCursor(0, 30);
      display.print(" Alarm on/off");   //seçili uygulama 
       display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      break;
    case 5:
      display.clearDisplay();
      still();
       display.setCursor(32, 0);
      display.print("Ana Menu");  //menü başlığı
      display.setCursor(0, 20);
      display.print(" Press Game");
      display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      display.setCursor(0, 30);
      display.print(">Alarm on/off");   //seçili uygulama
       display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      break;
    case 6:
         display.clearDisplay();
      still();
      display.setCursor(32, 0);
      display.print("Fener Menu");   //menü başlığı
      display.setCursor(0, 20);
      display.print(">Fener Ac");   //seçili uygulama
      display.setCursor(0, 30);
      display.print(" Cakar");
      display.setCursor(0, 40);
      display.print(" Sos isigi");
       display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      break;
    case 7:
       display.clearDisplay();
      still();
      display.setCursor(32, 0);
      display.print("Fener Menu");   //menü başlığı
      display.setCursor(0, 20);
      display.print(" Fener Ac");   //seçili uygulama
      display.setCursor(0, 30);
      display.print(">Cakar");
      display.setCursor(0, 40);
      display.print(" Sos isigi");
       display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      break;
    case 8:
      display.clearDisplay();
      still();
      display.setCursor(32, 0);
      display.print("Fener Menu");   //menü başlığı
      display.setCursor(0, 20);
      display.print(" Fener Ac");   //seçili uygulama
      display.setCursor(0, 30);
      display.print(" Cakar");
      display.setCursor(0, 40);
      display.print(">Sos isigi");
       display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      break;
    case 9:
      display.clearDisplay();
      still();
      display.setCursor(32, 0);
      display.print("Menu");   //menü başlığı
      display.setCursor(0, 20);
      display.print(">Invert on/off");
      display.setCursor(0, 30);
      display.print(" Ekran Stili");   //seçili uygulama
      display.setCursor(0, 40);
      display.print(" Sifirla");
       display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      break;
    case 10:
      display.clearDisplay();
      still();
      display.setCursor(32, 0);
      display.print("Menu");   //menü başlığı
      display.setCursor(0, 20);
      display.print(" Invert on/off");
      display.setCursor(0, 30);
      display.print(">Ekran Stili");
      display.setCursor(0, 40);
      display.print(" Sifirla");   //seçili uygulama
       display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      break;
     case 11:
      display.clearDisplay();
      still();
      display.setCursor(32, 0);
      display.print("Menu");   //menü başlığı
      display.setCursor(0, 20);
      display.print(" Invert on/off");
      display.setCursor(0, 30);
      display.print(" Ekran Stili");
      display.setCursor(0, 40);
      display.print(">Sifirla");   //seçili uygulama
       display.drawBitmap(90, 30,  icon, 32, 32, 1);   //saat resmi
      break;   
    case 12:
      menu = 1;
      break;
  }
}

void executeAction() {
  switch (menu) {
    case 2:     //tek tek menü ve seçenek eşleştirmesi
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action4();
      break;
    case 5:
      action5();
      break;
    case 6:
      action6();
      break;
    case 7:
      action7();
      break;
    case 8:
      action8();
      break;
    case 9:
      action9();
      break;
    case 10:
      action10();
      break;
     case 11:
      action11();
      break;
      

  }
}
void action2() {  //kronometre durumunu true yapıyor void loop ta durum true ise kronometre çalışır
  durum = true;
}
void action3() { //sıcaklık için
  still();
  display.clearDisplay();
  display.setCursor(32, 0);
  display.print("Sicaklik");  //başlık
  display.setCursor(0, 20);
  display.print("Ortam sicakligi :");   
  display.setCursor(0, 30);
  display.print(sicaklikDeger);  //sıcaklık


}
void action4() {
   game = true;   //oyun açıyor void loop ta eğer game true ise press sayacı çalışmaya başlıyor
  le();

}
void action5() { //alarm kapıyor void loop ta alarmı çağırmıyor
    b = 1 ; 
   
    if (sabah == true) { //eğer ki ses açıksa  
        sabah = false;  //sesi kapat 
        b = 0;
    }
    if( b == 1){ 
     if (sabah == false) { //eğerki ses kapalı ise   
        sabah = true;//sesi aç (altlarda ses komutu var en altta)  
    }
    }
     b = 0;
    le();
}
void action6() {
  digitalWrite(led, HIGH);   //fener açıyor

}
void action7() {
  cakar = true;
 
}

void action8() {
  sos = true;
 
}
void action9() {
  
     b = 1 ; 
   if (inver == false) { //eğerki ses kapalı ise   
        inver = true;//sesi aç (altlarda ses komutu var en altta) 
          b = 0 ; 
    }
    le();//ledi açma kapama çağır(en altta var)   
    if( b == 1){ 
    if (inver == true) { //eğer ki ses açıksa  
        inver = false;  //sesi kapat 
    }}
 b = 0 ; 
}
void action10() {
    b = 1 ; 
 if (stil == false) { //eğerki ses kapalı ise   
        stil = true;//sesi aç (altlarda ses komutu var en altta)
       b = 0 ; 
    }
     if( b == 1){ 
    if (stil == true) { //eğer ki ses açıksa  
        stil = false;  //sesi kapat
    }
    }
     b = 1 ; 
     if (stil2 == false) { //eğerki ses kapalı ise   
        stil2 = true;//sesi aç (altlarda ses komutu var en altta)
         b = 0 ;   
    }
     if( b == 1){ 
    if (stil2 == true) { //eğer ki ses açıksa  
        stil2 = false;  //sesi kapat
    }
    }
le();//ledi açma kapama çağır(en altta var)
  b = 0 ;
}
void action11() {
   saat = true;    //açma kapama değişkenleri için ilk durumları
  durum = false;
  alarm = true;
  game = false;
  uyku = false;
  ses = true;
  sabah = false;
  cakar = false;
  sos = false;
  stil = true;
  stil2 = true;
  but = 0;

}
void Alarm() {  //alarm her ssat başı çalıyor


  if (myRTC.minutes % 60 == 0) {  //dakika altmışa kalansız bölünürse
       ses1();
      delay (200);
      digitalWrite(buzzer, LOW);    //ses çalmaz
      digitalWrite(led, LOW);
      delay (200);
      ses1();
      delay (200);
      digitalWrite(buzzer, LOW);    //ses çalmaz
      digitalWrite(led, LOW);
     alarm = false;
    }
    else{
      alarm = true;
  }
}

void alarmb() {  //alarm her çalıyor
   if (myRTC.hours == 18  &&  myRTC.minutes % 60 == 0) {  //dakika altmışa kalansız bölünürse
    alarm = false;
    saat = false;   //öteki durmları iptal eder
    durum = false;
    game = false;
    uyku = false;
    for (int a = 1 ; a <= 30 ; a++) {   //otuz kere
      display.clearDisplay();  
      display.setTextSize(3);  
      display.setCursor(0, 30);
      display.print("ALARM!");  //ekranda alarm yazar
      display.setTextSize(1);
      display.setCursor(0, 55);
      display.print("kapat");  //kapat düğmesi için
      ses1();   //ses açma kapma komuduna göre ses ya çalar ya da çalmaz
      delay (100);
      digitalWrite(buzzer, LOW);    //ses çalmaz
      digitalWrite(led, LOW);
      if (!digitalRead(selectButton)) {  //eğer alarm çalarken seçme butonuna tıklanırsa alarm çalmayı iptal eder
        digitalWrite(buzzer, LOW);
        digitalWrite(led, LOW);
       sabah = false;
      saat = true;
       alarm = true;
        delay(100);
        while (!digitalRead(upButton));
      }
    }
  }

  if (myRTC.hours == 13  &&  myRTC.minutes % 60 == 0) {  //dakika altmışa kalansız bölünürse
    alarm = false;
    saat = false;   //öteki durmları iptal eder
    durum = false;
    game = false;
    uyku = false;
    for (int a = 1 ; a <= 30 ; a++) {   //otuz kere
      display.clearDisplay();  
      display.setTextSize(3);  
      display.setCursor(0, 30);
      display.print("ALARM!");  //ekranda alarm yazar
      display.setTextSize(1);
      display.setCursor(0, 55);
      display.print("kapat");  //kapat düğmesi için
      ses1();   //ses açma kapma komuduna göre ses ya çalar ya da çalmaz
      delay (100);
      digitalWrite(buzzer, LOW);    //ses çalmaz
      digitalWrite(led, LOW);
      if (!digitalRead(selectButton)) {  //eğer alarm çalarken seçme butonuna tıklanırsa alarm çalmayı iptal eder
        digitalWrite(buzzer, LOW);
        digitalWrite(led, LOW);
       sabah = false;
      saat = true;
       alarm = true;
        delay(100);
        while (!digitalRead(upButton));
      }
    }
  }
   if (myRTC.hours == 23  &&  myRTC.minutes % 60 == 0) {  //dakika altmışa kalansız bölünürse
    alarm = false;
    saat = false;   //öteki durmları iptal eder
    durum = false;
    game = false;
    uyku = false;
    for (int a = 1 ; a <= 30 ; a++) {   //otuz kere
      display.clearDisplay();  
      display.setTextSize(3);  
      display.setCursor(0, 30);
      display.print("ALARM!");  //ekranda alarm yazar
      display.setTextSize(1);
      display.setCursor(0, 55);
      display.print("kapat");  //kapat düğmesi için
      ses1();   //ses açma kapma komuduna göre ses ya çalar ya da çalmaz
      delay (100);
      digitalWrite(buzzer, LOW);    //ses çalmaz
      digitalWrite(led, LOW);
      if (!digitalRead(selectButton)) {  //eğer alarm çalarken seçme butonuna tıklanırsa alarm çalmayı iptal eder
        digitalWrite(buzzer, LOW);
        digitalWrite(led, LOW);
       sabah = false;
        saat = true;
         alarm = true;
        delay(100);
        while (!digitalRead(upButton));
      }
    }
  }
   if (myRTC.hours  == 13  &&  myRTC.minutes % 60 == 0) {  //dakika altmışa kalansız bölünürse
    alarm = false;
    saat = false;   //öteki durmları iptal eder
    durum = false;
    game = false;
    uyku = false;
    for (int a = 1 ; a <= 30 ; a++) {   //otuz kere
      display.clearDisplay();  
      display.setTextSize(3);  
      display.setCursor(0, 30);
      display.print("ALARM!");  //ekranda alarm yazar
      display.setTextSize(1);
      display.setCursor(0, 55);
      display.print("kapat");  //kapat düğmesi için
      ses1();   //ses açma kapma komuduna göre ses ya çalar ya da çalmaz
      delay (100);
      digitalWrite(buzzer, LOW);    //ses çalmaz
      digitalWrite(led, LOW);
      if (!digitalRead(selectButton)) {  //eğer alarm çalarken seçme butonuna tıklanırsa alarm çalmayı iptal eder
        digitalWrite(buzzer, LOW);
        digitalWrite(led, LOW);
       sabah = false;
        saat = true;
         alarm = true;
        delay(100);
        while (!digitalRead(upButton));
      }
    }
  }
  
}
void ses1() {  //ses komudumuz
  if ( ses == false) {  //ses kapalı ise
    digitalWrite(buzzer, LOW);  //buzzer çalmasın
    digitalWrite(led, LOW);   //led yanmasın
  }
  if (ses == true) {  //ses açık ise
    digitalWrite(buzzer, HIGH);   //buzzer çalsın
    digitalWrite(led, HIGH);      //led yansın
  }
}
void le () {
  digitalWrite(led, HIGH);
  delay(300);
  digitalWrite(led, LOW);
}
void still(){
 if ( stil == false) {  //ses kapalı ise
    
  }
  if (stil == true) {  //ses açık ise
  display.setTextSize(1);
  display.drawRect(0, 0, 128, 64, WHITE);
   display.setCursor(40, 50);
  display.print("ARSAL");   //menü dekoru
  }
}
void stilll(){
 if ( stil2 == false) {  //ses kapalı ise
    
  }
  if (stil2 == true) {  //ses açık ise
  display.setTextSize(1);
  display.drawRect(0, 15, 128, 64, WHITE);
  display.setTextSize(1);
  display.drawRect(0, 50, 128, 64, WHITE);
  }
}
