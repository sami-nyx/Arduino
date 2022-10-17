/*
 Name:    pass_the_ring.ino
 Created: 29-Sep-22 4:30:24 PM
 Author:  sami abo srehan
*/

#define HOME_PIN 7
#define LINE_PIN 5
#define TARGET_PIN  6
#define EN_NEXT_PIN 9

#define BUZZER_PIN 8

#define HOME 0
#define ON_AIR 1
#define FAIL 2
#define PASS 3

int state = 0;
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(HOME_PIN, INPUT_PULLUP);
  pinMode(LINE_PIN, INPUT_PULLUP);
  pinMode(TARGET_PIN, INPUT_PULLUP);
  pinMode(EN_NEXT_PIN, OUTPUT);
  
  pinMode(BUZZER_PIN, OUTPUT);

  //Serial.println("starting");
}

// the loop function runs over and over again until power down or reset
void loop() {
  //Serial.println(digitalRead(HOME_PIN));
  switch (state)
  {
  case HOME:
  {
    //Serial.println("home");
    if (digitalRead(HOME_PIN)) {
      state = ON_AIR;
    }
    if (!digitalRead(LINE_PIN)) {
      state = FAIL;
    }

  }
  break;
  case ON_AIR:
  {
    //Serial.println("on air");
    if (!digitalRead(TARGET_PIN)) {
      state = PASS;
    }
    if (!digitalRead(LINE_PIN)) {
      state = FAIL;
    }
    if (!digitalRead(HOME_PIN)) {
      state = HOME;
    }
  }
  break;
  case FAIL:
  {
  //  Serial.println("fail");
    //buzzer on
    alarm();
    if (!digitalRead(HOME_PIN)) {
      state = HOME;
    }
  }
  break;
  case PASS:
  { Serial.println("pass:=");
    digitalWrite(EN_NEXT_PIN, HIGH);
    if (!digitalRead(HOME_PIN))
    {
   
      digitalWrite(EN_NEXT_PIN, LOW);

      state = HOME;
    }
    //green light on
  }
  break;

  default:
    break;
  }
}

void alarm() {
  for (size_t i = 0; i < 2; i++)
  {
    for (size_t j = 0; j < 5; j++)
    {

      digitalWrite(BUZZER_PIN, HIGH);
      delay(35);
      digitalWrite(BUZZER_PIN, LOW);
      delay(35);
    }

    delay(100);

  }
}
