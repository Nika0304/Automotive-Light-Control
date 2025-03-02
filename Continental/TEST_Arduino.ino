#include <Arduino.h>
#include "LiquidCrystal.h"
#define LOW_BEAM_INPUT 2
#define POSITION_INPUT 3
#define LEFT_TURN_INPUT 4
#define RIGHT_TURN_INPUT 5
#define IGNITION_INPUT 6

#define LOW_BEAM_LEFT_OUT 8
#define LOW_BEAM_RIGHT_OUT 9
#define POSITION_OUT 10
#define LEFT_TURN_OUT 11
#define RIGHT_TURN_OUT 12

#define ENTER_BUTTON 0
#define SCROLL_BUTTON 1

LiquidCrystal lcd(0);
unsigned long timer;
unsigned long timer2;
int test;
int menuIndex = 0;
const int numTests = 6;
int ScrollValue = 0;
int ButtonValue = 0;




void setup() {
  pinMode(LOW_BEAM_INPUT, OUTPUT);
  pinMode(POSITION_INPUT, OUTPUT);
  pinMode(LEFT_TURN_INPUT, OUTPUT);
  pinMode(RIGHT_TURN_INPUT, OUTPUT);
  pinMode(IGNITION_INPUT, OUTPUT);
  
  pinMode(LOW_BEAM_LEFT_OUT, INPUT);
  pinMode(LOW_BEAM_RIGHT_OUT, INPUT);
  pinMode(POSITION_OUT, INPUT);
  pinMode(LEFT_TURN_OUT, INPUT);
  pinMode(RIGHT_TURN_OUT, INPUT);

  digitalWrite(LOW_BEAM_INPUT, HIGH);
  digitalWrite(POSITION_INPUT, HIGH);
  digitalWrite(LEFT_TURN_INPUT, HIGH);
  digitalWrite(RIGHT_TURN_INPUT, HIGH);
  digitalWrite(IGNITION_INPUT, HIGH);

  pinMode(ENTER_BUTTON, INPUT_PULLUP);
  pinMode(SCROLL_BUTTON, INPUT_PULLUP);


  // Start serial communication 
  Serial.begin(9600);
  lcd.begin(16, 2);

  lcd.setBacklight(1);

  // Clear the screen and display the initial menu
  clearScreen();
  displayMenu();
}

void loop() {
    if (digitalRead(SCROLL_BUTTON) == LOW && ScrollValue == 0) { // Down
      menuIndex = (menuIndex + 1) % numTests;
      ScrollValue = 1;
      displayMenu();
    }
    if(digitalRead(SCROLL_BUTTON) == HIGH){
      ScrollValue = 0;
    }
    if (digitalRead(ENTER_BUTTON) == LOW && ButtonValue == 0) {
      ButtonValue = 1;  // Enter
      runSelectedTest();
      delay(1000); // Delay to allow user to see the test result before redisplaying the menu
      clearScreen();
      displayMenu();
    }
    if(digitalRead(ENTER_BUTTON) == HIGH){
      ButtonValue = 0;
  }
}

void clearScreen() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  // Clear screen and move cursor to home position
}


void displayMenu() {
  clearScreen();
  
    switch (menuIndex) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.print(">Test 1");
        lcd.setCursor(0, 1);
        lcd.print("Test 2");
        break;
      case 1:
        lcd.setCursor(0, 0);
        lcd.print(">Test 2");
        lcd.setCursor(0, 1);
        lcd.print("Test 3");
        break;
      case 2:
        lcd.setCursor(0, 0);
        lcd.print(">Test 3");
        lcd.setCursor(0, 1);
        lcd.print("Test 4");
        break;
      case 3:
        lcd.setCursor(0, 0);
        lcd.print(">Test 4");
        lcd.setCursor(0, 1);
        lcd.print("Test 5");
        break;
      case 4:
        lcd.setCursor(0, 0);
        lcd.print(">Test 5");
        lcd.setCursor(0, 1);
        lcd.print("Test 6");
        break;
      case 5:
        lcd.setCursor(0, 0);
        lcd.print(">Test 6");
        lcd.setCursor(0, 1);
        lcd.print("Test 1");
        break;
    }
}


