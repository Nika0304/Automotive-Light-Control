#include <Arduino.h>

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

#define PHASE_DURATION 400

bool leftValue = 0;
bool rightValue = 0;
unsigned long leftTimer = 0;
unsigned long rightTimer = 0;
unsigned long phaseTimer = 0;
unsigned long phaseTimer2 = 0;

void setup() {
  pinMode(LOW_BEAM_INPUT, INPUT);
  pinMode(POSITION_INPUT, INPUT);
  pinMode(LEFT_TURN_INPUT, INPUT);
  pinMode(RIGHT_TURN_INPUT, INPUT);
  pinMode(IGNITION_INPUT, INPUT);
  
  pinMode(LOW_BEAM_LEFT_OUT, OUTPUT);
  pinMode(LOW_BEAM_RIGHT_OUT, OUTPUT);
  pinMode(POSITION_OUT, OUTPUT);
  pinMode(LEFT_TURN_OUT, OUTPUT);
  pinMode(RIGHT_TURN_OUT, OUTPUT);

  digitalWrite(LOW_BEAM_LEFT_OUT, LOW);
  digitalWrite(LOW_BEAM_RIGHT_OUT, LOW);  
  digitalWrite(POSITION_OUT, LOW);
  digitalWrite(LEFT_TURN_OUT, LOW);
  digitalWrite(RIGHT_TURN_OUT, LOW);
}


void loop() {
  if (digitalRead(IGNITION_INPUT) == LOW) {
    
    // Left turn signal
    // Cu acest IF ne asiguram ca actualizam timpul doar o data, atunci cand dam de maneta
    // variabila LeftValue retine faptul ca timpul a fost deja pentru a nu-l citi mereu cand left flasher e ON
    if (digitalRead(LEFT_TURN_INPUT) == LOW && leftValue == 0){ 
      leftTimer = millis();
      leftValue = 1;
    }
    if ((digitalRead(LEFT_TURN_INPUT) == HIGH && leftValue == 1) || digitalRead(RIGHT_TURN_INPUT) == LOW ){   
      leftValue = 0;      // Daca mutam maneta pe OFF reactualizam variabila LeftTimer pentru a putea citi timpul iar                                                                  
    }

    // conditii de activare:  left turn e ON sau n-au fost facute 3 cicluri SI right turn e OFF
    if( (digitalRead(LEFT_TURN_INPUT) == LOW || millis() - leftTimer <2399 ) && digitalRead(RIGHT_TURN_INPUT) == HIGH){ 
      //  phase_duration = 400
      if (digitalRead(LEFT_TURN_OUT) == LOW && ( millis() - phaseTimer >= PHASE_DURATION)) { // light phase 
        phaseTimer = millis();                // actualizam PhaseTimer cand trecem dintr - o stare in alta
        digitalWrite(LEFT_TURN_OUT, HIGH);
      }
      else
        if (digitalRead(LEFT_TURN_OUT) == HIGH && millis() - phaseTimer >= PHASE_DURATION) { // dark phase
          phaseTimer = millis();              // actualizam PhaseTimer cand trecem dintr - o stare in alta
          digitalWrite(LEFT_TURN_OUT, LOW);
        }
    }
    else {   // daca conditiile de activare nu sunt indeplinite actualizam leftTimer si stingem semnalizarea
      leftTimer=2500;
      digitalWrite(LEFT_TURN_OUT, LOW);
    }
      

    // Right turn signal
    if (digitalRead(RIGHT_TURN_INPUT) == LOW && rightValue == 0){
      rightTimer = millis();
      rightValue = 1;
    }
    if ((digitalRead(RIGHT_TURN_INPUT) == HIGH && rightValue == 1) || digitalRead(LEFT_TURN_INPUT) == LOW ){
      rightValue = 0;
    }
    if( (digitalRead(RIGHT_TURN_INPUT) == LOW || millis() - rightTimer <2399 ) && digitalRead(LEFT_TURN_INPUT) == HIGH){
      if (digitalRead(RIGHT_TURN_OUT) == LOW && ( millis() - phaseTimer2 >= PHASE_DURATION)) { // light phase
        phaseTimer2 = millis();
        digitalWrite(RIGHT_TURN_OUT, HIGH);
      }
      else
        if (digitalRead(RIGHT_TURN_OUT) == HIGH && millis() - phaseTimer2 >= PHASE_DURATION) { // dark phase
          phaseTimer2 = millis();
          digitalWrite(RIGHT_TURN_OUT, LOW);
        }
    }
    else {
      rightTimer = 2500;
      digitalWrite(RIGHT_TURN_OUT, LOW);
    }


    // Low beam headlights
    if (digitalRead(LOW_BEAM_INPUT) == LOW) {
      digitalWrite(LOW_BEAM_LEFT_OUT, HIGH);
      digitalWrite(LOW_BEAM_RIGHT_OUT, HIGH);
    } else {
      digitalWrite(LOW_BEAM_LEFT_OUT, LOW);
      digitalWrite(LOW_BEAM_RIGHT_OUT, LOW);
    }
  } else {
    // Turn off all lights if ignition is off
    digitalWrite(LOW_BEAM_LEFT_OUT, LOW);
    digitalWrite(LOW_BEAM_RIGHT_OUT, LOW);
    digitalWrite(LEFT_TURN_OUT, LOW);
    digitalWrite(RIGHT_TURN_OUT, LOW);
  }

  // Automatically turn on position lights if headlights are on
  if (digitalRead(POSITION_OUT) == LOW && (digitalRead(POSITION_INPUT) == LOW || digitalRead(LOW_BEAM_INPUT) == LOW)) {
    digitalWrite(POSITION_OUT, HIGH);
  }

  // Automatically turn off position lights if headlights are off
  if (digitalRead(POSITION_OUT) == HIGH && digitalRead(POSITION_INPUT) == HIGH && digitalRead(LOW_BEAM_INPUT) == HIGH) {
    digitalWrite(POSITION_OUT, LOW);
  }
}