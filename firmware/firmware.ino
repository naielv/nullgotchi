#include "DebouncedSwitch/debounced_switch.h"
#include "EdgeTrigger/edge_trigger.h"

#include "dpy.h"
#include "icons.h"
#include "pins.h"

DebouncedSwitch select_switch, action_switch;
EdgeTrigger select_trigger, action_trigger;

const byte MAX_STAT = 64-8;
const int TICK_DELTA = 5000;

byte hygiene, rest, food, entertainment;

unsigned long next_tick;

enum WakeState {
  WS_SLEEPING,
  WS_AWAKE,
  WS_LEFT,
};

enum MenuState {
  STATUS,
  ENTERTAIN,
  FEED,
  WASH,
  SLEEP,
  WAKE,
  RESET,
};

MenuState menu_state;
WakeState wake_state;

void setup() {
  digitalWrite(PIN_CS, HIGH);
  digitalWrite(PIN_CLK, LOW);
  digitalWrite(PIN_MOSI, LOW);
  digitalWrite(PIN_DC, LOW);

  pinMode(PIN_CLK, OUTPUT);
  pinMode(PIN_MOSI, OUTPUT);
  pinMode(PIN_CS, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_ANALOG_IN, INPUT);

  dpy_init();
  dpy_clear();

  reset();
  tick();
  update_display();
}

void loop() {
  // Buttons
  int button_val = analogRead(PIN_ANALOG_IN);
  select_switch.poll(button_val <= 0x100);
  action_switch.poll((button_val <= 0x300) && (button_val > 0x100));
  select_trigger.update(select_switch.state());
  action_trigger.update(action_switch.state());

  if(action_trigger.triggered()) {
    on_action();
  }

  if(select_trigger.triggered()) {
    on_select();
  }

  select_trigger.clear();
  action_trigger.clear();

  // Tick
  if(millis() >= next_tick) {
    if(menu_state == STATUS) {
      tick();
      update_display();
    } else {
      next_tick = millis() + TICK_DELTA;
    }
  }
}

void reset() {
  hygiene = MAX_STAT;
  rest = MAX_STAT>>1;
  food = MAX_STAT;
  entertainment = (MAX_STAT>>1) + (MAX_STAT>>2);

  wake_state = WS_SLEEPING;
  menu_state = STATUS;
}

void on_select() {
  switch(menu_state) {
    case RESET:
      reset();
      break;
    case WASH:
      if(wake_state == WS_AWAKE) {
        hygiene = change(hygiene, 5);
        entertainment = change(entertainment, 2);
      }
      menu_state = STATUS;
      update_display();
      break;
    case ENTERTAIN:
      if(wake_state == WS_AWAKE) {
        entertainment = change(entertainment, 5);
        rest = change(rest, -4);
        hygiene = change(hygiene, -1);
      }
      menu_state = STATUS;
      update_display();
      break;
    case SLEEP:
      if(wake_state == WS_AWAKE) {
        wake_state = WS_SLEEPING;
      }
      menu_state = STATUS;
      update_display();
      break;
    case FEED:
      if(wake_state == WS_AWAKE) {
        food = change(food, 5);
        hygiene = change(hygiene, -2);
        entertainment = change(entertainment, 2);
      }
      menu_state = STATUS;
      update_display();
      break;
    case WAKE:
      if(wake_state == WS_SLEEPING) {
        wake_state = WS_AWAKE;
      }
      menu_state = STATUS;
      update_display();
      break;
  }
}

byte change(byte val, int delta) {
  if(delta < 0) {
    delta = -delta;
    if(val <= delta) { return 0; }
    return val - delta;
  }
  if(MAX_STAT-val <= delta) { return MAX_STAT; }
  return val + delta;
}

void tick() {
  next_tick = millis() + TICK_DELTA;

  switch(wake_state) {
    case WS_SLEEPING:
      rest = change(rest, 3);
      hygiene = change(hygiene, -1);
      food = change(food, -2);
      if(rest == MAX_STAT) {
        wake_state = WS_AWAKE;
      }
      break;
    case WS_AWAKE:
      rest = change(rest, -1);
      hygiene = change(hygiene, -1);
      entertainment = change(entertainment, -2);
      food = change(food, -3);
      break;
  }

  byte max_stat = max(max(hygiene, food), max(rest, entertainment));
  if(max_stat == 0) {
    wake_state = WS_LEFT;
  }
}

