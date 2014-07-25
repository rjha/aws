#include <avr/wdt.h>

void setup ()
{
	Serial.begin (9600);
	Serial.println ("Restarted.");
	// reset after one second, if no "pat the dog" received
	wdt_enable (WDTO_1S);  
}  // end of setup

void loop ()
{
	Serial.println ("Entered loop ...");
	wdt_reset ();  
	Serial.print("token1");
	delay(200);
	Serial.print("token2");
	delay(200);
	while (true) ; 
	Serial.print("token3");
}
