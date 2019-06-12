/*---------------------------------------------SMART HOME SYSTEM(AKILLI EV SİSTEMİ)-----------------------------------------------

  AUTHOR:ARIF MANDAL
  DATE:06.06.2019*/


//LCD EKRAN
#include <LiquidCrystal.h>
LiquidCrystal myLcd(12, 11, 5, 4, 3, 2);//lcd kütüphanemizi dahil ettik ve pin atamalarını yaptık.

//NEM VE ISI SENSORU
#include<dht11.h>
#define DHT11PIN 7//dht11 kütüphanemizi dahil ettik ve pin 7 olarak atadık.
dht11 DHT11;//dht kütüphanesinden obje oluşturduk.

//LDR
int ldrPin = A0;//ldr pinimiz
int ldrValue = 0;//ilk değeri 0 olarak atadık.
String oneri = "";//Işık seviyesine göre tavsiye tutan değişken

//POTANSIYOMETRE
int potPin = A1;//potansiyometre pinimiz
int potValue = 0;//ilk değeri 0 olarak atadık.

//YANGIN SENSOR
int firePin = A2;//yangın sensörü pinimiz.
int fireValue = 0;//ilk değeri 0 olarak atadık.

//BUZER VE LED
int buzerPin = 6;//yangın uyarma sistemi için buzer ve led pinlerimiz
int ledPin = 8;

//BUTON
int menuButton = 9;// menü, aşağı, yukarı buton pinlerimiz.
int asagiButton = 10;
int yukariButton = 13;

int menuValue = 0;// buton pinlerimizin ilk değerlerinide 0 olarak atadık.
int asagiValue = 0;
int yukariValue = 0;

int durum = 0;// kalıcı buton için durum değişkeni oluşturduk.


void setup() {
  Serial.begin(9600);// kontroller için serial ekranımızı başlattık.
  myLcd.begin(16, 2);//lcd ekranımızı başlattık.
  pinMode(ldrPin, INPUT);//ldr pinimiz giriş
  pinMode(potPin, INPUT);//potansimetre pinimiz giriş
  pinMode(buzerPin, OUTPUT);// buzer çıkış
  pinMode(ledPin, OUTPUT);// led çıkış
  pinMode(menuButton, INPUT);// buton pinleri giriş pini olarak atadık.
  pinMode(asagiButton, INPUT);
  pinMode(yukariButton, INPUT);

}

void loop() {
  //dth11
  int DTH11value = DHT11.read(DHT11PIN);// dht sensörümüzü okuduk.
  int nem = DHT11.humidity; // nem değerini aldık.
  int sicaklik = DHT11.temperature;// sıcaklık değerini aldık.

  //ldr
  ldrValue = analogRead(ldrPin);//ldr sensörümüzü okuduk.
  if (ldrValue < 30) {//ısık 30 değerinden düşükse uyarı ışık düşük
    oneri = "LOW LIGHT";
  } else if (ldrValue >= 30 && ldrValue <= 150) {//150 ile 30 arasında ise uygun ışık.
    oneri = "SENSIBLE...";

  } else {//150 den yüksekse rahatsız edici uyarısında bulunuyor.
    oneri = "DISTURB...";

  }

  //NOTE: DEGERLERİ KONTROL ETMEK İÇİN SERİAL MONİTÖRDE BAKABİLİRSİNİZ.

  //pot
  //potValue=analogRead(potPin);// potansiyometreden okunan değer kontrol için kullanabilirsiniz.
  //Serial.println(potValue);



  //Fire Sensor
  fireValue = analogRead(firePin);// ateş algılayıcı sensörümüzü okuduk.(analog olarak)
  //Serial.println(fireValue);// kontrol amaçlı değerimizi yazdırdık.
  if (fireValue < 80) {// eğer ateş sensörü değerimiz 80 den küçükse buzer ve led çalışıyor.
    digitalWrite(buzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    myLcd.clear();
    myLcd.setCursor(0, 0);//lcd ekrandaki yerini ayarladık.
    myLcd.print("FIRE ALARM!!!");//yangin alarmi uyarisi.
    delay(1000);
    myLcd.clear();


  } else {// diğer durumlarda buzer ve ledimiz durgun halde.
    digitalWrite(buzerPin, LOW);
    digitalWrite(ledPin, LOW);

  }

  //buton
  menuValue = digitalRead(menuButton);// butonlarımızı okuduk.
  //Serial.println(menuValue);// kontrol için yorumu kaldırıp bakabilirsiniz.
  asagiValue = digitalRead(asagiButton);
  //Serial.println(asagiValue);
  yukariValue = digitalRead(yukariButton);
  //Serial.println(yukariValue);

  if (menuValue == 1) {//eğer butona basıldıysa

    if (durum == 0) {// durum değişkenimizde 0 ise;
      durum = 1;//durumu 1 yapıyoruz birdaha basınca çıkmak için
      myLcd.setCursor(0, 0);//lcd ekrandaki yerini ayarladık.
      myLcd.print("1)TEMP-HUM:");//menümüzü oluşturduk.
      myLcd.setCursor(0, 1);//alt kısma geçtik
      myLcd.print("2)LIGHT AMOUNT:");//kalan yazımızı yazdık.


    } else {// diğer durumda bir daha basılıp çekilirse
      durum = 0;//durum 0 yap
      myLcd.clear();// ekranı temizle
      myLcd.setCursor(3, 0);// lcd deki yerini belirle
      myLcd.print("GOOD DAY");// yazını yazdır.
      myLcd.setCursor(0, 1);// diğer kısımda da lcd deki yerini belirle
      myLcd.print("CHOOSE:1) ve 2) ");//yazını yazdır.
      delay(1000);//kapanması için 1 sn bekle.
      myLcd.clear();//ekranı temizle.

    }
    delay(150);// butona basıldığının anlaşılması için 150ms bekle

  }

  if (asagiValue == 1) {// eğer sol taraftaki butona basıldıysa sıcaklık ve nem değerlerini ekranda göster.
    myLcd.clear();
    myLcd.setCursor(0, 0);
    myLcd.print("TEMPERATURE:");
    myLcd.print(sicaklik);
    myLcd.setCursor(0, 1);
    myLcd.print("HUMIDITY:");
    myLcd.print(nem);


  } else if (yukariValue == 1) {// eğer sağ taraftaki butona basıldıysa ışık miktarını ekranda göster.
    myLcd.clear();
    myLcd.setCursor(0, 0);
    myLcd.print("LIGHT AMOUNT:");
    myLcd.print(ldrValue);
    myLcd.setCursor(0, 1);
    myLcd.print(oneri);// kullanıcıya ışık miktarına göre uyarı veriyor.

  }

}

//THANK YOU...
