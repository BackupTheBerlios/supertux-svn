//
// C Interface: text
//
// Description: 
//
//
// Author: Tobias Glaesser <tobi.web@gmx.de>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef SUPERTUX_TEXT_H
#define SUPERTUX_TEXT_H

#include "texture.h"

/* Text type */
struct text_type
{
  Surface* chars;
  Surface* shadow_chars;
  int kind;
  int w;
  int h;
};

/* Kinds of texts. */
enum {
   TEXT_TEXT,
   TEXT_NUM
};

enum TextHAlign {
   A_LEFT,
   A_HMIDDLE,
   A_RIGHT,
};

enum TextVAlign {
   A_TOP,
   A_VMIDDLE,
   A_BOTTOM,
};

void text_load(text_type* ptext, const std::string& file, int kind, int w, int h);
void text_draw(text_type* ptext, const char* text, int x, int y, int shadowsize, int update = NO_UPDATE);
void text_draw_chars(text_type* ptext, Surface* pchars, const char* text, int x, int y, int update = NO_UPDATE);
void text_drawf(text_type* ptext, const char* text, int x, int y, TextHAlign halign, TextVAlign valign, int shadowsize, int update = NO_UPDATE);
void text_draw_align(text_type* ptext, const char* text, int x, int y, TextHAlign halign, TextVAlign valign, int shadowsize, int update = NO_UPDATE);
void text_free(text_type* ptext);
void erasetext(text_type* ptext, const char * text, int x, int y, Surface* surf, int update, int shadowsize);
void erasecenteredtext(text_type* ptext, const char * text, int y, Surface* surf, int update, int shadowsize);

#endif /*SUPERTUX_TEXT_H*/

