#!/usr/bin/env python3
"""
Dump font to memory.

Usage:
    dump.py <image>

Dump each 8x8 block of <image> to a file with the same name as <image> but the
extension changed to ".dump". Blocks are dumpred in row-major order. Each block
is dumped as 8 bytes representing each column.

"""
import os

import docopt
import PIL.Image as Image
import numpy as np

def main():
    opts = docopt.docopt(__doc__)

    font_fn = opts['<image>']
    im = Image.open(font_fn).convert('L')

    out_bytes = []

    data = np.asarray(im)
    for row_idx in range(0, im.height, 8):
        for col_idx in range(0, im.width, 8):
            block = data[row_idx:(row_idx+8), col_idx:(col_idx+8)]
            for block_col in block.T:
                val = 0
                for px_idx, px in enumerate(block_col):
                    if px > 128:
                        val += 1<<px_idx
                out_bytes.append(val)

    output_fn = os.path.splitext(font_fn)[0] + '.dat'
    with open(output_fn, 'wb') as out_fobj:
        out_fobj.write(bytearray(out_bytes))

if __name__ == '__main__':
    main()
