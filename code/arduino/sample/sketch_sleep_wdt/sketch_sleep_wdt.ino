//Program :  sleep test with WDT interrupt.
/*
  Board: InduinoX
  [The Arduino has several sleep modes that can be used to reduce power consumption.
  The most useful for sensor networks is probably the one that uses the watchdog timer.]
  ---from
  http://citizen-sensing.org/2013/07/arduino-watchdog/
  
  Story of Program:
  # Start Program.
  # Set up WDT configuration to Interrupt + Reset.
  # Put sleep deep one to MCU.
  # after 8s system reboots.
  # Hence wakee up from sleep....
  # confusion is that is exactly happening?
  # please explain me.
  
  #basically WDT gives us a max time gap of 8s.
  #basically i am invoking arduino after every 8 s to toggle LED state and put back to sleep...
*/

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED 13
volatile boolean flag = true;

/***************************************************
 *  Name:        ISR(WDT_vect)
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Watchdog Interrupt Service. This
 *               is executed when watchdog timed out.
 *
 ***************************************************/
ISR(WDT_vect)
{
  if(flag == false)
  {
    flag= true;
    //setting up the flag here.
    //nothing more is happening here...
    //after thsi ISR reset system will be done by WDT.
    //hence arduino will wake up ...
  }
  else
  {
    Serial.println("WDT Overrun!!!");
  }
}
void setup(){
  Serial.begin(9600);
  Serial.println("Initialising...");
  delay(100); //Allow for serial print to complete.
  pinMode(LED,OUTPUT);
  digitalWrite( LED, HIGH);
  setUpWatchDogInterrupt();
  Serial.println("Now exiting from setup block...");
  
}

void loop(){
  
  Serial.println("In main Loop of the Program now...");  
  
  if( flag == true ){
    flag = false;
    digitalWrite( LED, !digitalRead( LED) );
    Serial.println("Wait for 5 seconds please...!!!");
    delay( 5000 );
    Serial.println("Now i am going to sleep...Bye!!!");
    delay( 100 );
    Serial.println("See u soon after 8 Seconds...!!!");
    delay( 100 );
    //delay( 1000 );
    sleepNowPlease();      //make arduino sleep for one time only.
  }
 
  
}

/***************************************************
 *  Name:        sleepNowPlease
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void sleepNowPlease(){
  
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   //deep power down/  deep power sleep mode.
    sleep_enable();
    
    /* Now enter sleep mode. */
    sleep_mode();
    
    /* The program will continue from here after the WDT timeout*/
    sleep_disable(); /* First thing to do is disable sleep. */
    
}

void setUpWatchDogInterrupt(){
  /*** Setup the WDT ***/
  
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  Serial.println("Done configuring WDT, with 8s as timer.");
}


