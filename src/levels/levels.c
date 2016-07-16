#include "levels.h"

uint32_t key = 0;

int get_current_level() {
  if(persist_exists(key)) {
    //Read value
    return persist_read_int(key);
  }
  else {
    return 1;
  }
}

void set_current_level(int newLevel) {
  persist_write_int(key, newLevel);
}
