#include <pebble.h>
#include "health/health.h"
#include "window/window.h"

void init() {
  health_init();
  window_stack_push();
}

void terminate() {
  window_ui_destroy();
}

int main(void) {
  init();
  terminate();
}
