#pragma once
#ifndef GUARD_DEBOUNCED_SWITCH_H__
#define GUARD_DEBOUNCED_SWITCH_H__

class DebouncedSwitch {
public:
    DebouncedSwitch(bool activeLow = false);

    void poll(int newState);
    int state() const;
private:
    const unsigned long DEBOUNCE_TIME = 20; // milliseconds

    bool            activeLow_;
    int             state_;
    unsigned long   previousReadTime_;
    int             previousState_;
};

#endif // GUARD_DEBOUNCED_SWITCH_H__
