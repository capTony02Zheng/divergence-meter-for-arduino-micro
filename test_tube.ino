//this is for testing the tubes are working or not or the chips are working or not
//用于测试芯片是否虚焊和辉光管是否正常
//by capTOZ
#include <SPI.h>
#include <Wire.h>

#define num1 0x8000
#define num2 0x4000
#define num3 0x2000
#define num4 0x1000
#define num5 0x0800
#define num6 0x0400
#define num7 0x0200
#define num8 0x0100
#define num9 0x0080
#define num0 0x0040
#define decimalpoint 0x0020 
#define no_display 0x0000

#define latchPin A5
#define SDA 2
#define SCL 3
#define IRdata 7


#define tDelay 1000

uint16_t chartoint(long i) {
  switch(i) {
    case 0: return num0;
    case 1: return num1;
    case 2: return num2;
    case 3: return num3;
    case 4: return num4;
    case 5: return num5;
    case 6: return num6;
    case 7: return num7;
    case 8: return num8;
    case 9: return num9;
    case 10: return decimalpoint;
    case 11: return no_display;
    default: 
    return no_display;
  }
}

void updatetube(uint16_t data) {
   SPI.transfer16(data);
}

void spistart() {
  SPI.begin();
  digitalWrite(latchPin, LOW);
  SPI.beginTransaction(SPISettings(80000, LSBFIRST, SPI_MODE1));
}

void spiend() {
  SPI.endTransaction();
  digitalWrite(latchPin, HIGH);
  SPI.end();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  pinMode(latchPin, OUTPUT);
  pinMode(IRdata, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  for (long i = 0; i < 12; i++) {
    spistart();
    for (int j = 0; j < 8; j++) {
      updatetube(chartoint(i));
    }
    spiend();
    delay(tDelay);
  }
}
