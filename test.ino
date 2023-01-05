#include "SPI.h"
#include "LCD.h"



void setup() {
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_DC, HIGH);
  pinMode(PIN_MOSI, OUTPUT);
  digitalWrite(PIN_MOSI, LOW);
  pinMode(PIN_SCK, OUTPUT);
  digitalWrite(PIN_SCK, LOW);
  pinMode(PIN_RST, OUTPUT);
  digitalWrite(PIN_RST, HIGH);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  init_lcd();
}

void loop(void) {
  fill_screen(GC9A01A_PURPLE);
  fill_screen(GC9A01A_BLUE);
  fill_screen(0x03FF);
  fill_screen(0x1BE5);
  fill_screen(GC9A01A_YELLOW);
  fill_screen(GC9A01A_ORANGE);
  fill_screen(GC9A01A_RED);
//  delay(100);
}
