#include <avr/pgmspace.h>

#include "font.h"
#include "icons.h"

void draw_icon_real__(PGM_VOID_P icon_data, unsigned int icon_len,
                      byte column, byte row)
{
  byte height = pgm_read_byte_near(icon_data);
  byte sum = 0;
  for(unsigned int i=1; i<icon_len; ++i) {
    byte run = pgm_read_byte_near(icon_data + i);
    digitalWrite(3, (run & 0x2) ? HIGH : LOW);
    delay(30);
  }
}

#define draw_icon(name, column, page) \
  draw_icon_real__(name, name##_len, column, page)

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 3 as an output.
  pinMode(3, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  draw_icon(breakup_rle, 0, 0);
  draw_icon(hygiene_level_rle, 0, 0);
  draw_icon(bath_rle, 0, 0);
  draw_icon(archers_rle, 0, 0);
  draw_icon(cake_rle, 0, 0);
  draw_icon(happy_rle, 0, 0);
  draw_icon(rest_level_rle, 0, 0);
  draw_icon(crying_rle, 0, 0);
  draw_icon(sleep_rle, 0, 0);
  draw_icon(entertain_level_rle, 0, 0);
  draw_icon(wake_with_tea_rle, 0, 0);
  draw_icon(sad_rle, 0, 0);
  draw_icon(food_level_rle, 0, 0);

  for(unsigned int i=0; i<font_dat_len; ++i) {
    digitalWrite(3, (pgm_read_byte_near(font_dat + i) & 0x2) ? HIGH : LOW);
  }
}