void draw_status_display() {
  dpy_clear_rect(0, 64, 0, 7);
  dpy_draw_rle_icon(thought_bubble_rle, 64, 0);

  byte min_stat = min(min(hygiene, food), min(rest, entertainment));

  switch(wake_state) {
    case WS_SLEEPING:
      dpy_draw_rle_icon(sleeping_rle, 8, 1);
      //dpy_draw_rle_icon(sleep_rle, 8, 1);
      dpy_draw_tall_text("Zzz", 64+32-3*4, 1);
      break;
    case WS_AWAKE:
      if(min_stat < (MAX_STAT>>2)) {
        dpy_draw_rle_icon(crying_rle, 8, 1);
      } else if(min_stat < (MAX_STAT>>1)) {
        dpy_draw_rle_icon(sad_rle, 8, 1);
      } else {
        dpy_draw_rle_icon(happy_rle, 8, 1);
      }

      if(min_stat > (MAX_STAT>>1)) {
        dpy_draw_text("I love", 64+32-6*4, 1);
        dpy_draw_text("you", 64+32-3*4, 2);
      } else if(min_stat == hygiene) {
        dpy_draw_text("I'm", 64+32-3*4, 1);
        dpy_draw_text("stinky", 64+32-6*4, 2);
      } else if(min_stat == food) {
        dpy_draw_tall_text("HUNGRY", 64+32-6*4, 1);
      } else if(min_stat == rest) {
        dpy_draw_tall_text("Zzz?!", 64+32-6*4, 1);
      } else if(min_stat == entertainment) {
        dpy_draw_text("I'm so", 64+32-6*4, 1);
        dpy_draw_text("bored", 64+32-5*4, 1);
      }
      break;
    case WS_LEFT:
      dpy_draw_tall_text("Clem", 32-4*4, 0);
      dpy_draw_tall_text("has", 32-3*4, 2);
      dpy_draw_tall_text("left", 32-4*4, 4);
      dpy_draw_tall_text("you", 32-3*4, 6);
      dpy_draw_text("Bye", 64+32-3*4, 1);
      dpy_draw_text("meanie", 64+32-6*4, 2);
      break;
    }

  dpy_clear_rect(64+8-1, 64+8, 4, 7);

  dpy_draw_text("!", 64-1, 4);
  dpy_draw_progress(64+8, 4, 64-8, rest);

  dpy_draw_text("$", 64-1, 5);
  dpy_draw_progress(64+8, 5, 64-8, entertainment);

  dpy_draw_text("#", 64-1, 6);
  dpy_draw_progress(64+8, 6, 64-8, food);

  dpy_draw_text("\"", 64-1, 7);
  dpy_draw_progress(64+8, 7, 64-8, hygiene);
}

void update_display() {
  switch(menu_state) {
    case STATUS:
      draw_status_display();
      break;
    case ENTERTAIN:
      dpy_draw_rle_icon(archers_rle, 0, 0);
      dpy_clear_rect(64, 127, 0, 7);
      dpy_draw_tall_text("Listen", 64+32-6*4, 1);
      dpy_draw_tall_text("to The", 64+32-6*4, 3);
      dpy_draw_tall_text("Archers", 64+32-7*4, 5);
      break;
    case FEED:
      dpy_draw_rle_icon(cake_rle, 0, 0);
      dpy_clear_rect(64, 127, 0, 7);
      dpy_draw_tall_text("Eat 5", 64+32-5*4, 1);
      dpy_draw_tall_text("slices", 64+32-6*4, 3);
      dpy_draw_tall_text("of cake", 64+32-7*4, 5);
      break;
    case WASH:
      dpy_draw_rle_icon(bath_rle, 0, 0);
      dpy_clear_rect(64, 127, 0, 7);
      dpy_draw_tall_text("Soak in", 64+32-7*4, 1);
      dpy_draw_tall_text("a warm", 64+32-6*4, 3);
      dpy_draw_tall_text("bath", 64+32-4*4, 5);
      break;
    case SLEEP:
      dpy_clear_rect(0, 64, 0, 7);
      dpy_draw_rle_icon(sleeping_rle, 8, 1);
      dpy_clear_rect(64, 127, 0, 7);
      dpy_draw_tall_text("Put in", 64+32-6*4, 2);
      dpy_draw_tall_text("a bed", 64+32-5*4, 4);
      break;
    case WAKE:
      dpy_draw_rle_icon(wake_with_tea_rle, 0, 0);
      dpy_clear_rect(64, 127, 0, 7);
      dpy_draw_tall_text("Wake", 64+32-4*4, 1);
      dpy_draw_tall_text("with a", 64+32-6*4, 3);
      dpy_draw_tall_text("cuppa", 64+32-5*4, 5);
      break;
    case RESET:
      dpy_clear();
      dpy_draw_tall_text("Beg mercy", 64-8*4, 3); 
      break;
  }
}

void on_action() {
  MenuState new_state;

  switch(menu_state) {
    case STATUS:
      if(wake_state == WS_LEFT) {
        new_state = RESET;
      } else {
        new_state = ENTERTAIN;
      }
      break;
    case ENTERTAIN:
      new_state = FEED;
      break;
    case FEED:
      new_state = WASH;
      break;
    case WASH:
      new_state = SLEEP;
      break;
    case SLEEP:
      new_state = WAKE;
      break;
    case WAKE:
      new_state = STATUS;
      break;
    case RESET:
      new_state = STATUS;
      break;
  }

  if(new_state != menu_state) {
    menu_state = new_state;
    update_display();
  }
}
