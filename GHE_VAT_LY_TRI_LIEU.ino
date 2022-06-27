#include <SimpleTimer.h>
#include <Servo.h>
const int bt_up = 19;
const int bt_down = 20;
const int bt_mode1 = 2;
const int bt_mode2 = 3;
const int bt_mode3 = 18;
int mode=0;
int setups=0;
int speeds=10;
int num=0;
unsigned long timesDelay=millis();
const int sv_nanggoiPin=4; 
const int sv_nangduiPin=5; 
Servo sv_nanggoi; 
Servo sv_nangdui; 
const int goc_nanggoi=111;
const int goc_nangdui=111;
SimpleTimer timer;
int timerID1;

void setup() {
  Serial.begin(9600);
  pinMode(bt_up,INPUT_PULLUP);
  pinMode(bt_down,INPUT_PULLUP);
  pinMode(bt_mode1,INPUT_PULLUP);
  pinMode(bt_mode2,INPUT_PULLUP);
  pinMode(bt_mode3,INPUT_PULLUP);
  sv_nanggoi.attach(sv_nanggoiPin);  
  sv_nangdui.attach(sv_nangduiPin); 
  delay(500);
  sv_nanggoi.write(0);
  sv_nangdui.write(0);
  sv_nanggoi.detach();
  sv_nangdui.detach(); 
  attachInterrupt(digitalPinToInterrupt(bt_up), ISR_speeds, FALLING);
  attachInterrupt(digitalPinToInterrupt(bt_down), ISR_speeds, FALLING);
  attachInterrupt(digitalPinToInterrupt(bt_mode1), ISR_mode, FALLING);
  attachInterrupt(digitalPinToInterrupt(bt_mode2), ISR_mode, FALLING);
  attachInterrupt(digitalPinToInterrupt(bt_mode3), ISR_mode, FALLING);
}

void loop() {
  timer.run();
  if(mode==1){
    long times=millis();
    for(int i=0;i<goc_nanggoi;++i){
      sv_nanggoi.write(i);
      delay(60000/(speeds*goc_nanggoi*2));
      timer.run();
    }
    for(int i=goc_nanggoi;i>0;--i){
      sv_nanggoi.write(i);
      delay(60000/(speeds*goc_nanggoi*2));
      timer.run();
    }
    if(mode==0) sv_nanggoi.detach();
    num++;
    Serial.println("T" + String(millis()-times)+"\tS"+String(speeds));
  }else if(mode==2){
    long times=millis();
    for(int i=0;i<goc_nangdui;++i){
      sv_nangdui.write(i);
      delay(60000/(speeds*goc_nangdui*2));
      timer.run();
    }
    for(int i=goc_nangdui;i>0;--i){
      sv_nangdui.write(i);
      delay(60000/(speeds*goc_nangdui*2));
      timer.run();
    }
    if(mode==0) sv_nangdui.detach();
    num++;
    Serial.println("T" + String(millis()-times)+"\tS"+String(speeds));
  }else if(mode==3){
    long times=millis();
    for(int i=0;i<goc_nanggoi;++i){
      sv_nanggoi.write(i);
      delay((60000/(speeds*goc_nanggoi*2))/2);
      timer.run();
    }
    for(int i=0;i<goc_nangdui;++i){
      sv_nangdui.write(i);
      delay((60000/(speeds*goc_nangdui*2))/2);
      timer.run();
    }
    for(int i=goc_nangdui;i>0;--i){
      sv_nangdui.write(i);
      delay((60000/(speeds*goc_nangdui*2))/2);
      timer.run();
    }
    for(int i=goc_nanggoi;i>0;--i){
      sv_nanggoi.write(i);
      delay((60000/(speeds*goc_nanggoi*2))/2);
      timer.run();
    }
    if(mode==0){
      sv_nanggoi.detach();
      sv_nangdui.detach();
    }
    num++;
    Serial.println("T" + String(millis()-times)+"\tS"+String(speeds));
  }
}
void ISR_speeds(){
  if(millis()-timesDelay>500){
    if(digitalRead(bt_up)==LOW){
      if(speeds<60){
        speeds+=5;
      }
    }else if(digitalRead(bt_down)==LOW){
      if(speeds>10){
        speeds-=5;
      }
    }
    Serial.println("Speeds: " +String(speeds));
    if(timer.isEnabled(timerID1)){
      num=0;
      timer.restartTimer(timerID1);
    }
    timesDelay=millis();
  }
}
void ISR_mode(){
  if(millis()-timesDelay>500){
    if(digitalRead(bt_mode1)==LOW){
      if(mode!=1){
        mode=1;
        sv_nanggoi.attach(sv_nanggoiPin);
        delay(500);
      }else{
        mode=0;
      }
    }else if(digitalRead(bt_mode2)==LOW){
      if(mode!=2){
        mode=2;
        sv_nangdui.attach(sv_nangduiPin);
        delay(500);
      }else{
        mode=0;
      }
    }else if(digitalRead(bt_mode3)==LOW){
      if(mode!=3){
        mode=3; 
        sv_nanggoi.attach(sv_nanggoiPin);
        sv_nangdui.attach(sv_nangduiPin);
        delay(500);
      }else{
        mode=0;
      }
    }
    Serial.println("Mode: " + String(mode));
    if(mode==0){
      if(timer.isEnabled(timerID1)){
        timer.deleteTimer(timerID1);
      }
    }else{
      timerID1 = timer.setInterval(60000,handleTimerID1);
      num=0;
    }
    timesDelay=millis();
  }
}
void handleTimerID1(){
  Serial.println("Số lần/phút: " + String(num));
  num=0;
}
