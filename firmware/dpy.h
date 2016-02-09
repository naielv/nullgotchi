#include <avr/pgmspace.h>

typedef enum {
  DPY_MEM_MODE_HORIZONTAL = 0x0,
  DPY_MEM_MODE_VERTICAL = 0x1,
  DPY_MEM_MODE_PAGE = 0x2,
} dpy_memory_mode_t;

// Low-level interface
void dpy_command(byte value);
void dpy_data(byte value);

// High-level interface
void dpy_init();
void dpy_set_column_start_address(byte addr);
void dpy_set_memory_mode(dpy_memory_mode_t mode);

void dpy_clear_rect(byte start_col, byte end_col,
                    byte start_page, byte end_page, byte val = 0x0);
inline void dpy_clear(byte val = 0x0) { dpy_clear_rect(0, 127, 0, 7); }

// Icons
#define dpy_draw_rle_icon(icon_name, col, page) \
  dpy_draw_rle_icon__real(icon_name, icon_name##_len, col, page);

void dpy_draw_rle_icon__real(PGM_VOID_P icon, unsigned int icon_len,
                             unsigned int column, unsigned int page);

void dpy_draw_text(const char* text, unsigned int column, unsigned int page);
void dpy_draw_tall_text(const char* text, unsigned int column,
                        unsigned int page);

void dpy_draw_progress(unsigned int column, unsigned int page,
                       unsigned int width, unsigned int filled);