void runSelectedTest() {
  switch (menuIndex) {
    case 0:
      lcd.print("Running Test 1...");
      test1();
      break;
    case 1:
      lcd.print("Running Test 2...");
      test2();
      break;
    case 2:
      lcd.print("Running Test 3...");
      test3();
      break;
    case 3:
      lcd.print("Running Test 4...");
      test4();
      break;
    case 4:
      lcd.print("Running Test 5...");
      test5();
      break;
    case 5:
      lcd.print("Running Test 6...");
      test6();
      break;
  }
}

void test1() {
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print("Test 1");
  digitalWrite(LOW_BEAM_INPUT, HIGH);
  digitalWrite(POSITION_INPUT, HIGH);
  delay(100);
  digitalWrite(POSITION_INPUT, LOW);
  timer = millis();
  while(digitalRead(POSITION_OUT) == LOW && millis() - timer < 2000){
    timer2=0;
  }
  timer2 = millis();
  if(timer2 - timer<=500){
    lcd.setCursor(0, 1);
    lcd.print("Succesful");
    //lcd.print(timer2 - timer);
  }
  else {
    lcd.setCursor(0, 1);
    lcd.print("Failed");
  }
  delay(2000);
  digitalWrite(POSITION_INPUT, HIGH);
}

void test2() {
  unsigned long timer2;
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print("Test 2");
  lcd.setCursor(0, 1);
  digitalWrite(IGNITION_INPUT,LOW);
  digitalWrite(LEFT_TURN_INPUT,HIGH);
  delay(1000); // Simulate a test taking some time
  digitalWrite(LEFT_TURN_INPUT,LOW);
  unsigned long startTime=millis();
  while(digitalRead(LEFT_TURN_OUT)==LOW && millis()- startTime<100){
  }
  unsigned long timer1=millis();
  if (startTime-timer1>100){
    //lcd.print("Test 2 Failed because the flasher does not start ");
    lcd.print("Failed");
    digitalWrite(LEFT_TURN_INPUT,HIGH);
    digitalWrite(IGNITION_INPUT,HIGH);
    return;
  }
  //lcd.print("Flahser is on");
  short count=0;
  while(count<3){

    while(digitalRead(LEFT_TURN_OUT)==HIGH && millis() - timer1<=440){
    }
    timer2=millis();
    if (timer2-timer1>440 || timer2-timer1<360){
        //lcd.print("timpul de executie");
        //lcd.print(timer2-timer1);
        //lcd.print("numarul cicluluui");
        //lcd.print(count);
        //lcd.print("Test 2 Failed because light face is not between 360 and 440");
        lcd.print("Failed");
        digitalWrite(LEFT_TURN_INPUT,HIGH);
        digitalWrite(IGNITION_INPUT,HIGH);
        return;
    }
    if(count==0){
      digitalWrite(LEFT_TURN_INPUT,HIGH);
    }

    while(digitalRead(LEFT_TURN_OUT)==LOW && millis() - timer2 <=440){
    }
    timer1=millis();
    if ((timer1-timer2>440 || timer1-timer2<360) && count<2 ){
        //lcd.print("Test 2 Failed because dark face is not between 360 and 440");
        lcd.print("Failed");

        digitalWrite(LEFT_TURN_INPUT,HIGH);
        digitalWrite(IGNITION_INPUT,HIGH);
        return;
    }
    count++;
  }
  if(digitalRead(LEFT_TURN_OUT)==LOW){
     lcd.print("Succesful");
     digitalWrite(LEFT_TURN_INPUT,HIGH);
     digitalWrite(IGNITION_INPUT,HIGH);
  }
  else{
    //lcd.print("Test 2 Failed because the flasher does not end after 3 cicle");
    lcd.print("Failed");
    digitalWrite(LEFT_TURN_INPUT,HIGH);
    digitalWrite(IGNITION_INPUT,HIGH);
  }
  
}

