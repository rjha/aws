//Program : Simple Sleepprogram.
/*
  Invoking parameter : INT0.
  Why:                  INTs are (also) available in DEEP SLEEP MODE.
  Board on :            InduinoX.
  Story of Program:
  Program starts, HIGH up a LED.
  after 5 seconds it sleeps to DEEP SLEEP MODE.
  one interrupt come on to INT0 in this scenario.
  Arduino wakes up.
  Executes ISR.
  Detach interrupt from INT0.
  Now pin 2 behave a normal one.
  In futre INT0 is not listened.
  Done.
  
  Small test on Sleep and External INTssss...
*/
#include <avr/sleep.h>


#define LED  13  //led is on pin D13 of induinoX.
#define INTERRUPT_0 0

volatile boolean flag = true;
void setup(){
  Serial.begin( 9600 );
  
  pinMode( LED, OUTPUT);
  Serial.println("Led pin set.");
  Serial.println("Initializing ...");
  delay ( 100 );
  Serial.println("Exiting from setup block now.");
  
}


void loop(){
  Serial.println("In main Loop of the Program now...");
  digitalWrite( LED, HIGH );
  
  
  if( flag == true ){
    flag = false;
    Serial.println("Wait for 5 seconds please...!!!");
    delay( 5000 );
    Serial.println("Now i am going to sleep...Bye!!!");
    delay( 1000 );
    sleepNowPlease();      //make arduino sleep for one time only.
  }
  Serial.println("Working now... in normal working/Active state...");
  for(; true; ){
    digitalWrite(LED,!digitalRead( LED ) );
    delay( 250 );
  }
}

void sleepNowPlease(){
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here:Best Mode.
  
  sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin
  //Now it is time to enable an interrupt and attach also on INT0 .
  
  attachInterrupt( INTERRUPT_0 ,wakeUpNow, CHANGE); // use interrupt 0 (pin 2) and run function
                                       // wakeUpNow when pin 2 gets LOW
  sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
 
  sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
  
  detachInterrupt(0);      // disables interrupt 0 on pin 2 so the
                             // wakeUpNow code will not be executed
                             // during normal running time.
}
void wakeUpNow(){
  Serial.println("I am up buddies.");
}
