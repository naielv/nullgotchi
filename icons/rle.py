#!/usr/bin/env python3
"""
Run-length encode icons.

Usage:
    rle.py <icon>...

Each icon is run length encoded and saved with the extension replaced with
".rle".

The format is the row width encoded as one byte followed by one-byte runs.  The
MSB of the byte encodes the pixel value and the LSB encodes the length of the
run. Runs may not straddle rows.

"""
import itertools
import os

import docopt
import PIL.Image as Image
import numpy as np

def main():
    opts = docopt.docopt(__doc__)
    for icon_fn in opts['<icon>']:
        im = Image.open(icon_fn).convert('L')
        assert im.width < 256
        assert im.height < 256
        out_bytes = [im.width]

        data = np.asarray(Image.open(icon_fn).convert('L'))
        for row in data:
            for pix, run in itertools.groupby(row, lambda x: x):
                run_len = len(list(run))
                while run_len > 0:
                    run_len_to_write = min(run_len, 127)
                    run_len -= run_len_to_write
                    if pix > 128:
                        run_len_to_write += 128
                    out_bytes.append(run_len_to_write)

        output_fn = os.path.splitext(icon_fn)[0] + '.rle'
        with open(output_fn, 'wb') as out_fobj:
            out_fobj.write(bytearray(out_bytes))


if __name__ == '__main__':
    main()
