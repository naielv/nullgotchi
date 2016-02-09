#include <Arduino.h>

#include "registers.h"
#include "dpy.h"
#include "pins.h"

#include "font.h"

#define USE_SOFTWARE_SPI 0
#define USE_TINY_SPI 1

#if USE_TINY_SPI
#include "tinySPI.h"
#endif

static void dpy_set_page_address(byte start, byte end) {
  dpy_command(0x22);
  dpy_command(start);
  dpy_command(end);
}

static void dpy_set_column_address(byte start, byte end) {
  dpy_command(0x21);
  dpy_command(start);
  dpy_command(end);
}

enum TransferType {
  DATA, COMMAND,
};

static void dpy_begin(TransferType type) {
  digitalWrite(PIN_DC, type == DATA ? HIGH : LOW);

#if USE_SOFTWARE_SPI
  digitalWrite(PIN_CLK, LOW);
  digitalWrite(PIN_CS, LOW);
#endif

#if 0
  SPI.beginTransaction(SPISettings(800000, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_CS, LOW);
#endif

#if USE_TINY_SPI
  digitalWrite(PIN_CS, LOW);
#endif
}

static void dpy_end() {
#if USE_SOFTWARE_SPI
  digitalWrite(PIN_CLK, LOW);
  digitalWrite(PIN_CS, HIGH);
#endif

#if 0
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
#endif

#if USE_TINY_SPI
  digitalWrite(PIN_CS, HIGH);
#endif
}

static void dpy_transfer(byte value) {
#if USE_SOFTWARE_SPI
  shiftOut(PIN_MOSI, PIN_CLK, MSBFIRST, value);
#endif

#if 0
  SPI.transfer(value);
#endif

#if USE_TINY_SPI
  SPI.transfer(value);
#endif
}

void dpy_command(byte value) {
  dpy_begin(COMMAND);
  dpy_transfer(value);
  dpy_end();
}

void dpy_data(byte value) {
  dpy_begin(DATA);
  dpy_transfer(value);
  dpy_end();
}

void dpy_init() {
#if USE_TINY_SPI
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
#endif

  // Init sequence for 128x64 OLED module
  dpy_begin(COMMAND);
  dpy_transfer(DPY_DISPLAYOFF);                    // 0xAE
  dpy_transfer(DPY_SETDISPLAYCLOCKDIV);            // 0xD5
  dpy_transfer(0x80);                                  // the suggested ratio 0x80
  dpy_transfer(DPY_SETMULTIPLEX);                  // 0xA8
  dpy_transfer(0x3F);
  dpy_transfer(DPY_SETDISPLAYOFFSET);              // 0xD3
  dpy_transfer(0x0);                                   // no offset
  dpy_transfer(DPY_SETSTARTLINE | 0x0);            // line #0
  dpy_transfer(DPY_CHARGEPUMP);                    // 0x8D
  dpy_transfer(0x14);
  dpy_transfer(DPY_MEMORYMODE);                    // 0x20
  dpy_transfer(0x00);                                  // 0x0 act like ks0108
  dpy_transfer(DPY_SEGREMAP | 0x1);
  dpy_transfer(DPY_COMSCANDEC);
  dpy_transfer(DPY_SETCOMPINS);                    // 0xDA
  dpy_transfer(0x12);
  dpy_transfer(DPY_SETCONTRAST);                   // 0x81
  dpy_transfer(0xCF);
  dpy_transfer(DPY_SETPRECHARGE);                  // 0xd9
  dpy_transfer(0xF1);
  dpy_transfer(DPY_SETVCOMDETECT);                 // 0xDB
  dpy_transfer(0x40);
  dpy_transfer(DPY_DISPLAYALLON_RESUME);           // 0xA4
  dpy_transfer(DPY_NORMALDISPLAY);                 // 0xA6
  dpy_end();

  dpy_clear();
  dpy_command(DPY_DISPLAYON);//--turn on oled panel
}

void dpy_set_column_start_address(byte addr) {
  dpy_command(DPY_SETLOWCOLUMN + (addr & 0xf));
  dpy_command(DPY_SETHIGHCOLUMN + ((addr >> 4) & 0xf));
}

void dpy_set_memory_mode(dpy_memory_mode_t mode) {
  dpy_command(DPY_MEMORYMODE);
  dpy_command(mode);
}

void dpy_clear(byte val) {
  dpy_set_column_address(0, 127);
  dpy_set_page_address(0, 7);
  dpy_set_memory_mode(DPY_MEM_MODE_HORIZONTAL);

  dpy_begin(DATA);
  for(int page=0; page<8; ++page) {
    for(int col=0; col<128; ++col) {
      dpy_transfer(val);
    }
  }
  dpy_end();
}

void dpy_draw_rle_icon__real(PGM_VOID_P icon, unsigned int icon_len,
                             unsigned int column, unsigned int page)
{
  // Read height of icon
  byte height = pgm_read_byte_near(icon);

  dpy_set_memory_mode(DPY_MEM_MODE_VERTICAL);
  dpy_set_column_address(column, 127);
  dpy_set_page_address(page, (page + (height>>3)) - 1);

  dpy_begin(DATA);

  // Read runs
  byte next_val = 0, bit_idx = 0;
  for(unsigned int i=1; i<icon_len; ++i) {
    byte run = pgm_read_byte_near(icon + i);
    byte bit_val = (run & 0x80) ? 1 : 0;
    byte run_len = run & 0x7f;
    for(byte j=0; j<run_len; ++j) {
      next_val |= bit_val << bit_idx;
      if(bit_idx < 7) {
        ++bit_idx;
      } else {
        dpy_transfer(next_val);
        next_val = 0;
        bit_idx = 0;
      }
    }
  }

  dpy_end();
}

void dpy_draw_text(const char* text, unsigned int column, unsigned int page) {
  int font_idx = 0;

  dpy_set_memory_mode(DPY_MEM_MODE_HORIZONTAL);
  dpy_set_page_address(page, page);
  dpy_set_column_address(column, 127);

  dpy_begin(DATA);
  for(; *text != '\0'; ++text) {
    if((*text < 32) || (*text >= 128)) {
      font_idx = 0;
    } else {
      font_idx = (*text) - 32;
    }
    font_idx <<= 3;
    for(int i=0; i<8; ++i, ++font_idx) {
      dpy_transfer(pgm_read_byte_near(font_dat + font_idx));
    }
  }
  dpy_end();
}

void dpy_draw_tall_text(const char* text, unsigned int column,
                        unsigned int page) {
  int font_idx = 0;
  byte pattern;

  dpy_set_memory_mode(DPY_MEM_MODE_VERTICAL);
  dpy_set_page_address(page, page+1);
  dpy_set_column_address(column, 127);

  dpy_begin(DATA);
  for(; *text != '\0'; ++text) {
    if((*text < 32) || (*text >= 128)) {
      font_idx = 0;
    } else {
      font_idx = (*text) - 32;
    }
    font_idx <<= 3;
    for(int i=0; i<8; ++i, ++font_idx) {
      pattern = pgm_read_byte_near(font_dat + font_idx);
      dpy_transfer(
        ((pattern & 0x01) ? 0x03 : 0x00) |
        ((pattern & 0x02) ? 0x0c : 0x00) |
        ((pattern & 0x04) ? 0x30 : 0x00) |
        ((pattern & 0x08) ? 0xc0 : 0x00)
      );
      dpy_transfer(
        ((pattern & 0x10) ? 0x03 : 0x00) |
        ((pattern & 0x20) ? 0x0c : 0x00) |
        ((pattern & 0x40) ? 0x30 : 0x00) |
        ((pattern & 0x80) ? 0xc0 : 0x00)
      );
    }
  }
  dpy_end();
}

void dpy_draw_progress(unsigned int column, unsigned int page,
                       unsigned int width, unsigned int filled) {
  dpy_set_memory_mode(DPY_MEM_MODE_HORIZONTAL);
  dpy_set_page_address(page, page);
  dpy_set_column_address(column, (column+width)-1);

  dpy_begin(DATA);
  for(unsigned int i=0; i<width; ++i) {
    if(i < filled) {
      dpy_transfer(0x7e);
    } else {
      dpy_transfer((i & 0x1) ? 0x54 : 0x2A);
    }
  }
  dpy_end();
}
