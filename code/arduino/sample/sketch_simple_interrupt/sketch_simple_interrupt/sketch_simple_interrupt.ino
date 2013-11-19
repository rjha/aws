//Program : Interrupt test.
/*
  board connection: induinoX
  pin 2 - one push button.
  interrupt attached on pin 2nd Pin, .i.e is ) number Interrupt.
  interrupt : PIN state on CHANGE.
  Led - D13
*/

#define LIGHTLED 13
volatile int state = HIGH;

void setup(){
  Serial.begin(9600);
  
  pinMode( LIGHTLED, OUTPUT );
  attachInterrupt( 0, blinkLed, CHANGE );
}

void loop(){


  digitalWrite(LIGHTLED, state);
  Serial.println("in loop");
  
  delay( 1000 );
  
}

void blinkLed(){
  state = !state;
  Serial.println("ISR executed."); // classic dummy message
  detachInterrupt(0);  //interrupt is detached for ever.
}
