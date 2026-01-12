#ifndef __UI_INCLUDE
#define __UI_INCLUDE

#include "res/dialogs.h"

void ui_init(void);
void ui_draw(void);
void ui_show_pause(void);

void ui_draw_dialog_borders(void);
void ui_draw_fullscreen(const char *text, u8 len);
void ui_draw_typewitter(const String *text, u8 start);
void ui_play_dialog_sequence(const DialogSequence *seq);

#endif