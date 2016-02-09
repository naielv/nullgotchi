#include "DebouncedSwitch/debounced_switch.h"
#include "EdgeTrigger/edge_trigger.h"

#include "dpy.h"
#include "icons.h"
#include "pins.h"

DebouncedSwitch select_switch, action_switch;
EdgeTrigger select_trigger, action_trigger;

enum MenuState {
  STATUS,
  ENTERTAIN,
  FEED,
  WASH,
  SLEEP,
  WAKE,
};

MenuState menu_state = STATUS;

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

  select_trigger.clear();
  action_trigger.clear();
}

void update_display() {
  switch(menu_state) {
    case STATUS:
      dpy_clear();
      dpy_draw_tall_text("STATUS", 10, 2);
      break;
    case ENTERTAIN:
      dpy_clear();
      dpy_draw_rle_icon(archers_rle, 0, 0);
      dpy_draw_tall_text("Listen", 64+32-6*4, 1);
      dpy_draw_tall_text("to The", 64+32-6*4, 3);
      dpy_draw_tall_text("Archers", 64+32-7*4, 5);
      break;
    case FEED:
      dpy_clear();
      dpy_draw_rle_icon(cake_rle, 0, 0);
      dpy_draw_tall_text("Eat 5", 64+32-5*4, 1);
      dpy_draw_tall_text("slices", 64+32-6*4, 3);
      dpy_draw_tall_text("of cake", 64+32-7*4, 5);
      break;
    case WASH:
      dpy_clear();
      dpy_draw_tall_text("WASH", 10, 2);
      break;
    case SLEEP:
      dpy_clear();
      dpy_draw_tall_text("SLEEP", 10, 2);
      break;
    case WAKE:
      dpy_clear();
      dpy_draw_tall_text("WAKE", 10, 2);
      break;
  }
}

void on_action() {
  MenuState new_state;

  switch(menu_state) {
    case STATUS:
      new_state = ENTERTAIN;
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
  }

  if(new_state != menu_state) {
    menu_state = new_state;
    update_display();
  }
}
