from __future__ import division

import os

import PIL.Image as Image
import numpy as np

import dirs

def blank_screen():
    return np.zeros((64, 128), np.uint8)

def draw_percentage_bar(screen, percentage, left, right, page):
    full = np.array([
        0, 255, 255, 255, 255, 255, 255, 0
    ], np.uint8)
    empty = np.array([
        0, 255, 0, 255, 0, 255, 0, 0
    ], np.uint8)
    for col_idx in range(left, right+1):
        col_percentage = (1e2 * (col_idx - left)) // (right - left + 1)
        if col_percentage < percentage:
            screen[(page<<3):(page<<3)+8, col_idx] = full
        elif col_idx % 2 == 0:
            screen[(page<<3):(page<<3)+8, col_idx] = empty
        else:
            screen[(page<<3):(page<<3)+8, col_idx] = empty[::-1]
    return screen

def draw_icon(screen, name, left, page):
    im = Image.open(os.path.join(dirs.ICONS_DIR, name + '.png')).convert('L')
    imdata = np.asarray(im)
    screen[(page<<3):(page<<3)+im.height, left:left+im.width] = imdata
    return screen

def draw_text(screen, text, column, page):
    font_im = Image.open(os.path.join(dirs.FONT_DIR, 'font.png')).convert('L')
    font = np.asarray(font_im)
    font_chars = []
    for row_idx in range(0, font_im.height, 8):
        for col_idx in range(0, font_im.width, 8):
            font_chars.append(font[row_idx:row_idx+8, col_idx:col_idx+8])
    font_chars = np.dstack(font_chars)

    for ch in text:
        ch_idx = ord(ch) - ord(' ')
        screen[(page<<3):(page<<3)+8, column:column+8] = font_chars[..., ch_idx]
        column += 8

    return screen

def draw_double_text(screen, text, column, page):
    font_im = Image.open(os.path.join(dirs.FONT_DIR, 'font.png')).convert('L')
    font = np.asarray(font_im)
    font_chars = []
    for row_idx in range(0, font_im.height, 8):
        for col_idx in range(0, font_im.width, 8):
            font_chars.append(font[row_idx:row_idx+8, col_idx:col_idx+8])
    font_chars = np.dstack(font_chars)

    for ch in text:
        ch_idx = ord(ch) - ord(' ')
        ch_data = np.zeros((16, 8), np.uint8)
        ch_data[0::2, :] = font_chars[..., ch_idx]
        ch_data[1::2, :] = font_chars[..., ch_idx]
        screen[(page<<3):(page<<3)+16, column:column+8] = ch_data
        column += 8

    return screen

def save_screen(screen, fn):
    im = Image.fromarray(screen)
    im.save(fn)
