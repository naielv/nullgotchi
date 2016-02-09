// Data is clocked MSB first D/C# sampled every 8th clock. Rising clock edge is
// trigger edge.

/* Uno */
#if 0
const int PIN_CLK = 13;
const int PIN_MOSI = 11;
const int PIN_CS = 10;
const int PIN_DC = 9;
#endif

/* Tiny */
#if 1
const int PIN_CLK = 2;
const int PIN_MOSI = 1;
const int PIN_CS = 3;
const int PIN_DC = 0;
#endif

