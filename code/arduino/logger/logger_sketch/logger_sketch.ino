/*
 * data logger sketch
 *
 * DHT11 for T/H (digital)
 * LDR for reading light (analog)
 * 
 * 
 */


#include <Wire.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <DS1307RTC.h>
#include <dht11.h>

#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal.h>

// @change pins
#define DHT11_PIN 2 
#define SERIAL_DEBUG 1

int LCD_PIN = 13;    
int LDR_PIN = A0 ;
const int chipSelect = 9;

dht11 DHT11;
File dataFile;
LiquidCrystal lcd(7,8, 3, 4, 5, 6);


void setup() {

    #if defined(SERIAL_DEBUG)
    Serial.begin(9600);
    #endif

    // DS1307 CLK 
    setSyncProvider(RTC.get);   
    micro_delay(20);

    if(timeStatus()!= timeSet) {
        // wait a bit
        micro_delay(100);
    }

    // try again
    if(timeStatus()!= timeSet) {
        // CLK error
        char error[] = "clock error" ;
        display_output(error);
        while (1) ;
    }

    // initialize SD card
    pinMode(SS, OUTPUT);
    if (!SD.begin(chipSelect)) {
        char error[] = "Card failed, or not present";
        display_output(error);
        // don't do anything more:
        while (1) ;
    }
    
    Alarm.timerRepeat(10, log_data);

}

void display_output(char* buffer) {
    #if defined(SERIAL_DEBUG)
        Serial.println(buffer);
    #endif

    pinMode(LCD_PIN, OUTPUT);
    digitalWrite(LCD_PIN, HIGH);
    lcd.begin(16,2);       
    lcd.clear();

    // first line
    lcd.setCursor(0,0);
    int i = 0 ;
    while( i < 16 && (buffer[i] != '\0')) {
        lcd.print(buffer[i]) ;
        i++ ;
    }
    
    if(i == 15) {
        while( i < 16 && (buffer[i] != '\0')) {
            lcd.print(buffer[i]) ;
            i++ ;
        }
    }

}


void log_data() {

    char ts[18] ;

    // timestamp
    int hh = hour();
    int mm = minute();
    int ss = second();
	int yyyy = year();
	int mt = month();
	int dd = day();

	// 18 char excel timestamp
    sprintf(ts,"%02d-%02d-%02d %02d:%02d:%02d",yyyy,mt,dd,hh,mm,ss);
	ts[17] = '\0' ;	


    // sensors data
	char buffer[12] ;
	get_sensor_data(buffer);

    // print on serial and LCD
    #if defined (SERIAL_DEBUG)
        Serial.println(ts);
    #endif
    display_output(buffer);

    // write to SD card
    dataFile = SD.open("yuktix.log", O_CREAT | O_WRITE | O_APPEND);
    if (! dataFile) {
        char error[] = "error opening yuktix.log";
        display_output(error);    
        // Wait forever since we cant write data
        while (1) ;
    }

    dataFile.print(ts);
    dataFile.print(",");
    dataFile.println(buffer);
	micro_delay(20);
    dataFile.close();  
    micro_delay(20);

}

void get_sensor_data(char* buffer) {
    
    int dht11_code = DHT11.read(DHT11_PIN);
    micro_delay(200);
	int ldr = analogRead(LDR_PIN); 
	
    if(dht11_code != 0 ) {
        // error
        sprintf(buffer,"%11s","SENSOR_ERR");

    } else {
		// 11 char wide data
        sprintf(buffer,"%-03d,%-02d,%-04d",DHT11.temperature,DHT11.humidity,ldr);
    }

    buffer[11] = '\0' ;
}

// param n is to ensure n*10 ms delay
void micro_delay(int n) {
    for(int i = 0 ; i < n ; i++) {
        delayMicroseconds(10000);
        // pat watchdog?
    }
}

void loop() {
  
  // alarm wont work w/o alarm delay
  Alarm.delay(1000);
 
}

