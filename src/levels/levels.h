/** Filename:  pebble.h
  * Purpose:   Serve as the library of the window.c file.
  * Functions:  get_current_level, set_current_level
  */

#pragma once
#include <pebble.h>
#include "../window/window.h"

int get_current_level();
void set_current_level(int newLevel);