#include <SoftwareSerial.h>

#include <HX711_ADC.h>
#include <SoftwareSerial.h>
#define calibration_factor 1100 //This value is obtained using the SparkFun_HX711_Calibration sketch

SoftwareSerial mySerial(3,1); // RX, TX (RX not used in program)
HX711_ADC scale(2, 4); //( dout, sck)

void setup() {
  delay(1000);
  scale.begin();
  scale.start(3000);
  scale.setCalFactor(1458.0); // user set calibration factor (float)
  delay(200);
  int i = 0;
  for (i = 0; i<20; i++)
  {
    delay(200);
    scale.update();
  }
  delay(200);
  
  scale.tareNoDelay(); 
  
  delay(500);
  mySerial.begin(9600);
  delay(100);
}

void loop() {
int i = 0;
for (i = 0; i<10; i++)
{
  delay(30);
  scale.update();
}
delay(50);
mySerial.println(scale.getData());
delay(400);
}


