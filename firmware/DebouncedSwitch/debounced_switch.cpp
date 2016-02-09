#include <Arduino.h>

#include "debounced_switch.h"

DebouncedSwitch::DebouncedSwitch(bool activeLow) :
    activeLow_(activeLow), state_(0),
    previousReadTime_(millis()), previousState_(0)
{ }

void DebouncedSwitch::poll(int newState) {
    if(millis() - previousReadTime_ > DebouncedSwitch::DEBOUNCE_TIME) {
        state_ = previousState_;
    }

    if(activeLow_) {
        newState = !newState;
    }

    if(newState != previousState_) {
        previousReadTime_ = millis();
        previousState_ = newState;
    }
}

int DebouncedSwitch::state() const {
    return state_;
}

