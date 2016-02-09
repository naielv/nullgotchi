// Simple screen test
#include "dpy.h"
#include "icons.h"
#include "pins.h"

void setup() {
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CLK, LOW);
  digitalWrite(PIN_MOSI, LOW);
  digitalWrite(PIN_DC, LOW);

  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_DC, OUTPUT);

  dpy_init();
}

void loop() {
  dpy_draw_text("Hello there", 64 - 11*4, 3);
  dpy_draw_text("small world", 64 - 11*4, 4);
  dpy_draw_progress(30, 0, 80, 40);
  dpy_draw_progress(0, 1, 127, 32);
  dpy_draw_progress(0, 2, 60, 30);
  dpy_draw_progress(67, 2, 60, 15);
  dpy_draw_progress(0, 5, 127, 0);
  dpy_draw_progress(0, 6, 127, 127);
  delay(2000);
  dpy_clear();
  dpy_draw_tall_text("Hello there", 64 - 11*4, 2);
  dpy_draw_tall_text("tall world", 64 - 10*4, 4);
  delay(1000);
  dpy_draw_rle_icon(archers_rle, 0, 0);
  dpy_draw_rle_icon(cake_rle, 64, 0);
  delay(1000);
  dpy_draw_rle_icon(sleep_rle, 0, 0);
  dpy_draw_rle_icon(wake_with_tea_rle, 64, 0);
  delay(1000);
  dpy_draw_rle_icon(thought_bubble_rle, 0, 0);
  dpy_draw_rle_icon(thought_bubble_rle, 0, 4);
  dpy_draw_rle_icon(thought_bubble_rle, 64, 0);
  dpy_draw_rle_icon(thought_bubble_rle, 64, 4);
  delay(1000);
  dpy_clear();
  delay(500);
}

