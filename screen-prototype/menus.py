#!/usr/bin/env python3
import draw as d

def main():
    screen = d.blank_screen()
    screen = d.draw_icon(screen, 'archers', 0, 0)
    screen = d.draw_double_text(screen, 'Listen', 64 + 32 - 6*4, 1)
    screen = d.draw_double_text(screen, 'to The', 64 + 32 - 6*4, 3)
    screen = d.draw_double_text(screen, 'Archers', 64 + 32 - 7*4, 5)
    d.save_screen(screen, 'entertain_menu.png')

    screen = d.blank_screen()
    screen = d.draw_icon(screen, 'cake', 0, 0)
    screen = d.draw_double_text(screen, 'Eat 5', 64 + 32 - 5*4, 1)
    screen = d.draw_double_text(screen, 'slices', 64 + 32 - 6*4, 3)
    screen = d.draw_double_text(screen, 'of cake', 64 + 32 - 7*4, 5)
    d.save_screen(screen, 'feed_menu.png')

    screen = d.blank_screen()
    screen = d.draw_icon(screen, 'bath', 0, 0)
    screen = d.draw_double_text(screen, 'Soak in', 64 + 32 - 7*4, 1)
    screen = d.draw_double_text(screen, 'a warm', 64 + 32 - 6*4, 3)
    screen = d.draw_double_text(screen, 'bath', 64 + 32 - 4*4, 5)
    d.save_screen(screen, 'wash.png')

    screen = d.blank_screen()
    screen = d.draw_icon(screen, 'wake-with-tea', 0, 0)
    screen = d.draw_double_text(screen, 'Bring a', 64 + 32 - 7*4, 1)
    screen = d.draw_double_text(screen, 'cup of', 64 + 32 - 6*4, 3)
    screen = d.draw_double_text(screen, 'tea', 64 + 32 - 3*4, 5)
    d.save_screen(screen, 'wake.png')

    screen = d.blank_screen()
    screen = d.draw_icon(screen, 'sleep', 0, 0)
    screen = d.draw_double_text(screen, 'Put in', 64 + 32 - 6*4, 2)
    screen = d.draw_double_text(screen, 'a bed', 64 + 32 - 5*4, 4)
    d.save_screen(screen, 'sleep.png')

if __name__ == '__main__':
    main()
