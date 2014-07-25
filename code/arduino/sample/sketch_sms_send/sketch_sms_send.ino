#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(8, 9);

void setup() {
 
  Serial.begin(9600);
  gsmSerial.begin(9600);
  
  gsmSerial.println("AT");
  delay(500);
  toSerial();
  gsmSerial.println("AT+CMGF=1\r");
  delay(500);
  toSerial();
}

void loop() {
 uint8_t count = 1 ;
 
 while(count < 3) {
    gsmSerial.print("\r");  
    delay(1000);
    gsmSerial.print("AT+CMGF=1\r");
    delay(1000);
    toSerial();
    gsmSerial.print("AT+CMGS=\"+919755182991\"\r"); 
    delay(1000);
    toSerial();
    gsmSerial.print("Telit sms test");
    gsmSerial.print(count);
    gsmSerial.println();
    // CTRL-Z
    gsmSerial.write(0x1A);
    toSerial();
    count++ ;
    delay(3000); 
    
    
  }
  
  // keep waiting
  while(1);
  
  
}
void toSerial(){
  if( gsmSerial.available()){
    Serial.print( gsmSerial.readString());
  }
}
