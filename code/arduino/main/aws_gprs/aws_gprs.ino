/*
 * main aws sketch
 *
 * DHT22 for T/H (digital)
 * BMP085 for T/P (analog)
 * LDR for reading light (analog)
 * LCD for display 
 *
 * Davis 7852 Rain Gauge on INT0 (pin2) 
 * GSM modem on hardware rx,tx
 *
 *
 * 
 */

#include <stdlib.h>
#include <avr/wdt.h>
#include <Wire.h>
#include <DHT22.h>
#include <Adafruit_BMP085.h>
#include <LiquidCrystal.h>
#include <Time.h>
#include <TimeAlarms.h>

// #define AWS_DEBUG 1 
// @change pins and output modes 
#define DHT22_PIN 6

// Analog pin A4(SDA),A5(SCL)
Adafruit_BMP085 bmp;
DHT22 myDHT22(DHT22_PIN);


// @change lcd pins
LiquidCrystal lcd(7,8, 9, 10, 11, 12);
int lcd_pin = 13;    


// globals
volatile int rain_counter = 0 ;
volatile bool rain_counter_reset = false ;

volatile unsigned long irq_time ;
volatile unsigned long last_irq_time ;

float dht22_temp ;
float humidity ;
int32_t pressure ;

// return codes
DHT22_ERROR_t dht22_code ;
char device_key[37] = "b54c5cf8-ade6-4daf-83b5-926a60d18d6a" ;

void setup() {

    Serial.begin(9600);
    // watchdog enabled
    wdt_enable(WDTO_8S);
    micro_delay(20);

    bmp.begin();  
    // INT0 interrupt
    attachInterrupt(0,isr_pin2,RISING);
    last_irq_time = 0 ;
    Alarm.timerRepeat(10, send_bulletin);

}

void isr_pin2() {
    
    // @imp millis() donot advance inside an ISR
    irq_time = millis();    
    if((irq_time - last_irq_time) > 250) {
        rain_counter = rain_counter +1 ;
        last_irq_time = irq_time ;
    }
}

// param n is to ensure n*10 ms delay
void micro_delay(int n) {
    for(int i = 0 ; i < n ; i++) {
        delayMicroseconds(10000);
        // pat watchdog every 100*10 ms
        if((i > 100) && (i %100 == 0 ))
            wdt_reset();
    }
}

void get_sensor_data(char* buffer, char* bencode) {
    
    // DHT22 pin needs 2 seconds
    micro_delay(250);
    dht22_code = myDHT22.readData();

    buffer[0] = 'T' ;

    if(dht22_code == DHT_ERROR_NONE) {

        dht22_temp = myDHT22.getTemperatureC();
        humidity = myDHT22.getHumidity();
        dtostrf(dht22_temp,5,1,buffer+1);  

        buffer[6] = ' ' ;
        buffer[7] = 'H' ;
        dtostrf(humidity,4,1,buffer +8);  
        buffer[12] = '%' ;
        for(int i = 13; i < 16 ;i++) { buffer[i] = ' ' ; }

    } else {

        buffer[1] = ' ';
        buffer[2] = 'E';
        buffer[3] = 'R';
        buffer[4] = 'R';
        for(int i = 5; i < 16 ;i++) { buffer[i] = ' ' ; }
        dht22_temp = 9999 ;
        humidity = 200;
    }

    // pat watchdog
    wdt_reset();
    pressure = bmp.readPressure();
    pressure = round(pressure/100.0) ;

    buffer[16] = 'P' ;
    sprintf(buffer+17,"%-5lu",pressure);
    buffer[22] = ' ' ;

    buffer[23] = 'R' ;
    // more rain in a day than cherapunji 
    // receives in a year!
    sprintf(buffer+24,"%-5d",rain_counter);
    for(int i = 29; i < 32 ;i++) { buffer[i] = ' ' ; }
    buffer[32] = '\0' ;

    // Bencode dictionary
    bencode[0] = 'd' ;

    sprintf(bencode+1,"%s","1:Tf");
    dtostrf(dht22_temp,5,1,bencode+5);  

    sprintf(bencode+10,"%s","e1:Hf");
    dtostrf(humidity,4,1,bencode+15);  

    sprintf(bencode+19,"%s","e1:Pi");
    sprintf(bencode+24,"%-5lu",pressure);

    sprintf(bencode+29,"%s","e1:Ri");
    sprintf(bencode+34,"%-5d",rain_counter);

    sprintf(bencode+39,"%s","e5:_sno_6:aws001");
    bencode[55] = 'e' ;
    bencode[56] = '\0' ;
}

void send_bulletin() {

    char buffer[33] ;
    char bencode[57] ;

    get_sensor_data(buffer,bencode);

    #if defined(AWS_DEBUG)
    int hh = hour();
    int mm = minute();
    int ss = second();
     
    char ts[9] ;
    sprintf(ts,"%02d:%02d:%02d",hh,mm,ss);
    ts[8] = '\0' ;        
    Serial.println(ts); 
    Serial.println(buffer);
    #endif

    // pat watchdog
    wdt_reset();

    // output
    lcd_output(buffer);
    Serial.println(bencode);

    int hr = hour();

    // reset rain counter at 9AM
    // condition >= is for a power failure 
    // missing the 9'0 clock window
    if(!rain_counter_reset && (hr >= 9)) {
        rain_counter_reset = true ;
        rain_counter = 0 ;
    } 

    if(hr != 9 && rain_counter_reset) {
        rain_counter_reset = false ;
    }

}


void lcd_output(char* buffer) {
  
    pinMode(lcd_pin, OUTPUT);
    digitalWrite(lcd_pin, HIGH);
    lcd.begin(16,2);       
    lcd.clear();
    // line1
    lcd.setCursor(0,0);
    for(int i = 0; i < 16 ;i++) { lcd.print(buffer[i]); }

    // line2
    lcd.setCursor(0,1);
    for(int i = 16; i < 32 ;i++) { lcd.print(buffer[i]); }
}

void loop() {

    // alarm wont work w/o alarm delay
    Alarm.delay(1000);
    // pat the watchdog
    wdt_reset();
}

