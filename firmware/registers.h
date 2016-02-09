
const byte DPY_LCDWIDTH =                  128;
const byte DPY_LCDHEIGHT =                 64;

const byte DPY_SETCONTRAST = 0x81;
const byte DPY_DISPLAYALLON_RESUME = 0xA4;
const byte DPY_DISPLAYALLON = 0xA5;
const byte DPY_NORMALDISPLAY = 0xA6;
const byte DPY_INVERTDISPLAY = 0xA7;
const byte DPY_DISPLAYOFF = 0xAE;
const byte DPY_DISPLAYON = 0xAF;

const byte DPY_SETDISPLAYOFFSET = 0xD3;
const byte DPY_SETCOMPINS = 0xDA;

const byte DPY_SETVCOMDETECT = 0xDB;

const byte DPY_SETDISPLAYCLOCKDIV = 0xD5;
const byte DPY_SETPRECHARGE = 0xD9;

const byte DPY_SETMULTIPLEX = 0xA8;

const byte DPY_SETLOWCOLUMN = 0x00;
const byte DPY_SETHIGHCOLUMN = 0x10;

const byte DPY_SETSTARTLINE = 0x40;

const byte DPY_MEMORYMODE = 0x20;

const byte DPY_COMSCANINC = 0xC0;
const byte DPY_COMSCANDEC = 0xC8;

const byte DPY_SEGREMAP = 0xA0;

const byte DPY_CHARGEPUMP = 0x8D;

const byte DPY_EXTERNALVCC = 0x1;
const byte DPY_SWITCHCAPVCC = 0x2;

// Scrolling #defines
const byte DPY_ACTIVATE_SCROLL = 0x2F;
const byte DPY_DEACTIVATE_SCROLL = 0x2E;
const byte DPY_SET_VERTICAL_SCROLL_AREA = 0xA3;
const byte DPY_RIGHT_HORIZONTAL_SCROLL = 0x26;
const byte DPY_LEFT_HORIZONTAL_SCROLL = 0x27;
const byte DPY_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL = 0x29;
const byte DPY_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL = 0x2A;

