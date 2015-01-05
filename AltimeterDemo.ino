/*
Based on the Adafruit_SSD1306 library demo and the DCS-BIOS template sketch.

This is meant to be compiled with Energia and to run on a
Texas Instruments Tiva C Series Launchpad board.

It should run on an Arduino (adjust the #defines accordingly
to specify the pins you used), but I have not tested whether
the 16 MHz AVR is fast enough.
The Tiva C Launchpad has an 80 MHz ARM processor.

The fonts are not perfectly aligned because they were made
in a hurry to get a proof of concept...

*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DcsBios.h>
#include <Servo.h>

#include "digits_32x64.cpp"
#include "digits_00_32x64.cpp"
#include "digits_1to9_32x64.cpp"

// If using software SPI (the default case):
#define OLED_MOSI  PE_1
#define OLED_CLK   PE_2
#define OLED_DC    PD_3
#define OLED_CS    PB_7
#define OLED_RESET PD_2
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

DcsBios::ProtocolParser parser;

void drawDigit(unsigned int index, unsigned int offset, unsigned char* bits) {
  //double y_offset = (double)offset / 65535.0d;
  //unsigned int y_offset_lines = y_offset * 640;
  unsigned int y_offset_lines = offset / 103;
   display.fillRect(32*index, 0, 32, 64, BLACK);
   display.drawXBitmap(32*index, 0, &bits[4*y_offset_lines], 32, 64, WHITE);
   display.display();   
}

void setup()   {                
  Serial.begin(500000);
  
  display.begin(SSD1306_SWITCHCAPVCC);

  display.clearDisplay();
  drawDigit(0, 0, digits_1to9_32x64_bits);
  drawDigit(1, 0, digits_32x64_bits);
  drawDigit(2, 0, digits_32x64_bits);
  drawDigit(3, 0, digits_00_32x64_bits);
}


void loop() {
  while(Serial.available()) parser.processChar(Serial.read());
}

void onDcsBiosWrite(unsigned int address, unsigned int value) {
  address = address & 0xffff;
  value = value & 0xffff;
  if (address == 0x1080) {
    unsigned int alt10000ftCntValue = (value & 0xffff) >> 0;
    drawDigit(0, alt10000ftCntValue, digits_1to9_32x64_bits);
  }
  
  if (address == 0x1082) {
    unsigned int alt1000ftCntValue = (value & 0xffff) >> 0;
    drawDigit(1, alt1000ftCntValue, digits_32x64_bits);
  }
  
  if (address == 0x1084) {
    unsigned int alt100ftCntValue = (value & 0xffff) >> 0;
    drawDigit(2, alt100ftCntValue, digits_32x64_bits);
  }
}

