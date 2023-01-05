#include "SPI.h"


#define PIN_DC 7
#define PIN_CS 10
#define PIN_MOSI 11
#define PIN_SCK 13
#define PIN_RST 8

//#define DEF_FREQ 8000000

#define GC9A01A_TFTWIDTH 240  ///< Display width in pixels
#define GC9A01A_TFTHEIGHT 240 ///< Display height in pixels
#define GC9A01A_SWRESET 0x01 ///< Software reset register

//#define GC9A01A 0x04   ///< Read display identification information
//#define GC9A01A 0x09   ///< Read Display Status

#define GC9A01A_SLPIN 0x10  ///< Enter Sleep Mode
#define GC9A01A_SLPOUT 0x11 ///< Sleep Out
#define GC9A01A_PTLON 0x12  ///< Partial Mode ON
#define GC9A01A_NORON 0x13  ///< Normal Display Mode ON

#define GC9A01A_INVOFF 0x20   ///< Display Inversion OFF
#define GC9A01A_INVON 0x21    ///< Display Inversion ON
#define GC9A01A_DISPOFF 0x28  ///< Display OFF
#define GC9A01A_DISPON 0x29   ///< Display ON

#define GC9A01A_CASET 0x2A ///< Column Address Set
#define GC9A01A_PASET 0x2B ///< Page Address Set
#define GC9A01A_RAMWR 0x2C ///< Memory Write

#define GC9A01A_PTLAR 0x30    ///< Partial Area
#define GC9A01A_VSCRDEF 0x33  ///< Vertical Scrolling Definition
#define GC9A01A_TEOFF 0x34    ///< Tearing effect line off
#define GC9A01A_TEON 0x35     ///< Tearing effect line on
#define GC9A01A_MADCTL 0x36   ///< Memory Access Control
#define GC9A01A_VSCRSADD 0x37 ///< Vertical Scrolling Start Address
#define GC9A01A_PIXFMT 0x3A   ///< COLMOD: Pixel Format Set
// 0x3C = Write memory continue
// 0x44 = Set tear scanline
// 0x45 = Get scanline
// 0x51 = Write display brightness
// 0x53 = Write CTRL display

// 0xA7 = Vcore voltage control

// 0xB0 = RGB interface signal control
//#define ILI9341_FRMCTR1                                                        \
  0xB1 ///< Frame Rate Control (In Normal Mode/Full Colors)
//#define ILI9341_FRMCTR2 0xB2 ///< Frame Rate Control (In Idle Mode/8 colors)
//#define ILI9341_FRMCTR3                                                        \
  0xB3 ///< Frame Rate control (In Partial Mode/Full Colors)
//#define ILI9341_INVCTR 0xB4  ///< Display Inversion Control
// 0xB5 = Blanking porch control
#define GC9A01A1_DFUNCTR 0xB6 ///< Display Function Control
// 0xBA = TE control

// 0xC1 = Power criterion control
#define GC9A01A1_VREG1A 0xC3 ///< Vreg1a voltage control
#define GC9A01A1_VREG1B 0xC4 ///< Vreg1b voltage control
#define GC9A01A1_VREG2A 0xC9 ///< Vreg2a voltage control

#define GC9A01A_RDID1 0xDA ///< Read ID 1
#define GC9A01A_RDID2 0xDB ///< Read ID 2
#define GC9A01A_RDID3 0xDC ///< Read ID 3

#define ILI9341_GMCTRP1 0xE0 ///< Positive Gamma Correction
#define ILI9341_GMCTRN1 0xE1 ///< Negative Gamma Correction
#define ILI9341_FRAMERATE 0xE8 ///< Frame rate control
#define GC9A01A_INREGEN2 0xEF ///< Inter register enable 2
#define GC9A01A_GAMMA1 0xF0 ///< Set gamma 1
#define GC9A01A_GAMMA2 0xF1 ///< Set gamma 2
#define GC9A01A_GAMMA3 0xF2 ///< Set gamma 3
#define GC9A01A_GAMMA4 0xF3 ///< Set gamma 4
#define GC9A01A_INREGEN1 0xFE ///< Inter register enable 1

#define MADCTL_MY 0x80  ///< Bottom to top
#define MADCTL_MX 0x40  ///< Right to left
#define MADCTL_MV 0x20  ///< Reverse Mode
#define MADCTL_ML 0x10  ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00 ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08 ///< Blue-Green-Red pixel order
#define MADCTL_MH 0x04  ///< LCD refresh right to left

// Color definitions
#define GC9A01A_BLACK 0x0000       ///<   0,   0,   0
#define GC9A01A_NAVY 0x000F        ///<   0,   0, 123
#define GC9A01A_DARKGREEN 0x03E0   ///<   0, 125,   0
#define GC9A01A_DARKCYAN 0x03EF    ///<   0, 125, 123
#define GC9A01A_MAROON 0x7800      ///< 123,   0,   0
#define GC9A01A_PURPLE 0x780F      ///< 123,   0, 123
#define GC9A01A_OLIVE 0x7BE0       ///< 123, 125,   0
#define GC9A01A_LIGHTGREY 0xC618   ///< 198, 195, 198
#define GC9A01A_DARKGREY 0x7BEF    ///< 123, 125, 123
#define GC9A01A_BLUE 0x001F        ///<   0,   0, 255
#define GC9A01A_GREEN 0x07E0       ///<   0, 255,   0
#define GC9A01A_CYAN 0x07FF        ///<   0, 255, 255
#define GC9A01A_RED 0xF800         ///< 255,   0,   0
#define GC9A01A_MAGENTA 0xF81F     ///< 255,   0, 255
#define GC9A01A_YELLOW 0xFFE0      ///< 255, 255,   0
#define GC9A01A_WHITE 0xFFFF       ///< 255, 255, 255
#define GC9A01A_ORANGE 0xFD20      ///< 255, 165,   0
#define GC9A01A_GREENYELLOW 0xAFE5 ///< 173, 255,  41
#define GC9A01A_PINK 0xFC18        ///< 255, 130, 198


void sendCommand(uint8_t commandByte, uint8_t *dataBytes, uint8_t numDataBytes);

void init_lcd();

void set_zero();

void fill_screen(uint16_t color);
