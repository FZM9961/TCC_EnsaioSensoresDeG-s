#include <Adafruit_Sensor.h>
#include <WebServer.h>
#include <Wire.h> // I2C library
#include "ccs811.h" // CCS811 library
#define MQ5S 15
#define MQ7S 2
#define MQ135S 4

CCS811 ccs811(23); //Data pin <-> pin 23
int val1, val2;

void setup(){
  
   Serial.begin(115200);
   Serial.println("");
   Serial.println("setup: Starting CCS811 basic demo");
   Serial.print("setup: ccs811 lib version: ");
   Serial.println(CCS811_VERSION);
  
   // Enable I2C
   Wire.begin();
   // Enable CCS811
   ccs811.set_i2cdelay(50); // Needed for ESP8266 because it doesn't handle I2C clock
  stretch correctly
   bool ok = ccs811.begin();
  
   if ( !ok ) Serial.println("setup: CCS811 begin FAILED");
  
  // Print CCS811 versions
  Serial.print("setup: hardware version: "); Serial.println(ccs811.hardware_version(),HEX);
  Serial.print("setup: bootloader version: ");
  Serial.println(ccs811.bootloader_version(), HEX);
  Serial.print("setup: application version: ");
  Serial.println(ccs811.application_version(), HEX);
  
   // Start measuring
   ok = ccs811.start(CCS811_MODE_1SEC);
   if ( !ok ) Serial.println("setup: CCS811 start FAILED");
}
void loop(){
  
   // Read
   uint16_t eco2, etvoc, errstat, raw;
   ccs811.read(&eco2, &etvoc, &errstat, &raw);
   int sensorMQ5val = analogRead(MQ5S);
   int sensorMQ7val = analogRead(MQ7S);
   int sensorMQ135val = analogRead(MQ135S);
   // Print measurement results based on status
   if ( errstat == CCS811_ERRSTAT_OK )
   {
   val1 = eco2;
   val2 = etvoc;
   Serial.print("CCS811 (eco2 ppm) : ");  Serial.print(val1);  Serial.print(" : ");  //CCS eCO2 Values
    
   Serial.print(" etvoc (ppb): ");  Serial.print(val2);   Serial.print(" : "); //CCS eTVOC values
   
   Serial.print(" MQ-5: ");   Serial.print(sensorMQ5val); //MQ-5 values
   Serial.print(": MQ-7: ");   Serial.print(sensorMQ7val); //MQ-7 values
   Serial.print(": MQ-135: ");   Serial.print(sensorMQ135val); //MQ-135 values
   
   Serial.println(); //change row
   
   }
   else if ( errstat == CCS811_ERRSTAT_OK_NODATA ){
   Serial.println("CCS811: waiting for (new) data");
   } else if ( errstat & CCS811_ERRSTAT_I2CFAIL ){
   Serial.println("CCS811: I2C error");
   }
   else{
   Serial.print("CCS811: errstat=");
   Serial.print(errstat, HEX);
   Serial.print("=");
   Serial.println( ccs811.errstat_str(errstat) );
   }
  
   delay(1000); //tempo entre medições de 1s
  
}
