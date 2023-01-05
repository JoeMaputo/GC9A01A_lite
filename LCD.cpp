
#include "LCD.h"

#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

// initialization code [cmd, numBytes, ...bytes]
const uint8_t initcmd[] = {
  GC9A01A_INREGEN2, 0,
  0xEB, 1, 0x14,
  GC9A01A_INREGEN1, 0,
  GC9A01A_INREGEN2, 0,
  0xEB, 1, 0x14,
  0x84, 1, 0x40,
  0x85, 1, 0xFF,
  0x86, 1, 0xFF,
  0x87, 1, 0xFF,
  0x88, 1, 0x0A,
  0x89, 1, 0x21,
  0x8A, 1, 0x00,
  0x8B, 1, 0x80,
  0x8C, 1, 0x01,
  0x8D, 1, 0x01,
  0x8E, 1, 0xFF,
  0x8F, 1, 0xFF,
  0xB6, 2, 0x00, 0x00,
  GC9A01A_MADCTL, 1, MADCTL_MX | MADCTL_BGR,
  GC9A01A_PIXFMT, 1, 0x05,
  0x90, 4, 0x08, 0x08, 0x08, 0x08,
  0xBD, 1, 0x06,
  0xBC, 1, 0x00,
  0xFF, 3, 0x60, 0x01, 0x04,
  GC9A01A1_VREG1A, 1, 0x13,
  GC9A01A1_VREG1B, 1, 0x13,
  GC9A01A1_VREG2A, 1, 0x22,
  0xBE, 1, 0x11,
  ILI9341_GMCTRN1, 2, 0x10, 0x0E,
  0xDF, 3, 0x21, 0x0c, 0x02,
  GC9A01A_GAMMA1, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
  GC9A01A_GAMMA2, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
  GC9A01A_GAMMA3, 6, 0x45, 0x09, 0x08, 0x08, 0x26, 0x2A,
  GC9A01A_GAMMA4, 6, 0x43, 0x70, 0x72, 0x36, 0x37, 0x6F,
  0xED, 2, 0x1B, 0x0B,
  0xAE, 1, 0x77,
  0xCD, 1, 0x63,
  0x70, 9, 0x07, 0x07, 0x04, 0x0E, 0x0F, 0x09, 0x07, 0x08, 0x03,
  ILI9341_FRAMERATE, 1, 0x34,
  0x62, 12, 0x18, 0x0D, 0x71, 0xED, 0x70, 0x70,
            0x18, 0x0F, 0x71, 0xEF, 0x70, 0x70,
  0x63, 12, 0x18, 0x11, 0x71, 0xF1, 0x70, 0x70,
            0x18, 0x13, 0x71, 0xF3, 0x70, 0x70,
  0x64, 7, 0x28, 0x29, 0xF1, 0x01, 0xF1, 0x00, 0x07,
  0x66, 10, 0x3C, 0x00, 0xCD, 0x67, 0x45, 0x45, 0x10, 0x00, 0x00, 0x00,
  0x67, 10, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x01, 0x54, 0x10, 0x32, 0x98,
  0x74, 7, 0x10, 0x85, 0x80, 0x00, 0x00, 0x4E, 0x00,
  0x98, 2, 0x3e, 0x07,
  GC9A01A_TEON, 0,
  GC9A01A_INVON, 0,
  GC9A01A_SLPOUT, 0x80, // Exit sleep
  GC9A01A_DISPON, 0x80, // Display on
  0x00                  // End of list
};

void sendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes) {
  digitalWrite(PIN_CS, LOW);
  digitalWrite(PIN_DC, LOW);
  SPI.transfer(commandByte);
  digitalWrite(PIN_DC, HIGH);
  if (numDataBytes) {
    for (int i = 0; i < numDataBytes; i++) {
      dataBytes++;
      SPI.transfer(*dataBytes); // Send the data bytes
    }
  }
  dataBytes++;
  digitalWrite(PIN_CS, HIGH);
  
}

void init_lcd()
{
  digitalWrite(PIN_RST, LOW);
  delay(20);
  digitalWrite(PIN_RST, HIGH);
  delay(80);
  uint8_t cmd, x, numArgs;
  const uint8_t *addr = initcmd;
  cmd = *addr;
  while (cmd > 0) {
    addr++;
    x = *addr;
    numArgs = x & 0x7F;
    sendCommand(cmd, addr, numArgs);
    addr += numArgs;
    addr++;
    cmd = *addr;
    if (x & 0x80)
      delay(150);
  }
}

void set_zero() {
  digitalWrite(PIN_CS, LOW);
  digitalWrite(PIN_DC, LOW);
  SPI.transfer(GC9A01A_CASET);
  digitalWrite(PIN_DC, HIGH);
  SPI.transfer(0x00); // start 0 horizon
  SPI.transfer(0x00);
  SPI.transfer(0x00); // end 239 horizon
  SPI.transfer(0xEF);
  digitalWrite(PIN_DC, LOW);
  SPI.transfer(GC9A01A_PASET);
  digitalWrite(PIN_DC, HIGH);
  SPI.transfer(0x00); // start 0 vertical
  SPI.transfer(0x00);
  SPI.transfer(0x00); // end 239 vertical
  SPI.transfer(0xEF);
  digitalWrite(PIN_DC, LOW);
  SPI.transfer(GC9A01A_RAMWR);
  digitalWrite(PIN_DC, HIGH);
  digitalWrite(PIN_CS, HIGH);
}


void fill_screen(uint16_t color) {
  set_zero();
  digitalWrite(PIN_CS, LOW);
  digitalWrite(PIN_DC, HIGH);
  //uint32_t len = GC9A01A_TFTWIDTH * GC9A01A_TFTHEIGHT;
  for (uint32_t i = 0; i < 57600; i++) {
//    shiftOut(PIN_MOSI, PIN_SCK, MSBFIRST, first);
//    shiftOut(PIN_MOSI, PIN_SCK, MSBFIRST, second);
    SPI.transfer(color >> 8);
    SPI.transfer(color);
  }
  digitalWrite(PIN_CS, HIGH);
}
