#!/usr/bin/env python3
import draw as d

def main():
    screen = d.blank_screen()
    screen = d.draw_percentage_bar(screen, 50, 72, 127, 4)
    screen = d.draw_percentage_bar(screen, 33, 72, 127, 5)
    screen = d.draw_percentage_bar(screen, 75, 72, 127, 6)
    screen = d.draw_percentage_bar(screen, 100, 72, 127, 7)
    screen = d.draw_icon(screen, 'rest-level', 64, 4)
    screen = d.draw_icon(screen, 'hygiene-level', 64, 5)
    screen = d.draw_icon(screen, 'entertain-level', 64, 6)
    screen = d.draw_icon(screen, 'food-level', 64, 7)
    screen = d.draw_icon(screen, 'thought-bubble', 64, 0)
    screen = d.draw_text(screen, 'Grumpy', 64 + 32 - 6*4, 1)
    screen = d.draw_text(screen, 'Hungry', 64 + 32 - 6*4, 2)
    screen = d.draw_icon(screen, 'crying', 8, 2)
    d.save_screen(screen, 'status.png')

if __name__ == '__main__':
    main()
