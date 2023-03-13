#include <Servo.h>
#include <SoftwareSerial.h>   // TX RX software library for bluetooth

Servo r1; 
Servo l2;

int bluetoothTx = 0; // bluetooth tx to 0 pin
int bluetoothRx = 1; // bluetooth rx to 1 pin
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

int pos = 0;
int liml=2, limh=83; //angle limits for synchronization
int del=20; //default delay between steps
int del2=200; //delay between half cycle
int endCount=50, count=0; //cycle count
int val=0; //variable to receive bluetooth input

void setup() {
  r1.attach(9);
  l2.attach(10);
  r1.write(liml);
  l2.write(limh+90);
  //Setup usb serial connection to computer
  Serial.begin(9600);
  //Setup Bluetooth serial connection to android
  bluetooth.begin(9600);
  delay(5000);
  delay(5000);
  delay(5000);
}

void loop() {
  //Read from bluetooth and write to usb serial
  if(bluetooth.available()>=2) {
    val=bluetooth.read();
    if(val==1){ //slow
      del=40;
    }
    if(val==2){ //medium
      del=20;
    }
    if(val==3){ //fast
      del=10;
    }
    if(val>=50 && val<=250){ //cycle limit
      endCount=val;
    }
  }
  
  Serial.println(val);
  Serial.println(del);
  Serial.println(endCount);
  
  if (count<endCount) {
    r1.write(liml);
    l2.write(limh+90);
    delay(del2);
    for (pos = liml; pos <=limh; pos += 1) {
      r1.write(pos);
      l2.write(limh+90-pos+liml);
      delay(del);
    }
    r1.write(limh);
    l2.write(liml+90);
    delay(del2);
    for (pos = limh; pos >= liml; pos -= 1) {
      l2.write(limh+90-pos+liml);
      r1.write(pos);
      delay(del);
    }
    count++;
  }
}