void test21() {
  unsigned long timer2;
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print("Test 2");
  lcd.setCursor(0, 1);
  digitalWrite(IGNITION_INPUT,LOW);
  digitalWrite(RIGHT_TURN_INPUT,HIGH);
  delay(1000); // Simulate a test taking some time
  digitalWrite(RIGHT_TURN_INPUT,LOW);
  unsigned long startTime=millis();
  while(digitalRead(RIGHT_TURN_OUT)==LOW && millis()- startTime<100){
  }

  unsigned long timer1=millis();

  if (startTime-timer1>100){
    //lcd.print("Test 2 Failed because the flasher does not start ");
    Serial.println("Test 2 Failed because the flasher does not start ");
    lcd.print("Failed 1");
    lcd.print(startTime-timer1);
    digitalWrite(RIGHT_TURN_INPUT,HIGH);
    digitalWrite(IGNITION_INPUT,HIGH);
    return;
  }

  Serial.println("digitalRead(RIGHT_TURN_OUT): ");
  Serial.println(digitalRead(RIGHT_TURN_OUT));

  //lcd.print("Flahser is on");
  short count=0;

  while(count<3){
    

    while(digitalRead(RIGHT_TURN_OUT)==HIGH && millis() - timer1<=440){
    }
    timer2=millis();
    unsigned long result = timer2-timer1;
    Serial.println("timer2-timer1: ");
    Serial.print(timer1);
    Serial.print(timer2);
    if (timer2-timer1>440 || timer2-timer1<360){
        //lcd.print("timpul de executie");
        //lcd.print(timer2-timer1);
        //lcd.print("numarul cicluluui");
        //lcd.print(count);
        //lcd.print("Test 2 Failed because light fase is not between 360 and 440");
        lcd.print("Failed 2");
        digitalWrite(RIGHT_TURN_INPUT,HIGH);
        digitalWrite(IGNITION_INPUT,HIGH);
        return;
    }
    if(count==0){
      digitalWrite(RIGHT_TURN_INPUT,HIGH);
    }
    
    while(digitalRead(RIGHT_TURN_OUT)==LOW && millis() - timer2 <=440){
    }
    timer1=millis();
    if ((timer1-timer2>440 || timer1-timer2<360) && count<2 ){
        //lcd.print("Test 2 Failed because dark face is not between 360 and 440");
        lcd.print("Failed 3 ");

        digitalWrite(RIGHT_TURN_INPUT,HIGH);
        digitalWrite(IGNITION_INPUT,HIGH);
        return;
    }
    count++;
  }
  if(digitalRead(LEFT_TURN_OUT)==LOW){
     lcd.print("Succesful");
     digitalWrite(RIGHT_TURN_INPUT,HIGH);
     digitalWrite(IGNITION_INPUT,HIGH);
  }
  else{
    //lcd.print("Test 2 Failed because the flasher does not end after 3 cicle");
    lcd.print("Failed");
    digitalWrite(RIGHT_TURN_INPUT,HIGH);
    digitalWrite(IGNITION_INPUT,HIGH);
  }
  
}

void test3() {
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print("Test 3");
  lcd.setCursor(0, 1);
  digitalWrite(LOW_BEAM_INPUT, LOW);
  digitalWrite(IGNITION_INPUT, HIGH);

  delay(100);
  digitalWrite(IGNITION_INPUT, LOW);

  timer = millis();
  while((digitalRead(LOW_BEAM_LEFT_OUT) == LOW || digitalRead(LOW_BEAM_RIGHT_OUT) == LOW) && millis() - timer < 2000);
  if(millis() - timer <= 500) {
    lcd.print("Succesful.");
    //lcd.print(millis() - timer);
  }
  else {
    lcd.print("Failed.");
  }

  delay(1000);
  digitalWrite(LOW_BEAM_INPUT, HIGH);
  digitalWrite(IGNITION_INPUT, HIGH);

}

