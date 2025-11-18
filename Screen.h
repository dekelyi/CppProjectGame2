#pragma once
#include "Vector.h"


void gotoxy(V pos);

void hideCursor();

void cls();
void drawAt(V pos, V size, char glyph);