/***********************************
 * 
 *    TimesUp
 * 
 *    Afrooz Samei, Shreeya Tyagi  &  Orlando Bascunan
 * 
 * 
 * 
 *    Arduino Code 
 *     
 *    December 2016
 * 
 */ //////////////////////////////////
 
bool onMode = false;

int workTime = 60; // a minute
int restTime = 30; // 30 seconds

int startTime = 0;

int vibPin = 0;
int ledPin = 1;
int buttonPin = 2;

bool buttonIsPressed = false;
int startTimePressed = 0;
int longPressed = 1; // a second

bool workTimerSelected = true;
bool doubleNotified = false;


int ledFade = 0;

void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(vibPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);

}
// Check Time calculates the percentage of time left and displays blinking lights sped accordingly.
void checkTime() {
  int percent;
  if ( workTimerSelected) {
    percent = ( millis() / 1000 - startTime ) * 100 / workTime;
  } else {
    percent = ( millis() / 1000 - startTime ) * 100 / restTime;
  }
  int d = max(500 - percent * 5, 50);
  digitalWrite(ledPin, HIGH);
  delay(d);
  digitalWrite(ledPin, LOW);

  delay(d);
  digitalWrite(ledPin, HIGH);
  delay(d);
  digitalWrite(ledPin, LOW);

  delay(d);
  digitalWrite(ledPin, HIGH);
  delay(d);
  digitalWrite(ledPin, LOW);

  delay(d);
  digitalWrite(ledPin, HIGH);
  delay(d);
  digitalWrite(ledPin, LOW);

}

// Pick Timer switches between work and play mode and displays lights accordingly.
void pickTimer() {
  workTimerSelected = ! workTimerSelected;
  if (workTimerSelected) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
  }
  // show the lights
}
void startTimer() {
  startTime = millis() / 1000;
  onMode = true;
}
void stopTimer() {
  onMode = false;
}

//Alarm when the time is up.
void alarm() {
  notification();
  notification();
  notification();
  workTimerSelected = !workTimerSelected;
  startTimer();

  if ( workTimerSelected ) {
  }
}

void notification() {
  digitalWrite(vibPin, HIGH);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(vibPin, LOW);
  digitalWrite(ledPin, LOW);
  delay(100);
}
void shortPress() {
  if ( onMode ) {
    // checkTime
    checkTime();
  } else {
    // go through the time lapses
    pickTimer();
  }
}
void longPress() {
  if ( onMode) {
    // Stop
    stopTimer();
    notification();
    notification();
  } else {
    // Start
    startTimer();
    notification();
  }
}
void clearButton() {
  buttonIsPressed = false;
  doubleNotified = false;
  if ( onMode ) {
    // checkTime
    // checkTime();
  } else {
    // go through the time lapses
    // pickTimer();
  }
}
int fadeLedUp() {
  ledFade += 3;
  if (ledFade > 255) {
    ledFade = 0;
  }
  return ledFade;
}

// Main Loop
void loop() {

// onMode represents when the timer is running
  if (onMode) {
    if ( workTimerSelected) { //Check on the timers first
      if ( startTime + workTime < millis() / 1000) {
        //workTime is done
        alarm();
      }
    } else {
      analogWrite(ledPin, fadeLedUp() );
      delay(50);
      if ( startTime + restTime < millis() / 1000) {
        digitalWrite(ledPin, LOW);

        //restTime is done
        alarm();
      }
    }
  }

// The following code is to capture tap and hold actions with one button.
  
  if (!digitalRead(buttonPin) && !buttonIsPressed) {
    // start pressing the button, register the time
    startTimePressed = millis() / 1000;
    buttonIsPressed = true;
  }
  if (!digitalRead(buttonPin) && millis() / 1000 - startTimePressed > longPressed) {
    //if the button is held more
    if ( !doubleNotified ) {
      longPress();
      doubleNotified = true;
    }
  }

  if (digitalRead(buttonPin) && buttonIsPressed) {
    //if the button is released
    if (  millis() / 1000 - startTimePressed < longPressed ) {
      shortPress();
    }
    clearButton();
  }
}