void test4() {
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print("Test 4");
  lcd.setCursor(0, 1);

  digitalWrite(LEFT_TURN_INPUT, HIGH);
  digitalWrite(RIGHT_TURN_INPUT, HIGH);
  digitalWrite(IGNITION_INPUT, LOW);
  delay(500);
  int cycles = 0;
  int switchCount = 10;
  unsigned long timerLightPhase;
  unsigned long timerDarkPhase;
  while(cycles < switchCount) {
    digitalWrite(RIGHT_TURN_INPUT, HIGH);
    digitalWrite(LEFT_TURN_INPUT, LOW);
    delay(200);
    if(digitalRead(LEFT_TURN_OUT) == LOW) {
      //lcd.print("Test 4 Failed because the left flasher did not turn on");
      //lcd.print(cycles);
      lcd.print("Failed 1");
      //return;
    }
    
    if(digitalRead(RIGHT_TURN_OUT) == HIGH) {
      //lcd.print("Test 4 Failed because the right flasher was turned on when the left flasher should've been on");
      lcd.print("Failed 2");
      //lcd.print(cycles);
    }

    digitalWrite(LEFT_TURN_INPUT, HIGH);
    digitalWrite(RIGHT_TURN_INPUT, LOW);
    timerLightPhase = millis();
    delay(200);

  
    if(digitalRead(RIGHT_TURN_OUT) == LOW) {
      //lcd.print("Test 4 Failed because the right flasher did not turn on");
      //lcd.print(cycles);
      lcd.print("Failed 3");
      //return;
    }
    
    if(digitalRead(LEFT_TURN_OUT) == HIGH) {
      //lcd.print("Test 4 Failed because the left flasher was turned on when the right flasher should've been on");
      //lcd.print(cycles);
      lcd.print("Failed 4");
    }


    cycles++;
  }


  short count=0;
  while(count<3){

    while(digitalRead(RIGHT_TURN_OUT)==HIGH && millis() - timerLightPhase<=440){
    }
    timerDarkPhase=millis();
    if (timerDarkPhase-timerLightPhase>440 || timerDarkPhase-timerLightPhase<360){
        //lcd.print("timpul de executie");
        //lcd.print(timerDarkPhase-timerLightPhase);
        //lcd.print("numarul cicluluui");
        //lcd.print(count);
        //serial.println(count);
        //lcd.print("Test 4 Failed because light face is not between 360 and 440");
        lcd.print("Failed 5");
        digitalWrite(RIGHT_TURN_INPUT,HIGH);
        digitalWrite(IGNITION_INPUT,HIGH);
        return;
    }
    if(count==0){
      digitalWrite(RIGHT_TURN_INPUT,HIGH);
    }

    while(digitalRead(RIGHT_TURN_OUT)==LOW && millis() - timerDarkPhase <=440){
    }
    timerLightPhase=millis();
    if ((timerLightPhase-timerDarkPhase>440 || timerLightPhase-timerDarkPhase<360) && count<2 ){
        //lcd.print("Test 4 Failed because dark face is not between 360 and 440");
        lcd.print("Failed 6");
        digitalWrite(RIGHT_TURN_INPUT,HIGH);
        digitalWrite(IGNITION_INPUT,HIGH);
        return;
    }
    count++;
  }
  if(digitalRead(RIGHT_TURN_OUT)==LOW){
     lcd.print("Succesful");
     digitalWrite(RIGHT_TURN_INPUT,HIGH);
     digitalWrite(IGNITION_INPUT,HIGH);
  }
  else{
    //lcd.print("Test 4 Failed because the flasher does not end after 3 cicle");
    lcd.print("Failed 7");
    digitalWrite(RIGHT_TURN_INPUT,HIGH);
    digitalWrite(IGNITION_INPUT,HIGH);
  }
  

}

void test5() {
  clearScreen();
  lcd.setCursor(0, 0);
  lcd.print("Test 5");
  lcd.setCursor(0, 1);
  digitalWrite(LEFT_TURN_INPUT, HIGH);
  digitalWrite(RIGHT_TURN_INPUT, HIGH);
  digitalWrite(IGNITION_INPUT, LOW);

  delay(100);
  digitalWrite(LEFT_TURN_INPUT, LOW);

  timer = millis();
  while((digitalRead(LEFT_TURN_OUT) == LOW) && millis() - timer < 2000);
  if(millis() - timer <= 100) {
    lcd.print("Succesful");
    //lcd.print(millis() - timer);
  }
  else {
    lcd.print("Failed");
    //lcd.print(millis() - timer);
  }

  delay(1000);
  digitalWrite(LEFT_TURN_INPUT, HIGH);
  digitalWrite(IGNITION_INPUT, HIGH);
}

void test6() {
  test1();
  delay(2000);
  test2();
  delay(2000);
  test3();
  delay(2000);
  test4();
  delay(2000);
  test5();
  delay(2000);
}
